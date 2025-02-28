#pragma once

#include "Raycaster/raycaster.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <sstream>

using std::shared_ptr;

struct Color {
    Uint8 r, g, b;
};

class Render {
  private:
    const int FPS = 16;

    const float characterWidth = 20.0f;
    const float characterHeight = 20.0f;

    int getTilemapWidth() const { return worldMapWidth; }
    int getTilemapHeight() const { return worldMapHeight; }

  private:
    SDL_Window *window;
    SDL_Renderer *renderer;

    SDL_Texture *wallTexture;
    SDL_Texture *floorTexture;
    SDL_Texture *ceilingTexture;
    SDL_Texture *worldMapTexture;

    SDL_Surface *wallSurface;
    SDL_Surface *floorSurface;
    SDL_Surface *ceilingSurface;
    SDL_Surface *worldMapSurface;

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

    float dirX = -1.0f, dirY = 0.0f;
    float planeX = 0.0f, planeY = 0.6f;
    int worldMapWidth;
    int worldMapHeight;
    Uint32 lastTime = SDL_GetTicks();

    float speed = 15.0f;
    float deltaTime = 0.0f;
    float radiusObjPlayer = 20.0f;
    float cameraAngle = 0.0f;

    bool quit = false;
    bool showMiniMap = false;

    std::vector<std::vector<Color>> wallTextures;

    shared_ptr<Raycaster> raycaster;

  private:
    // Draw
    void draw();
    // Draw minimap
    void drawMiniMap();
    void drawPlayerOnMiniMap(int x, int y, float scaleX, float scaleY);
    void drawCircle(SDL_Renderer *renderer, float x, float y, int radius,
                    SDL_Color color);
    void drawTilemapOnMiniMap(int x, int y, int width, int height);
    void drawPlayerVisionOnMiniMap(int x, int y, float scaleX, float scaleY);
    // Draw World Raycasting
    void drawPlayerVision();
    void drawFloor(int column, float wallHeight);
    void drawCeiling(int column, float wallHeight);
    void drawRaycaster(const vector<Raycaster::Ray> &rays);
    void drawWall(int column, float wallHeight, int side, float distanceToWall);

    // Window
    void loadWall();
    void loadFloor();
    void initWindow();
    void initTexture();
    void loadCeiling();
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

    void destroySurface();
    void destroyTexture();

  public:
    Render();
    ~Render();
    void run();
};
