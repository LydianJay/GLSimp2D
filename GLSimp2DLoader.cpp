#include "GLSimp2DLoader.h"
GLuint(__stdcall* glCreateShader)(GLenum shaderType) = nullptr;
GLuint(__stdcall* glCreateProgram)() = nullptr;
GLint(__stdcall* glGetUniformLocation)(GLuint program, const GLchar* name) = nullptr;
BOOL(__stdcall* wglSwapIntervalEXT) (int interval) = nullptr;
void(__stdcall* glGenBuffers)(GLsizei n, GLuint* buffers) = nullptr;
void(__stdcall* glBindBuffer)(GLenum target, GLuint buffer) = nullptr;
void(__stdcall* glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage) = nullptr;
void(__stdcall* glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data) = nullptr;
void(__stdcall* glVertexAttribPointer)(GLuint index, GLint size, GLenum	type, GLboolean	normalized, GLsizei	stride, const GLvoid* pointer) = nullptr;
void(__stdcall* glGetShaderInfoLog)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog) = nullptr;
void(__stdcall* glShaderSource)(GLuint shader, GLsizei count, const GLchar** string, const GLint* length) = nullptr;
void(__stdcall* glGetShaderiv)(GLuint shader, GLenum pname, GLint* params) = nullptr;
void(__stdcall* glAttachShader)(GLuint program, GLuint shader) = nullptr;
void(__stdcall* glGenVertexArrays)(GLsizei n, GLuint* arrays) = nullptr;
void(__stdcall* glEnableVertexAttribArray)(GLuint index) = nullptr;
void(__stdcall* glDisableVertexAttribArray)(GLuint index) = nullptr;
void(__stdcall* glCompileShader)(GLuint shader) = nullptr;
void(__stdcall* glLinkProgram)(GLuint program) = nullptr;
void(__stdcall* glDeleteShader)(GLuint shader) = nullptr;
void(__stdcall* glBindVertexArray)(GLuint array) = nullptr;
void(__stdcall* glUseProgram)(GLuint program) = nullptr;
void(__stdcall* glDeleteBuffers)(GLsizei n, const GLuint* buffers) = nullptr;
void(__stdcall* glUniform1i)(GLint location, GLint v0) = nullptr;
void(__stdcall* glActiveTexture)(GLenum texture) = nullptr;
void(__stdcall* glUniform1iv)(GLint location, GLsizei count, const GLint* value) = nullptr;
void(__stdcall* glGetProgramiv)(GLuint program, GLenum pname, GLint* params) = nullptr;
void(__stdcall* glGetProgramInfoLog) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog) = nullptr;
static void * get_proc_address(const char *name)
{
	static HMODULE image = GetModuleHandleA("opengl32.dll");
	
	if (image == NULL) { return nullptr; }

	void * Func = wglGetProcAddress(reinterpret_cast<LPCSTR>(name));
	if(!Func) { return nullptr; }

	return Func;
	
}



bool LoadGL()
{
	glGenBuffers = reinterpret_cast<void(_stdcall *)(GLsizei, GLuint * )>(get_proc_address("glGenBuffers"));
	if (glGenBuffers == nullptr)return false;
	glBindBuffer = reinterpret_cast<void(_stdcall *)(GLenum, GLuint)>(get_proc_address("glBindBuffer"));
	if (!glBindBuffer)return false;
	glBufferData = reinterpret_cast<void(__stdcall *)(GLenum, GLsizeiptr, const void* , GLenum)>(get_proc_address("glBufferData"));
	if (!glBufferData)return false;
	bool result[23];
	result[0] = glBufferSubData =				(void(__stdcall*)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data))get_proc_address("glBufferSubData");
	result[1] = glVertexAttribPointer =			(void(__stdcall*)(GLuint index, GLint size, GLenum	type, GLboolean	normalized, GLsizei	stride, const GLvoid * pointer))get_proc_address("glVertexAttribPointer");
	result[2] = glGetShaderInfoLog =			(void(__stdcall*)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog))get_proc_address("glGetShaderInfoLog");
	result[3] = glShaderSource =				(void(__stdcall*)(GLuint shader, GLsizei count, const GLchar * *string, const GLint * length))get_proc_address("glShaderSource");
	result[4] = glGetShaderiv =					(void(__stdcall*)(GLuint shader, GLenum pname, GLint * params))get_proc_address("glGetShaderiv");
	result[5] = glAttachShader =				(void(__stdcall*)(GLuint program, GLuint shader))get_proc_address("glAttachShader");
	result[6] = glGenVertexArrays =				(void(__stdcall*)(GLsizei n, GLuint * arrays))get_proc_address("glGenVertexArrays");
	result[7] = glEnableVertexAttribArray =		(void(__stdcall*)(GLuint index))get_proc_address("glEnableVertexAttribArray");
	result[8] = glDisableVertexAttribArray =	(void(__stdcall*)(GLuint index))get_proc_address("glDisableVertexAttribArray");
	result[9] = glCreateShader =				(GLuint(__stdcall*)(GLenum shaderType))get_proc_address("glCreateShader");
	result[10] = glCompileShader =				(void(__stdcall*)(GLuint shader))get_proc_address("glCompileShader");
	result[11] = glCreateProgram =				(GLuint(__stdcall*)())get_proc_address("glCreateProgram");
	result[12] = glLinkProgram =				(void(__stdcall*)(GLuint program))get_proc_address("glLinkProgram");
	result[13] = glDeleteShader =				(void(__stdcall*)(GLuint shader))get_proc_address("glDeleteShader");
	result[14] = glBindVertexArray =			(void(__stdcall*)(GLuint array))get_proc_address("glBindVertexArray");
	result[15] = glUseProgram =					(void(__stdcall*)(GLuint program))get_proc_address("glUseProgram");
	result[16] = glDeleteBuffers =				(void(__stdcall*)(GLsizei n, const GLuint * buffers))get_proc_address("glDeleteBuffers");
	result[17] = glGetUniformLocation =			(GLint(__stdcall*)(GLuint program, const GLchar * name))get_proc_address("glGetUniformLocation");
	result[18] = glUniform1i =					(void(__stdcall*)(GLint location, GLint v0))get_proc_address("glUniform1i");
	result[19] = glActiveTexture =				(void(__stdcall*)(GLenum texture))get_proc_address("glActiveTexture");
	result[20] = glUniform1iv =					(void(__stdcall*)(GLint location, GLsizei count, const GLint * value))get_proc_address("glUniform1iv");
	result[21] = glGetProgramiv =				(void(__stdcall*)(GLuint program, GLenum pname, GLint * params))get_proc_address("glGetProgramiv");
	result[22] = glGetProgramInfoLog =			(void(__stdcall*) (GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog))get_proc_address("glGetProgramInfoLog");
	for (int i = 0; i < 23; i++) {
		if (result[i] == false)return false;
	}

	wglSwapIntervalEXT = (BOOL (__stdcall*) (int interval))wglGetProcAddress("wglSwapIntervalEXT");

	

	return true;
}