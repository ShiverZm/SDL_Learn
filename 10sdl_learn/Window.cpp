#include <string>
#include <stdexcept>
#include <memory>
#include <fstream>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <algorithm>
#include "Window.h"


//Initialize the unique_ptr's deleters here
std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> Window::m_pWindow = std::unique_ptr<SDL_Window, void (*)(SDL_Window*)>(nullptr, SDL_DestroyWindow);
std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> Window::m_pRenderer = std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)>(nullptr, SDL_DestroyRenderer);
bool Window::m_eventQuit = false;

//Other static members
SDL_Rect Window::m_pBox;

int Window::SCREEN_WIDTH = 640;
int Window::SCREEN_HEIGHT = 480;

void Window::Init(std::string title, int screenWidth, int screenHeight) {
    //initialize all SDL subsystems
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        throw std::runtime_error("SDL Init Failed");
    if (TTF_Init() == -1)
        throw std::runtime_error("TTF Init Failed");

    //Setup our window size
    m_pBox.x = 0;
    m_pBox.y = 0;
    m_pBox.w = screenWidth;
    m_pBox.h = screenHeight;

    SCREEN_WIDTH = screenWidth;
    SCREEN_HEIGHT = screenHeight;

    //Create our window
    m_pWindow.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, m_pBox.w, m_pBox.h, SDL_WINDOW_SHOWN));
    //Make sure it created ok
    if (m_pWindow == nullptr)
        throw std::runtime_error("Failed to create window");

    //Create the renderer
    m_pRenderer.reset(SDL_CreateRenderer(m_pWindow.get(), -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    //Make sure it created ok
    if (m_pRenderer == nullptr)
        throw std::runtime_error("Failed to create renderer");
}


void Window::Quit() {
    TTF_Quit();
    SDL_Quit();
}

void Window::Draw(SDL_Texture* texture, SDL_Rect& dstRect, SDL_Rect* clip,
    float degrees, SDL_Point * center,SDL_RendererFlip flipType)
{
    //Clear screen
    SDL_SetRenderDrawColor(Window::m_pRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    Window::Clear();


    //Set rendering space and render to screen
    SDL_Rect renderQuad = { dstRect.x, dstRect.y, dstRect.w, dstRect.h };

    //Set clip rendering dimensions
    if (clip != NULL)
    {
    	renderQuad.w = clip->w;
    	renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(Window::m_pRenderer.get(), texture, clip, &renderQuad, degrees, center, flipType);
    //Update screen
    Window::Present();
}

SDL_Texture* Window::LoadImage(std::string file) {
    SDL_Surface* bmp = IMG_Load(file.c_str());
    if (bmp == nullptr)
    {
        throw std::runtime_error("SDL_LoadBMP Error");
        std::cout << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    //SDL_Texture来用硬件加速绘制图像
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_pRenderer.get(), bmp);

     SDL_FreeSurface(bmp);
    if (tex == nullptr)
    {
        std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL_CreateTextureFromSurface Error");
        return nullptr;
    }

	return tex;
}

SDL_Texture* Window::RenderText(std::string message, std::string fontFile, SDL_Color color, int fontSize){
        //Open the font
        TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
        if (font == nullptr) {
            printf("TTF_OpenFont error");
            return nullptr;
        }
        //We need to first render to a surface as that's what TTF_RenderText returns, then
        //load that surface into a texture
        SDL_Surface* surf = TTF_RenderText_Blended(font, message.c_str(), color);
        if (surf == nullptr) {
            TTF_CloseFont(font);
            printf("TTF_RenderText error");
            return nullptr;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(m_pRenderer.get(), surf);
        if (texture == nullptr) {
            printf("CreateTexture");
            return nullptr;
        }
        //Clean up the surface and font
        SDL_FreeSurface(surf);
        TTF_CloseFont(font); 

        SDL_Rect* clip = nullptr;

        SDL_Rect pos = { m_pBox.x,m_pBox.y, m_pBox.w, m_pBox.h};

        //The angle to draw at, so we can play with it
        int angle = 0;

        SDL_RenderCopy(m_pRenderer.get(), texture, clip, &pos);

        SDL_RenderPresent(m_pRenderer.get());
        
        return texture;
}

void Window::HandleEvents(std::function<void(SDL_Event&)>cbEventFunction, std::function<void(void)> cbRenderFunction) {
    m_eventQuit = false;
    SDL_Event e;
    while (!m_eventQuit) {
        while (SDL_PollEvent(&e)) {
            cbEventFunction(e);
        }
        
        if (cbRenderFunction != nullptr) {
            cbRenderFunction();
        }
    }
}
void Window::Clear(){
    SDL_RenderClear(m_pRenderer.get());
}

void Window::Present() {
    SDL_RenderPresent(m_pRenderer.get());
}

SDL_Rect Window::Box() {
    //Update mBox to match the current window size
    SDL_GetWindowSize(m_pWindow.get(), &m_pBox.w, &m_pBox.h);
    return m_pBox;
}

