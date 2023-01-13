#include "GLSimp2DWindow.h"


namespace s2d {

	bool S2DWindow::m_bMouseDrag = false;
	COORD S2DWindow::m_WindowDim = { 0,0 };
	S2DWindow* S2DWindow::m_wWindow = nullptr;
	void(*S2DWindow::ButtonOut)(S2DWindow*, int msg) = nullptr;
	void(*S2DWindow::KeyCall)(S2DWindow* window, unsigned char key, unsigned char keystate) = nullptr;
	void(*S2DWindow::MouseCall)(S2DWindow*, float x, float y, unsigned char status, unsigned char mouse) = nullptr;
	void(*S2DWindow::ResizeCall)(int newWidth, int newHeigth) = nullptr;


	S2DWindow::S2DWindow(unsigned int width, unsigned int height, const char* windowname, unsigned long winmode)
	{

		this->m_iHeigth = height;
		this->m_iWidth = width;
		m_WindowDim.X = (SHORT)width;
		m_WindowDim.Y = (SHORT)height;

		HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
		m_wStyle = winmode;

		m_bQuitstate = false;

		m_hInstance = GetModuleHandle(NULL);

		m_wWndcls.hbrBackground = brush;
		m_wWndcls.hInstance = m_hInstance;
		m_wWndcls.lpfnWndProc = this->WndProc;
		m_wWndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_wWndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_wWndcls.lpszClassName = windowname;
		m_wWndcls.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
		RegisterClass(&m_wWndcls);

		this->m_hWinhwn = CreateWindow(windowname, windowname, winmode, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, m_hInstance, nullptr);
		S2DWindow::m_wWindow = this;
		ShowWindow(m_hWinhwn, SW_SHOW);

		

	}
	void S2DWindow::ProcessMessage()
	{

		if (PeekMessage(&m_msg, this->m_hWinhwn, 0, 0, PM_REMOVE)) {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}


	//with blocking
	void S2DWindow::ProcessMessageB() {

		if (GetMessage(&m_msg, m_hWinhwn, 0, 0)) {
			TranslateMessage(&m_msg);
			DispatchMessage(&m_msg);
		}
	}

	void S2DWindow::SetResizeWindowCallFunc(void(*resizecall)(int, int)) { ResizeCall = resizecall; }
	void S2DWindow::SetMouseCallFunc(void(*mcall)(S2DWindow*, float x, float y, unsigned char, unsigned char)) { S2DWindow::MouseCall = mcall; }
	


	S2DWindow::S2DWindow(){}

	S2DWindow::~S2DWindow(){
		this->DeleteOpenGLContext();
	}



	void S2DWindow::CreateOpenGLContext() {
		m_hdc = GetDC(this->m_hWinhwn);

		PIXELFORMATDESCRIPTOR m_pxlform = { 0 };
		int m_iPxlFormat;
		m_pxlform = {
			sizeof(PIXELFORMATDESCRIPTOR),              // Size Of This Pixel Format Descriptor
			1,                                          // Version Number
			PFD_DRAW_TO_WINDOW |                        // Format Must Support CWindow
			PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
			PFD_DOUBLEBUFFER,                           // Must Support Double Buffering
			PFD_TYPE_RGBA,                              // Request An RGBA Format
			32,                                         // Select Our Color Depth
			0, 0, 0, 0, 0, 0,                           // Color Bits Ignored
			8,                                          // No Alpha Buffer
			0,                                          // Shift Bit Ignored
			0,                                          // No Accumulation Buffer
			0, 0, 0, 0,                                 // Accumulation Bits Ignored
			16,                                         // 16Bit Z-Buffer (Depth Buffer)  
			0,                                          // No Stencil Buffer
			0,                                          // No Auxiliary Buffer
			PFD_MAIN_PLANE,                             // Main Drawing Layer
			0,                                          // Reserved
			0, 0, 0                                     // Layer Masks Ignored
		};


		m_iPxlFormat = ChoosePixelFormat(m_hdc, &m_pxlform);
		SetPixelFormat(m_hdc, m_iPxlFormat, &m_pxlform);
		m_hglrc = wglCreateContext(m_hdc);

		wglMakeCurrent(m_hdc, m_hglrc);


	}

	void S2DWindow::CreateOpenGLContext(unsigned int glMajor, unsigned int glMinor, unsigned int glProfile) {
		CreateOpenGLContext();
		wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
		wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");
		DeleteOpenGLContext();
		char windowname[128];
		windowname[127] = 0;
		int width, heigth;
		GetWindowSize(&width, &heigth);
		GetWindowText(m_hWinhwn, windowname, 127);
		DestroyWindow(m_hWinhwn);

		m_bQuitstate = false;

		m_hInstance = GetModuleHandle(NULL);

		//m_wWndcls.hbrBackground = brush;
		m_wWndcls.hInstance = m_hInstance;
		m_wWndcls.lpfnWndProc = this->WndProc;
		m_wWndcls.hCursor = LoadCursor(NULL, IDC_ARROW);
		m_wWndcls.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		m_wWndcls.lpszClassName = windowname;
		m_wWndcls.style = CS_OWNDC;
		RegisterClass(&m_wWndcls);

		this->m_hWinhwn = CreateWindow(windowname, windowname, m_wStyle, CW_USEDEFAULT, CW_USEDEFAULT, width, heigth, nullptr, nullptr, m_hInstance, nullptr);
		S2DWindow::m_wWindow = this;
		ShowWindow(m_hWinhwn, SW_SHOW);

		m_hdc = GetDC(this->m_hWinhwn);


		int pixel_format_attribs[] = {
			WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
			WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
			WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,         32,
			WGL_DEPTH_BITS_ARB,         24,
			WGL_STENCIL_BITS_ARB,       8,
			0
		};
		int pixelformat;
		UINT num_formats;
		wglChoosePixelFormatARB(m_hdc, pixel_format_attribs, 0, 1, &pixelformat, &num_formats);


		PIXELFORMATDESCRIPTOR pfd;
		DescribePixelFormat(m_hdc, pixelformat, sizeof(pfd), &pfd);
		SetPixelFormat(m_hdc, pixelformat, &pfd);


		int glattribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB, glMajor,
			WGL_CONTEXT_MINOR_VERSION_ARB, glMinor,
			WGL_CONTEXT_PROFILE_MASK_ARB,  glProfile,
			0,
		};

		m_hglrc = wglCreateContextAttribsARB(m_hdc, NULL, glattribs);
		wglMakeCurrent(m_hdc, m_hglrc);
	}

