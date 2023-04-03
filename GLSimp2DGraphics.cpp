#include "GLSimp2DGraphics.h"


const static unsigned long long ___glyphdata[36] = { // what an actual abomination
0b1110011111011011101111011011110110000001101111011011110110111101,
0b1000011110111011101110111000001110111011101110111011101110000111,
0b1110001111011111101111111011111110111111101111111101111111100011,
0b1000011110111011101111011011110110111101101111011011101110000111,
0b1000001110111111101111111000001110111111101111111011111110000011,
0b1000001110111111101111111000011110111111101111111011111110111111,
0b1100001110111101011111110111111101100001011111011011110111000011,
0b1011110110111101101111011000000110111101101111011011110110111101,
0b1000001111101111111011111110111111101111111011111110111110000011,
0b1100000111110111111101111111011111110111011101111011011111001111,
0b1011101110110111101011111001111110011111101011111011011110111011,
0b1011111110111111101111111011111110111111101111111011111110000111,
0b0111110100111001010101010110110101111101011111010111110101111101,
0b1011110110011101101011011010110110110101101101011011100110111101,
0b1110011111011011101111011011110110111101101111011101101111100111,
0b1000011110111011101110111011101110000111101111111011111110111111,
0b1110011111011011101111011011110110101101101101011101101111100101,
0b1000011110111011101110111011011110001111101101111011101110111011,
0b1100011110111011101111111000011111111011111110111011101111000111,
0b0000000111101111111011111110111111101111111011111110111111101111,
0b0111110101111101011111010111110101111101011111010111110110000011,
0b0111110101111101011111011011101110111011101110111101011111101111,
0b0111110101111101011111010110110101101101011011010101010110111011,
0b0111111010111101110110111110011111100111110110111011110101111110,
0b0111110101111101101110111101011111101111111011111110111111101111,
0b0000000111111101111110111110011111011111101111110111111100000001,
0b1100001110111101101111011011110110111101101111011011110111000011,
0b1100111110101111111011111110111111101111111011111110111110000011,
0b1100011110111011111110111111011111001111101111111011111110000011,
0b1100011111111011111110111100011111111011111110111111101111000111,
0b1101101111011011110110111100001111111011111110111111101111111011,
0b1000001110111111101111111000001111111011111110111011101111000111,
0b1000001110111111101111111000001110111011101110111011101110000011,
0b1000011111110111111101111111011111000011111101111111011111110111,
0b1000000110111101101111011000000110111101101111011011110110000001,
0b1000001110111011101110111000001111111011111110111111101111111011,
};

