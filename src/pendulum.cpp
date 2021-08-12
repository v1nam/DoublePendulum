#include "pendulum.hpp"
#include <cmath>

DoublePendulum::DoublePendulum(Pendulum p1, Pendulum p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

void DoublePendulum::moveObjects()
{
    const float &m1 = p1.mass;
    const float &m2 = p2.mass;

    double &t1 = p1.angle;
    double &t2 = p2.angle;

    double &v1 = p1.velocity;
    double &v2 = p2.velocity;

    const int &l1 = p1.stringLen;
    const int &l2 = p2.stringLen;

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

    p1.velocity += p1Acc;
    p2.velocity += p2Acc;

    p1.velocity /= damp;
    p2.velocity /= damp;

    p1.angle += p1.velocity;
    p2.angle += p2.velocity;
}

void DoublePendulum::updatePos()
{
    p1.position.x = (float)(p1.start.x + sin(p1.angle) * p1.stringLen);
    p1.position.y = (float)(p1.start.y + cos(p1.angle) * p1.stringLen);

    p2.start.x = p1.position.x;
    p2.start.y = p1.position.y;

    p2.position.x = (float)(p2.start.x + sin(p2.angle) * p2.stringLen);
    p2.position.y = (float)(p2.start.y + cos(p2.angle) * p2.stringLen);
}
