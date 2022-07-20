#pragma once

/*
					MIT License

	Copyright (c) 2022 LydianJay

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

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


