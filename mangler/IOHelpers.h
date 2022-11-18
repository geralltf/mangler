#ifndef IOHELPERS_H
#define IOHELPERS_H

/// Copy a shader from a plain text file into a character array.
bool ReadShaderSource(const char *file_name, char *shader_str, int max_len);

#endif
