#include "widget.h"

#include <QVector2D>
#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QPen>

void Widget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen;
    pen.setWidth(3);
    painter.setPen(pen);

    drawEdges(painter);

    QFont font = painter.font();
    font.setPixelSize(20);
    painter.setFont(font);

    drawNodes(painter);
}

void Widget::drawEdges(QPainter& painter)
{
    const float TriangleHeight = 20.f;
    const float TriangleWidth = 8.f;

    QVector2D angleVec, nodeEdge, baseVec;

    for (int i = 0; i < m_edgeMatrix.size(); ++i)
    {
        for (int j = 0; j < m_edgeMatrix.size(); ++j)
        {
            if (!m_edgeMatrix[i][j]) continue;

            painter.drawLine(static_cast<int>(m_nodes[i].circle().x),
                             static_cast<int>(m_nodes[i].circle().y),
                             static_cast<int>(m_nodes[j].circle().x),
                             static_cast<int>(m_nodes[j].circle().y));

            angleVec.setX(m_nodes[j].circle().x - m_nodes[i].circle().x);
            angleVec.setY(m_nodes[j].circle().y - m_nodes[i].circle().y);
            angleVec.normalize();

            nodeEdge.setX(m_nodes[j].circle().x - angleVec.x() * Node::Radius);
            nodeEdge.setY(m_nodes[j].circle().y - angleVec.y() * Node::Radius);

            baseVec = nodeEdge - angleVec * TriangleHeight;

            QPainterPath path;
            path.moveTo(nodeEdge.toPoint());
            path.lineTo(QVector2D(baseVec + QVector2D(angleVec.y(), -angleVec.x()) * TriangleWidth).toPoint());
            path.lineTo(QVector2D(baseVec + QVector2D(-angleVec.y(), angleVec.x()) * TriangleWidth).toPoint());
            path.lineTo(nodeEdge.toPoint());

            painter.fillPath(path, Qt::black);
        }
    }
}

void Widget::drawNodes(QPainter& painter)
{
    switch (m_mode)
    {
    case EditorMode::Connect:
    case EditorMode::ConnectBoth:
        for (int i = 0; i != m_nodes.size(); ++i)
        {
            if (m_selectedNode != -1 && m_selectedNode != i && !m_edgeMatrix[m_selectedNode][i])
                drawNode(painter, i, NodePalette::Connectable);
            else
                drawNode(painter, i, NodePalette::Regular);
        }
        break;

    case EditorMode::Disconnect:
    case EditorMode::DisconnectBoth:
        for (int i = 0; i != m_nodes.size(); ++i)
        {
            if (m_selectedNode != -1 && m_selectedNode != i && m_edgeMatrix[m_selectedNode][i])
                drawNode(painter, i, NodePalette::Disconnectable);
            else
                drawNode(painter, i, NodePalette::Regular);
        }
        break;

    case EditorMode::Place:
    case EditorMode::Move:
        for (int i = 0; i != m_nodes.size(); ++i)
            drawNode(painter, i, NodePalette::Regular);
        break;
    }
}

void Widget::drawNode(QPainter &painter, int nodeIndex, const NodePalette& nodePalette)
{
    bool hovered = m_hoveredNode == nodeIndex;
    const auto& circle = m_nodes[nodeIndex].circle();

    QPen pen;
    pen.setWidth(2);

    if (hovered)
    {
        pen.setColor(nodePalette.BorderHoverColor);
        painter.setBrush(nodePalette.HoverColor);
    }
    else
    {
        pen.setColor(nodePalette.BorderColor);
        painter.setBrush(nodePalette.Color);
    }

    painter.setPen(pen);
    painter.drawEllipse(static_cast<int>(circle.x - circle.r),
                        static_cast<int>(circle.y - circle.r),
                        static_cast<int>(circle.r * 2),
                        static_cast<int>(circle.r * 2));

    painter.drawText(circle.rect(), QString::number(nodeIndex + 1), QTextOption(Qt::AlignCenter));
}
