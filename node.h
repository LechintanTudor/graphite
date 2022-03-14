#ifndef NODE_H
#define NODE_H

#include "circle.h"

class QPainter;

class Node
{
public:
    static const float Radius;

    Node() = default;
    Node(float x, float y);

    void move(float offsetX, float offsetY);
    void setPosition(float newX, float newY);

    const Circle& circle() const;
    float x() const;
    float y() const;

private:
    Circle m_circle;
};

#endif // NODE_H
