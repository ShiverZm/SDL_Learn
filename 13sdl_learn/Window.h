#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <stdexcept>
#include <memory>
#include <SDL2/SDL.h>
#include <functional>
#include <vector>

//What we think our window class should look like
class Window {
public:
    //Start SDL and TTF, create the window and renderer
    static void Init(std::string title = "Window",int screenWidth = 640,int screenHeight = 480);
    //Quit SDL and TTF
    static void Quit();
    //Draw an SDL_Texture
    static void Draw(SDL_Texture* texture, SDL_Rect& dstRect, SDL_Rect* clip = NULL,
        float degrees = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flipType = SDL_RendererFlip::SDL_FLIP_NONE);
    //Load an image
    static SDL_Texture* LoadImage(std::string file);
    //Render some text
    static SDL_Texture* RenderText(std::string strText, std::string file, SDL_Color color, int fontSize);

    static void HandleEvents(std::function<void(SDL_Event&)>cbEventFunction, std::function<void(void)>cbRenderFunction = NULL);
    //Clear window
    static void Clear();
    //Present renderer
    static void Present();
    //Get the window's box
    static SDL_Rect Box();

public:
    static std::unique_ptr<SDL_Window, void (*)(SDL_Window*)> m_pWindow;
    static std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer*)> m_pRenderer;
    static SDL_Rect m_pBox;
    static bool m_eventQuit;

    static int SCREEN_WIDTH;
    static int SCREEN_HEIGHT;
};

#endif