	void S2DWindow::DeleteOpenGLContext() {
		wglMakeCurrent(0, 0);
		wglDeleteContext(m_hglrc);
		ReleaseDC(S2DWindow::m_hWinhwn, m_hdc);
	}



	bool S2DWindow::WindowShouldClose(){
		return m_bQuitstate;
	}



	void S2DWindow::SwapWindowBuffers(){
		SwapBuffers(m_hdc);
 	}
	void S2DWindow::SetKeyCallFunc(void(*keycallback)(S2DWindow*, unsigned char, unsigned char)){
		KeyCall = keycallback;
	}





	void S2DWindow::ShowMyWindow(){
		ShowWindow(this->m_hWinhwn, SW_SHOW);
	}

	void S2DWindow::HideWindow(){
		ShowWindow(this->m_hWinhwn, SW_HIDE);
	}

	s2d::KeyState S2DWindow::GetKeyState(){ 
		s2d::KeyState temp = m_keyState;
		m_keyState = { 0, S2D_KEY_NO_INPUT };
		return temp;
	}

	s2d::MouseState S2DWindow::GetMouseState() {
		s2d::MouseState temp = m_mouseState;
		m_mouseState = { S2D_MOUSE_IDLE, S2D_MOUSE_IDLE, {-1.0f, -1.0f} };
		return temp;
	}



	void S2DWindow::GetWindowSize(int* width, int* heigth){
		if (width != nullptr && heigth != nullptr){
			*width = m_iWidth;
			*heigth = m_iHeigth;
		}
	}

