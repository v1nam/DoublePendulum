#include <cmath>
#include <vector>

#include "display.hpp"
#include "pendulum.hpp"
#include "raylib.h"

int main()
{
    const int screenWidth = 600;
    const int screenHeight = 700;

    bool paused = true;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Double Pendulum");

    double initialAngle = PI / 2.0;
    std::vector<DoublePendulum> pendulums;

    Color colArr[4] = {Color{0, 255, 178, 255}, Color{0, 178, 255, 255}, Color{255, 0, 78, 255}, Color{255, 158, 0, 255}};

    for (int i(0); i < 4; i++)
        pendulums.emplace_back(DoublePendulum(
            Pendulum(9.f, 140, initialAngle + i * 0.0008, Vector2{screenWidth / 2, 350.f}, Vector2{screenWidth / 2, 500.f}, 10.f),
            Pendulum(9.f, 140, initialAngle + i * 0.0008, Vector2{screenWidth / 2, 500.f}, Vector2{screenWidth / 2, 650.f}, 10.f), colArr[i]));

    RenderTexture2D traceBG = LoadRenderTexture(screenWidth, screenHeight);
    Shader shader = LoadShader(0, "glow.glsl");
    int trailUniformLoc = GetShaderLocation(shader, "showOldTrails");
    int showOldTrails = 0;

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
            BeginTextureMode(traceBG);
            ClearBackground(BLACK);
            EndTextureMode();
            fadeBG = 0.0;
        } else if (IsKeyPressed(KEY_T))
            showOldTrails = !showOldTrails;

        if (paused)
            handleBobDrag(pendulums[0]);
        else {
            float dt = GetFrameTime();
            fadeBG += dt;

            BeginTextureMode(traceBG);
            if (fadeBG >= 0.05) {
                fadeBG = 0.0;
                BeginBlendMode(BLEND_ALPHA);
                DrawRectangle(0, 0, screenWidth, screenHeight, ColorFromNormalized(Vector4{0, 0.f, 0.f, 0.05f}));
                EndBlendMode();
            }
            for (DoublePendulum& pend: pendulums) {
                pend.moveObjects(dt);
                DrawLine(pend.p2.position.x, screenHeight - pend.p2.position.y, pend.p2.oldPos.x,
                        screenHeight - pend.p2.oldPos.y, pend.trailColor);
                pend.p2.oldPos = pend.p2.position;
            }
            EndTextureMode();
        }

        BeginShaderMode(shader);
        DrawTexture(traceBG.texture, 0, 0, WHITE);
        EndShaderMode();
        
        for (DoublePendulum& pend: pendulums) {
            pend.updatePos();
            drawDoublePend(pend, Color{60, 60, 65, 255});
        }
        DrawFPS(10, 10);

        EndDrawing();
    }
    UnloadShader(shader);
    UnloadRenderTexture(traceBG);
    CloseWindow();
    return 0;
}
