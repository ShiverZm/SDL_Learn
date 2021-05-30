#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Window.h"


#include <thread>
int main() {

	Window::Init("Simple Animation");


	SDL_Texture* move[4] = { nullptr };

	move[0] = Window::LoadImage("../../res/14sdl_learn/move.0.png");

	move[1] = Window::LoadImage("../../res/14sdl_learn/move.1.png");

	move[2]= Window::LoadImage("../../res/14sdl_learn/move.2.png");

	move[3] = Window::LoadImage("../../res/14sdl_learn/move.3.png");


	Window::Clear();

	float degrees = 0.0f;

	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	//Window::Draw(arrowTexture, Window::Box(), NULL, degrees, NULL, flipType);

	//Window::Present();// SDL_RenderPresent来更新屏幕的画面

	int index = 0;
	Window::HandleEvents([&degrees, &flipType, &index](SDL_Event e) {
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			Window::m_eventQuit = true;
		}
		else if (e.key.keysym.sym == SDLK_0) {
			index = 0;
		}
		else if (e.key.keysym.sym == SDLK_1) {
			index = 1;
		}
		else if (e.key.keysym.sym == SDLK_2) {
			index = 2;
		}
		else if (e.key.keysym.sym == SDLK_3) {
			index = 3;
		}
		}, [&degrees, &flipType, &move,&index]() {
				
			if (index == 3) {
				index = 0;
			}

			Window::Draw(move[index++], Window::Box(), NULL, degrees, NULL, flipType);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		});

	SDL_Quit();
	return 0;
}
