
#include "GLSimp2DGraphics.h"

s2d::Simp2DGraphics::Simp2DGraphics(UINT32 width, UINT32 height) {
	LoadGL();
	m_rectExistCount = 0;
	m_scrWidth = width;
	m_scrHeight = height;
	m_maxRectCount = 64;

	


	
	int size = offsetof(Vertex, Vertex::col);


	m_rects = new Rect[m_maxRectCount];
	
	

	for (size_t i = 0; i < m_maxRectCount; i++) {

			
			
		m_rects[i].vertex[0] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f} };
		m_rects[i].vertex[1] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f} };
		m_rects[i].vertex[2] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f} };
		m_rects[i].vertex[3] = { {0.0f, 0.0f},		{0.0f, 0.0f, 0.0f, 0.0f} };
			

			
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
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	std::string vertexShaderCode = 
		"#version 330 core\n"
		"layout (location = 0) in vec2 pos;\n"
		"layout (location = 1) in vec4 col;\n"
		"out vec4 oCol;\n"
		"void main()\n"
		"{\n"
		"	oCol = col;\n"
		"   gl_Position = vec4(pos.x, pos.y, 1.0, 1.0);\n"
		"}\0";

	std::string pixelShaderCode = 
		"#version 330 core\n"
		"out vec4 FragColor;\n"
		"in vec4 oCol;"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(oCol);\n"
		"}\n";

	unsigned int px = glCreateShader(GL_FRAGMENT_SHADER), vx = glCreateShader(GL_VERTEX_SHADER);
	const char* p = &pixelShaderCode.c_str()[0];
	glShaderSource(px, 1, &p, nullptr);
	const char* a = &vertexShaderCode.c_str()[0];
	glShaderSource(vx, 1, &a, nullptr);
	glCompileShader(px);
	
	glCompileShader(vx);
	int success = 0;
	glGetShaderiv(vx, GL_COMPILE_STATUS, &success);

	m_glShaderProgID = glCreateProgram();
	glAttachShader(m_glShaderProgID, px);
	glAttachShader(m_glShaderProgID, vx);

	glLinkProgram(m_glShaderProgID);
	glUseProgram(m_glShaderProgID);
	
	glDeleteShader(px);
	glDeleteShader(vx);
	
	
	if(wglSwapIntervalEXT != nullptr)
		wglSwapIntervalEXT(0);

	
}

s2d::Simp2DGraphics::Simp2DGraphics(){}



s2d::Simp2DGraphics::~Simp2DGraphics(){
	//delete[] m_rects;
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



void s2d::Simp2DGraphics::drawRect(FLOAT32 x, FLOAT32 y, FLOAT32 width, FLOAT32 height, PIXCOLOR color)
{
	if (m_rectExistCount >= m_maxRectCount - 1) resizeRectCount();
	if (m_rects == nullptr)return;

	UINT32 index = m_rectExistCount;
	
	float fx = (float)((float)(x / (float)m_scrWidth) * 2) - 1;
	float fy = (float)((float)(y / (float)m_scrHeight) * 2) - 1;
	float fOffX = (float)((float)((x + width) / (float)m_scrWidth) * 2) - 1;
	float fOffY = (float)((float)((y + height) / (float)m_scrHeight) * 2) - 1;
	float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;
	m_rects[index].vertex[0] = { fx,	fy,		fR, fG, fB, fA};
	m_rects[index].vertex[1] = { fx,	fOffY,	fR, fG, fB, fA };
	m_rects[index].vertex[2] = { fOffX, fOffY,	fR, fG, fB, fA };
	m_rects[index].vertex[3] = { fOffX, fy,		fR, fG, fB, fA };
	
	m_rectExistCount++;
	
}



void s2d::Simp2DGraphics::flushBuffer() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	if (m_rectExistCount > 0)
		glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(Rect) * m_rectExistCount), m_rects);
	glDrawElements(GL_TRIANGLES, 6 * m_rectExistCount, GL_UNSIGNED_INT, nullptr);
	m_rectExistCount = 0;
	
}
 
