#include "raylib.h"
#include <cmath>

struct Pendulum
{
    const float radius;
    const int stringLen;
    double angle;
    Vector2 start;
    Vector2 position;
    bool moveBob;
    const float mass;
    double velocity{0.0};
};

double distanceSquared(Vector2 p1, Vector2 p2)
{
    return pow(p2.x - p1.x, 2.0) + pow(p2.y - p1.y, 2.0);
}

int main()
{
    const int screenWidth = 600;
    const int screenHeight = 700;
    const double g = 0.5;

    bool paused = true;
    Vector2 oldTrailPos = Vector2{-1., 0.};

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "Pendulum");

    Pendulum pendulum1 =
        Pendulum{12.0, 150, PI / 2.0, Vector2{screenWidth / 2, 350.0}, Vector2{screenWidth / 2, 500.0}, false, 10.0};
    Pendulum pendulum2 =
        Pendulum{12.0, 150, PI / 2.0, pendulum1.position, Vector2{screenWidth / 2, 650.0}, false, 10.0};

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
        {
            Vector2 mousePos = GetMousePosition();
            if (distanceSquared(mousePos, pendulum1.position) <= pow(pendulum1.radius, 2.0) && IsMouseButtonPressed(0))
            {
                pendulum1.moveBob = true;
                pendulum1.velocity = 0.0;
                pendulum2.velocity = 0.0;
            }
            if (distanceSquared(mousePos, pendulum2.position) <= pow(pendulum2.radius, 2.0) && IsMouseButtonPressed(0))
            {
                pendulum2.moveBob = true;
                pendulum1.velocity = 0.0;
                pendulum2.velocity = 0.0;
            }

            if (IsMouseButtonReleased(0))
            {
                if (pendulum1.moveBob)
                    pendulum1.moveBob = false;
                if (pendulum2.moveBob)
                    pendulum2.moveBob = false;
            }

            if (pendulum1.moveBob)
                pendulum1.angle = atan2(mousePos.x - pendulum1.start.x, mousePos.y - pendulum1.start.y);
            if (pendulum2.moveBob)
                pendulum2.angle = atan2(mousePos.x - pendulum2.start.x, mousePos.y - pendulum2.start.y);
        }
        else
        {
            const float &m1 = pendulum1.mass;
            const float &m2 = pendulum2.mass;

            double &t1 = pendulum1.angle;
            double &t2 = pendulum2.angle;

            double &v1 = pendulum1.velocity;
            double &v2 = pendulum2.velocity;

            const int &l1 = pendulum1.stringLen;
            const int &l2 = pendulum2.stringLen;

            double p1Acc = -g * (2.0 * m1 + m2) * sin(t1);
            p1Acc = p1Acc - g * m2 * sin(t1 - 2.0 * t2);
            p1Acc = p1Acc - 2.0 * m2 * v2 * v2 * l2 * sin(t1 - t2);
            p1Acc = p1Acc - m2 * v1 * v1 * l1 * sin(2.0 * (t1 - t2));
            p1Acc = p1Acc / (l1 * (2.0 * m1 + m2 - m2 * cos(2.0 * (t1 - t2))));

            double p2Acc = (m1 + m2) * l1 * v1 * v1;
            p2Acc = p2Acc + g * (m1 + m2) * cos(t1);
            p2Acc = p2Acc + v2 * v2 * l2 * m2 * cos(t1 - t2);
            p2Acc = p2Acc * 2.0 * sin(t1 - t2);
            p2Acc = p2Acc / (l2 * (2.0 * m1 + m2 - m2 * cos(2.0 * (t1 - t2))));

            pendulum1.velocity += p1Acc;
            pendulum2.velocity += p2Acc;

            //pendulum1.velocity /= 1.0002;
            //pendulum2.velocity /= 1.0002;

            pendulum1.angle += pendulum1.velocity;
            pendulum2.angle += pendulum2.velocity;
            BeginTextureMode(traceBG);
            if (oldTrailPos.x != -1.)
            {
                DrawLine(pendulum2.position.x, screenHeight - pendulum2.position.y, oldTrailPos.x,
                         screenHeight - oldTrailPos.y, RED);
            }
            EndTextureMode();
            oldTrailPos = pendulum2.position;
        }

        pendulum1.position = Vector2{(float)(pendulum1.start.x + sin(pendulum1.angle) * pendulum1.stringLen),
                                     (float)(pendulum1.start.y + cos(pendulum1.angle) * pendulum1.stringLen)};

        pendulum2.start.x = pendulum1.position.x;
        pendulum2.start.y = pendulum1.position.y;

        pendulum2.position = Vector2{(float)(pendulum2.start.x + sin(pendulum2.angle) * pendulum2.stringLen),
                                     (float)(pendulum2.start.y + cos(pendulum2.angle) * pendulum2.stringLen)};

        DrawTexture(traceBG.texture, 0, 0, WHITE);

        DrawLineV(pendulum1.start, pendulum1.position, BLACK);
        DrawCircleV(pendulum1.position, pendulum1.radius, BLACK);

        DrawLineV(pendulum2.start, pendulum2.position, BLACK);
        DrawCircleV(pendulum2.position, pendulum2.radius, BLUE);

        DrawFPS(10, 10);

        EndDrawing();
    }
    UnloadRenderTexture(traceBG);
    CloseWindow();
    return 0;
}
