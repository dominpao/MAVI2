#include "Game.h"
#include <iostream>

Game::Game()
    : level(nullptr)
    , gameState(GameState::WAITING)
    , stateTimer(0.0f)
    , capyTex{}
    , platTex{}
{
}

Game::~Game() {
    if (level) Shutdown();
}

// Init: Crea la ventana, carga texturas, inicializa el nivel
void Game::Init() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Capy Adventure - Final");
    SetTargetFPS(60);

    capyTex = LoadTexture("Resources/capibara.png");
    platTex = LoadTexture("Resources/plataforma.png");

    level = new Level();
    level->Init();

    gameState = GameState::WAITING;
    stateTimer = 0.0f;
}

// Shutdown: Libera nivel, texturas y cierra la ventana
void Game::Shutdown() {
    delete level;
    level = nullptr;
    UnloadTexture(capyTex);
    UnloadTexture(platTex);
    CloseWindow();
}

// Run: Loop principal del juego
void Game::Run() {
    Init();

    while (!WindowShouldClose()) {
        HandleInput();
        Update();
        Draw();
    }

    Shutdown();
}

// HandleInput: Solo escucha la tecla R para reiniciar el juego
void Game::HandleInput() {
    if (IsKeyPressed(KEY_R)) {
        level->Reset();
        gameState = GameState::WAITING;
        std::cout << "Juego reiniciado!" << std::endl;
    }
}

// Update: Avanza la fisica, procesa input del jugador y ejecuta la maquina de estados del juego
void Game::Update() {
    level->Step(1.0f / 60.0f);

    auto* player = level->GetPlayer();
    auto* contactListener = level->GetContactListener();

    // Input del jugador: solo si no gano ni esta sobre la plataforma final
    if (gameState != GameState::FINISHED && !player->IsOnPlatform()) {
        b2Vec2 vel = player->GetVelocity();

        if (IsKeyDown(KEY_RIGHT)) {
            player->MoveRight();
        } else if (IsKeyDown(KEY_LEFT)) {
            player->MoveLeft();
        } else {
            player->Stop();
        }

        if (IsKeyPressed(KEY_UP) && player->IsGrounded()) {
            player->Jump();
            std::cout << "Salto!" << std::endl;
        }

        if (IsKeyReleased(KEY_UP)) {
            player->CutJump();
        }
    } else if (gameState == GameState::FINISHED) {
        player->GetBody()->SetLinearVelocity(b2Vec2(0, 0));
    }

    // Maquina de estados del juego
    switch (gameState) {
    case GameState::WAITING:
        gameState = GameState::RUNNING;
        std::cout << "Estado: RUNNING" << std::endl;
        break;

    case GameState::RUNNING:
        // Si el jugador toco el checkpoint
        if (contactListener->checkpointReached) {
            gameState = GameState::CHECKPOINT_REACHED;
            b2Vec2 checkpointPosition(950 / SCALE, (400 - PLAT_H / 2 - CAPY_SIZE / 2) / SCALE);
            player->SetLastCheckpointPos(checkpointPosition);
            player->SetCheckpointUnlocked(true);
            contactListener->checkpointReached = false;
            std::cout << "Estado: CHECKPOINT_REACHED" << std::endl;
            stateTimer = 0.0f;
        }

        // Si el jugador llego a la plataforma final
        if (contactListener->goalReached) {
            gameState = GameState::FINISHED;
            level->StartFinalPlatform();
            player->UpdateOnPlatform(true);
            std::cout << "Estado: FINISHED - Victoria!" << std::endl;
        }

        // Si el jugador cae del mapa, respawnea
        level->RespawnIfFallen();
        break;

    case GameState::CHECKPOINT_REACHED:
        // Espera 2 segundos mostrando el mensaje y vuelve a RUNNING
        stateTimer += GetFrameTime();
        if (stateTimer > 2.0f) {
            gameState = GameState::RUNNING;
            std::cout << "Estado: RUNNING" << std::endl;
        }
        break;

    case GameState::FINISHED:
        // La plataforma final se mueve sola en Level::Step
        break;
    }
}

// Draw: Limpia la pantalla y dibuja plataformas, jugador e interfaz
void Game::Draw() {
    BeginDrawing();
    ClearBackground(SKYBLUE);

    DrawPlatforms();
    DrawPlayer();
    DrawUI();

    EndDrawing();
}

