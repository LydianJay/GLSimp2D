#pragma once
#include <vector>

/*
* NOTES TO SELF:
*	-This types has been already defined in some libaries... rename this shit or remove
* 
*/

namespace s2d {

	typedef unsigned char			UINT8;
	typedef unsigned short			UINT16;
	typedef unsigned int			UINT32;
	typedef unsigned long long int	UINT64;
	typedef char					INT8;
	typedef short					INT16;
	typedef int						INT32;
	typedef long long int			INT64;
	typedef float					F32;
	typedef double					FLOAT64;
	typedef int						TEXID;

	struct Color {
		UINT8	R;
		UINT8	G;
		UINT8	B;
		UINT8	A;
	};

	struct Glyph {
		Color color;
		unsigned int texIdx;
	};
	
	struct String {
		UINT64 bitIndex;
		Color color;
		float x, y, w, h;
	};

	constexpr Color PXLCLR_BLUE = { 0, 0, 255, 255};
	constexpr Color PXLCLR_RED =	{ 255, 0, 0, 255 };
	constexpr Color PXLCLR_GREEN = { 0, 255, 0, 255 };
	constexpr Color PXLCLR_WHITE = { 255, 255, 255, 255 };
	constexpr Color PXLCLR_BLACK = { 0, 0, 0, 255 };
	struct Point;
	struct Vec2f {
		F32 x, y;
		Vec2f();
		Vec2f(float _x, float _y);
		Vec2f(Point p);
	};

	struct Vec4f {
		F32 x, y, z, w;
	};

	struct VertexCol {
		F32 r, g, b, a;
	};
	struct Vertex {
		Vec2f pos;
		VertexCol col;
		Vec2f textCoord;
		F32 texID;
	};
	struct RectIndexes {
		UINT32	index[6];
	};
	struct TextureContext {
		UINT32* texID;
		UINT32 count;
		UINT32 maxCount;
	};

	struct Texture {
		TEXID texID;
		UINT32 width, height;
		Vec2f texCoords[4];
	};
	

	struct Point {
		F32 x, y;
		Point();
		Point(float _x, float _y);
		Point(Vec2f v);
		void decX(float a);
		void decY(float a);
		void dec(float _x, float _y);
		void incX(float a);
		void incY(float a);
		void inc(float _x, float _y);
	};

	struct MouseState {
		UINT8 mouse;
		UINT8 state;
		Vec2f pos;
	};

	struct KeyState {
		UINT32 key;
		UINT8 state;
	};


	Point operator-(Point a, Point b);
}

 
