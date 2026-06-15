#ifndef RENDERER_H
#define RENDERER_H

#include <box2d.h>
#include <raylib.h>
#include "Constants.h"

class Renderer {
private:
    Texture2D spriteCapy;
    bool textureLoaded;

    Vector2 toScreen(b2Vec2 world);

public:
    Renderer();
    ~Renderer();

    void beginDrawing();
    void endDrawing();
    void clearBackground(Color color);
    void drawGround();
    void drawPlatform(b2Vec2 position, float width, float height, Color color);
    void drawPlayer(b2Vec2 position, Color color, bool isActive);
    void drawLine(b2Vec2 start, b2Vec2 end, float thickness, Color color);
    void drawVictoryScreen();
    void drawUI(const char* text, int x, int y, int fontSize, Color color);
    bool isTextureLoaded();
};

#endif