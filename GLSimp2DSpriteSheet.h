#pragma once
#include "GLSimp2DTypes.h"


namespace s2d {

	

	class S2DSpriteSheet {

	public:
		S2DSpriteSheet(Texture& texture, UINT32 segmentW, UINT32 segmentH);
		S2DSpriteSheet();

		void createSpriteSheet(Texture& texture, UINT32 segmentW, UINT32 segmentH);
		/*
		* Get the textue at specified row and colomn
		* @param row - the row
		* @param col - the column
		* @param isFlipped - specifies if the texture should be flipped vertically
		* @return - the texture
		*/
		const s2d::Texture& getTexture(UINT32 row, UINT32 col, bool isFlipped = false);
		const s2d::Texture& getTexture(UINT32 index, bool isFlipped = false);
		UINT16				getRowCount();
		UINT16				getColCount();
	private:

		s2d::TEXID m_texID;
		UINT32 m_width, m_height;
		float m_texWidth, m_texHeight;
		UINT32 m_texCount;
		UINT16	m_rowCount, m_columnCount;
		

	};
}



