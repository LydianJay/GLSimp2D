#include "GLSimp2DShapes.h"

s2d::S2DRect::S2DRect(FLOAT32 x, FLOAT32 y, FLOAT32 w, FLOAT32 h) :x(x), y(y), w(w), h(h){
	m_texture.texID = -1;
}

s2d::S2DRect::S2DRect(){
	m_texture.texID = -1;
}

s2d::Vec4f s2d::S2DRect::getRect(){
	return {x,y,w,h};
}

void s2d::S2DRect::setRect(FLOAT32 x, FLOAT32 y, FLOAT32 w, FLOAT32 h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void s2d::S2DRect::moveRect(FLOAT32 x, FLOAT32 y){
	this->x += x;
	this->y += y;
}

void s2d::S2DRect::setSize(FLOAT32 w, FLOAT32 h){
	this->w = w;
	this->h = h;
}

void s2d::S2DRect::rotateRect(FLOAT32 fAngleDeg){

	
	fAngleDeg = fAngleDeg * 3.14156f/180.0f;

	x = x + cosf(fAngleDeg);
	y = y + sinf(fAngleDeg);
}

const s2d::S2DRect& s2d::S2DRect::getRotatedRect(FLOAT32 fAngleDeg)
{
	S2DRect rect = *this;

	rect.x = x + cosf(fAngleDeg);
	rect.y = y + sinf(fAngleDeg);
	return rect;
}

void s2d::S2DRect::scaleRect(FLOAT32 multiplier) {
	w *= multiplier;
	h *= multiplier;
}

void s2d::S2DRect::setTexture(Texture& texture) {
	m_texture = texture;
}

const s2d::Texture& s2d::S2DRect::getTexture(){
	return m_texture;
}
