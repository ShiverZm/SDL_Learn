#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <windows.h> 
#include <iostream>

/*******************************
	sdl渲染一张图片
*******************************/

int loadBMP() {
	if (SDL_Init(SDL_INIT_VIDEO))
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);

	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	
	// SDL_CreateRenderer 窗口句柄 指定显卡驱动（-1为自动选择） 
	// SDL_RENDERER_ACCELERATED，因为我们想使用硬件加速的renderer 
	// SDL_RENDERER_PRESENTVSYNC标志，因为我们想要使用SDL_RendererPresent这个函数，这个函数将会以显示器的刷新率来更新画面
	SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRender Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::string imagePath = "F:/workspace/project/vs2019/example/SDL_Learn/res/01sdl_learn/preview2.bmp";
	//std::string imagePath = "D:\\workspace\\vs2015\\example\\sdl_learn\\res\\01sdl_learn\\preview2.jpg";

	
	//Surface 可以把它理解为一个Buffer，它是一块屏幕缓冲区。
	//每个Window(窗口)对应一个Surface，任何View都是画在Surface上的
	//SDL_Surface *bmp = SDL_LoadBMP(imagePath.c_str());
	SDL_Surface *bmp = IMG_Load(imagePath.c_str());
	if (bmp == nullptr)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	//SDL_Texture来用硬件加速绘制图像
	SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, bmp);

	SDL_FreeSurface(bmp);
	if (tex == nullptr)
	{
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}


	SDL_RenderClear(ren);
	//texture画上去
	//第一个NULL是一个指向源矩形的指针，也就是说，从图像上裁剪下的一块矩形
	//而另一个是指向目标矩形的指针。我们将NULL传入这两个参数，
	//是告诉SDL绘制整个源图像（第一个NULL），并把它画在屏幕上（0，0 ）的位置，并拉伸这个图像让它填满整个窗口（第二个NULL）
	SDL_RenderCopy(ren, tex, NULL, NULL);// 
	SDL_RenderPresent(ren);// SDL_RenderPresent来更新屏幕的画面
	SDL_Delay(1000);


	//必要释放掉我们这个窗口、renderer还有texture所用的全部内存
	SDL_DestroyTexture(tex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
}


int  main(int argc, char* args[])
{
	
	loadBMP();

	system("pause");
	return 0;

}