// DrawPlatforms: Dibuja cada plataforma con su textura.
//                La final en dorado. El checkpoint con su bandera
void Game::DrawPlatforms() {
    for (auto* p : level->GetPlatforms()) {
        Color platColor = WHITE;
        if (p->IsFinal()) platColor = GOLD;

        DrawTexturePro(platTex,
            { 0, 0, (float)platTex.width, (float)platTex.height },
            { p->GetX(), p->GetY(), PLAT_W, PLAT_H },
            { PLAT_W / 2, PLAT_H / 2 },
            0, platColor);

        if (p->IsCheckpoint()) {
            float flagX = p->GetX();
            float flagY = p->GetY() - PLAT_H / 2 - 5;
            bool unlocked = level->GetPlayer()->IsCheckpointUnlocked();
            DrawFlag(flagX, flagY, unlocked);
        }
    }
}

// DrawPlayer: Dibuja la capibara en la posicion del cuerpo Box2D convertida de metros a pixeles (SCALE)
void Game::DrawPlayer() {
    b2Vec2 pos = level->GetPlayer()->GetPosition();
    Vector2 drawPos = { pos.x * SCALE, pos.y * SCALE };
    DrawTexturePro(capyTex,
        { 0, 0, (float)capyTex.width, (float)capyTex.height },
        { drawPos.x, drawPos.y, CAPY_SIZE, CAPY_SIZE },
        { CAPY_SIZE / 2.0f, CAPY_SIZE / 2.0f },
        0, WHITE);
}

// DrawFlag: Dibuja un mastil con una bandera verde (desbloqueada)
//           o roja (bloqueada) arriba del checkpoint
void Game::DrawFlag(float x, float y, bool isUnlocked) {
    float poleHeight = 50.0f;
    float flagWidth = 30.0f;
    float flagHeight = 20.0f;

    DrawLineEx({ x, y }, { x, y - poleHeight }, 4.0f, DARKGRAY);

    Color flagColor = isUnlocked ? GREEN : RED;

    Rectangle flagRect = {
        x,
        y - poleHeight + 2,
        flagWidth,
        flagHeight
    };

    DrawRectangleRec(flagRect, flagColor);
    DrawRectangleLinesEx(flagRect, 2.0f, DARKGRAY);
    DrawCircleV({ x, y - poleHeight }, 3.0f, DARKGRAY);
}

// DrawUI: Muestra textos en pantalla: estado del juego, checkpoint, controles y mensaje de victoria 
void Game::DrawUI() {
    std::string stateText;
    Color stateColor = WHITE;

    switch (gameState) {
    case GameState::RUNNING:
        stateText = "Sube la escalera y llega a la meta!";
        stateColor = WHITE;
        break;
    case GameState::CHECKPOINT_REACHED:
        stateText = "Checkpoint alcanzado!";
        stateColor = YELLOW;
        break;
    case GameState::FINISHED:
        if (level->IsPlatformFinished()) {
            stateText = "VICTORIA! Has llegado a la meta!";
        } else {
            stateText = "La plataforma te lleva al final!";
        }
        stateColor = GOLD;
        break;
    default:
        stateText = "Sube la escalera y llega a la meta!";
        stateColor = WHITE;
        break;
    }

    DrawText(stateText.c_str(), SCREEN_WIDTH / 2 - MeasureText(stateText.c_str(), 30) / 2, 20, 30, stateColor);

    bool unlocked = level->GetPlayer()->IsCheckpointUnlocked();
    std::string checkpointText = unlocked ? "Checkpoint: DESBLOQUEADO" : "Checkpoint: BLOQUEADO";
    Color checkpointColor = unlocked ? GREEN : RED;
    DrawText(checkpointText.c_str(), SCREEN_WIDTH / 2 - MeasureText(checkpointText.c_str(), 24) / 2, 65, 24, checkpointColor);

    DrawText("Flechas para mover | Arriba para saltar | R para reiniciar", 20, SCREEN_HEIGHT - 40, 20, DARKGRAY);

    if (gameState == GameState::FINISHED && level->IsPlatformFinished()) {
        int fontSize = 60;
        const char* winText = "CAMPEON!";
        int textWidth = MeasureText(winText, fontSize);
        DrawText(winText, SCREEN_WIDTH / 2 - textWidth / 2, SCREEN_HEIGHT / 2 - 80, fontSize, GOLD);

        int fontSizeSmall = 30;
        const char* restartText = "Presiona R para reiniciar";
        int restartWidth = MeasureText(restartText, fontSizeSmall);
        DrawText(restartText, SCREEN_WIDTH / 2 - restartWidth / 2, SCREEN_HEIGHT / 2 + 10, fontSizeSmall, WHITE);
    }
}
