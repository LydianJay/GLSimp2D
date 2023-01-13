
/*
*
*	MIT License
*
*	Copyright (c) 2022 LydianJay ( Lloyd Jay Arpilleda Edradan )
*
*	Permission is hereby granted, free of charge, to any person obtaining a copy
*	of this software and associated documentation files (the "Software"), to deal
*	in the Software without restriction, including without limitation the rights
*	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*	copies of the Software, and to permit persons to whom the Software is
*	furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included in all
*	copies or substantial portions of the Software.
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*	SOFTWARE.
*
*/




#pragma once

#include "GLSimp2DWindow.h"
#include "GLSimp2DLoader.h"
#include "GLSimp2DSpriteSheet.h"
#include "GLSimp2DShapes.h"
#include "GLSimp2DTypes.h"



/*
* -------------------------------------------------------------------------------------
* NOTES TO SELF/TO DO LIST: 
*	-[ADDITION]				Need to implement a drawCircle (hooray cos/sin stuff again!)
*	-[ADDITION]				Add the capability to draw String/characters/glyps, TrueType font
*	-[DOCUMENTATION]		This project need some proper documentation
*	-[ADDITION]				Add a bitmap Loader (S2DBitMap - GLSimp2DEXTBitMap)
*	-[NEED_TO_FIX/ADDITION]	No implentation of handling when textures slots are all used - this might be quite a big issue
*	-[NEED_TO_FIX]			The implementation of drawTriangle sucks as it basically draws two triangles...
*	-[OPTIMIZE]				The IBO only accomodates rects thats why drawTriangle draws two triangles...
*	-[TEST/ADDITION]		Make test program to test some ligthing effects... or some shit like that
*	-[MODIFICATION]			Maybe make the texture its own class instead of a struct
*	-[OTHER]				Maybe make this a DLL or a static libraries?... Probably when a lot of things have been added
* -------------------------------------------------------------------------------------
*/


namespace s2d {
	/*
	* The graphics renderer
	* 
	*/
	class S2DGraphics
	{
	
		

	public:

		/*
		* Attach a texture to a texture slot in opengl
		* @param pixels the pixel buffer which contains an RGB/A format
		* @param width the width of the texture
		* @param height the height of the texture
		* @return texture slot/id for that texture
		* 
		* Note that there are limited texture slot in openGL
		*/
		Texture createTexture(unsigned char * pixels, int width, int height);

		/*
		* Set blending state
		* @param state the blending state requested
		* @return none
		*/
		void setBlendingState(bool state);

		/*
		* Draws triange specified at 3 points
		* @param Point p1, p2 ,p3 - the 3 vertex which triangle will be drawn
		* @param color - the color of the triangle. Defaults to WHITE
		* this can also be used to tint when textures are applied in this triangle
		* @param Texture - the texture for the triangle. Leave it to default value if no textures are intended
		* @return - none
		* 
		*/
		void drawTriangle(Point p1, Point p2, Point p3, S2D_COLOR color = PXLCLR_WHITE);
		/*
		* Draws a rectangle with the specified coordinates, size, color, and texture
		* @param x, y - the coordinates in space where top left corner is 0,0
		* @param width, height - width and height of the rectangle
		* @param color - the color of the rectangle. Can be used to tint when texture is applied
		
		* @return - none
		*/
		void drawRect(F32 x, F32 y, F32 width, F32 height, S2D_COLOR color = PXLCLR_WHITE, Texture texture = { -1,0,0 });
		/*
		* Draws a rectangle with the specified coordinates, size, color, and texture
		* @param rect - A rect structure that defines the coordinate,size, and texture
		* @param color - the color of the rectangle. Can be used to tint when texture is applied
		* @return - none
		*/
		void drawRect(S2DRect rect, S2D_COLOR color = PXLCLR_WHITE);
		/*
		* Draws a rectangle with the specified coordinates, size, color, rotation, and texture
		* @param x, y - the coordinates in space where top left corner is 0,0
		* @param width, height - width and height of the rectangle
		* @param fAngle - the angle which the rectangle will be rotated
		* @param color - the color of the rectangle. Can be used to tint when texture is applied
		* @param texture - the texture for the rectangle. Leave to default value when no texture are applied
		* @return - none
		*/
		void drawRotatedRect(F32 x, F32 y, F32 width, F32 height, float fAngle, S2D_COLOR color = PXLCLR_WHITE, Texture texture = { -1,0,0 });
		/*
		* Draws a rectangle with the specified coordinates, size, color, and texture
		* @param rect - A rect structure that defines the coordinate,size, and texture
		* @param fAngle - the angle of rotation
		* @param color - the color of the rectangle. Can be used to tint when texture is applied(set to white by default)
		* @return - none
		*/
		void drawRotatedRect(S2DRect rect, float fAnge, S2D_COLOR color = PXLCLR_WHITE);
		
