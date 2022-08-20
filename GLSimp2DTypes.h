#pragma once


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

	struct S2D_COLOR {
		UINT8	R;
		UINT8	G;
		UINT8	B;
		UINT8	A;
	};

	constexpr S2D_COLOR PXLCLR_BLUE = { 0, 0, 255, 255};
	constexpr S2D_COLOR PXLCLR_RED =	{ 255, 0, 0, 255 };
	constexpr S2D_COLOR PXLCLR_GREEN = { 0, 255, 0, 255 };
	constexpr S2D_COLOR PXLCLR_WHITE = { 255, 255, 255, 255 };
	constexpr S2D_COLOR PXLCLR_BLACK = { 0, 0, 0, 255 };

	struct Vec2f {
		F32 x, y;
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



}


