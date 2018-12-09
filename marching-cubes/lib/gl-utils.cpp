// gl-utils.cc

#include <iostream>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/gl.h>

#include "gl-utils.h"

/*****************************************************************************\
 * print_opengl_error                                                        *
\*****************************************************************************/
bool print_opengl_error(const char *file, int line)
{
  /* Returns true if an OpenGL error occurred, false otherwise. */
  GLenum error;
  bool   ret_code = false;

  error = glGetError();
  while (error != GL_NO_ERROR)
  {
    std::cerr << "glError in file " << file << ", line " << line << ": ";
    switch (error)
    {
      case GL_INVALID_ENUM: std::cerr << "GL_INVALID_ENUM"; break;
      case GL_INVALID_VALUE: std::cerr << "GL_INVALID_VALUE"; break;
      case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
      case GL_OUT_OF_MEMORY: std::cerr << "GL_OUT_OF_MEMORY"; break;
      case GL_STACK_UNDERFLOW: std::cerr << "GL_STACK_UNDERFLOW"; break;
      case GL_STACK_OVERFLOW: std::cerr << "GL_STACK_OVERFLOW"; break;
    }
    std::cerr << std::endl;
    ret_code = true;
    error = glGetError();
  }
  return ret_code;
}

