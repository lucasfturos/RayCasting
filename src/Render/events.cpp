#include "render.hpp"

void Render::handleQuitEvent() { quit = true; }

void Render::handleMouseMotionEvent() {
    int x, y;
    SDL_GetMouseState(&x, &y);

    int dx = x - initPosWindow.x;

    float sensitivity = 0.005f;
    float angle = dx * sensitivity;
    cameraAngle += angle;
    if (cameraAngle < 0) {
        cameraAngle += 2 * M_PI;
    } else if (cameraAngle >= 2 * M_PI) {
        cameraAngle -= 2 * M_PI;
    }

    SDL_WarpMouseInWindow(window, initPosWindow.x, initPosWindow.y);
}

void Render::handleKeyDownEvent(const SDL_Event &event) {
    switch (event.key.keysym.sym) {
    case SDLK_a:
        playerVelocity.x = speed;
        break;
    case SDLK_d:
        playerVelocity.x = -speed;
        break;
    case SDLK_s:
        playerVelocity.y = speed;
        break;
    case SDLK_w:
        playerVelocity.y = -speed;
        break;
    case SDLK_ESCAPE:
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
