#include "Simp2DCommon.h"
#include "GLSimp2DTypes.h"
#ifndef RENDER_SYS_H
#define RENDER_SYS_H





#define S2D_MOUSE_CLICK		0b00000000
#define S2D_MOUSE_HELD		0b00000001
#define S2D_MOUSE_RELEASED	0b00000010
#define S2D_MOUSE_IDLE		0b00000011

#define S2D_MOUSE_LEFT		0b00000000
#define S2D_MOUSE_RIGHT		0b00000100

#define S2D_KEY_PRESSED		0b00001000
#define S2D_KEY_RELEASED	0b00010000
#define S2D_KEY_NO_INPUT	0b00000000

#define S2D_NO_RESIZE WS_VISIBLE | WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU
#define S2D_DEF_WINDOW WS_VISIBLE | WS_OVERLAPPEDWINDOW

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int *attribList);
typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
static wglChoosePixelFormatARB_type * wglChoosePixelFormatARB;
static wglCreateContextAttribsARB_type * wglCreateContextAttribsARB;
#define S2D_GL_CONTEXT_CORE_PROFILE				0x00000001
#define S2D_GL_CONTEXT_COMPAT_PROFILE			0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB			0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB			0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_DRAW_TO_WINDOW_ARB					0x2001
#define WGL_ACCELERATION_ARB					0x2003
#define WGL_SUPPORT_OPENGL_ARB					0x2010
#define WGL_DOUBLE_BUFFER_ARB					0x2011
#define WGL_PIXEL_TYPE_ARB						0x2013
#define WGL_COLOR_BITS_ARB						0x2014
#define WGL_DEPTH_BITS_ARB						0x2022
#define WGL_STENCIL_BITS_ARB					0x2023
#define WGL_FULL_ACCELERATION_ARB				0x2027
#define WGL_TYPE_RGBA_ARB						0x202B


namespace s2d {

	class S2DWindow{
	

	public:

		S2DWindow(unsigned int width, unsigned int height, const char* windowname, unsigned long winmode);
		S2DWindow();
		~S2DWindow();
		void ProcessMessage();
		void ProcessMessageB();
		bool WindowShouldClose();
		void SwapWindowBuffers();
		void SetKeyCallFunc(void(*keycallback)(S2DWindow*, unsigned char, unsigned char));
		void SetResizeWindowCallFunc(void(*resizecall)(int, int));
		void SetMouseCallFunc(void(*mcall) (S2DWindow*, float x, float y, unsigned char, unsigned char));
		void SetWindowTitle(const char* title);
		void GetWindowSize(int*, int*);
		void ShowMyWindow();
		void HideWindow();
		s2d::KeyState GetKeyState();
		s2d::MouseState GetMouseState();
		/*----------for openGL-----------*/
		void CreateOpenGLContext();
		void CreateOpenGLContext(unsigned int glMajor, unsigned int glMinor, unsigned int glProfile);
		void DeleteOpenGLContext();
		/*-------------------------------*/

	private:
		unsigned long			m_wStyle = 0;
		unsigned int			m_iWidth, m_iHeigth;
		bool					m_bQuitstate = false;
		HINSTANCE				m_hInstance = nullptr;
		WNDCLASS				m_wWndcls = {};
		HWND					m_hWinhwn = nullptr;
		HDC						m_hdc;
		HGLRC					m_hglrc;
		MSG						m_msg;
		s2d::MouseState			m_mouseState;
		s2d::KeyState			m_keyState;
		static S2DWindow*		m_wWindow;
		static bool				m_bMouseDrag;
		static COORD			m_WindowDim;
	private:
		static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
		static void(*KeyCall)(S2DWindow* window, unsigned char key, unsigned char keystate);
		static void(*MouseCall)(S2DWindow*, float x, float y, unsigned char status, unsigned char mouse);
		static void(*ButtonOut)(S2DWindow*, int msg);
		static void(*ResizeCall)(int newWidth, int newHeigth);

	};
}







#endif