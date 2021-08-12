#include <cmath>

#include "display.hpp"
#include "pendulum.hpp"
#include "raylib.h"

int main()
{
    const int screenWidth = 600;
    const int screenHeight = 700;

    bool paused = true;
    Vector2 oldTrailPos = Vector2{-1., 0.};

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Double Pendulum");

    DoublePendulum pend1 = DoublePendulum(
        Pendulum{9.0, 150, PI / 2.0, Vector2{screenWidth / 2, 350.0}, Vector2{screenWidth / 2, 500.0}, false, 10.0},
        Pendulum{9.0, 150, PI / 2.0, Vector2{screenWidth / 2, 500.0}, Vector2{screenWidth / 2, 650.0}, false, 10.0});

    RenderTexture2D traceBG = LoadRenderTexture(screenWidth, screenHeight);
    BeginTextureMode(traceBG);
    ClearBackground(RAYWHITE);
    EndTextureMode();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyPressed(KEY_SPACE))
            paused = !paused;

        if (IsKeyPressed(KEY_C))
        {
            oldTrailPos.x = -1.;
            BeginTextureMode(traceBG);
            ClearBackground(RAYWHITE);
            EndTextureMode();
        }

        if (paused)
            handleBobDrag(pend1);
        else
        {
            pend1.moveObjects();

            BeginTextureMode(traceBG);
            if (oldTrailPos.x != -1.)
            {
                DrawLine(pend1.p2.position.x, screenHeight - pend1.p2.position.y, oldTrailPos.x,
                         screenHeight - oldTrailPos.y, RED);
            }
            EndTextureMode();
            oldTrailPos = pend1.p2.position;
        }

        pend1.updatePos();

        DrawTexture(traceBG.texture, 0, 0, WHITE);
        drawDoublePend(pend1, BLACK);
        DrawFPS(10, 10);

        EndDrawing();
    }
    UnloadRenderTexture(traceBG);
    CloseWindow();
    return 0;
}
