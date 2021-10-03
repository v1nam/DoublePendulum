#include "pendulum.hpp"
#include "raylib.h"
#include <cmath>

double distanceSquared(Vector2 p1, Vector2 p2)
{
    return pow(p2.x - p1.x, 2.0) + pow(p2.y - p1.y, 2.0);
}

void drawDoublePend(DoublePendulum &doubp, Color col)
{
    DrawLineEx(doubp.p1.start, doubp.p1.position, 8.f, col);
    DrawLineEx(doubp.p2.start, doubp.p2.position, 8.f, col);

    //DrawCircleV(doubp.p1.position, doubp.p1.radius, col);
    //DrawCircleV(doubp.p2.position, doubp.p2.radius, col);
}

void handleBobDrag(DoublePendulum &doubp)
{
    Vector2 mousePos = GetMousePosition();
    Pendulum &p1 = doubp.p1;
    Pendulum &p2 = doubp.p2;
    if (distanceSquared(mousePos, p1.position) <= pow(p1.radius, 2.0) && IsMouseButtonPressed(0))
    {
        p1.moveBob = true;
        p1.velocity = 0.0;
        p2.velocity = 0.0;
    }
    if (distanceSquared(mousePos, p2.position) <= pow(p2.radius, 2.0) && IsMouseButtonPressed(0))
    {
        p2.moveBob = true;
        p1.velocity = 0.0;
        p2.velocity = 0.0;
    }

    if (IsMouseButtonReleased(0))
    {
        if (p1.moveBob)
            p1.moveBob = false;
        if (p2.moveBob)
            p2.moveBob = false;
    }

    if (p1.moveBob)
        p1.angle = atan2(mousePos.x - p1.start.x, mousePos.y - p1.start.y);
    if (p2.moveBob)
        p2.angle = atan2(mousePos.x - p2.start.x, mousePos.y - p2.start.y);
}
