#pragma once


#if defined (_WIN32) && defined(__cplusplus)
	#if defined (_MSC_VER)
		#pragma comment(lib, "opengl32.lib")
	#else
		#warning "Please link opengl libriaries "opengl32.lib" 
	#endif // _MSC_VER
#else
	#error "For C++ Windows only!!!"
#endif

#include "glfunctions.h"
#include "GLSimp2DTypes.h"
#include <string>

namespace s2d {

	class Simp2D
	{
	private:
		UINT32				m_scrWidth, m_scrHeight;
		UINT32				m_pixelRow, m_pixelCol;
		FLOAT32				m_fPixelW, m_fPixelH;
		PIXCOLOR*			m_pixelBuffer;
		UINT32				m_glVertexBufferID;
		UINT32				m_glVertexObjectID;
		UINT32				m_glShaderProgID;
		UINT32				m_glVertexCount;
		Rect*				m_rects;
	public:
		
		
		void drawRect(UINT32 x, UINT32 y, UINT32 width, UINT32 height, PIXCOLOR color = PXLCLR_WHITE);
		void fillRect(UINT32 x, UINT32 y, UINT32 width, UINT32 height, PIXCOLOR color = PXLCLR_WHITE);
		void drawTriangle(UINT32 p1, UINT32 p2, UINT32 p3, PIXCOLOR color = PXLCLR_WHITE);
		void fillTriangle(UINT32 p1, UINT32 p2, UINT32 p3, PIXCOLOR color = PXLCLR_WHITE);
		void drawCircle(UINT32 origX, UINT32 origY, FLOAT32 fRadius, PIXCOLOR color = PXLCLR_WHITE);
		void fillCircle(UINT32 origX, UINT32 origY, FLOAT32 fRadius, PIXCOLOR color = PXLCLR_WHITE);
		void flushBuffer();


		Simp2D(UINT32 width, UINT32 height, UINT32 pixelRow, UINT32 pixelCol);
		~Simp2D();
	};

}


