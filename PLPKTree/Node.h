#pragma once
#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphWidget, QString node_name, QColor color = QColor(Qt::yellow), QColor dark_color = QColor(Qt::darkYellow));

    void AddEdge(Edge *edge);
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }

    void CalculateForces();
    bool Advance();
	QPointF GetPos();

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	void SetNewPos(QPointF newPos);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QList<Edge *> edge_list;
    QPointF new_pos;
    GraphWidget *graph;
    QString node_name;
	QColor color;
	QColor dark_color;
};