s2d::S2DGraphics::S2DGraphics(UINT32 width, UINT32 height) {
	if (!LoadGL())return;
	m_scrWidth = width;
	m_scrHeight = height;
	m_vertexCount = 0;
	m_maxVertexCount = 64 * 4;
	m_vertexIndexCount = 0;

	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxTextureSlot);
	m_textureSlot.count = 0;
	m_textureSlot.maxCount = m_maxTextureSlot;
	m_textureSlot.texID = new UINT32[m_maxTextureSlot];

	
	m_vertices = new Vertex[m_maxVertexCount];

	for (size_t i = 0; i < m_maxVertexCount; i+= 4) {

		m_vertices[i + 0] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
		m_vertices[i + 1] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
		m_vertices[i + 2] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
		m_vertices[i + 3] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
	}

	
	glGenVertexArrays(1, &m_glVertexObjectID);
	glBindVertexArray(m_glVertexObjectID);
	glGenBuffers(1, &m_glVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferID);
	glGenBuffers(1, &m_glIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBufferID);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Vertex)* m_maxVertexCount), m_vertices, GL_DYNAMIC_DRAW);
	GLsizei stride = sizeof(Vertex);
	
	UINT32 indexCount = m_maxVertexCount / 4 * 6;
	m_indexRect = new RectIndexes[indexCount];
	UINT32 offset = 0;
	for (size_t i = 0; i < indexCount; i++){


		m_indexRect[i].index[0] = 0 + offset;
		m_indexRect[i].index[1] = 1 + offset;
		m_indexRect[i].index[2] = 2 + offset;
		m_indexRect[i].index[3] = 0 + offset;
		m_indexRect[i].index[4] = 3 + offset;
		m_indexRect[i].index[5] = 2 + offset;
		offset += 4;
	}
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(RectIndexes) * indexCount), m_indexRect, GL_STATIC_DRAW);

	delete[] m_indexRect;


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::col));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::textCoord));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::texID));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	const char * vertexShaderCode = 
		"#version 330 core\n"
		"layout (location = 0) in vec2 pos;\n"
		"layout (location = 1) in vec4 col;\n"
		"layout (location = 2) in vec2 texPos;\n"
		"layout (location = 3) in float texID;\n"
		"out vec4 oCol;\n"
		"out vec2 texCoord;\n"
		"out float isTexured; \n"
		"out float fTexID;\n"
		"void main()\n"
		"{\n"
		"	fTexID = texID;\n"	
		"	texCoord = texPos;\n"
		"	oCol = col;\n"
		"	isTexured = 1.0;\n"
		"   if(texID < 0.0f){\n"
		"		isTexured = -1.0;\n"
		"	}\n"
		"   gl_Position = vec4(pos.x, pos.y, 0.0, 1.0);\n"
		"}\0";


		const char * pixelShaderCode =
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 oCol;\n"
		"in float fTexID;\n"
		"in vec2 texCoord;\n"
		"in float isTexured;\n"
		"uniform sampler2D textSampler[32];\n"
		"void main()\n"
		"{\n"
		"	vec4 finalCol;\n"
		"	int index = int(fTexID); \n"
		"	if(isTexured >= 0.0)\n"
		"		finalCol =  texture(textSampler[index], texCoord) * oCol;\n"
		"	else\n"
		"		finalCol = oCol; \n"
		"	FragColor = finalCol;\n"
		"}\n";

	UINT32 px = glCreateShader(GL_FRAGMENT_SHADER), vx = glCreateShader(GL_VERTEX_SHADER);
	
	glShaderSource(px, 1, &pixelShaderCode, nullptr);
	glShaderSource(vx, 1, &vertexShaderCode, nullptr);
	glCompileShader(px);
	glCompileShader(vx);

	m_glShaderProgID = glCreateProgram();
	glAttachShader(m_glShaderProgID, px);
	glAttachShader(m_glShaderProgID, vx);
	
	glLinkProgram(m_glShaderProgID);


	glUseProgram(m_glShaderProgID);
	
	
	m_glUniformLoc = glGetUniformLocation(m_glShaderProgID, "textSampler");
	
	glDeleteShader(px);
	glDeleteShader(vx);
	

	int temp[32] = { 0,1 };

	for (int i = 0; i < 32; i++) {
		temp[i] = i;
	}
	glUniform1iv(m_glUniformLoc, 32, temp);

	if(wglSwapIntervalEXT != nullptr)
		wglSwapIntervalEXT(0);

	

}

s2d::S2DGraphics::S2DGraphics() :S2DGraphics(50, 50) {}

s2d::S2DGraphics::S2DGraphics(S2DGraphics& copy){
	m_glVertexBufferID	= copy.m_glVertexBufferID;
	m_glVertexObjectID	= copy.m_glVertexObjectID;
	m_glIndexBufferID	= copy.m_glIndexBufferID;
	m_glShaderProgID	= copy.m_glShaderProgID;
	m_vertexCount		= copy.m_vertexCount;
	m_maxVertexCount	= copy.m_maxVertexCount;
	m_maxTextureSlot	= copy.m_maxTextureSlot;
	m_glUniformLoc		= copy.m_glUniformLoc;
	m_scrHeight			= copy.m_scrHeight;
	m_scrWidth			= copy.m_scrWidth;
	m_glyphTexture = copy.m_glyphTexture;
	m_glyphs = copy.m_glyphs;

	m_textureSlot.count = copy.m_textureSlot.count;
	m_textureSlot.maxCount = copy.m_textureSlot.maxCount;
	int size = m_textureSlot.maxCount * sizeof(UINT32);
	memcpy_s(m_textureSlot.texID, size, copy.m_textureSlot.texID, size);

	size = m_maxVertexCount * sizeof(Vertex);
	memcpy_s(m_vertices, size, copy.m_vertices, size);
}

