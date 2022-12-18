#pragma once
#include "GLSimp2DTypes.h"
#include <cmath>

/*
* Defines the Shapes that are supported by this Framework
* 
* 
* 
*/


namespace s2d {

	class S2DRect {
	public:

		S2DRect(F32 x, F32 y, F32 w, F32 h);
		S2DRect();
		/*
		* 
		* @return - returns the position and size
		*/
		Vec4f getRect();
		Vec2f getPos();
		Vec2f getSize();
		void setRect(F32 x, F32 y, F32 w, F32 h);
		void moveRect(F32 x, F32 y);
		void setSize(F32 w, F32 h);
		void scaleRect(F32 multiplier);
		void setTexture(Texture& texture);
		const Texture& getTexture();
	private:
		F32 x, y, w, h;
		Texture	m_texture;
	};

}
