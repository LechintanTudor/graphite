#include "widget.h"
#include "ui_widget.h"

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QBrush>
#include <QPen>

const int Widget::WindowWidth = 800;
const int Widget::WindowHeight = 600;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    resize(WindowWidth, WindowHeight);
    setStyleSheet("background-color: darkgrey;");
    setMouseTracking(true);
    setWindowIcon(QIcon(":/icons/icon.png"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::addNode()
{
    m_nodes.push_back(Node(m_cursorX, m_cursorY));

    m_edgeMatrix.resize(m_nodes.size());
    for (auto& row : m_edgeMatrix) row.resize(m_nodes.size());

    repaint();
}

void Widget::removeNode()
{
    if (m_hoveredNode == -1) return;

    m_edgeMatrix.remove(m_hoveredNode);
    for (auto& row : m_edgeMatrix) row.remove(m_hoveredNode);

    m_nodes.remove(m_hoveredNode);
    m_hoveredNode = -1;

    repaint();
}

void Widget::connectNode()
{
    m_mode = EditorMode::Connect;
    m_selectedNode = m_hoveredNode;
    repaint();
}

void Widget::connectBothNodes()
{
    m_mode = EditorMode::ConnectBoth;
    m_selectedNode = m_hoveredNode;
    repaint();
}

void Widget::disconnectNode()
{
    m_mode = EditorMode::Disconnect;
    m_selectedNode = m_hoveredNode;
    repaint();
}

void Widget::disconnectBothNodes()
{
    m_mode = EditorMode::DisconnectBoth;
    m_selectedNode = m_hoveredNode;
    repaint();
}

void Widget::clearNodes()
{
    m_nodes.clear();
    m_edgeMatrix.clear();
    m_hoveredNode = -1;

    repaint();
}

void Widget::mouseMoveEvent(QMouseEvent* event)
{
    switch (m_mode)
    {
    case EditorMode::Place:
    case EditorMode::Connect:
    case EditorMode::ConnectBoth:
    case EditorMode::Disconnect:
    case EditorMode::DisconnectBoth:
    {
        m_cursorX = static_cast<float>(event->pos().x());
        m_cursorY = static_cast<float>(event->pos().y());

        int lastHoveredNode = m_hoveredNode;
        m_hoveredNode = -1;

        for (int i = m_nodes.size() - 1; i >= 0; --i)
        {
            if (m_nodes[i].circle().contains(
                static_cast<float>(event->pos().x()),
                static_cast<float>(event->pos().y())
            ))
            {
                m_hoveredNode = i;
                break;
            }
        }

        if (lastHoveredNode != m_hoveredNode) repaint();

        break;
    }
    case EditorMode::Move:
    {
        float newCursorX = static_cast<float>(event->pos().x());
        float newCursorY = static_cast<float>(event->pos().y());

        if (m_hoveredNode != -1)
        {
            m_nodes[m_hoveredNode].move(newCursorX - m_cursorX, newCursorY - m_cursorY);
            repaint();
        }

        m_cursorX = newCursorX;
        m_cursorY = newCursorY;

        break;
    }
    }
}

void Widget::createGraphContextMenu(QMouseEvent* event)
{
    QAction addNodeAction(tr("Add node"));
    connect(&addNodeAction, &QAction::triggered, this, &Widget::addNode);

    QAction clearNodesAction(tr("Clear nodes"));
    connect(&clearNodesAction, &QAction::triggered, this, &Widget::clearNodes);

    QAction loadFromJsonAction(tr("Load from JSON"));
    connect(&loadFromJsonAction, &QAction::triggered, this, &Widget::loadFromJson);

    QAction saveAsJsonAction(tr("Save as JSON"));
    connect(&saveAsJsonAction, &QAction::triggered, this, &Widget::saveAsJson);

    QAction exportGraphAction(tr("Export graph"));
    connect(&exportGraphAction, &QAction::triggered, this, &Widget::exportGraph);

    QMenu contextMenu;
    contextMenu.addAction(&addNodeAction);
    contextMenu.addAction(&clearNodesAction);
    contextMenu.addSeparator();
    contextMenu.addAction(&loadFromJsonAction);
    contextMenu.addAction(&saveAsJsonAction);
    contextMenu.addSeparator();
    contextMenu.addAction(&exportGraphAction);
    contextMenu.exec(event->globalPos());
}

void Widget::createNodeContextMenu(QMouseEvent* event)
{
    QMenu contextMenu;

    QAction connectNodeAction(tr("Connect node"));
    connect(&connectNodeAction, &QAction::triggered, this, &Widget::connectNode);

    QAction connectBothNodesAction(tr("Connect both nodes"));
    connect(&connectBothNodesAction, &QAction::triggered, this, &Widget::connectBothNodes);

    QAction disconnectNodeAction(tr("Disconnect node"));
    connect(&disconnectNodeAction, &QAction::triggered, this, &Widget::disconnectNode);

    QAction disconnectBothNodesAction(tr("Disconnect both nodes"));
    connect(&disconnectBothNodesAction, &QAction::triggered, this, &Widget::disconnectBothNodes);

    if (m_nodes.size() > 1)
    {
        contextMenu.addAction(&connectNodeAction);
        contextMenu.addAction(&connectBothNodesAction);
        contextMenu.addSeparator();
    }

    if (hasNeighbours(m_hoveredNode))
    {
        contextMenu.addAction(&disconnectNodeAction);
        contextMenu.addAction(&disconnectBothNodesAction);
        contextMenu.addSeparator();
    }

    QAction removeNodeAction(tr("Remove node"));
    connect(&removeNodeAction, &QAction::triggered, this, &Widget::removeNode);
    contextMenu.addAction(&removeNodeAction);

    contextMenu.exec(event->globalPos());
}

void Widget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        switch (m_mode)
        {
        case EditorMode::Connect:
        case EditorMode::ConnectBoth:
            if (m_hoveredNode != -1 && m_selectedNode != m_hoveredNode)
            {
                m_edgeMatrix[m_selectedNode][m_hoveredNode] = true;

                if (m_mode == EditorMode::ConnectBoth)
                    m_edgeMatrix[m_hoveredNode][m_selectedNode] = true;

                m_selectedNode = -1;
                repaint();
            }

            m_mode = EditorMode::Move;
            break;

        case EditorMode::Disconnect:
        case EditorMode::DisconnectBoth:
            if (m_hoveredNode != -1 && m_selectedNode != m_hoveredNode)
            {
                m_edgeMatrix[m_selectedNode][m_hoveredNode] = false;

                if (m_mode == EditorMode::DisconnectBoth)
                    m_edgeMatrix[m_hoveredNode][m_selectedNode] = false;

                m_selectedNode = -1;
                repaint();
            }

            m_mode = EditorMode::Move;
            break;

        default:
            if (m_hoveredNode != -1) m_mode = EditorMode::Move;
        }
    }
    else if (event->button() == Qt::RightButton && m_mode == EditorMode::Place)
    {
        if (m_hoveredNode == -1)
            createGraphContextMenu(event);
        else
            createNodeContextMenu(event);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) m_mode = EditorMode::Place;
}

void Widget::resizeEvent(QResizeEvent* event)
{
    if (event->oldSize() == event->size()) return;

    float oldW = static_cast<float>(event->oldSize().width());
    float oldH = static_cast<float>(event->oldSize().height());
    float newW = static_cast<float>(event->size().width());
    float newH = static_cast<float>(event->size().height());
    float normX = 0.f;
    float normY = 0.f;

    for (auto& node : m_nodes)
    {
        normX = node.circle().x / oldW;
        normY = node.circle().y / oldH;

        node.setPosition(normX * newW, normY * newH);
    }
}

bool Widget::hasNeighbours(int nodeIndex)
{
    for (int i = 0; i != m_nodes.size(); ++i)
    {
        if (m_edgeMatrix[nodeIndex][i]) return true;
    }

    return false;
}
