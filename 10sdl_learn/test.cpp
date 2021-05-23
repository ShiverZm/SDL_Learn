#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Window.h"

int main(int argc, char* argv[])
{
	Window::Init("Draw A Rect");

	int paint = 0;
	Window::HandleEvents([&paint](SDL_Event e) {
		if (e.key.keysym.sym == SDLK_1) {
			paint = 1;
		}
		else if (e.key.keysym.sym == SDLK_2) {
			paint = 2;
		}
		else if (e.key.keysym.sym == SDLK_3) {
			paint = 3;
		}
		else if (e.key.keysym.sym == SDLK_4) {
			paint = 4;
		}
		}, [&paint]() {
			//Clear screen
			SDL_SetRenderDrawColor(Window::m_pRenderer.get(), 0x00, 0x00, 0x00, 0x00);
			Window::Clear();

			if (paint == 1) {
				//渲染红色填充四边形
				SDL_Rect fillRect = { Window::SCREEN_WIDTH / 4, Window::SCREEN_HEIGHT / 4, Window::SCREEN_WIDTH / 2, Window::SCREEN_HEIGHT / 2 };
				SDL_SetRenderDrawColor(Window::m_pRenderer.get(), 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(Window::m_pRenderer.get(), &fillRect);
			}
			else if (paint == 2) {
				//渲染绿色轮廓的四边形
				SDL_Rect outlineRect = { Window::SCREEN_WIDTH / 6, Window::SCREEN_HEIGHT / 6, Window::SCREEN_WIDTH * 2 / 3, Window::SCREEN_HEIGHT * 2 / 3 };
				SDL_SetRenderDrawColor(Window::m_pRenderer.get(), 0x00, 0xFF, 0x00, 0xFF);
				SDL_RenderDrawRect(Window::m_pRenderer.get(), &outlineRect);
			}
			else if (paint == 3) {
				//画蓝色横线
				SDL_SetRenderDrawColor(Window::m_pRenderer.get(), 0x00, 0x00, 0xFF, 0xFF);
				SDL_RenderDrawLine(Window::m_pRenderer.get(), 0, Window::SCREEN_HEIGHT / 2, Window::SCREEN_WIDTH, Window::SCREEN_HEIGHT / 2);
			}
			else if (paint == 4) {
				//画出黄点的垂直线
				SDL_SetRenderDrawColor(Window::m_pRenderer.get(), 0xFF, 0xFF, 0x00, 0xFF);
				for (int i = 0; i < Window::SCREEN_HEIGHT; i += 4)
				{
					SDL_RenderDrawPoint(Window::m_pRenderer.get(), Window::SCREEN_WIDTH / 2, i);
				}
			}
			Window::Present();
		});
}
