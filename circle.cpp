#include "circle.h"

#include <QVector2D>

float square(float value)
{
    return value * value;
}

Circle::Circle(float x, float y, float r) :
    x(x), y(y), r(r)
{}

bool Circle::intersects(const Circle& other) const
{
    return QVector2D(other.x - x, other.y - y).length() <= r + other.r;
}

bool Circle::contains(float pointX, float pointY) const
{
    return QVector2D(pointX - x, pointY - y).length() <= r;
}

QRect Circle::rect() const
{
    return QRect(static_cast<int>(x - r),
                 static_cast<int>(y - r),
                 static_cast<int>(r * 2),
                 static_cast<int>(r * 2));
}
