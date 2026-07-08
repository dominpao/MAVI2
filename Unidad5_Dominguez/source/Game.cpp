#include "Game.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>

const float Game::PPM = 30.0f;

Game::Game()
    : estado(GameState::WAITING), goles(0), atajadas(0), eventTimer(0.0f) {

    srand((unsigned int)time(NULL));

    b2Vec2 gravity(0.0f, 0.0f);
    world = new b2World(gravity);
    world->SetContactListener(&contactListener);

    limites.crear(world, SCREEN_WIDTH, SCREEN_HEIGHT, PPM);
    arco.crear(world, contactListener.bodyIDs);
    arquero.crear(world, contactListener.bodyIDs);

    printf("=== ARQUERO GAME ===\n");
    printf("Press SPACE to shoot\n");
    printf("Use UP/DOWN to move the goalkeeper\n");
    printf("State: WAITING\n");
}

Game::~Game() {
    delete world;
}

void Game::crearPelota() {
    pelota.crear(world, contactListener.bodyIDs);
}

void Game::resetearJuego() {
    pelota.destruir(world, contactListener.bodyIDs);
    contactListener.pelotaAtajada = false;
    contactListener.gol = false;
    eventTimer = 0.0f;

    arquero.resetPosition();

    estado = GameState::WAITING;
    printf("State: WAITING (reset)\n");
}

void Game::update() {
    // Avanza la simulacion fisica de Box2D
    float dt = 1.0f / 60.0f;
    world->Step(dt, 8, 3);

    // El arquero solo se mueve en los estados activos
    if (estado == GameState::WAITING || estado == GameState::RUNNING) {
        arquero.update(PPM, SCREEN_HEIGHT);
    }

    // En los estados finales el arquero queda congelado
    if (estado == GameState::FINISHED || estado == GameState::EVENT_DETECTED) {
        arquero.freeze();
    }

    // Maquina de estados del juego
    switch (estado) {
    case GameState::WAITING:
        // Espera a que el jugador presione SPACE para lanzar
        if (IsKeyPressed(KEY_SPACE)) {
            crearPelota();
            estado = GameState::RUNNING;
            printf("State: RUNNING\n");
        }
        break;

    case GameState::RUNNING:
        // Verifica si hubo gol o atajada por contacto fisico
        if (contactListener.pelotaAtajada || contactListener.gol) {
            if (contactListener.pelotaAtajada) {
                atajadas++;
                printf("SAVED! (%d)\n", atajadas);
            } else if (contactListener.gol) {
                goles++;
                printf("GOAL! (%d)\n", goles);
            }
            estado = GameState::EVENT_DETECTED;
            eventTimer = 1.5f;
            printf("State: EVENT_DETECTED\n");
        }

        // Detecta si la pelota salio de la pantalla sin colision
        if (pelota.estaActiva() && pelota.getBody() != nullptr) {
            b2Vec2 pos = pelota.getBody()->GetPosition();
            float screenX = pos.x * PPM;

            if (screenX > SCREEN_WIDTH + 50) {
                if (!contactListener.gol && !contactListener.pelotaAtajada) {
                    goles++;
                    printf("GOAL! Ball passed\n");
                    estado = GameState::EVENT_DETECTED;
                    eventTimer = 1.5f;
                    printf("State: EVENT_DETECTED\n");
                }
            }
        }
        break;

    case GameState::EVENT_DETECTED:
        // Temporizador de pausa antes de mostrar FINISHED
        eventTimer -= GetFrameTime();
        if (eventTimer <= 0.0f) {
            estado = GameState::FINISHED;
            printf("State: FINISHED\n");
        }
        break;

    case GameState::FINISHED:
        // Espera que el jugador presione R para reiniciar
        if (IsKeyPressed(KEY_R)) {
            resetearJuego();
        }
        break;
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(GetColor(0x1a1a2e));

    drawField();
    arco.dibujar(PPM, SCREEN_HEIGHT, pelota.estaActiva(), estado == GameState::RUNNING);
    arquero.dibujar(PPM, SCREEN_HEIGHT);
    pelota.dibujar(PPM, SCREEN_HEIGHT);

    if (pelota.estaActiva() && estado == GameState::RUNNING) {
        pelota.dibujarTrayectoria(PPM, SCREEN_HEIGHT);
    }

    drawUI();

    if (estado == GameState::EVENT_DETECTED) {
        drawEventMessage();
    }

    EndDrawing();
}

bool Game::shouldClose() {
    return WindowShouldClose();
}

void Game::drawField() {
    // Fondo verde de la cancha
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GetColor(0x1a472a));
    // Linea de medio campo
    DrawLine(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, ColorAlpha(WHITE, 0.15f));
    // Circulo central
    DrawCircleLines(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, ColorAlpha(WHITE, 0.15f));
}

