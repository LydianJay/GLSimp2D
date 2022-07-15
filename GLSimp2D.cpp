
#include "GLSimp2D.h"


s2d::Simp2D::Simp2D(UINT32 width, UINT32 height, UINT32 tileRows, UINT32 tileCols) {
	LoadGL();
	m_scrWidth = width;
	m_scrHeight = height;
	m_pixelCol = tileCols;
	m_pixelRow = tileRows;

	m_fPixelW = (float)width / tileRows;
	m_fPixelH = (float)height / tileCols;

	m_pixelBuffer = new PIXCOLOR[tileRows * tileCols];
	PIXCOLOR color = {0,0,0,0};

	memset(m_pixelBuffer, 0, sizeof(PIXCOLOR) * tileRows * tileCols);

	
	int size = offsetof(Vertex, Vertex::col);


	m_rects = new Rect[m_pixelCol * m_pixelRow];
	float fOffX = (float)m_fPixelW / m_scrWidth * 2;
	float fOffY = (float)m_fPixelH / m_scrHeight * 2;
	for (size_t y = 0; y < m_pixelCol; y++) {

		for (size_t x = 0; x < m_pixelRow; x++) {

			float fx = (float)((float)(m_fPixelW * x / (float)m_scrWidth) * 2) - 1;

			float fy = (float)((float)(m_fPixelH * y / (float)m_scrHeight) * 2) - 1;
			PIXCOLOR px = m_pixelBuffer[x + (m_pixelCol - (y + 1)) * m_pixelRow];
			
			m_rects[x + y * m_pixelRow].vertex[0] = { {fx, fy},							{0.0f, 0.0f, 0.0f, 1.0f} };
			m_rects[x + y * m_pixelRow].vertex[1] = { {fx, fy + fOffY},					{0.0f, 0.0f, 0.0f, 1.0f} };
			m_rects[x + y * m_pixelRow].vertex[2] = { {fx + fOffX, fy + fOffY},			{0.0f, 0.0f, 0.0f, 1.0f} };
			m_rects[x + y * m_pixelRow].vertex[3] = { {fx + fOffX, fy},					{0.0f, 0.0f, 0.0f, 1.0f} };
			

			
		}
	}



	glGenVertexArrays(1, &m_glVertexObjectID);
	glBindVertexArray(m_glVertexObjectID);
	glGenBuffers(1, &m_glVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, m_glVertexBufferID);

	int r = m_pixelCol * m_pixelRow;
	m_glVertexCount = r * 4;
	
	/*Rect fuck;

	fuck.vertex[0].pos = { 0.0f, 0.0f };
	fuck.vertex[1].pos = { 0.0f, 0.5f };
	fuck.vertex[2].pos = { 0.5f, 0.5f };
	fuck.vertex[3].pos = { 0.5f, 0.0f };
	*/
	

	//glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Rect)), &fuck, GL_DYNAMIC_DRAW);
	
	glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)(sizeof(Rect)* r), m_rects, GL_DYNAMIC_DRAW);
	GLsizei stride = sizeof(Vertex);

	


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

s2d::Simp2D::~Simp2D(){
	delete[] m_pixelBuffer;
}

void s2d::Simp2D::drawRect(UINT32 x, UINT32 y, UINT32 width, UINT32 height, PIXCOLOR color) {
 
	y = m_pixelCol - y - 1;
	
	
	for (UINT32 col = y; col < height + y; col++) {

		for (UINT32 row = x; row < width + x; row++) {
			
			float fR = (float)color.R / 255, fG = (float)color.G / 255, fB = (float)color.B / 255, fA = (float)color.A / 255;
			m_rects[row + col * m_pixelRow].vertex[0].col = { fR, fG, fB, fA };
			m_rects[row + col * m_pixelRow].vertex[1].col = { fR, fG, fB, fA };
			m_rects[row + col * m_pixelRow].vertex[2].col = { fR, fG, fB, fA };
			m_rects[row + col * m_pixelRow].vertex[3].col = { fR, fG, fB, fA };
		}

	}

	
}

void s2d::Simp2D::fillRect(UINT32 x, UINT32 y, UINT32 width, UINT32 height, PIXCOLOR color)
{
}

void s2d::Simp2D::drawTriangle(UINT32 p1, UINT32 p2, UINT32 p3, PIXCOLOR color)
{
}

void s2d::Simp2D::fillTriangle(UINT32 p1, UINT32 p2, UINT32 p3, PIXCOLOR color)
{
}

void s2d::Simp2D::drawCircle(UINT32 origX, UINT32 origY, FLOAT32 fRadius, PIXCOLOR color)
{
}

void s2d::Simp2D::fillCircle(UINT32 origX, UINT32 origY, FLOAT32 fRadius, PIXCOLOR color)
{
}

void s2d::Simp2D::flushBuffer() {
	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, (GLsizeiptr)(sizeof(Rect) * m_pixelCol * m_pixelRow), m_rects);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, m_glVertexCount);
	
	/*for (size_t y = 0; y < m_pixelCol; y++) {

		for (size_t x = 0; x < m_pixelRow; x++) {
			
			float fx = (((float)m_fPixelW * x / m_scrWidth) * 2) - 1;
			
			float fy = (( m_fPixelH * y / m_scrHeight) * 2) - 1;
			PIXCOLOR px = m_pixelBuffer[x + (m_pixelCol - (y + 1)) * m_pixelRow];
			glColor4f((float)px.R / 255, px.G / 255, px.B / 255, px.A / 255);
			glBegin(GL_QUADS);	
			
			glVertex2f(fx, fy);
			glVertex2f(fx, fy + m_fPixelH);
			glVertex2f(fx + m_fPixelW, fy + m_fPixelH);
			glVertex2f(fx + m_fPixelW, fy);
			glEnd();
			
		}
	}*/
	
}





