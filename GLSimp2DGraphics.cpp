
#include "GLSimp2DGraphics.h"


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

	//m_rects = new Rect[m_maxRectCount];
	m_vertices = new Vertex[m_maxVertexCount];

	for (size_t i = 0; i < m_maxVertexCount; i+= 4) {

		m_vertices[i + 0] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
		m_vertices[i + 1] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
		m_vertices[i + 2] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
		m_vertices[i + 3] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f };
	}

	/*for (size_t i = 0; i < m_maxRectCount; i++) {

		m_rects[i].vertex[0] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
		m_rects[i].vertex[1] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
		m_rects[i].vertex[2] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
		m_rects[i].vertex[3] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
			
	}*/
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


	m_textureSlot.count = copy.m_textureSlot.count;
	m_textureSlot.maxCount = copy.m_textureSlot.maxCount;
	int size = m_textureSlot.maxCount * sizeof(UINT32);
	memcpy_s(m_textureSlot.texID, size, copy.m_textureSlot.texID, size);

	size = m_maxVertexCount * sizeof(Vertex);
	memcpy_s(m_vertices, size, copy.m_vertices, size);
}

s2d::S2DGraphics::S2DGraphics(S2DGraphics&& move){
	
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
	move.m_textureSlot.texID = nullptr;
	move.m_vertices = nullptr;
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
	move.m_textureSlot.texID = nullptr;
	move.m_vertices = nullptr;
	return *this;
}

//void s2d::S2DGraphics::resizeRectCount() {
//
//    m_maxVertexCount += 32 * 4;
//	Vertex* temp = new Vertex[m_maxVertexCount];
//	memcpy_s(temp, m_maxVertexCount * sizeof(Vertex), m_vertices, (m_maxVertexCount - 32*4) * sizeof(Vertex));
//	delete[] m_vertices;
//    m_vertices = temp;
//	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(m_maxVertexCount * sizeof(Vertex)), m_vertices,GL_DYNAMIC_DRAW);
//	UINT32 indexCount = m_maxVertexCount / 4 * 6;
//	m_indexRect = new RectIndexes[indexCount];
//	UINT32 offset = 0;
//	for (size_t i = 0; i < indexCount; i++) {
//
//
//		m_indexRect[i].index[0] = 0 + offset;
//		m_indexRect[i].index[1] = 1 + offset;
//		m_indexRect[i].index[2] = 2 + offset;
//		m_indexRect[i].index[3] = 0 + offset;
//		m_indexRect[i].index[4] = 3 + offset;
//		m_indexRect[i].index[5] = 2 + offset;
//		offset += 4;
//	}
//
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(RectIndexes) * indexCount), m_indexRect, GL_STATIC_DRAW);
//
//	delete[] m_indexRect;
//	m_indexRect = nullptr;
//}






