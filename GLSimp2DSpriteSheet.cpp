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

const s2d::Texture& s2d::S2DSpriteSheet::getTexture(UINT32 row, UINT32 col,bool isFlipped)
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
   
    if (isFlipped) {

        // fliped
        texture.texCoords[1] = { fX, fY };                          // top left
        texture.texCoords[0] = { fX, fY + fOffsetY };               // bot left
        texture.texCoords[3] = { fX + fOffsetX, fY + fOffsetY };    // bot right
        texture.texCoords[2] = { fX + fOffsetX, fY };               // top rigth

    }

   //else if (isUpsideDown) {
   //    texture.texCoords[0] = { fX + fOffsetX, fY + fOffsetY };
   //    texture.texCoords[1] = { fX + fOffsetX, fY };
   //    texture.texCoords[2] = { fX, fY };
   //    texture.texCoords[3] = { fX, fY + fOffsetY };
   //}


    else {
        texture.texCoords[0] = { fX, fY };
        texture.texCoords[1] = { fX, fY + fOffsetY };
        texture.texCoords[2] = { fX + fOffsetX, fY + fOffsetY };
        texture.texCoords[3] = { fX + fOffsetX, fY };
    }


   
   
    return texture;
}

const s2d::Texture& s2d::S2DSpriteSheet::getTexture(UINT32 index, bool isFlipped) {

    // col = t - row / maxRow
    // row = t mod maxRow
    if (index >= m_texCount)index = m_texCount - 1;
    
    unsigned int row = (index % m_rowCount), col;
    col = (index - row) / m_rowCount;


    if (col >= m_columnCount)col = m_columnCount - 1;
    if (row >= m_rowCount) row = m_rowCount - 1;

    col = m_columnCount - col - 1;

    s2d::Texture texture;
    texture.texID = m_texID;
    texture.width = m_texWidth;
    texture.height = m_texHeight;

    float fX = (float)(m_texWidth * (float)row) / (float)m_width,
        fY = (float)(m_texHeight * (float)col) / (float)m_height;

    float fOffsetX = (float)(m_texWidth / (float)m_width),
        fOffsetY = (float)(m_texHeight / (float)m_height);
    
    if (isFlipped) {

        // fliped
        texture.texCoords[1] = { fX, fY };                          // top left
        texture.texCoords[0] = { fX, fY + fOffsetY };               // bot left
        texture.texCoords[3] = { fX + fOffsetX, fY + fOffsetY };    // bot right
        texture.texCoords[2] = { fX + fOffsetX, fY };               // top rigth

    }
    
    //else if (isUpsideDown) {
    //    texture.texCoords[0] = { fX + fOffsetX, fY + fOffsetY };
    //    texture.texCoords[1] = { fX + fOffsetX, fY };
    //    texture.texCoords[2] = { fX, fY };
    //    texture.texCoords[3] = { fX, fY + fOffsetY };
    //}


    else {
        texture.texCoords[0] = { fX, fY };
        texture.texCoords[1] = { fX, fY + fOffsetY };
        texture.texCoords[2] = { fX + fOffsetX, fY + fOffsetY };
        texture.texCoords[3] = { fX + fOffsetX, fY };
    }
   


   
   

    return texture;
}

unsigned short s2d::S2DSpriteSheet::getRowCount()
{
    return m_rowCount;
}

unsigned short s2d::S2DSpriteSheet::getColCount(){
    return m_columnCount;
}
