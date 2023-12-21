#include "render.hpp"

void Render::handleQuitEvent() { quit = true; }

void Render::handleMouseMotionEvent() {
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    float dx = mousePos.x - initPosWindow.x;
    float dy = mousePos.y - initPosWindow.y;
    float angle = atan2(dy, dx);

    cameraPos.x = initPosWindow.x + cos(angle) * radiusObjPlayer;
    cameraPos.y = initPosWindow.y + sin(angle) * radiusObjPlayer;

    std::stringstream ss;
    ss << "Raycasting - Mouse: X=" << mousePos.x << " Y=" << mousePos.y
       << " Player: X=" << playerPos.x << " Y=" << playerPos.y;

    SDL_SetWindowTitle(window, ss.str().c_str());
}

void Render::handleKeyDownEvent(const SDL_Event &event) {
    switch (event.key.keysym.sym) {
    case SDLK_a:
        playerVelocity.x = -speed;
        break;
    case SDLK_d:
        playerVelocity.x = speed;
        break;
    case SDLK_s:
        playerVelocity.y = speed;
        break;
    case SDLK_w:
        playerVelocity.y = -speed;
        break;
    case SDLK_q:
        quit = true;
        break;
    default:
        break;
    }
}

void Render::handleKeyUpEvent(const SDL_Event &event) {
    switch (event.key.keysym.sym) {
    case SDLK_a:
    case SDLK_d:
        playerVelocity.x = 0.0f;
        break;
    case SDLK_s:
    case SDLK_w:
        playerVelocity.y = 0.0f;
        break;
    default:
        break;
    }
}

void Render::handleEvents() {
    Uint32 currentTime = SDL_GetTicks();
    deltaTime = (currentTime - lastTime) / 1000.0f;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
        case SDL_QUIT:
            handleQuitEvent();
            break;
        case SDL_MOUSEMOTION:
            handleMouseMotionEvent();
            break;
        case SDL_KEYDOWN:
            handleKeyDownEvent(event);
            break;
        case SDL_KEYUP:
            handleKeyUpEvent(event);
            break;
        default:
            break;
        }
    }

    lastTime = currentTime;
}

