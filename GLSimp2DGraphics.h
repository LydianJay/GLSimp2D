#pragma once
#include "GLSimp2DLoader.h"
//#include "Simp2DCommon.h"



namespace s2d {
	
	

	class Simp2DGraphics
	{
	
		

	public:
		
		void drawRect(FLOAT32 x, FLOAT32 y, FLOAT32 width, FLOAT32 height, PIXCOLOR color = PXLCLR_WHITE);
		void flushBuffer();


		Simp2DGraphics(UINT32 width, UINT32 height);
		Simp2DGraphics();
		~Simp2DGraphics();

	private:
		UINT32				m_scrWidth, m_scrHeight;
		UINT32				m_maxRectCount;
		UINT32				m_glVertexBufferID;
		UINT32				m_glVertexObjectID;
		UINT32				m_glIndexBufferID;
		UINT32				m_glShaderProgID;
		UINT32				m_glVertexCount;
		UINT32				m_rectExistCount;
		Rect* m_rects;
		RectIndexes* m_indexRect;


		void resizeRectCount();
	};

}