		/*
		* Draws a wireframe/unfilled circle in a specified location and radius.
		* The parameter nSegmentCount cannot be less than 4. It will be automatically sets to 6 if less than 4 is inputted.
		* The default segment count is 16
		* 
		* @param p1 - the position
		* @param fRadius - the radius
		* @param lw - the thickness of the line
		* @color - the color(set to white by default)
		* @nSegment - the number of segments the circle will composed of
		*/
		void drawWireFrameCircle(Point p1, float fRadius, float lw, S2D_COLOR color = PXLCLR_WHITE, uint32_t nSegmentCount = 16);
		
		/*
		* Draws a filled circle in a specified location and radius.
		* The parameter nSegmentCount cannot be less than 4. It will be automatically sets to 6 if less than 4 is inputted.
		* The default segment count is 16
		*
		* @param p1 - the position
		* @param fRadius - the radius
		* @color - the color(set to white by default)
		* @nSegment - the number of segments the circle will composed of
		*/
		void drawCircle(Point p1, float fRadius, S2D_COLOR color = PXLCLR_WHITE, uint32_t nSegmentCount = 16);
		/*
		* Draws a line specified by two points
		* @param x1, y1 - The first point location
		* @param x2, y2 - The 2nd point location
		* @param w - the width/thickness of the line
		* @param color - the color of the line(set to white by default)
		*/
		void drawLine(float x1, float y1, float x2, float y2, float w, S2D_COLOR color = PXLCLR_WHITE);
		
		/*
		* Draws a line specified by two points
		* @param p1 - The first point location
		* @param p2 - The 2nd point location
		* @param w - the width/thickness of the line
		* @param color - the color of the line(set to white by default)
		*/
		void drawLine(Point p1, Point p2, float w, S2D_COLOR color = PXLCLR_WHITE);
		
		/*
		* Draws a wireframe/unfilled rectangle
		* @param pos - position of the rectangle
		* @param sz - the width and height
		* @param lw - the width/thickness of the lines
		* @param color - the color(set to white by default)
		*/
		void drawWireFrameRect(Vec2f pos, Vec2f sz, float lw = 1, S2D_COLOR color = PXLCLR_WHITE);
		/*
		* Draws a wireframe/unfilled rectangle
		* @param pos - position of the rectangle
		* @param sz - the width and height
		* @param lw - the width/thickness of the lines
		* @param fAngle - the angle of rotation (set to zero by default)
		* @param color - the color (set to white by default)
		*/
		void drawWireFrameRect(Vec2f pos, Vec2f sz, float lw = 1, float fAngle = 0, S2D_COLOR color = PXLCLR_WHITE);
		/*
		* Draws a triangle in specified by 3 points
		* @param p1,p2,p3 - the three points/vertex of the triangle
		* @param lw - the width/thickness of the line
		* @param color - the color of the line default to white
		* @return none
		*/
		void drawWireFrameTriangle(Point p1, Point p2, Point p3, float lw = 1, S2D_COLOR color = PXLCLR_WHITE);
		/*void drawWireFrameTriangle(Point p1, Point p2, Point p3, float lw = 1, float fAngle = 0, S2D_COLOR color = PXLCLR_WHITE);*/


		/*
		* This is the actual draw call. This should be called once every frame as this flushes all
		* draw*() stuff on the screen
		* @param - none
		* @return - none
		*/
		void flushBuffer();
		/*
		* Sets the vsync states
		* @param state - the state of the vsync
		*/
		void setVSYNC(bool state = true);

		/*
		* This reserves memory so reallocation may prevented
		* Its a good idea to call this at initialization so the API does not reallocates every
		* time the buffer has been exceeded
		* @param count - the count of how many rectangle might be drawn to the screen
		* @return none
		* 
		*/
		void reservedMaxRect(uint32_t count);


		S2DGraphics(UINT32 width, UINT32 height);
		S2DGraphics();
		S2DGraphics(S2DGraphics& copy);
		S2DGraphics(S2DGraphics&& move) noexcept;
		S2DGraphics(s2d::S2DWindow& window);
		~S2DGraphics();
		S2DGraphics& operator=(S2DGraphics&& move) noexcept;

	private:
		UINT32				m_scrWidth, m_scrHeight;				// screen width and heigth
		UINT32				m_glVertexBufferID;						// The opengGL Vertex BufferID
		UINT32				m_glVertexObjectID;
		UINT32				m_glIndexBufferID;
		UINT32				m_glShaderProgID;						// shader
		UINT32				m_vertexCount;
		UINT32				m_maxVertexCount;						
		UINT32				m_vertexIndexCount;
		INT32				m_maxTextureSlot;						// currently useless

		INT32				m_glUniformLoc;							
		TextureContext		m_textureSlot;
		RectIndexes*		m_indexRect;							// Index Buffer
		Vertex*				m_vertices;								// Vertex Buffer

		
		s2d::Vec2f normalizePoint(s2d::Vec2f point);


		/*
		* Rotates a point in screen
		* @param point - a point in screen specified by x and y
		* @param m - the mid point
		* @fAngle - the rotation angle
		* @return none
		* 
		*/
		static s2d::Vec2f rotatePoint(s2d::Vec2f point, s2d::Vec2f m, float fAngle);
		
	};


	

}


