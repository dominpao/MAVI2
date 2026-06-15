#include "Renderer.h"

// Constructor: intenta cargar capibara.png, si no existe usa modo fallback
Renderer::Renderer() : spriteCapy{ 0 }, textureLoaded(false) {
    Image capyImage;
    if (FileExists("capibara.png")) {
        capyImage = LoadImage("capibara.png");
        if (capyImage.data != NULL) {
            spriteCapy = LoadTextureFromImage(capyImage);
            textureLoaded = true;
            UnloadImage(capyImage);
        }
    }
}

Renderer::~Renderer() {
    if (textureLoaded) UnloadTexture(spriteCapy);
}

// Convierte coordenadas Box2D a pantalla (invierte Y)
Vector2 Renderer::toScreen(b2Vec2 world) {
    return { world.x * PPM, (float)SCREEN_HEIGHT - (world.y * PPM) };
}

void Renderer::beginDrawing() { BeginDrawing(); }
void Renderer::endDrawing() { EndDrawing(); }
void Renderer::clearBackground(Color color) { ClearBackground(color); }
void Renderer::drawGround() { DrawRectangle(0, SCREEN_HEIGHT - 45, SCREEN_WIDTH, 45, DARKGRAY); }

void Renderer::drawPlatform(b2Vec2 position, float width, float height, Color color) {
    Vector2 screenPos = toScreen(position);
    float widthPixels = width * 2 * PPM;
    float heightPixels = height * 2 * PPM;
    DrawRectangle((int)(screenPos.x - widthPixels / 2), (int)(screenPos.y - heightPixels / 2),
        (int)widthPixels, (int)heightPixels, color);
}

void Renderer::drawPlayer(b2Vec2 position, Color color, bool isActive) {
    Vector2 screenPos = toScreen(position);
    float sizeX = 45.0f, sizeY = 55.0f;
    Vector2 origin = { sizeX / 2.0f, sizeY / 2.0f };

    if (textureLoaded && spriteCapy.id > 0) {
        DrawTexturePro(spriteCapy,
            { 0, 0, (float)spriteCapy.width, (float)spriteCapy.height },
            { screenPos.x, screenPos.y, sizeX, sizeY }, origin, 0, color);
    }
    else {
        // Fallback: cubos de colores si no hay textura
        DrawRectangle((int)(screenPos.x - origin.x), (int)(screenPos.y - origin.y),
            (int)sizeX, (int)sizeY, color);
        DrawRectangleLines((int)(screenPos.x - origin.x), (int)(screenPos.y - origin.y),
            (int)sizeX, (int)sizeY, WHITE);
    }
}

void Renderer::drawLine(b2Vec2 start, b2Vec2 end, float thickness, Color color) {
    DrawLineEx(toScreen(start), toScreen(end), thickness, color);
}

void Renderer::drawVictoryScreen() {
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.5f));
    DrawText("VICTORIA", 450, 350, 100, YELLOW);
    DrawText("Presiona R para jugar de nuevo", 520, 450, 20, WHITE);
}

void Renderer::drawUI(const char* text, int x, int y, int fontSize, Color color) {
    DrawText(text, x, y, fontSize, color);
}

bool Renderer::isTextureLoaded() { return textureLoaded; }