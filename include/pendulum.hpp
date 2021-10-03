#pragma once

#include "raylib.h"

struct Pendulum
{
    float radius;
    int stringLen;
    double angle;
    Vector2 start;
    Vector2 position;
    bool moveBob;
    float mass;
    double velocity{0.0};
};

class DoublePendulum
{
  public:
    const double g = 980;
    const float damp = 1.0;
    Pendulum p1;
    Pendulum p2;
    DoublePendulum(Pendulum p1, Pendulum p2);
    void moveObjects(float dt);
    void updatePos();
};
