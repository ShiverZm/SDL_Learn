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
    static void Init(std::string title);
    //Quit SDL and TTF
    static void Quit();
    //Draw an SDL_Texture
    static void Draw(SDL_Texture* tex, SDL_Rect& dstRect, SDL_Rect* clip,
        float angle, int xPivot, int yPivot,SDL_RendererFlip flip);
    //Load an image
    static SDL_Texture* LoadImage(std::string file);
    //Render some text
    static SDL_Texture* RenderText(std::string strText, std::string file, SDL_Color color, int fontSize);
    static void HandleEvents(const std::function<void(SDL_Event&)>& cbFunction);
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
};

#endif