s2d::S2DGraphics::S2DGraphics(S2DGraphics&& move) noexcept{
	
	m_glVertexBufferID		= move.m_glVertexBufferID;
	m_glVertexObjectID		= move.m_glVertexObjectID;
	m_glIndexBufferID		= move.m_glIndexBufferID;
	m_glShaderProgID		= move.m_glShaderProgID;
	m_vertexCount			= move.m_vertexCount;
	m_maxTextureSlot		= move.m_maxTextureSlot;
	m_maxVertexCount		= move.m_maxVertexCount;
	m_glUniformLoc			= move.m_glUniformLoc;
	m_scrHeight				= move.m_scrHeight;
	m_scrWidth				= move.m_scrWidth;
	m_textureSlot			= move.m_textureSlot;
	m_vertices				= move.m_vertices;
	m_indexRect				= move.m_indexRect;
	m_vertexIndexCount		= move.m_vertexIndexCount;
	m_glyphTexture = move.m_glyphTexture;
	m_glyphs = move.m_glyphs;
	move.m_textureSlot.texID = nullptr;
	move.m_vertices = nullptr;
	move.m_indexRect = nullptr;
}

s2d::S2DGraphics::S2DGraphics(s2d::S2DWindow& window)
{
	window.CreateOpenGLContext();
	window.GetWindowSize((int*) & m_scrWidth, (int*) & m_scrHeight);
	*this = std::move(S2DGraphics(m_scrWidth, m_scrHeight));
}



s2d::S2DGraphics::~S2DGraphics(){
	if(m_vertices!=nullptr)
		delete[] m_vertices;
	m_vertices = nullptr;

	if(m_textureSlot.texID != nullptr)
		delete[] m_textureSlot.texID;
	m_textureSlot.texID = nullptr;
}

s2d::S2DGraphics& s2d::S2DGraphics::operator=(S2DGraphics&& move) noexcept
{
	m_glVertexBufferID		= move.m_glVertexBufferID;
	m_glVertexObjectID		= move.m_glVertexObjectID;
	m_glIndexBufferID		= move.m_glIndexBufferID;
	m_glShaderProgID		= move.m_glShaderProgID;
	m_vertexCount			= move.m_vertexCount;
	m_maxTextureSlot		= move.m_maxTextureSlot;
	m_maxVertexCount		= move.m_maxVertexCount;
	m_glUniformLoc			= move.m_glUniformLoc;
	m_scrHeight				= move.m_scrHeight;
	m_scrWidth				= move.m_scrWidth;
	m_textureSlot			= move.m_textureSlot;
	m_vertices				= move.m_vertices;
	m_glyphTexture			= move.m_glyphTexture;
	m_glyphs				= move.m_glyphs;
	m_indexRect = move.m_indexRect;
	m_vertexIndexCount = move.m_vertexIndexCount;
	move.m_textureSlot.texID = nullptr;
	move.m_vertices = nullptr;
	move.m_indexRect = nullptr;
	
	return *this;
}



s2d::Texture s2d::S2DGraphics::createTexture(unsigned char* pixels, int width, int height) {
	if (m_textureSlot.count >= m_textureSlot.maxCount)return { -1,0,0 };
	
	
	
	int index = m_textureSlot.count;
	glGenTextures(1, &m_textureSlot.texID[index]);
	
	glActiveTexture(GL_TEXTURE0 + m_textureSlot.count);
	glBindTexture(GL_TEXTURE_2D, m_textureSlot.texID[index]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	m_textureSlot.count++;

	Texture texture = { m_textureSlot.count - 1, width, height };

	texture.texCoords[0] = { 0.0f, 0.0f };
	texture.texCoords[1] = { 0.0f, 1.0f };
	texture.texCoords[2] = { 1.0f, 1.0f };
	texture.texCoords[3] = { 1.0f, 0.0f };

	return texture;

}

void s2d::S2DGraphics::setBlendingState(bool state) {
	if (state) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		return;
	}

	glDisable(GL_BLEND);
}

