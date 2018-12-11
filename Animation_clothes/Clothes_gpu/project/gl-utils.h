// gl-utils.h

#ifndef GL_UTILS_H
#define GL_UTILS_H

#include <vector>
#include <string>

#define GLEW_STATIC 1
#include <GL/glew.h>
#include <GL/gl.h>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))
#define PRINT_OPENGL_ERROR() print_opengl_error(__FILE__, __LINE__)

bool print_opengl_error (const char *file, int line);

void check_framebuffer_status (void);

void get_gl_version (int *major, int *minor);

GLint get_uni_loc (GLuint program, const GLchar *name);

GLuint read_shader (const std::string &vertex_filename,
                    const std::string &fragment_filename);

#endif