void Game::drawUI() {
    Color estadoColor = getStateColor();
    std::string estadoStr = getStateString();

    // Indicador de estado en la esquina superior izquierda
    DrawRectangle(5, 5, MeasureText(estadoStr.c_str(), 20) + 20, 30, ColorAlpha(BLACK, 0.7f));
    DrawText(estadoStr.c_str(), 15, 10, 20, estadoColor);

    // Marcador de goles y atajadas en la esquina superior derecha
    char marcador[100];
    sprintf_s(marcador, sizeof(marcador), "GOALS: %d   SAVES: %d", goles, atajadas);
    int textWidth = MeasureText(marcador, 25);
    DrawRectangle(SCREEN_WIDTH - textWidth - 20, 5, textWidth + 20, 35, ColorAlpha(BLACK, 0.7f));
    DrawText(marcador, SCREEN_WIDTH - textWidth - 10, 10, 25, WHITE);

    // Instrucciones en la parte inferior segun el estado
    if (estado == GameState::WAITING) {
        DrawRectangle(0, SCREEN_HEIGHT - 80, SCREEN_WIDTH, 80, ColorAlpha(BLACK, 0.6f));
        DrawText("Press SPACE to shoot", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 65, 22, YELLOW);
        DrawText("Use UP/DOWN arrows to move the goalkeeper", SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 35, 18, WHITE);
    } else if (estado == GameState::FINISHED) {
        DrawRectangle(0, SCREEN_HEIGHT - 50, SCREEN_WIDTH, 50, ColorAlpha(BLACK, 0.6f));
        DrawText("Press R to restart", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT - 35, 22, YELLOW);
    }
}

void Game::drawEventMessage() {
    // Muestra SAVED! o GOAL! en el centro de la pantalla
    const char* mensaje = contactListener.pelotaAtajada ? "SAVED!" : "GOAL!";
    Color color = contactListener.pelotaAtajada ? GREEN : RED;

    int textWidth = MeasureText(mensaje, 80);
    DrawRectangle(SCREEN_WIDTH / 2 - textWidth / 2 - 30, SCREEN_HEIGHT / 2 - 100, textWidth + 60, 110, ColorAlpha(BLACK, 0.8f));
    DrawRectangle(SCREEN_WIDTH / 2 - textWidth / 2 - 25, SCREEN_HEIGHT / 2 - 95, textWidth + 50, 100, ColorAlpha(color, 0.2f));
    DrawText(mensaje, SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2 - 40, 80, color);

    if (eventTimer > 0.75f) {
        DrawText("*", SCREEN_WIDTH / 2 - 20, SCREEN_HEIGHT / 2 - 90, 50, GOLD);
    }
}

std::string Game::getStateString() const {
    switch (estado) {
    case GameState::WAITING: return "WAITING - Press SPACE";
    case GameState::RUNNING: return "RUNNING - Ball in motion...";
    case GameState::EVENT_DETECTED: return "EVENT DETECTED";
    case GameState::FINISHED: return "FINISHED - Press R";
    default: return "UNKNOWN";
    }
}

Color Game::getStateColor() const {
    switch (estado) {
    case GameState::WAITING: return BLUE;
    case GameState::RUNNING: return YELLOW;
    case GameState::EVENT_DETECTED: return ORANGE;
    case GameState::FINISHED: return GREEN;
    default: return WHITE;
    }
}