// Not working with textures
void s2d::S2DGraphics::drawTriangle(Point p1, Point p2, Point p3, Color color){
	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);
	UINT32 index = m_vertexCount;
	Point normalP1 = { (float)((float)(p1.x / (float)m_scrWidth) * 2) - 1, (float)((float)(p1.y / (float)m_scrWidth) * 2) - 1 };
	Point normalP2 = { (float)((float)(p2.x / (float)m_scrWidth) * 2) - 1, (float)((float)(p2.y / (float)m_scrWidth) * 2) - 1 };
	Point normalP3 = { (float)((float)(p3.x / (float)m_scrWidth) * 2) - 1, (float)((float)(p3.y / (float)m_scrWidth) * 2) - 1 };

	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;
	m_vertices[index + 0] = { {normalP1.x, normalP1.y},	fR, fG, fB, fA, { -1.0f,-1.0f }, -1.0f };
	m_vertices[index + 1] = { {normalP2.x, normalP2.y},	fR, fG, fB, fA, { -1.0f,-1.0f }, -1.0f };
	m_vertices[index + 2] = { {normalP3.x, normalP3.y},	fR, fG, fB, fA, { -1.0f,-1.0f }, -1.0f };
	m_vertices[index + 3] = { {normalP2.x, normalP2.y},	fR, fG, fB, fA, { -1.0f,-1.0f }, -1.0f };
	m_vertexCount += 4;
	m_vertexIndexCount += 6;
}


void s2d::S2DGraphics::drawString(const char* str, float x, float y, float w, float h, float angle) {
	int i = 0;
	int step = 0;
	int j = 0;
	int row = m_glyphs.getColCount() - 1;

	while (str[i] != '\0') {


		unsigned int index = 0;
		
		if (str[i] >= 'A' && str[i] <= 'Z') {
			index = row - (str[i] - 'A');
		}
		else if (str[i] >= 'a' && str[i] <= 'z') {
			index = row - (str[i] - 'a');
		}
		else if (str[i] >= '0' && str[i] <= '1') {
			index = row - (str[i] - '0');
		}
		else if (str[i] == ' ') {
			step++;
			i++;
			continue;
		}
		else if (str[i] == '\n') {
			j++;
			step = 0;
			i++;
			continue;
		}
		else {
			step++;
			i++;
			continue;
		}
		
		drawRotatedRect(x + (w * step), y + (h * j), w, h, angle, s2d::PXLCLR_WHITE, m_glyphs.getTexture(index));
		i++;
		step++;
	}

}

void s2d::S2DGraphics::drawString(std::string str, float x, float y, float w, float h, float angle) {
	drawString(str.c_str(), x, y, w, h, angle);
}


void s2d::S2DGraphics::drawRect(F32 x, F32 y, F32 width, F32 height, Color color, Texture texture)
{
	
	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);

	y = m_scrHeight - y;
	
	UINT32 index = m_vertexCount;
	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;
	
	Vec2f fP = normalizePoint({ x, y });
	Vec2f fOff = normalizePoint({ x + width, y - height });


	Vec2f pos[4] = {};
	
	if (texture.texID >= 0) {
		pos[0] = texture.texCoords[0];
		pos[1] = texture.texCoords[1];
		pos[2] = texture.texCoords[2];
		pos[3] = texture.texCoords[3];

	}
	else {
		pos[0] = { -1.0f,-1.0f };
		pos[1] = { -1.0f,-1.0f };
		pos[2] = { -1.0f,-1.0f };
		pos[3] = { -1.0f,-1.0f };
	}

	m_vertices[index + 0] = { fP,						fR, fG, fB, fA, pos[0], (float)texture.texID };
	m_vertices[index + 1] = { {fP.x,	fOff.y},		fR, fG, fB, fA, pos[1], (float)texture.texID };
	m_vertices[index + 2] = { {fOff.x,	fOff.y},		fR, fG, fB, fA, pos[2], (float)texture.texID };
	m_vertices[index + 3] = { {fOff.x,	fP.y},			fR, fG, fB, fA, pos[3], (float)texture.texID };
	m_vertexCount += 4;
	m_vertexIndexCount += 6;
}

s2d::Vec2f s2d::S2DGraphics::normalizePoint(s2d::Vec2f point) {
	return { ( point.x / (float)m_scrWidth * 2.0f) - 1.0f,(point.y / (float)m_scrHeight * 2.0f) - 1.0f };
}

