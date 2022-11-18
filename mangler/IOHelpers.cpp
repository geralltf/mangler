#include "IOHelpers.h"

#include <cstdio>

/// Copy a shader from a plain text file into a character array.
bool ReadShaderSource(const char *file_name, char *shader_str, int max_len)
{
	FILE *file = fopen(file_name, "r");
	if (!file)
	{
		//gl_log_err("ERROR: opening file for reading: %s\n", file_name);
		return false;
	}
	size_t cnt = fread(shader_str, 1, max_len - 1, file);
	if ((int)cnt >= max_len - 1)
	{
		// WARNING: file is too big - truncated.
	}
	if (ferror(file))
	{
		// ERROR: reading shader file.
		fclose(file);
		return false;
	}

	shader_str[cnt] = 0; // append \0 to end of file string
	fclose(file);

	return true;
}