
#include "GLSimp2DGraphics.h"


s2d::Simp2DGraphics::Simp2DGraphics(UINT32 width, UINT32 height) {
	if (!LoadGL())return;
	m_rectExistCount = 0;
	m_scrWidth = width;
	m_scrHeight = height;
	m_maxRectCount = 64;

	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_maxTextureSlot);
	m_textureSlot.count = 0;
	m_textureSlot.maxCount = m_maxTextureSlot;
	m_textureSlot.texID = new UINT32[m_maxTextureSlot];

	m_rects = new Rect[m_maxRectCount];
	
	for (size_t i = 0; i < m_maxRectCount; i++) {

		m_rects[i].vertex[0] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
		m_rects[i].vertex[1] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
		m_rects[i].vertex[2] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
		m_rects[i].vertex[3] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f}, {-1.0f, -1.0f}, -1.0f};
			
	}
	glGenVertexArrays(1, &m_glVertexObjectID);
	glBindVertexArray(m_glVertexObjectID);
	glGenBuffers(1, &m_glVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferID);
	glGenBuffers(1, &m_glIndexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_glIndexBufferID);
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Rect)* m_maxRectCount), m_rects, GL_DYNAMIC_DRAW);
	GLsizei stride = sizeof(Vertex);
	
	
	m_indexRect = new RectIndexes[m_maxRectCount];
	UINT32 offset = 0;
	for (size_t i = 0; i < m_maxRectCount; i++){


		m_indexRect[i].index[0] = 0 + offset;
		m_indexRect[i].index[1] = 1 + offset;
		m_indexRect[i].index[2] = 2 + offset;
		m_indexRect[i].index[3] = 0 + offset;
		m_indexRect[i].index[4] = 3 + offset;
		m_indexRect[i].index[5] = 2 + offset;
		offset += 4;
	}
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(RectIndexes) * m_maxRectCount), m_indexRect, GL_STATIC_DRAW);

	delete[] m_indexRect;


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::pos));
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::col));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::textCoord));
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Vertex::texID));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	std::string vertexShaderCode = 
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


	std::string pixelShaderCode =
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

	unsigned int px = glCreateShader(GL_FRAGMENT_SHADER), vx = glCreateShader(GL_VERTEX_SHADER);
	const char* p = &pixelShaderCode.c_str()[0];
	glShaderSource(px, 1, &p, nullptr);
	const char* a = &vertexShaderCode.c_str()[0];
	glShaderSource(vx, 1, &a, nullptr);
	glCompileShader(px);
	
	
	glCompileShader(vx);

	m_glShaderProgID = glCreateProgram();
	glAttachShader(m_glShaderProgID, px);
	glAttachShader(m_glShaderProgID, vx);
	
	glLinkProgram(m_glShaderProgID);


	glUseProgram(m_glShaderProgID);
	
	int error = glGetError();
	m_glUniformLoc = glGetUniformLocation(m_glShaderProgID, "textSampler");
	
	glDeleteShader(px);
	glDeleteShader(vx);
	


	
	if(wglSwapIntervalEXT != nullptr)
		wglSwapIntervalEXT(0);

	
}

s2d::Simp2DGraphics::Simp2DGraphics() :Simp2DGraphics(50, 50) {}

s2d::Simp2DGraphics::Simp2DGraphics(Simp2DGraphics& copy){
	m_maxRectCount		= copy.m_maxRectCount;
	m_glVertexBufferID	= copy.m_glVertexBufferID;
	m_glVertexObjectID	= copy.m_glVertexObjectID;
	m_glIndexBufferID	= copy.m_glIndexBufferID;
	m_glShaderProgID	= copy.m_glShaderProgID;
	m_glVertexCount		= copy.m_glVertexCount;
	m_rectExistCount	= copy.m_rectExistCount;
	m_maxTextureSlot	= copy.m_maxTextureSlot;
	m_glUniformLoc		= copy.m_glUniformLoc;
	m_scrHeight			= copy.m_scrHeight;
	m_scrWidth			= copy.m_scrWidth;


	m_textureSlot.count = copy.m_textureSlot.count;
	m_textureSlot.maxCount = copy.m_textureSlot.maxCount;
	int size = m_textureSlot.maxCount * sizeof(UINT32);
	memcpy_s(m_textureSlot.texID, size, copy.m_textureSlot.texID, size);

	size = m_rectExistCount * sizeof(Rect);
	memcpy_s(m_rects, size, copy.m_rects, size);
}

s2d::Simp2DGraphics::Simp2DGraphics(Simp2DGraphics&& move){
	m_maxRectCount			= move.m_maxRectCount;
	m_glVertexBufferID		= move.m_glVertexBufferID;
	m_glVertexObjectID		= move.m_glVertexObjectID;
	m_glIndexBufferID		= move.m_glIndexBufferID;
	m_glShaderProgID		= move.m_glShaderProgID;
	m_glVertexCount			= move.m_glVertexCount;
	m_rectExistCount		= move.m_rectExistCount;
	m_maxTextureSlot		= move.m_maxTextureSlot;
	m_glUniformLoc			= move.m_glUniformLoc;
	m_scrHeight				= move.m_scrHeight;
	m_scrWidth				= move.m_scrWidth;
	m_textureSlot			= move.m_textureSlot;
	m_rects					= move.m_rects;
	move.m_textureSlot.texID = nullptr;
	move.m_rects = nullptr;
}