void s2d::S2DGraphics::initGlyphs(s2d::Color foregrnd, s2d::Color backgrnd) {

	unsigned int w = 8, h = 8, glyphCount = 36;
	unsigned int* colorData = new unsigned int[w * h * glyphCount];
	unsigned const long long* b = ___glyphdata;
	
	unsigned int foreGroundColor = foregrnd.A;
	foreGroundColor <<= 8;
	foreGroundColor |= foregrnd.R; foreGroundColor <<= 8;
	foreGroundColor |= foregrnd.G; foreGroundColor <<= 8;
	foreGroundColor |= foregrnd.B;

	unsigned int backGroundColor = backgrnd.A;
	backGroundColor <<= 8;
	backGroundColor |= backgrnd.B; backGroundColor <<= 8;
	backGroundColor |= backgrnd.G; backGroundColor <<= 8;
	backGroundColor |= backgrnd.R;


	for (int i = 0; i < glyphCount; i++) {
		unsigned long long shifter = 0b1000000000000000000000000000000000000000000000000000000000000000;

		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {

				unsigned long long res = b[i] & shifter;
				if (res == 0) {
					colorData[x + (y + (i * h)) * 8] = foreGroundColor;
				}
				else {
					colorData[x + (y + (i * h)) * 8] = backGroundColor;
				}
				shifter >>= 1;
			}
		}

	}

	m_glyphTexture = createTexture((unsigned char*)colorData, w, glyphCount * h);
	m_glyphs.createSpriteSheet(m_glyphTexture, 8, 8);
	delete[] colorData;

}

void s2d::S2DGraphics::drawRect(S2DRect rect, Color color) {
	s2d::Vec4f v = rect.getRect();
	drawRect(v.x, v.y, v.z, v.w, color, rect.getTexture());
}

s2d::Vec2f s2d::S2DGraphics::rotatePoint(s2d::Vec2f point, s2d::Vec2f m, float fAngle) {
	fAngle = fAngle * 3.14159f / 180.0;
	float s = sinf(fAngle);
	float c = cosf(fAngle);
	point = {point.x - m.x, point.y - m.y};
	return { (point.x * c - point.y * s) + m.x , (point.x * s + point.y * c) + m.y};

}

void s2d::S2DGraphics::drawRotatedRect(F32 x, F32 y, F32 width, F32 height, float fAngle, Color color, Texture texture) {

	
	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);
	
	y = m_scrHeight - y;

	UINT32 index = m_vertexCount;

	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;

	Vec2f pos[4] = {};

	if (texture.texID >= 0) {
		
		pos[0] = texture.texCoords[0];
		pos[1] = texture.texCoords[1];
		pos[2] = texture.texCoords[2];
		pos[3] = texture.texCoords[3];

	}
	else {
		pos[0] = { -1.0f,-1.0f };
		pos[1] = { -1.0f,-1.0f };
		pos[2] = { -1.0f,-1.0f };
		pos[3] = { -1.0f,-1.0f };
	}

	Vec2f fP = normalizePoint({ x, y });
	Vec2f fOff = normalizePoint({ x + width, y - height });
	
	float w = abs(fOff.x - fP.x), h = abs(fOff.y - fP.y);
	float hw = w / 2;
	float hh = h / 2;
	
	Vec2f m = {fP.x + hw, fP.y - hh};
	
	Vec2f v[4] = {
		fP,
		{fP.x,		fOff.y},
		{fOff.x,	fOff.y},
		{fOff.x,	fP.y}
	};
	
	for (int i = 0; i < 4; i++) {
		m_vertices[index + i] = { rotatePoint(v[i], m, fAngle), fR, fG, fB, fA, pos[i], (float)texture.texID };
	}
	


	m_vertexCount += 4;
	m_vertexIndexCount += 6;


}

void s2d::S2DGraphics::drawRotatedRect(S2DRect rect, float fAnge, Color color) {

	Vec4f pos = rect.getRect();

	drawRotatedRect(pos.x, pos.y, pos.z, pos.w, fAnge, color, rect.getTexture());

}

void s2d::S2DGraphics::drawWireFrameCircle(Point p1, float fRadius, float lw, Color color, uint32_t nSegmentCount) {

	
	float p = (3.14159f * 2.0f) / (float)nSegmentCount;
	float& r = fRadius;
	Point start = { cosf(0) * r + p1.x, sinf(0) * r + p1.y };
	Point s;
	for (uint32_t i = 1; i <= nSegmentCount; i++) {
		Point s = { cosf(i * p) * r + p1.x , sinf(i * p) * r + p1.y };
		drawLine(start, s, lw);
		start = s;
	}

}

