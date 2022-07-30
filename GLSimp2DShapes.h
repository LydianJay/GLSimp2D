#pragma once
#include "GLSimp2DTypes.h"
#include <cmath>
namespace s2d {

	class S2DRect {
	public:

		S2DRect(FLOAT32 x, FLOAT32 y, FLOAT32 w, FLOAT32 h);
		S2DRect();
		Vec4f getRect();
		void setRect(FLOAT32 x, FLOAT32 y, FLOAT32 w, FLOAT32 h);
		void moveRect(FLOAT32 x, FLOAT32 y);
		void setSize(FLOAT32 w, FLOAT32 h);
		void rotateRect(FLOAT32 fAngleDeg);
		const S2DRect& getRotatedRect(FLOAT32 fAngleDeg);
		void scaleRect(FLOAT32 multiplier);
		void setTexture(Texture& texture);
		const Texture& getTexture();
	private:
		FLOAT32 x, y, w, h;
		Texture	m_texture;
	};

}
