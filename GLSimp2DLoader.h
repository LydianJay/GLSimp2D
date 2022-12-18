#pragma once
#include "Simp2DCommon.h"

/*
	-------------------------------------------------------------------------

	This header defines some openGL declarations and constants
	


	--------------------------------------------------------------------------

*/



#define GL_ARRAY_BUFFER 0x8892
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_STATIC_DRAW 0x88E4
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_COMPILE_STATUS 0x8B81
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_TEXTURE0 0x84C0
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_LINK_STATUS 0x8B82

typedef unsigned int *	GLsizeiptr;
typedef int*			GLintptr;
typedef char			GLchar;


/*
* Loads an openGL function
* @param name - the function name
* @return - returns the address of that function
*/
static void * get_proc_address(const char *name);

/*
* Loads all gl function declared in this header
* @return - returns false if one of them does not load
*/
bool LoadGL();

extern void(__stdcall* glGenBuffers)(GLsizei n, GLuint* buffers);
extern void(__stdcall* glBindBuffer)(GLenum target, GLuint buffer);
extern void(__stdcall* glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
extern void(__stdcall* glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
extern void(__stdcall* glVertexAttribPointer)(GLuint index, GLint size, GLenum	type, GLboolean	normalized, GLsizei	stride, const GLvoid* pointer);
extern void(__stdcall* glEnableVertexAttribArray)(GLuint index);
extern void(__stdcall* glDisableVertexAttribArray)(GLuint index);
extern GLuint(__stdcall* glCreateShader)(GLenum shaderType);
extern void(__stdcall* glShaderSource)(GLuint shader,GLsizei count,const GLchar** string,const GLint* length);
extern void(__stdcall* glCompileShader)(GLuint shader);
extern void(__stdcall* glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
extern void(__stdcall* glGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
extern GLuint(__stdcall* glCreateProgram)();
extern void(__stdcall* glAttachShader)(GLuint program, GLuint shader);
extern void(__stdcall* glLinkProgram)(GLuint program);
extern void(__stdcall* glDeleteShader)(GLuint shader);
extern void(__stdcall* glGenVertexArrays)(GLsizei n, GLuint* arrays);
extern void(__stdcall* glBindVertexArray)(GLuint array);
extern void(__stdcall* glUseProgram)(GLuint program);
extern void(__stdcall* glDeleteBuffers)(GLsizei	n,const GLuint* buffers);
extern void(__stdcall* glUniform1i)(GLint location, GLint v0);
extern GLint(__stdcall* glGetUniformLocation)(GLuint program, const GLchar* name);
extern void(__stdcall* glActiveTexture)(GLenum texture);
extern void(__stdcall* glUniform1iv)(GLint location,GLsizei count,const GLint* value);
extern void(__stdcall* glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
extern BOOL(__stdcall* wglSwapIntervalEXT) (int interval);
extern void (__stdcall* glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);