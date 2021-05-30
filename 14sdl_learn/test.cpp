#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Window.h"

#include <mutex>

std::mutex actionMutex;

#include <thread>
int main() {

	Window::Init("Simple Animation");


	SDL_Texture* move[4] = { nullptr };

	move[0] = Window::LoadImage("../../res/14sdl_learn/penguin/move/move.0.png");

	move[1] = Window::LoadImage("../../res/14sdl_learn/penguin/move/move.1.png");

	move[2]= Window::LoadImage("../../res/14sdl_learn/penguin/move/move.2.png");

	move[3] = Window::LoadImage("../../res/14sdl_learn/penguin/move/move.3.png");

	SDL_Texture* stand[3] = { nullptr };

	stand[0] = Window::LoadImage("../../res/14sdl_learn/penguin/stand/stand.0.png");

	stand[1] = Window::LoadImage("../../res/14sdl_learn/penguin/stand/stand.1.png");

	stand[2] = Window::LoadImage("../../res/14sdl_learn/penguin/stand/stand.2.png");


	SDL_Texture* attack[9] = { nullptr };

	attack[0] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.0.png");

	attack[1] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.1.png");

	attack[2] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.2.png");

	attack[3] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.3.png");

	attack[4] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.4.png");

	attack[5] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.5.png");


	attack[6] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.6.png");

	attack[7] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.7.png");

	attack[8] = Window::LoadImage("../../res/14sdl_learn/penguin/attack/attack1.8.png");


	//SDL_Texture* bg = Window::LoadImage("../../res/14sdl_learn/background/Wizet.15.png");

	Window::Clear();

	float degrees = 0.0f;

	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	//Window::Draw(arrowTexture, Window::Box(), NULL, degrees, NULL, flipType);

	//Window::Present();// SDL_RenderPresent来更新屏幕的画面

	int action = 0;//1 move;2 attack;
	int index = 0;
	int max = 3;
	Window::HandleEvents([&degrees, &flipType, &action, &max, &index](SDL_Event e) {
		std::lock_guard<std::mutex> guard(actionMutex);
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			Window::m_eventQuit = true;
		}
		else if (e.key.keysym.sym == SDLK_LEFT) {

			if (e.key.state == SDL_PRESSED) {
				action = 1;
				max = 4;
			}
			if (e.key.state == SDL_RELEASED) {
				action = 0;
				max = 3;
				index = 0;
			}
		}
		else if (e.key.keysym.sym == SDLK_a) {
			if (e.key.state == SDL_PRESSED) {
				action = 2;
				max = 9;
			}
			if (e.key.state == SDL_RELEASED) {
				action = 0;
				max = 3;
				index = 0;
			}
		}
		}
		, [&degrees, &flipType,&bg, &attack,&move,&stand,&action,&index,&max]() {

			std::lock_guard<std::mutex> guard(actionMutex);
			if (index == max) {
				index = 0;
			}

			if (action == 0) {
				Window::Draw(stand[index++], Window::Box(), NULL, degrees, NULL, flipType);
			}
			else if (action == 1) {
				Window::Draw(move[index++], Window::Box(), NULL, degrees, NULL, flipType);
			}
			else if (action == 2) {
				Window::Draw(attack[index++], Window::Box(), NULL, degrees, NULL, flipType);
			}
			
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
		});

	SDL_Quit();
	return 0;
}
