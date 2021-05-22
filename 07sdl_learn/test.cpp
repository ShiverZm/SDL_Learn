#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Window.h"


int main() {
	//Show A Window
	//In main()
	//Load up an image and some text
	SDL_Texture* img, * msg;
	try {
		//Start our window
		Window::Init("Shiver's Window");

		int angle = 0;

		Window::HandleEvents([](SDL_Event e) {
			switch (e.key.keysym.sym) {
				case SDLK_q:
				case SDLK_ESCAPE:
					Window::m_eventQuit = true;
					break;
			}

			if (e.key.keysym.sym == SDLK_a) {
				printf("keydown A \n");
				Window::Clear();
				std::string fontFile = "..\\..\\res\\06sdl_learn\\sample.ttf";
				std::string text = "TTF Fonts too!";
				SDL_Color color = { 255, 255, 255 };
				SDL_Texture* texture = Window::RenderText(text, fontFile, color, 25);
				SDL_DestroyTexture(texture);
			}

			if (e.key.keysym.sym == SDLK_f) {
				printf("keydown F \n");
				Window::Clear();
				std::string imgFile = "..\\..\\res\\05sdl_learn\\image.png";
				SDL_Texture* image = Window::LoadImage(imgFile);
				SDL_DestroyTexture(image);
			}
		});



		Window::Quit();
	}
	catch (const std::runtime_error& e) {
		//Catch error and crash
		std::cout << e.what() << std::endl;
		Window::Quit();
		return -1;
	}
}