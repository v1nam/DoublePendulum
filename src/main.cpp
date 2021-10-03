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
        Pendulum{9.0, 140, PI / 2.0, Vector2{screenWidth / 2, 350.0}, Vector2{screenWidth / 2, 500.0}, false, 10.0},
        Pendulum{9.0, 140, PI / 2.0, Vector2{screenWidth / 2, 500.0}, Vector2{screenWidth / 2, 650.0}, false, 10.0});

    RenderTexture2D traceBG = LoadRenderTexture(screenWidth, screenHeight);
    Shader shader = LoadShader(0, "glow.glsl");
    int trailUniformLoc = GetShaderLocation(shader, "showOldTrails");
    int showOldTrails = 1;

    float fadeBG = 0.0;

    BeginTextureMode(traceBG);
    ClearBackground(BLACK);
    EndTextureMode();

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        SetShaderValue(shader, trailUniformLoc, &showOldTrails, SHADER_UNIFORM_INT);

        if (IsKeyPressed(KEY_SPACE))
            paused = !paused;

        else if (IsKeyPressed(KEY_C)) {
            oldTrailPos.x = -1.;
            BeginTextureMode(traceBG);
            ClearBackground(BLACK);
            EndTextureMode();
            fadeBG = 0.0;
        } else if (IsKeyPressed(KEY_T))
            showOldTrails = !showOldTrails;

        if (paused)
            handleBobDrag(pend1);
        else {
            float dt = GetFrameTime();
            pend1.moveObjects(dt);
            fadeBG += dt;

            BeginTextureMode(traceBG);
            if (oldTrailPos.x != -1.)
            {
                if (fadeBG >= 0.05) {
                    fadeBG = 0.0;
                    BeginBlendMode(BLEND_ALPHA);
                    DrawRectangle(0, 0, screenWidth, screenHeight, ColorFromNormalized(Vector4{0, 0.0666f, 0.0465f, 0.05f}));
                    EndBlendMode();
                }
                DrawLine(pend1.p2.position.x, screenHeight - pend1.p2.position.y, oldTrailPos.x,
                         screenHeight - oldTrailPos.y, Color{0, 255, 178, 255});
            }
            EndTextureMode();
            oldTrailPos = pend1.p2.position;
        }

        pend1.updatePos();

        BeginShaderMode(shader);
        DrawTexture(traceBG.texture, 0, 0, WHITE);
        EndShaderMode();

        drawDoublePend(pend1, Color{60, 60, 65, 255});
        DrawFPS(10, 10);

        EndDrawing();
    }
    UnloadShader(shader);
    UnloadRenderTexture(traceBG);
    CloseWindow();
    return 0;
}
