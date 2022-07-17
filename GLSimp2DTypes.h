#pragma once

namespace s2d {

	typedef unsigned char			UINT8;
	typedef unsigned short			UINT16;
	typedef unsigned int			UINT32;
	typedef unsigned long long int	UINT64;
	typedef char					INT8;
	typedef short					INT16;
	typedef int						INT32;
	typedef long long int			INT64;
	typedef float					FLOAT32;
	typedef double					FLOAT64;

	typedef int						TEXID;

	typedef struct PIXCOLOR {
		UINT8	R;
		UINT8	G;
		UINT8	B;
		UINT8	A;
	};

	constexpr PIXCOLOR PXLCLR_BLUE = { 0, 0, 255, 255};
	constexpr PIXCOLOR PXLCLR_RED =	{ 255, 0, 0, 255 };
	constexpr PIXCOLOR PXLCLR_GREEN = { 0, 255, 0, 255 };
	constexpr PIXCOLOR PXLCLR_WHITE = { 255, 255, 255, 255 };
	constexpr PIXCOLOR PXLCLR_BLACK = { 0, 0, 0, 255 };

	struct Vec2 {
		float x, y;
	};

	struct VertexCol {
		float r, g, b, a;
	};
	struct Vertex {
		Vec2 pos;
		VertexCol col;
		Vec2 textCoord;
		FLOAT32 texID;
	};
	struct Rect {
		Vertex vertex[4];
	};
	struct RectIndexes {
		UINT32	index[6];
	};

	struct TextureContext {
		UINT32* texID;
		UINT32 count;
		UINT32 maxCount;
	};


}


