#pragma once

#include "GLSimp2DWindow.h"
#include "GLSimp2DLoader.h"
#include "GLSimp2DTypes.h"

namespace s2d {
	
	

	class Simp2DGraphics
	{
	
		

	public:
		TEXID createTexture(unsigned char * pixels, int width, int height);

		void drawRect(FLOAT32 x, FLOAT32 y, FLOAT32 width, FLOAT32 height, PIXCOLOR color = PXLCLR_WHITE, TEXID texture = -1);
		void flushBuffer();


		Simp2DGraphics(UINT32 width, UINT32 height);
		Simp2DGraphics();
		Simp2DGraphics(Simp2DGraphics& copy);
		Simp2DGraphics(Simp2DGraphics&& move);
		Simp2DGraphics(s2d::Simp2DWindow& window);
		~Simp2DGraphics();
		Simp2DGraphics& operator=(Simp2DGraphics&& move) noexcept;

	private:
		UINT32				m_scrWidth, m_scrHeight;
		UINT32				m_maxRectCount;
		UINT32				m_glVertexBufferID;
		UINT32				m_glVertexObjectID;
		UINT32				m_glIndexBufferID;
		UINT32				m_glShaderProgID;
		UINT32				m_glVertexCount;
		UINT32				m_rectExistCount;
		INT32				m_maxTextureSlot;
		INT32				m_glUniformLoc;
		TextureContext		m_textureSlot;
		Rect*				m_rects;
		RectIndexes*		m_indexRect;


		void resizeRectCount();
	};

}


