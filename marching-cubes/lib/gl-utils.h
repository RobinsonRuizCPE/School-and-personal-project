// gl-utils.h

#ifndef GL_UTILS_H
#define GL_UTILS_H

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/gl.h>

#define BUFFER_OFFSET(bytes) ((GLubyte*)nullptr + (bytes))
#define PRINT_OPENGL_ERROR() print_opengl_error(__FILE__, __LINE__)

bool print_opengl_error(const char *file, int line);

#endif
