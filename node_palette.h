#ifndef NODE_PALETTE_H
#define NODE_PALETTE_H

#include <QColor>

struct NodePalette
{
    static const NodePalette Regular;
    static const NodePalette Connectable;
    static const NodePalette Disconnectable;

    NodePalette() = default;
    NodePalette(const QColor& color, const QColor& borderColor,
                const QColor& hoverColor, const QColor& borderHoverColor);

    QColor Color;
    QColor BorderColor;
    QColor HoverColor;
    QColor BorderHoverColor;
};

extern const NodePalette RegularNodePalette;
extern const NodePalette ConnectableNodePalette;
extern const NodePalette DisconnectableNodePalette;

#endif // NODE_PALETTE_H
