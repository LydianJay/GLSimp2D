#include "GLSimp2DGraphics.h"
#include "BECS/becs.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


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

	s2d::S2D_COLOR color;
	s2d::TEXID		texID;
	static unsigned int ID;
};
unsigned int Color::ID = 0;
unsigned int Vel::ID = 0;
unsigned int Rect::ID = 0;

struct Object {
	Rect rect;
	Vel vel;
	s2d::S2D_COLOR color;
};

class TextProgram {
public:
	TextProgram() {

		

		srand(time(0));
		w = 600;
		h = 600;
		prog = this;
		window = s2d::S2DWindow(w, h, "Test", S2D_WINDOW_NO_RESIZE);
		window.CreateOpenGLContext(3, 3, S2D_GL_CONTEXT_CORE_PROFILE);
		render = std::move(s2d::S2DGraphics(window));
		render.setBlendingState(false);
		Rect::ID = ecs.registerComponent(sizeof(Rect));
		Vel::ID = ecs.registerComponent(sizeof(Vel));
		Color::ID = ecs.registerComponent(sizeof(Color));

		playerX = w / 2;
		playerY = h / 2;
		
		int width, height, crap;
		
		unsigned char * img  = stbi_load("res/logo.png", &width, &height, &crap, 4);
		tex = render.createTexture(img, width, height);
		textures.push_back(tex);
		stbi_image_free(img);


		img = stbi_load("res/character.png", &width, &height, &crap, 4);
		s2d::Texture t = render.createTexture(img, width, height);		
		stbi_image_free(img);

		pokemonSprtSht.createSpriteSheet(t, 64, 64);

		playerRect = s2d::S2DRect(playerX, playerY, 50, 50);
		

		

		img = stbi_load("res/tileset.png", &width, &height, &crap, 4);
		s2d::Texture b = render.createTexture(img, width, height);
		stbi_image_free(img);


		tileset.createSpriteSheet(b, 64, 64);

		createRect();
		window.SetKeyCallFunc(keylistener);
	}

	
	void playerUpdate(float fElapseTime) {

		s2d::KeyState state = window.GetKeyState();
		float fSpeed = 120;
		if (state.state == S2D_KEY_PRESSED) {
			row++;
			switch (state.key)
			{
			case 'W':
				//playerY += fSpeed * fElapseTime;
				playerRect.moveRect(0,-fSpeed * fElapseTime);
				col = 3;
				break;

			case 'S':
				//playerY -= fSpeed * fElapseTime;
				playerRect.moveRect(0, (fSpeed * fElapseTime));
				col = 0;
				break;

			case 'D':
				//playerX += fSpeed * fElapseTime;
				playerRect.moveRect(fSpeed * fElapseTime,0);
				col = 2;
				break;

			case 'A':
				//playerX -= fSpeed * fElapseTime;
				playerRect.moveRect(-(fSpeed * fElapseTime), 0);
				col = 1;
				break;

			default:
				break;
			}

			if (row >= 4)row = 0;
			if (col >= 4)col = 0;

		}
		if (state.state == S2D_KEY_PRESSED && state.key == VK_UP) {
			fAngle += fElapseTime * 60;
		}
		else if (state.state == S2D_KEY_PRESSED && state.key == VK_DOWN) {
			fAngle -= fElapseTime * 60;
		}

		playerRect.setTexture((s2d::Texture&)pokemonSprtSht.getTexture(row, col, true));
		
		//playerRect.setTexture((s2d::Texture&)pokemonSprtSht.getTexture(0, false, false));
		render.drawRotatedRect(playerRect, fAngle);
		

		
	}

	void drawMap() {
		float fOffsetX = w / mapRow;
		float fOffsetY = h / mapCol;
		for (int y = 0; y < mapCol; y++) {


			for (int x = 0; x < mapRow; x++) {
				
				char c = map[x + y * mapRow];
				float fx = x * fOffsetX;
				float fy = y * fOffsetY;

				s2d::Texture tex;


				switch (c) {

				case '#':
					tex = tileset.getTexture(1, 1, false);
					break;
				case 'L':
					tex = tileset.getTexture(0, 1, false);
					break;
				case 'R':
					tex = tileset.getTexture(2, 1, false);
					break;
				case 'T':
					tex = tileset.getTexture(1, 2, false);
					break;
				case 'B':
					tex = tileset.getTexture(1, 0, false);

					break;
				default:
					tex = tileset.getTexture(1, 3, false);
					break;
				}


				render.drawRect(fx, fy, fOffsetX, fOffsetY, s2d::PXLCLR_WHITE, tex);


				
				
			}
		}

		
	}

	void rotate(float fElapseTime) {

		s2d::KeyState state = window.GetKeyState();

		if (state.state == S2D_KEY_PRESSED && state.key == VK_UP) {
			fAngle += fElapseTime * 60;
		}
		else if (state.state == S2D_KEY_PRESSED && state.key == VK_DOWN) {
			fAngle -= fElapseTime * 60;
		}
	}

	void update() {

		float fTimeAccum = 0.0f;
		unsigned int fpsTick = 0, accum = 0, count = 0;
		float posX = 25, posY = 25;
		float fElapseTime = 0.0f;
		while (!window.WindowShouldClose()) {
			auto t1 = std::chrono::high_resolution_clock::now();



			render.drawRect(0, 0, 50, 50, s2d::PXLCLR_WHITE, pokemonSprtSht.getTexture(0,0, true));
			
			render.flushBuffer();
			window.SwapWindowBuffers();
			window.ProcessMessage();

			int error = glGetError();
			if (error != 0) {
				std::cout << "Error Code: " << error << '\n';
			}

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
 				//std::cout << "FPS: " << fpsTick << " AVERAGE: " << averageFPS << "Rects: " << rects.size() << '\n';

				

				fpsTick = 0;
			}

		}

	}


private:
	s2d::S2DWindow		window;
	s2d::S2DGraphics		render;
	std::vector<EntityID>	rects;
	static TextProgram*		prog;
	s2d::Texture			tex;
	s2d::S2DSpriteSheet		pokemonSprtSht;
	s2d::S2DSpriteSheet		tileset;
	s2d::S2DRect			playerRect;

	BECS ecs;
	std::vector<s2d::Texture> textures;
	UINT32					w, h;
	const UINT32			mapRow = 10, mapCol = 10;
	UINT					row = 0, col = 0;
	float					playerX, playerY;
	float					fAngle = 0;
	const char* map =
		"T L##RL#RR "
		"#R BB  #R#"
		"#R TT   L#"
		"  L##R  L#"
		"  L##R    "
		"#R BB   L#"
		"#R     TL#"
		"B     ##RT"
		"#R     BL#"
		"##RL#L#R";


	static void keylistener(s2d::S2DWindow*, unsigned char key, unsigned char state) {
		

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
			if (tempRect->x <= 0 || tempRect->x >= w - tempRect->w) tempVel->x *= -1;
			if (tempRect->y <= 0 || tempRect->y >= h - tempRect->h) tempVel->y *= -1;

			

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
		int index = rand() % textures.size();
		Rect tempRect = {(rand() % 300) + 20, (rand() % 300) + 20,40,40 };
		Vel tempVel = { (float)(rand() % 150) * ((rand() % 2 == 1) ? -1 : 1), (float)(rand() % 150) * ((rand() % 2 == 1) ? -1 : 1) };
		//Color tempCol = { {(rand() % 255), (rand() % 255),(rand() % 255), (rand() % 255)}, -1 };
		
		Color tempCol = { {165,165,165,255},  -1 };


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