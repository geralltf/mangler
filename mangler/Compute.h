#ifndef COMPUTE_H
#define COMPUTE_H

#include <cstdlib>
#include <CL/cl.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

class Compute
{
public:

	size_t global;                      // global domain size for our calculation
	size_t local;                       // local domain size for our calculation
	size_t count;



	cl_device_id device_id;             // compute device id
	cl_context context;                 // compute context
	cl_command_queue commands;          // compute command queue
	cl_program program;                 // compute program
	cl_kernel kernel;                   // compute kernel

	cl_mem input;                       // device memory used for the input array
	cl_mem output;                      // device memory used for the output array

	const char *KernelSource = "\n" \
		"__kernel void Calculate_Julia_Set(                                                       \n" \
		"   __global float* input,                                              \n" \
		"   __global float* output,                                             \n" \
		"   const unsigned int count, int pixel_shift_x, int pixel_shift_y, int precision, float zoom, int x, int y)                                           \n" \
		"{                                                                      \n" \
		"   int i = get_global_id(0);                                           \n" \
		"   int j = i % 1280; int k = i / 1280;                                 \n" \
		"   float c_real = ((float)x) / zoom  - pixel_shift_x / 1280.0f;                    \n" \
		"   float c_imag = ((float)y) / zoom  - pixel_shift_y / 720.0f;                    \n" \
		"   float z_real = ((float)j) / zoom  - pixel_shift_x / 1280.0f;                                               \n" \
		"   float z_imag = ((float)k) / zoom  - pixel_shift_y / 720.0f; int iterations = 0;                             \n" \
		"   if(i < count)                                                       \n" \
		"   for (int l = 0; l < precision; l++)                                 \n" \
		"   {                                                                   \n" \
		"       float z1_real = z_real * z_real - z_imag * z_imag; float z1_imag = 2 * z_real * z_imag; \n" \
		"       z_real = z1_real + c_real; z_imag = z1_imag + c_imag; iterations++;                                 \n" \
		"       if (z_real * z_real + z_imag * z_imag > 4) { break; }                                \n" \
		"   }                                                                   \n" \
		"   output[i] = iterations;                                             \n" \
		"}                                                                      \n" \
		"\n";

public:

	stringstream* LoadKernelFromFile(string fileName);

	int ExecuteKernelRead(float* buffer);

	void ExecuteKernelWait();

	int ExecuteKernel();

	void ReadBuffer(void* buffer);

	int CreateCLContext();

};

#endif