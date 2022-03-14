#include "node.h"

const float Node::Radius = 26.f;

Node::Node(float x, float y) :
    m_circle(x, y, Radius)
{}

void Node::move(float offsetX, float offsetY)
{
    m_circle.x += offsetX;
    m_circle.y += offsetY;
}

void Node::setPosition(float newX, float newY)
{
    m_circle.x = newX;
    m_circle.y = newY;
}

const Circle& Node::circle() const
{
    return m_circle;
}

float Node::x() const
{
    return m_circle.x;
}

float Node::y() const
{
    return m_circle.y;
}