s2d::Texture s2d::S2DGraphics::createTexture(unsigned char* pixels, int width, int height) {
	if (m_textureSlot.count >= m_textureSlot.maxCount)return { -1,0,0 };
	
	
	
	int index = m_textureSlot.count;
	glGenTextures(1, &m_textureSlot.texID[index]);
	
	glActiveTexture(GL_TEXTURE0 + m_textureSlot.count);
	glBindTexture(GL_TEXTURE_2D, m_textureSlot.texID[index]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	m_textureSlot.count++;

	//int temp[32] = {1,2};

	//for (int i = 1; i < m_textureSlot.count + 1; i++) {
		//temp[i] = i;
	//}
	//glUniform1iv(m_glUniformLoc, 2, temp);

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
void s2d::S2DGraphics::drawTriangle(Point p1, Point p2, Point p3, S2D_COLOR color, Texture texture){
	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);
	UINT32 index = m_vertexCount;
	Point normalP1 = { (float)((float)(p1.x / (float)m_scrWidth) * 2) - 1, (float)((float)(p1.y / (float)m_scrWidth) * 2) - 1 };
	Point normalP2 = { (float)((float)(p2.x / (float)m_scrWidth) * 2) - 1, (float)((float)(p2.y / (float)m_scrWidth) * 2) - 1 };
	Point normalP3 = { (float)((float)(p3.x / (float)m_scrWidth) * 2) - 1, (float)((float)(p3.y / (float)m_scrWidth) * 2) - 1 };

	Vec2f pos[4] = {};

	if (texture.texID > 0) {

		//pos[0] = { 0.0f,0.0f };
		//pos[1] = { 0.0f,1.0f };
		//pos[2] = { 1.0f,1.0f };
		//pos[3] = { 1.0f,0.0f };

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


	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;


	m_vertices[index + 0] = { {normalP1.x, normalP1.y},	fR, fG, fB, fA, pos[0], (float)texture.texID };
	m_vertices[index + 1] = { {normalP2.x, normalP2.y},	fR, fG, fB, fA, pos[1], (float)texture.texID };
	m_vertices[index + 2] = { {normalP3.x, normalP3.y},	fR, fG, fB, fA, pos[2], (float)texture.texID };
	m_vertices[index + 3] = { {normalP2.x, normalP2.y},	fR, fG, fB, fA, pos[1], (float)texture.texID };
	m_vertexCount += 4;
	m_vertexIndexCount += 6;
}





void s2d::S2DGraphics::drawRect(F32 x, F32 y, F32 width, F32 height, S2D_COLOR color, Texture texture)
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

void s2d::S2DGraphics::drawRect(S2DRect rect, S2D_COLOR color) {
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

void s2d::S2DGraphics::drawRotatedRect(F32 x, F32 y, F32 width, F32 height, float fAngle, S2D_COLOR color, Texture texture) {

	
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

void s2d::S2DGraphics::drawRotatedRect(S2DRect rect, float fAnge, S2D_COLOR color) {

	Vec4f pos = rect.getRect();

	drawRotatedRect(pos.x, pos.y, pos.z, pos.w, fAnge, color, rect.getTexture());

}

void s2d::S2DGraphics::drawNDCRect(float x, float y, float w, float h, S2D_COLOR color, Texture texture) {
	

	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);

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

	Vec2f vertexPos[4] = {
		{x, y},
		{x,	y + h},
		{x - w,	y + h},
		{x - w, y}
	};

	for (uint32_t i = 0; i < 4; i++) {

		m_vertices[index + i] = { vertexPos[i], fR, fG, fB, fA, pos[0], (float)texture.texID};
	}
	m_vertexCount += 4;
	m_vertexIndexCount += 6;

}

void s2d::S2DGraphics::drawNDCRect(S2DRect rect, S2D_COLOR color, Texture texture) {
	drawNDCRect(rect.getPos().x, rect.getPos().y, rect.getSize().x, rect.getSize().y, color, texture);
}

void s2d::S2DGraphics::drawLine(Point p1, Point p2, float w, S2D_COLOR color) {

	


	if (m_vertexCount >= m_maxVertexCount - 1) reservedMaxRect(m_maxVertexCount + 32 * 4);

	/*y = m_scrHeight - y;*/

	UINT32 index = m_vertexCount;
	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;

	/*Vec2f fP = normalizePoint({ x, y });
	Vec2f fOff = normalizePoint({ x + width, y - height });*/

	
	Vec2f pos[4] = {};

	pos[0] = { -1.0f,-1.0f };
	pos[1] = { -1.0f,-1.0f };
	pos[2] = { -1.0f,-1.0f };
	pos[3] = { -1.0f,-1.0f };
	s2d::Vec2f vec = { (p2.x - p1.x), (p2.y - p1.y) };
	
	
	/*vec.x /= magnitude;
	vec.y /= magnitude;*/

	s2d::Vec2f p3 = rotatePoint( normalizePoint( { vec.x, vec.y } ), normalizePoint({p1.x, p1.y}), 85);
	
	float magnitude = sqrtf(powf(p3.x, 2.0) + powf(p3.y, 2.0));
	p3.x /= magnitude;
	p3.y /= magnitude;
	m_vertices[index + 0] = { normalizePoint({ p1.x, p1.y }),			1, 1, 1, fA, pos[0], -1.0f};
	m_vertices[index + 1] = { normalizePoint({ p2.x, p2.y }),			1, 1, 1, fA, pos[1], -1.0f};
	m_vertices[index + 2] = { p3,			0, 1, 1, fA, pos[2], -1.0f };
	//m_vertices[index + 3] = { normalizePoint({ p2.y, -p2.x }),			1, 1, 1, fA, pos[3], -1.0f};
	m_vertexCount += 3;
	m_vertexIndexCount += 3;
	
	
}



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
 
