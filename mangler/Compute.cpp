#include "Compute.h"

stringstream* Compute::LoadKernelFromFile(string fileName)
{
	stringstream ss;
	string source;
	ifstream* ifs = new ifstream(fileName);

	// READ
	if (ifs->is_open())
	{
		ss << ifs->rdbuf();

		//source = std::string(ss.str());
	}
	//const char* c = source.c_str();
	//return c;

	delete ifs;

	return &ss;
}

int Compute::ExecuteKernelRead(float* buffer)
{
	int err;

	// Execute the kernel over the entire range of our 1d input data set
	// using the maximum number of work group items for this device
	//
	global = count;
	err = clEnqueueNDRangeKernel(commands, kernel, 1, nullptr, &global, &local, 0, nullptr, nullptr);
	if (err)
	{
		printf("Error: Failed to execute kernel!\n");
		return EXIT_FAILURE;
	}

	// Wait for the command commands to get serviced before reading back results
	clFinish(commands);

	// Read back the results from the device
	err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * count, buffer, 0, nullptr, nullptr);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to read output array! %d\n", err);
		exit(1);
	}
	return CL_SUCCESS;
}

void Compute::ExecuteKernelWait()
{
	ExecuteKernel();

	// Wait for the command commands to get serviced before reading back results
	clFinish(commands);
}

int Compute::ExecuteKernel()
{
	// Execute the kernel over the entire range of our 1d input data set
	// using the maximum number of work group items for this device
	global = count;
	int err = clEnqueueNDRangeKernel(commands, kernel, 1, nullptr, &global, &local, 0, nullptr, nullptr);
	if (err)
	{
		printf("Error: Failed to execute kernel!\n");
		return EXIT_FAILURE;
	}
}

void Compute::ReadBuffer(void* buffer)
{
	int err;

	// Read back the results from the device
	err = clEnqueueReadBuffer(commands, output, CL_TRUE, 0, sizeof(float) * count, buffer, 0, nullptr, nullptr);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to read output array! %d\n", err);
		exit(1);
	}
}

int Compute::CreateCLContext()
{
	cl_int err;



#define CAPSBASIC_CHECK_ERRORS(ERR)        \
	if(ERR != CL_SUCCESS)                  \
    {                                      \
		cerr                                   \
		<< "OpenCL error with code " << ERR    \
		<< " happened in file " << __FILE__    \
		<< " at line " << __LINE__             \
		<< ". Exiting...\n";                   \
		exit(1);                               \
    }


	// Default substring for platform name
	const char* required_platform_subname = "Intel";

	cl_uint numPlatforms;

	err = clGetPlatformIDs(0, nullptr, &numPlatforms);

	cl_platform_id* platforms = new cl_platform_id[numPlatforms];
	// get IDs for all platforms:
	err = clGetPlatformIDs(numPlatforms, platforms, nullptr);

	// -----------------------------------------------------------------------
	// 4. List all platforms and select one.
	// We use platform name to select needed platform.

	cl_uint selected_platform_index = numPlatforms;

	for (cl_uint i = 0; i < numPlatforms; ++i)
	{
		// Get the length for the i-th platform name
		size_t platform_name_length = 0;
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			0,
			nullptr,
			&platform_name_length
		);
		CAPSBASIC_CHECK_ERRORS(err);

		// Get the name itself for the i-th platform
		char* platform_name = new char[platform_name_length];
		err = clGetPlatformInfo(
			platforms[i],
			CL_PLATFORM_NAME,
			platform_name_length,
			platform_name,
			nullptr
		);
		CAPSBASIC_CHECK_ERRORS(err);

		cout << "    [" << i << "] " << platform_name;

		// decide if this i-th platform is what we are looking for
		// we select the first one matched skipping the next one if any
		if (
			strstr(platform_name, required_platform_subname) &&
			selected_platform_index == numPlatforms // have not selected yet
			)
		{
			cout << " [Selected]";
			selected_platform_index = i;
			// do not stop here, just see all available platforms
		}

		cout << endl;
		delete[] platform_name;
	}

	if (selected_platform_index == numPlatforms)
	{
		cerr
			<< "There is no found platform with name containing \""
			<< required_platform_subname << "\" as a substring.\n";
		return 1;
	}

	cl_platform_id platform = platforms[selected_platform_index];




	// Connect to a compute device
	int gpu = 1;
	err = clGetDeviceIDs(platform, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &device_id, nullptr);
	if (err != CL_SUCCESS)
	{
		printf("Error: Failed to create a device group!\n");
		return EXIT_FAILURE;
	}

	// Create a compute context
	context = clCreateContext(0, 1, &device_id, nullptr, nullptr, &err);
	if (!context)
	{
		printf("Error: Failed to create a compute context!\n");
		return EXIT_FAILURE;
	}

	// Create a command commands
	commands = clCreateCommandQueueWithProperties(context, device_id, nullptr, &err);

	if (!commands)
	{
		printf("Error: Failed to create a command commands!\n");
		return EXIT_FAILURE;
	}

	// Create the compute program from the source buffer

	stringstream* src = LoadKernelFromFile("julia_set.cl");
	const char** sr = (const char **)(&KernelSource);

	const char source = *src->str().c_str();
	size_t sourceSize[] = { strlen(&source) };

	//const char* cc[];// = (&source);

	//const char** cac = (&cc);

	//program = clCreateProgramWithSource(context, 1, sr, sourceSize, nullptr);

	program = clCreateProgramWithSource(context, 1, sr, nullptr, &err); // KernelSource
	if (!program)
	{
		printf("Error: Failed to create compute program!\n");
		return EXIT_FAILURE;
	}

	// Build the program executable
	err = clBuildProgram(program, 0, nullptr, nullptr, nullptr, nullptr);
	if (err != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		exit(1);
	}

	// Create the compute kernel
	kernel = clCreateKernel(program, "Calculate_Julia_Set", &err);
	if (!kernel || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		exit(1);
	}

	return CL_SUCCESS;
}