	void S2DWindow::SetWindowTitle(const char* title){
		SetWindowText(S2DWindow::m_hWinhwn, title);
	}
	LRESULT CALLBACK S2DWindow::WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {


		switch (msg) {

		case WM_CREATE:

			break;

		case WM_QUIT:
			m_wWindow->m_bQuitstate = true;
			break;

		case WM_SIZE: {
			if (m_wWindow != nullptr) {
				m_wWindow->m_iWidth = LOWORD(lp);
				m_wWindow->m_iHeigth = HIWORD(lp);

				int PrevWidth = m_wWindow->m_WindowDim.X;
				int PrevHeigth = m_wWindow->m_WindowDim.Y;

				float pX = (float)LOWORD(lp) / PrevWidth;
				float pY = (float)HIWORD(lp) / PrevHeigth;
				if (m_wWindow->ResizeCall != nullptr)ResizeCall(m_wWindow->m_iWidth, m_wWindow->m_iHeigth);

			}

		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			m_wWindow->m_bQuitstate = true;
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;



		case WM_KEYDOWN:
			m_wWindow->m_keyState = { (UINT32)wp, S2D_KEY_PRESSED };
			if (m_wWindow->KeyCall != nullptr)
				m_wWindow->KeyCall(m_wWindow, (unsigned char)wp, S2D_KEY_PRESSED);
			break;

		case WM_KEYUP:
			m_wWindow->m_keyState = { (UINT32)wp, S2D_KEY_RELEASED };

			if (m_wWindow->KeyCall != nullptr)
				m_wWindow->KeyCall(m_wWindow, (unsigned char)wp, S2D_KEY_RELEASED);
			break;

		case WM_LBUTTONDOWN: {

			POINT pt = { LOWORD(lp), HIWORD(lp) };
			if (DragDetect(hwnd, pt)) {
				m_wWindow->m_bMouseDrag = true;
			}

			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);

			m_wWindow->m_mouseState = { S2D_MOUSE_LEFT, (UINT8)((m_wWindow->m_bMouseDrag) ? S2D_MOUSE_HELD : S2D_MOUSE_CLICK), {fx, fy} };
			if (MouseCall != nullptr)
			{
				


				if (m_wWindow->m_bMouseDrag)MouseCall(m_wWindow, fx, fy, S2D_MOUSE_HELD, S2D_MOUSE_LEFT);
				else MouseCall(m_wWindow, fx, fy, S2D_MOUSE_CLICK, S2D_MOUSE_LEFT);
			}

		}
		break;

		case WM_RBUTTONDOWN: {

			POINT pt = { LOWORD(lp), HIWORD(lp) };
			if (DragDetect(hwnd, pt))
			{
				m_wWindow->m_bMouseDrag = true;
			}

			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);

			m_wWindow->m_mouseState = { S2D_MOUSE_RIGHT, (UINT8)((m_wWindow->m_bMouseDrag) ? S2D_MOUSE_HELD : S2D_MOUSE_CLICK), {fx, fy} };


			if (MouseCall != nullptr)
			{
				
				if (m_wWindow->m_bMouseDrag)MouseCall(m_wWindow, fx, fy, S2D_MOUSE_HELD, S2D_MOUSE_RIGHT);
				else MouseCall(m_wWindow, fx, fy, S2D_MOUSE_CLICK, S2D_MOUSE_RIGHT);
			}

		}

		break;
		case WM_MOUSEMOVE:
			if (m_wWindow->m_bMouseDrag) {
				POINT pt = { LOWORD(lp), HIWORD(lp) };
				float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
				float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);

				m_wWindow->m_mouseState = { S2D_MOUSE_LEFT | S2D_MOUSE_RIGHT, (UINT8)(S2D_MOUSE_HELD), {fx, fy} };


				if (MouseCall != nullptr)
					MouseCall(m_wWindow, fx, fy, S2D_MOUSE_HELD, S2D_MOUSE_LEFT | S2D_MOUSE_RIGHT);

			}
			break;
		case WM_LBUTTONUP: {
			if (m_wWindow->m_bMouseDrag)m_wWindow->m_bMouseDrag = false;


			
			POINT pt = { LOWORD(lp), HIWORD(lp) };
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);

			m_wWindow->m_mouseState = { S2D_MOUSE_LEFT, (UINT8)(S2D_MOUSE_RELEASED), {fx, fy} };


			if (MouseCall != nullptr) {
			
				MouseCall(m_wWindow, fx, fy, S2D_MOUSE_RELEASED, S2D_MOUSE_LEFT);
			}
		}


		break;

		case WM_RBUTTONUP: {
			if (m_wWindow->m_bMouseDrag)m_wWindow->m_bMouseDrag = false;


			POINT pt = { LOWORD(lp), HIWORD(lp) };
			float fx = pt.x + (20 * (float)pt.x / m_wWindow->m_iWidth);
			float fy = pt.y + (40 * (float)pt.y / m_wWindow->m_iHeigth);

			m_wWindow->m_mouseState = { S2D_MOUSE_RIGHT, (UINT8)(S2D_MOUSE_RELEASED), {fx, fy} };
			if (MouseCall != nullptr) {
				
				MouseCall(m_wWindow, fx, fy, S2D_MOUSE_RELEASED, S2D_MOUSE_RIGHT);
			}
		}

		

		break;
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
			break;
		}

		
			


		return DefWindowProc(hwnd, msg, wp, lp);
	}




}