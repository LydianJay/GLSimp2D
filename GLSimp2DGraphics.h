#pragma once

#include "GLSimp2DWindow.h"
#include "GLSimp2DLoader.h"
#include "GLSimp2DSpriteSheet.h"
#include "GLSimp2DShapes.h"
#include "GLSimp2DTypes.h"


namespace s2d {
	

	

	

	class S2DGraphics
	{
	
		

	public:
		Texture createTexture(unsigned char * pixels, int width, int height);
		void setBlendingState(bool state);
		void drawTriangle(Point p1, Point p2, Point p3, PIXCOLOR color = PXLCLR_WHITE, Texture texture = {-1,0,0});
		void drawRect(FLOAT32 x, FLOAT32 y, FLOAT32 width, FLOAT32 height, PIXCOLOR color = PXLCLR_WHITE, Texture texture = { -1,0,0 });
		void drawRect(S2DRect rect, PIXCOLOR color = PXLCLR_WHITE);
		void flushBuffer();
		void setVSYNC(bool state = true);

		S2DGraphics(UINT32 width, UINT32 height);
		S2DGraphics();
		S2DGraphics(S2DGraphics& copy);
		S2DGraphics(S2DGraphics&& move);
		S2DGraphics(s2d::S2DWindow& window);
		~S2DGraphics();
		S2DGraphics& operator=(S2DGraphics&& move) noexcept;

	private:
		UINT32				m_scrWidth, m_scrHeight;
		UINT32				m_glVertexBufferID;
		UINT32				m_glVertexObjectID;
		UINT32				m_glIndexBufferID;
		UINT32				m_glShaderProgID;
		UINT32				m_vertexCount;
		UINT32				m_maxVertexCount;
		UINT32				m_vertexIndexCount;
		INT32				m_maxTextureSlot;

		INT32				m_glUniformLoc;
		TextureContext		m_textureSlot;
		RectIndexes*		m_indexRect;
		Vertex*				m_vertices;

		void resizeRectCount();
	};


	

}


