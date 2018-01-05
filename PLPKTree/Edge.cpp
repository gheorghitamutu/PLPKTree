#include "Edge.h"
#include "node.h"

#include <qmath.h>
#include <QPainter>

Edge::Edge(Node *source_node, Node *dest_node)
    : arrow_size(10)
{
    setAcceptedMouseButtons(0);
    source = source_node;
    dest = dest_node;
    source->AddEdge(this);
    dest->AddEdge(this);
    Adjust();
    setScale(2.4);
}

Node *Edge::SourceNode() const
{
    return source;
}

Node *Edge::DestNode() const
{
    return dest;
}

void Edge::Adjust()
{
	if (!source || !dest)
	{
		return;
	}

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.))
	{
        QPointF edge_offset((line.dx() * 10) / length, (line.dy() * 10) / length);
        source_point = line.p1() + edge_offset;
        dest_point = line.p2() - edge_offset;
    } 
	else
	{
        source_point = dest_point = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
	if (!source || !dest)
	{
		return QRectF();
	}

    qreal pen_width = 1;
    qreal extra = (pen_width + arrow_size) / 2.0;

    return QRectF(source_point, QSizeF(dest_point.x() - source_point.x(),
                                      dest_point.y() - source_point.y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	if (!source || !dest)
	{
		return;
	}

    QLineF line(source_point, dest_point);
	if (qFuzzyCompare(line.length(), qreal(0.)))
	{
		return;
	}

    // Draw the line itself
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(line);

    // Draw the arrows
    double angle = std::atan2(-line.dy(), line.dx());

	QPointF dest_arrow_p1 = dest_point + QPointF(sin(angle - M_PI / 3) * arrow_size,
		cos(angle - M_PI / 3) * arrow_size);
	QPointF dest_arrow_p2 = dest_point + QPointF(sin(angle - M_PI + M_PI / 3) * arrow_size,
		cos(angle - M_PI + M_PI / 3) * arrow_size);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p2() << dest_arrow_p1 << dest_arrow_p2);
}
