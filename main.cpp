#include "GLSimp2DGraphics.h"
#include "GLSimp2DWindow.h"
#include "BECS/becs.h"
#include <iostream>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <ctime>


struct Pos {
	float x, y;
	static unsigned int ID;
};

struct Vel {
	float x, y;
	static unsigned int ID;
};

struct Rect {
	float x, y, w, h;
	static unsigned int ID;
};

struct Color {

	s2d::PIXCOLOR color;
	static unsigned int ID;
};
unsigned int Color::ID = 0;
unsigned int Vel::ID = 0;
unsigned int Rect::ID = 0;

struct Object {
	Rect rect;
	Vel vel;
	s2d::PIXCOLOR color;
};

class TextProgram {
public:
	TextProgram() {
		srand(time(0));
		prog = this;
		window = s2d::Simp2DWindow(600, 600, "Fuck", S2D_NO_RESIZE);
		window.CreateOpenGLContext(3, 3, S2D_GL_CONTEXT_CORE_PROFILE);
		render = s2d::Simp2DGraphics(600, 600);
		
		Rect::ID = ecs.registerComponent(sizeof(Rect));
		Vel::ID = ecs.registerComponent(sizeof(Vel));
		Color::ID = ecs.registerComponent(sizeof(Color));
		
		createRect();
		createRect();
		window.SetKeyCallFunc(keylistener);
	}

	

	void update() {

		float fTimeAccum = 0.0f;
		unsigned int fpsTick = 0, accum = 0, count = 0;
		float posX = 25, posY = 25;
		float fElapseTime = 0.0f;
		while (!window.WindowShouldClose()) {
			auto t1 = std::chrono::high_resolution_clock::now();

		

			rectUpdate(fElapseTime);

			render.flushBuffer();
			window.SwapWindowBuffers();
			window.ProcessMessage();

			auto t2 = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> duration = t2 - t1;
			fElapseTime = duration.count();
			fTimeAccum += fElapseTime;
			fpsTick++;
			if (fTimeAccum >= 1) {
				count++;
				accum += fpsTick;
				unsigned int averageFPS = accum / count;
				fTimeAccum = 0;
 				std::cout << "FPS: " << fpsTick << " AVERAGE: " << averageFPS << " Entity Count: " << rects.size() << '\n';

				fpsTick = 0;
			}

		}

	}


private:
	s2d::Simp2DWindow		window;
	s2d::Simp2DGraphics		render;
	std::vector<EntityID>	rects;
	static TextProgram*		prog;
	BECS ecs;

	static void keylistener(s2d::Simp2DWindow*, unsigned char key, unsigned char state) {
		

		if(state== S2D_KEY_PRESSED)
		switch (key)
		{
		case ' ':
			prog->createRect();
			break;
		case 'Q':
			prog->deleteRect();
			break;
		default:
			break;
		}

	}
	

	void rectUpdate(float fElapeseTime) {


		for (EntityID id : rects) {

			Rect* tempRect = (Rect*)ecs.getEntityComponent(id, Rect::ID);
			Vel* tempVel = (Vel*)ecs.getEntityComponent(id, Vel::ID);
			Color* tempCol = (Color*)ecs.getEntityComponent(id, Color::ID);

			tempRect->x += tempVel->x * fElapeseTime;
			tempRect->y += tempVel->y * fElapeseTime;
			if (tempRect->x <= 0 || tempRect->x >= 600 - tempRect->w) tempVel->x *= -1;
			if (tempRect->y <= 0 || tempRect->y >= 600 - tempRect->h) tempVel->y *= -1;

			prog->render.drawRect(tempRect->x, tempRect->y, tempRect->w, tempRect->h, tempCol->color);

		}

	

	}

	void deleteRect() {
		if (rects.empty())return;

		ecs.destroyEntity(rects.back());
		rects.pop_back();

	}

	void createRect() {
		
		EntityID id = ecs.createEntity();
		ecs.addComponent(id, Rect::ID);
		ecs.addComponent(id, Vel::ID);
		ecs.addComponent(id, Color::ID);

		Rect tempRect = {(rand() % 300) + 20, (rand() % 300) + 20,40,40 };
		Vel tempVel = { (float)(rand() % 25) * ((rand() % 2 == 1) ? -1 : 1), (float)(rand() % 25) * ((rand() % 2 == 1) ? -1 : 1) };
		s2d::PIXCOLOR tempCol = { (rand() % 255), (rand() % 255),(rand() % 255) };


		
		memcpy_s(ecs.getEntityComponent(id, Rect::ID), sizeof(Rect), &tempRect, sizeof(Rect));
		memcpy_s(ecs.getEntityComponent(id, Vel::ID), sizeof(Vel), &tempVel, sizeof(Vel));
		memcpy_s(ecs.getEntityComponent(id, Color::ID), sizeof(Color), &tempCol, sizeof(Color));

		//Object obj = { tempRect, tempVel, tempCol };

		
		

		rects.push_back(id);
	}

};

TextProgram* TextProgram::prog;




int main() {

	TextProgram prog;
	prog.update();


	

	
	return 0;
}