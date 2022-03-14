#ifndef CIRCLE_H
#define CIRCLE_H

#include <QRect>

struct Circle
{
    Circle() = default;
    Circle(float x, float y, float r);

    bool intersects(const Circle& other) const;
    bool contains(float pointX, float pointY) const;

    QRect rect() const;

    float x = 0.f;
    float y = 0.f;
    float r = 0.f;
};

#endif // CIRCLE_H