void s2d::S2DGraphics::drawCircle(Point p1, float fRadius, Color color, uint32_t nSegmentCount) {


	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);
	p1.y = m_scrHeight - p1.y;
	
	if (nSegmentCount <= 4) nSegmentCount = 6;
	if (nSegmentCount % 2 != 0) nSegmentCount += 1;

	while (m_vertexCount + ((nSegmentCount/2)*4) >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);

	UINT32 index = m_vertexCount;

	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;


	float p = (3.14159f * 2.0f) / (float)nSegmentCount;
	float& r = fRadius;
	Point start = { cosf(0) * r + p1.x, sinf(0) * r + p1.y };
	Point s;
	
	uint32_t t = 0;
	for (uint32_t i = 0; i < nSegmentCount; i+=2) {

		Point p2 = { cosf(i * p) * r + p1.x , sinf(i * p) * r + p1.y };
		Point p3 = { cosf( (i + 1) * p) * r + p1.x , sinf( (i + 1) * p) * r + p1.y };
		Point p4 = { cosf( (i + 2) * p) * r + p1.x , sinf( (i + 2) * p) * r + p1.y };

		m_vertices[index + t] = { normalizePoint(p1),		fR, fG, fB, fA, { -1.0f, -1.0f }, -1.0f };
		m_vertices[index + t + 1] = { normalizePoint(p2),	fR, fG, fB, fA, { -1.0f, -1.0f }, -1.0f };
		m_vertices[index + t + 2] = { normalizePoint(p3),	fR, fG, fB, fA, { -1.0f, -1.0f }, -1.0f };
		m_vertices[index + t + 3] = { normalizePoint(p4),	fR, fG, fB, fA, {-1.0f, -1.0f}, -1.0f };
		t += 4;
	}

	m_vertexCount += (nSegmentCount / 2) * 4;
	m_vertexIndexCount += (nSegmentCount / 2) * 6;
}

void s2d::S2DGraphics::drawLine(float x1, float y1, float x2, float y2, float w, Color color) {

	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);
	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;
	
	y1 = m_scrHeight - y1;
	y2 = m_scrHeight - y2;

	UINT32 index = m_vertexCount;
	Vec2f vec = { x2 - x1, y2 - y1 };
	float m = sqrtf(pow(vec.x, 2) + pow(vec.y, 2)); // magnitude
	Vec2f unitVec = { vec.x / m, vec.y / m };
	unitVec = rotatePoint(unitVec, { 0,0 }, 90.0f);
	Vec2f p3 = { (w * unitVec.x) + x1,  (w * unitVec.y) + y1 };
	Vec2f p4 = { (w * unitVec.x) + x2,  (w * unitVec.y) + y2 };
	Vec2f p1 = normalizePoint({ x1, y1 });
	Vec2f p2 = normalizePoint({ x2, y2 });
	p3 = normalizePoint(p3);
	p4 = normalizePoint(p4);

	m_vertices[index + 0] = { p1,	fR, fG, fB, fA, {-1.0f, -1.0f}, -1.0f };
	m_vertices[index + 1] = { p3,	fR, fG, fB, fA, {-1.0f, -1.0f}, -1.0f };
	m_vertices[index + 2] = { p4,	fR, fG, fB, fA, {-1.0f, -1.0f}, -1.0f };
	m_vertices[index + 3] = { p2,	fR, fG, fB, fA, {-1.0f, -1.0f}, -1.0f };


	m_vertexCount += 4;
	m_vertexIndexCount += 6;

}

void s2d::S2DGraphics::drawLine(Point p1, Point p2, float w, Color color) {
	if (w <= 0.0f) w = 1.0f;
	drawLine(p1.x, p1.y, p2.x, p2.y, w, color);
}

void s2d::S2DGraphics::drawWireFrameRect(Vec2f pos, Vec2f sz, float lw, Color color) {
	if (lw <= 0.0f) lw = 1.0f;
	Point p1 = pos;
	Point p2 = { pos.x + sz.x, pos.y };
	Point p3 = { pos.x, pos.y + sz.y };
	Point p4 = { pos.x + sz.x, pos.y + sz.y };

	drawLine(p1, p2, lw, color);
	drawLine({ p2.x - lw, p2.y }, { p4.x - lw, p4.y }, lw, color);
	drawLine({ p4.x - lw, p4.y - lw }, { p3.x + lw, p3.y - lw}, lw, color);
	drawLine({ p3.x + lw, p3.y }, { p1.x + lw, p1.y }, lw, color);
}

