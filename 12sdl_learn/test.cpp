/**
 * 最简单的SDL2播放视频的例子（SDL2播放RGB/YUV）
 * Simplest Video Play SDL2 (SDL2 play RGB/YUV)
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序使用SDL2播放RGB/YUV视频像素数据。SDL实际上是对底层绘图
 * API（Direct3D，OpenGL）的封装，使用起来明显简单于直接调用底层
 * API。
 *
 * 函数调用步骤如下:
 *
 * [初始化]
 * SDL_Init(): 初始化SDL。
 * SDL_CreateWindow(): 创建窗口（Window）。
 * SDL_CreateRenderer(): 基于窗口创建渲染器（Render）。
 * SDL_CreateTexture(): 创建纹理（Texture）。
 *
 * [循环渲染数据]
 * SDL_UpdateTexture(): 设置纹理的数据。
 * SDL_RenderCopy(): 纹理复制给渲染器。
 * SDL_RenderPresent(): 显示。
 *
 * This software plays RGB/YUV raw video data using SDL2.
 * SDL is a wrapper of low-level API (Direct3D, OpenGL).
 * Use SDL is much easier than directly call these low-level API.
 *
 * The process is shown as follows:
 *
 * [Init]
 * SDL_Init(): Init SDL.
 * SDL_CreateWindow(): Create a Window.
 * SDL_CreateRenderer(): Create a Render.
 * SDL_CreateTexture(): Create a Texture.
 *
 * [Loop to Render data]
 * SDL_UpdateTexture(): Set Texture's data.
 * SDL_RenderCopy(): Copy Texture to Render.
 * SDL_RenderPresent(): Show.
 */

#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "Window.h"

int main() {

	Window::Init("Zoom and Rotate Pic");

	SDL_Texture* arrowTexture = Window::LoadImage("../../res/11sdl_learn/arrow.png");

	SDL_Texture* cropPic = Window::LoadImage("../../res/05sdl_learn/image.png");

	Window::Clear();

	float degrees = 0.0f;

	SDL_RendererFlip flipType = SDL_FLIP_NONE;

	//Window::Draw(arrowTexture, Window::Box(), NULL, degrees, NULL, flipType);

	//Window::Present();// SDL_RenderPresent来更新屏幕的画面


	int iW = 100, iH = 100;
	int x = Window::SCREEN_WIDTH / 2 - iW / 2;
	int y = Window::SCREEN_HEIGHT / 2 - iH / 2;
	//Setup the clips for our image
	SDL_Rect clips[4];
	int useClip = -1;
	//Since our clips our uniform in size we can generate a list of their
	//positions using some math (the specifics of this are covered in the lesson)
	for (int i = 0; i < 4; ++i) {
		clips[i].x = i / 2 * iW;
		clips[i].y = i % 2 * iH;
		clips[i].w = iW;
		clips[i].h = iH;
		SDL_Log("(%d,%d)", clips[i].x, clips[i].y);
	}

	Window::HandleEvents([&degrees, &flipType, &useClip](SDL_Event e) {
		//User requests quit
		if (e.type == SDL_QUIT)
		{
			Window::m_eventQuit = true;
		}
		else if (e.type == SDL_KEYDOWN)
		{
			useClip = -1;
			switch (e.key.keysym.sym)
			{
			case SDLK_a:
				degrees -= 60;
				break;
			case SDLK_d:
				degrees += 60;
				break;
			case SDLK_q:
				flipType = SDL_FLIP_HORIZONTAL;
				break;
			case SDLK_w:
				flipType = SDL_FLIP_NONE;
				break;
			case SDLK_e:
				flipType = SDL_FLIP_VERTICAL;
				break;
			case SDLK_1:
				useClip = 0;
				break;
			case SDLK_2:
				useClip = 1;
				break;
			case SDLK_3:
				useClip = 2;
				break;
			case SDLK_4:
				useClip = 3;
				break;
			}
		}
		}, [&degrees, &flipType, &arrowTexture,&cropPic, &clips,&useClip]() {
			
			if (useClip == -1) {//rotate
				Window::Draw(arrowTexture, Window::Box(), NULL, degrees, NULL, flipType);
			}
			else {//zoom
				Window::Draw(cropPic, Window::Box(), &clips[useClip], degrees, NULL, flipType);
			}
			
			
		});

	SDL_Quit();
	return 0;
}
