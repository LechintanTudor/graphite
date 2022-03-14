#include "node_palette.h"

const NodePalette NodePalette::Regular = NodePalette(
    QColor(210, 210, 210),
    QColor(10, 10, 10),
    QColor(240, 240, 240),
    QColor(30, 30, 30)
);

const NodePalette NodePalette::Connectable = NodePalette(
    QColor(100, 210, 100),
    QColor(10, 10, 10),
    QColor(130, 250, 130),
    QColor(30, 30, 30)
);

const NodePalette NodePalette::Disconnectable = NodePalette(
    QColor(210, 60, 60),
    QColor(10, 10, 10),
    QColor(250, 80, 80),
    QColor(30, 30, 30)
);

NodePalette::NodePalette(const QColor &color, const QColor &borderColor,
                         const QColor &hoverColor, const QColor &borderHoverColor) :
    Color(color),
    BorderColor(borderColor),
    HoverColor(hoverColor),
    BorderHoverColor(borderHoverColor)
{}
