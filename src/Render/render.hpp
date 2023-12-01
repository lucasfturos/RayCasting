#pragma once

#include "../Raycaster/raycaster.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <random>
#include <sstream>

class Render {
  private:
    const int WIDTH = 1080;
    const int HEIGHT = 720;
    const int FPS = 16;

    const float characterWidth = 20.0f;
    const float characterHeight = 20.0f;

    int getTilemapWidth() const { return wordlMapWidth; }
    int getTilemapHeight() const { return wordlMapHeight; }

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Surface *worldMapSurface;
    SDL_Texture *worldMapTexture;
    SDL_Color objColor = {255, 0, 0, 255};

  private:
    Map map;
    Mouse mousePos;
    Position initPosWindow = {
        .x = WIDTH / 2.0f,
        .y = HEIGHT / 2.0f,
    };
    Position playerPos = initPosWindow;
    Position cameraPos = initPosWindow;
    Velocity playerVelocity = {0.0f, 0.0f};

    int wordlMapWidth;
    int wordlMapHeight;
    Uint32 lastTime = SDL_GetTicks();

    float speed = 150.0f;
    float deltaTime = 0.0f;
    float radiusObjPlayer = 30.0f;

    bool quit = false;

    shared_ptr<Raycaster> raycaster;

  private:
    // Draw
    void draw();
    void drawMiniMap();
    // Draw minimap
    void drawPlayerOnMiniMap(int x, int y, float scaleX, float scaleY);
    void drawCircle(SDL_Renderer *renderer, float x, float y, int radius,
                    SDL_Color color);
    void drawPlayerVisionOnMiniMap(int x, int y, float scaleX, float scaleY);
    void drawTilemapOnMiniMap(int x, int y, int width, int height);
    // Draw World Raycasting
    void drawPlayerVision();
    void drawVerticalColumn(int column, float wallHeight);
    void drawRaycasterResults(const std::vector<Raycaster::RayResult> &results);

    // Window
    void initWindow();
    void loadTilemap();
    Uint32 getPixel(SDL_Surface *surface, int x, int y, Uint32 threshold);

    // Update
    void updateMovements();
    void updatePlayerPosition();
    void updateCameraPosition();
    bool checkPlayerCollision(Position next);

    // Events
    void handleEvents();
    void handleQuitEvent();
    void handleMouseMotionEvent();
    void handleKeyUpEvent(const SDL_Event &event);
    void handleKeyDownEvent(const SDL_Event &event);

  public:
    Render();
    ~Render();
    void run();
};