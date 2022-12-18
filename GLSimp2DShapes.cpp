#include "GLSimp2DShapes.h"



s2d::S2DRect::S2DRect(F32 x, F32 y, F32 w, F32 h) :x(x), y(y), w(w), h(h){
	m_texture.texID = -1;
}

s2d::S2DRect::S2DRect(){
	m_texture.texID = -1;
}

s2d::Vec4f s2d::S2DRect::getRect(){
	return {x,y,w,h};
}

s2d::Vec2f s2d::S2DRect::getPos()
{
	return {x,y};
}

s2d::Vec2f s2d::S2DRect::getSize() {
	return {w,h};
}

void s2d::S2DRect::setRect(F32 x, F32 y, F32 w, F32 h) {
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
}

void s2d::S2DRect::moveRect(F32 x, F32 y){
	this->x += x;
	this->y += y;
}

void s2d::S2DRect::setSize(F32 w, F32 h){
	this->w = w;
	this->h = h;
}

void s2d::S2DRect::scaleRect(F32 multiplier) {
	w *= multiplier;
	h *= multiplier;
}

void s2d::S2DRect::setTexture(Texture& texture) {
	m_texture = texture;
}

const s2d::Texture& s2d::S2DRect::getTexture(){
	return m_texture;
}
