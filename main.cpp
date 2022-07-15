#include "wingui.h"
#include "GLSimp2D.h"
#include <iostream>
#include <chrono>



int main() {

	CWindow window(600, 600, "Fuck", WG_WINDOW_NO_RESIZE, WG_WIN_COL_BLUE);
	window.CreateOpenGLContext(3,3, WG_GL_CONTEXT_CORE_PROFILE);
	
	
	s2d::Simp2D render(600, 600, 50, 50);

	
	




	float fElapseTime = 0.0f;
	int fpsTick = 0;
	
	while (!window.WindowShouldClose()) {
		auto t1 = std::chrono::high_resolution_clock::now();
		render.drawRect(0, 0, 1, 1, s2d::PXLCLR_BLUE);
		render.drawRect(1, 0, 1, 1, s2d::PXLCLR_BLUE);
		render.drawRect(0, 1, 1, 1, s2d::PXLCLR_RED);
		render.drawRect(1, 1, 1, 1, s2d::PXLCLR_RED);
		render.flushBuffer();
		
		

		window.SwapWindowBuffers();
		window.ProcessMessage();
		
		auto t2 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = t2 - t1;
		fElapseTime += duration.count();
		fpsTick++;
		if (fElapseTime >= 1) {
			fElapseTime = 0;
			std::cout << "FPS: " << fpsTick << '\n';
			
			fpsTick = 0;
		}

	}

	
	return 0;
}