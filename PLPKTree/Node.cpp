#include "Edge.h"
#include "Node.h"
#include "GraphWidget.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

Node::Node(GraphWidget *graph_widget, QString node_name, QColor color, QColor dark_color)
    : graph(graph_widget),
      node_name(node_name),
	  color(color),
	  dark_color(dark_color)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setScale(2.4);
}

void Node::AddEdge(Edge *edge)
{
    edge_list << edge;
    edge->Adjust();
}

QList<Edge *> Node::edges() const
{
    return edge_list;
}

void Node::CalculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this) {
        new_pos = pos();
        return;
    }

    // Sum up all forces pushing this item away
    qreal xvel = 0;
    qreal yvel = 0;
    foreach (QGraphicsItem *item, scene()->items()) {
        Node *node = qgraphicsitem_cast<Node *>(item);
        if (!node)
            continue;

        QPointF vec = mapToItem(node, 0, 0);
        qreal dx = vec.x();
        qreal dy = vec.y();
        double l = 2.0 * (dx * dx + dy * dy);
        if (l > 0) {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    // Now subtract all forces pulling items together
    double weight = (edge_list.size() + 1) * 10;
    foreach (Edge *edge, edge_list) {
        QPointF vec;
		if (edge->SourceNode() == this)
		{
			vec = mapToItem(edge->DestNode(), 0, 0);
		}
		else
		{
			vec = mapToItem(edge->SourceNode(), 0, 0);
		}
        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

	if (qAbs(xvel) < 0.1 && qAbs(yvel) < 0.1)
	{
		xvel = yvel = 0;
	}

    QRectF sceneRect = scene()->sceneRect();
	new_pos = pos();// +QPointF(xvel, yvel);                                                      // these lines make the graph elastic
    //newPos.setX(qMin(qMax(newPos.x(), sceneRect.left() + 10), sceneRect.right() - 10));    	  // these lines make the graph elastic
    //newPos.setY(qMin(qMax(newPos.y(), sceneRect.top() + 10), sceneRect.bottom() - 10));		  // these lines make the graph elastic
}

bool Node::Advance()
{
	if (new_pos == pos())
	{
		return false;
	}

    setPos(new_pos);
    return true;
}

QRectF Node::boundingRect() const
{
    qreal adjust = 2;
    return QRectF( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, 20, 20);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
	//draw shadow
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray);
    painter->drawEllipse(-7, -7, 20, 20);

	// draw gradient
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, color.light(120));
        gradient.setColorAt(0, dark_color.light(120));
    } else {
        gradient.setColorAt(0, color);
        gradient.setColorAt(1, dark_color);
    }
    painter->setBrush(gradient);

	// draw around the ellipse
    painter->setPen(QPen(Qt::black, 0));
    painter->drawEllipse(-10, -10, 20, 20);

    // Text
    QRectF text_rect(-10, -10, 20, 20);
    QString message(node_name);
    QFont font = painter->font();
    font.setPointSize(4);
    painter->setFont(font);
    painter->setPen(Qt::black);

    painter->drawText(text_rect, message, QTextOption(Qt::AlignCenter));
}

void Node::SetNewPos(QPointF newPos)
{
	this->new_pos = newPos;
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edge_list)
            edge->Adjust();
        graph->ItemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