s2d::Simp2DGraphics::Simp2DGraphics(s2d::Simp2DWindow& window)
{
	window.CreateOpenGLContext();
	window.GetWindowSize((int*) & m_scrWidth, (int*) & m_scrHeight);
	*this = std::move(Simp2DGraphics(m_scrWidth, m_scrHeight));
}



s2d::Simp2DGraphics::~Simp2DGraphics(){
	if(m_rects!=nullptr)
		delete[] m_rects;
	m_rects = nullptr;

	if(m_textureSlot.texID != nullptr)
		delete[] m_textureSlot.texID;
	m_textureSlot.texID = nullptr;
}

s2d::Simp2DGraphics& s2d::Simp2DGraphics::operator=(Simp2DGraphics&& move) noexcept
{
	m_maxRectCount = move.m_maxRectCount;
	m_glVertexBufferID = move.m_glVertexBufferID;
	m_glVertexObjectID = move.m_glVertexObjectID;
	m_glIndexBufferID = move.m_glIndexBufferID;
	m_glShaderProgID = move.m_glShaderProgID;
	m_glVertexCount = move.m_glVertexCount;
	m_rectExistCount = move.m_rectExistCount;
	m_maxTextureSlot = move.m_maxTextureSlot;
	m_glUniformLoc = move.m_glUniformLoc;
	m_scrHeight = move.m_scrHeight;
	m_scrWidth = move.m_scrWidth;
	m_textureSlot = move.m_textureSlot;
	m_rects = move.m_rects;
	move.m_textureSlot.texID = nullptr;
	move.m_rects = nullptr;
	return *this;
}

void s2d::Simp2DGraphics::resizeRectCount() {

    m_maxRectCount += 32;
	Rect* temp = new Rect[m_maxRectCount];
	memcpy_s(temp, m_maxRectCount * sizeof(Rect), m_rects, m_maxRectCount * sizeof(Rect) - 32);
	delete[] m_rects;
    m_rects = temp;
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(m_maxRectCount * sizeof(Rect)), m_rects,GL_DYNAMIC_DRAW);

	m_indexRect = new RectIndexes[m_maxRectCount];
	UINT32 offset = 0;
	for (size_t i = 0; i < m_maxRectCount; i++) {
		m_indexRect[i].index[0] = 0 + offset;
		m_indexRect[i].index[1] = 1 + offset;
		m_indexRect[i].index[2] = 2 + offset;
		m_indexRect[i].index[3] = 0 + offset;
		m_indexRect[i].index[4] = 3 + offset;
		m_indexRect[i].index[5] = 2 + offset;
		offset += 4;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(sizeof(RectIndexes) * m_maxRectCount), m_indexRect, GL_STATIC_DRAW);

	delete[] m_indexRect;

}



s2d::TEXID s2d::Simp2DGraphics::createTexture(unsigned char* pixels, int width, int height)
{
	if (m_textureSlot.count >= m_textureSlot.maxCount)return -1;
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

	int temp[32] = {0,1};

	for (int i = 0; i < m_textureSlot.count; i++) {
		temp[i] = i;
	}
	glUniform1iv(m_glUniformLoc, m_textureSlot.count, temp);

	
	return m_textureSlot.texID[index];

}

void s2d::Simp2DGraphics::drawRect(FLOAT32 x, FLOAT32 y, FLOAT32 width, FLOAT32 height, PIXCOLOR color, TEXID texID)
{
	if (m_rectExistCount >= m_maxRectCount - 1) resizeRectCount();
	if (m_rects == nullptr)return;

	UINT32 index = m_rectExistCount;
	
	float fx = (float)((float)(x / (float)m_scrWidth) * 2) - 1;
	float fy = (float)((float)(y / (float)m_scrHeight) * 2) - 1;
	float fOffX = (float)((float)((x + width) / (float)m_scrWidth) * 2) - 1;
	float fOffY = (float)((float)((y + height) / (float)m_scrHeight) * 2) - 1;
	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;
	

	Vec2 pos[4] = {};
	
	if (texID > 0) {

		pos[0] = { 0.0f,0.0f };
		pos[1] = { 0.0f,1.0f };
		pos[2] = { 1.0f,1.0f };
		pos[3] = { 1.0f,0.0f };
	}
	else {
		pos[0] = { -1.0f,-1.0f };
		pos[1] = { -1.0f,-1.0f };
		pos[2] = { -1.0f,-1.0f };
		pos[3] = { -1.0f,-1.0f };
	}



	m_rects[index].vertex[0] = { fx,	fy,		fR, fG, fB, fA, pos[0], (float)texID };
	m_rects[index].vertex[1] = { fx,	fOffY,	fR, fG, fB, fA, pos[1], (float)texID };
	m_rects[index].vertex[2] = { fOffX, fOffY,	fR, fG, fB, fA, pos[2], (float)texID };
	m_rects[index].vertex[3] = { fOffX, fy,		fR, fG, fB, fA, pos[3], (float)texID };
	
	m_rectExistCount++;
	
}



void s2d::Simp2DGraphics::flushBuffer() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	if (m_rectExistCount > 0)
		glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(Rect) * m_rectExistCount), m_rects);
	glDrawElements(GL_TRIANGLES, 6 * m_rectExistCount, GL_UNSIGNED_INT, nullptr);
	m_rectExistCount = 0;
	
}
 
