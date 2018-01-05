#pragma once
#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode);

    Node *SourceNode() const;
    Node *DestNode() const;

    void Adjust();

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source, *dest;

    QPointF source_point;
    QPointF dest_point;
    qreal arrow_size;
};