void s2d::S2DGraphics::drawWireFrameRect(Vec2f pos, Vec2f sz, float lw, float f, Color color) {
	if (lw <= 0.0f) lw = 1.0f;
	Point p1 = pos;
	Point p2 = { pos.x + sz.x, pos.y };
	Point p3 = { pos.x, pos.y + sz.y };
	Point p4 = { pos.x + sz.x, pos.y + sz.y };
	Vec2f m = { p1.x + (sz.x / 2.0f), p1.y + (sz.y / 2.0f) };

	drawLine( rotatePoint(p1, m, f), rotatePoint(p2, m, f), lw, color);
	drawLine( rotatePoint( { p2.x - lw, p2.y }, m, f), rotatePoint({ p4.x - lw, p4.y }, m, f), lw, color);
	drawLine( rotatePoint({ p4.x - lw, p4.y - lw }, m, f), rotatePoint({ p3.x + lw, p3.y - lw },m,f), lw, color);
	drawLine( rotatePoint({ p3.x + lw, p3.y }, m, f), rotatePoint({ p1.x + lw, p1.y },m,f) , lw, color);
}

void s2d::S2DGraphics::drawWireFrameTriangle(Point p1, Point p2, Point p3, float lw, Color color) {
	if (lw <= 0.0f) lw = 1.0f;

	drawLine(p1, p2, lw, color);
	drawLine(p2, p3, lw, color);
	drawLine(p3, p1, lw, color);
}

//void s2d::S2DGraphics::drawWireFrameTriangle(Point p1, Point p2, Point p3, float lw, float fAngle, S2D_COLOR color) {
//
//
//	Vec2f m1 = p2 - p1;
//	Vec2f m2 = p3 - p2;
//	Vec2f m3 = p1 - p3;
//
//	p1 = rotatePoint(p1, m1, fAngle);
//	p2 = rotatePoint(p2, m2, fAngle);
//	p3 = rotatePoint(p3, m3, fAngle);
//
//	drawWireFrameTriangle(p1, p2, p3, lw, color);
//}



void s2d::S2DGraphics::flushBuffer() {
	glClear(GL_COLOR_BUFFER_BIT);
	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(Vertex) * m_vertexCount), m_vertices);
	glDrawElements(GL_TRIANGLES, m_vertexIndexCount, GL_UNSIGNED_INT, nullptr);
	m_vertexCount = 0;
	m_vertexIndexCount = 0;
}

void s2d::S2DGraphics::setVSYNC(bool state) {
	if (wglSwapIntervalEXT != nullptr)
		(state) ? wglSwapIntervalEXT(0) : wglSwapIntervalEXT(1);
}

void s2d::S2DGraphics::reservedMaxRect(uint32_t count) {

	if (count <= m_maxVertexCount)return;
	m_maxVertexCount += count * 4;
	Vertex* temp = new Vertex[m_maxVertexCount];
	memcpy_s(temp, m_maxVertexCount * sizeof(Vertex), m_vertices, (m_maxVertexCount - count * 4) * sizeof(Vertex));
	delete[] m_vertices;
	m_vertices = temp;
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(m_maxVertexCount * sizeof(Vertex)), m_vertices, GL_DYNAMIC_DRAW);
	UINT32 indexCount = m_maxVertexCount / 4 * 6;
	m_indexRect = new RectIndexes[indexCount];
	UINT32 offset = 0;
	for (size_t i = 0; i < indexCount; i++) {


		m_indexRect[i].index[0] = 0 + offset;
		m_indexRect[i].index[1] = 1 + offset;
		m_indexRect[i].index[2] = 2 + offset;
		m_indexRect[i].index[3] = 0 + offset;
		m_indexRect[i].index[4] = 3 + offset;
		m_indexRect[i].index[5] = 2 + offset;
		offset += 4;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(RectIndexes) * indexCount), m_indexRect, GL_STATIC_DRAW);

	delete[] m_indexRect;
	m_indexRect = nullptr;
}
 
