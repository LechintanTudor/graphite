#ifndef WIDGET_H
#define WIDGET_H

#include "node.h"
#include "node_palette.h"

#include <QWidget>
#include <QVector>

namespace Ui {
class Widget;
}

enum class EditorMode
{
    Connect,
    ConnectBoth,
    Disconnect,
    DisconnectBoth,
    Place,
    Move
};

class Widget : public QWidget
{
    Q_OBJECT

public:
    static const int WindowWidth;
    static const int WindowHeight;

    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

private slots:
    void addNode();
    void removeNode();
    void connectNode();
    void connectBothNodes();
    void disconnectNode();
    void disconnectBothNodes();
    void clearNodes();
    void exportGraph();
    void saveAsJson();
    void loadFromJson();

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

private:
    bool hasNeighbours(int nodeIndex);
    void createGraphContextMenu(QMouseEvent* event);
    void createNodeContextMenu(QMouseEvent* event);
    void drawEdges(QPainter& painter);
    void drawNodes(QPainter& painter);
    void drawNode(QPainter& painter, int nodeIndex, const NodePalette& nodePalette);

    Ui::Widget *ui;

    EditorMode m_mode = EditorMode::Place;
    float m_cursorX = 0.f;
    float m_cursorY = 0.f;

    int m_hoveredNode = -1;
    int m_selectedNode = -1;

    QVector<Node> m_nodes;
    QVector<QVector<bool>> m_edgeMatrix;
};

#endif // WIDGET_H
