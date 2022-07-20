#include "GLSimp2DSpriteSheet.h"

s2d::S2DSpriteSheet::S2DSpriteSheet(Texture& texture, UINT32 segmentW, UINT32 segmentH): m_width(texture.width), m_height(texture.height) {
  
    m_texID = texture.texID;
    UINT16 rowCount, columCount;
    rowCount = texture.width / segmentW;
    columCount = texture.height / segmentH;

    m_columnCount = columCount;
    m_rowCount = rowCount;
    m_texHeight = segmentH;
    m_texWidth = segmentW;
    m_texCount = rowCount * columCount;

}

s2d::S2DSpriteSheet::S2DSpriteSheet():m_texID(-1), m_texCount(0) {}

void s2d::S2DSpriteSheet::createSpriteSheet(Texture& texture, UINT32 segmentW, UINT32 segmentH){


    m_width = texture.width;
    m_height = texture.height;
    m_texID = texture.texID;
    UINT16 rowCount, columCount;
    rowCount = texture.width / segmentW;
    columCount = texture.height / segmentH;
    m_columnCount = columCount;
    m_rowCount = rowCount;
    m_texHeight = segmentH;
    m_texWidth = segmentW;
    m_texCount = rowCount * columCount;

}

const s2d::Texture& s2d::S2DSpriteSheet::getTexture(UINT32 row, UINT32 col)
{
    
    if (col >= m_columnCount)col = m_columnCount - 1;
    if (row >= m_rowCount) row = m_rowCount - 1;
    
    col = m_columnCount - col - 1;

    s2d::Texture texture;
    texture.texID = m_texID;
    texture.width = m_texWidth;
    texture.height = m_texHeight;

    float fX = (float)(m_texWidth * (float)row) / (float)m_width,
        fY = (float)(m_texHeight * (float)col) / (float)m_height;

    float fOffsetX = (float)( m_texWidth / (float)m_width),
        fOffsetY = (float)( m_texHeight / (float)m_height);
    texture.texCoords[0] = { fX, fY };
    texture.texCoords[1] = { fX, fY + fOffsetY };
    texture.texCoords[2] = { fX + fOffsetX, fY + fOffsetY };
    texture.texCoords[3] = { fX + fOffsetX, fY };
    return texture;
}
