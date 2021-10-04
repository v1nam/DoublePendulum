#pragma once

#include "raylib.h"

struct Pendulum
{
    float radius;
    int stringLen;
    double angle;
    Vector2 start;
    Vector2 position;
    Vector2 oldPos;
    bool moveBob;
    float mass;
    double velocity{0.0};

    Pendulum(float r, int sl, double a, Vector2 st, Vector2 p, float m) : 
        radius(r), stringLen(sl), angle(a),
        start(st), position(p), oldPos(p),
        mass(m), moveBob(false) {}
    Pendulum() {}
};

class DoublePendulum
{
  public:
    const double g = 980;
    const float damp = 1.0;
    Pendulum p1;
    Pendulum p2;
    Color trailColor;
    DoublePendulum(Pendulum pen1, Pendulum pen2, Color col) : p1{pen1}, p2{pen2}, trailColor(col) {}
    void moveObjects(float dt);
    void updatePos();
};
