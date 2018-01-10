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
    setScale(2);
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

void Node::UpdatePosition()
{
	new_pos = pos();
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

QPointF Node::GetPos()
{
	return this->pos();
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
    if (option->state & QStyle::State_Sunken)
	{
        gradient.setCenter(3, 3);
        gradient.setFocalPoint(3, 3);
        gradient.setColorAt(1, color.light(120));
        gradient.setColorAt(0, dark_color.light(120));
    } 
	else
	{
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

void Node::SetColor(QColor color)
{
	this->color = color;
}

void Node::SetDarkColor(QColor dark_color)
{
	this->dark_color = dark_color;
}

bool Node::IsSelected()
{
	return this->is_selected;
}

void Node::SetSelected(bool selected)
{
	this->is_selected = selected;
}

void Node::RecursiveColorChange(Edge* p_edge, QColor color)
{
	foreach(QGraphicsItem *item, scene()->items())
	{
		if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
		{
			if (p_edge->GetSourceNode() == edge->GetDestNode() && p_edge->GetSourceNode() != this->graph->GetRootNode())
			{
				p_edge->GetSourceNode()->SetColor(Qt::green);
				p_edge->GetSourceNode()->SetDarkColor(Qt::darkGreen);
				p_edge->GetSourceNode()->update();

				edge->SetColor(Qt::green);
				edge->update();

				RecursiveColorChange(edge, color);
				break;
			}
		}
	}
}

QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change)
	{
    case ItemPositionHasChanged:
		foreach(Edge *edge, edge_list)
		{
			edge->Adjust();
		}
        graph->ItemMoved();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(event->MouseButtonPress && event->button() ==  Qt::LeftButton)
	{
		this->color = Qt::blue;
		this->dark_color = Qt::darkBlue;
		this->is_selected = true;
		foreach(QGraphicsItem *item, scene()->items())
		{
			if (Edge *edge = qgraphicsitem_cast<Edge *>(item))
			{
				if (edge->GetSourceNode() == this)
				{
					edge->SetColor(Qt::red);
					edge->update();

					edge->GetDestNode()->SetColor(Qt::white);
					edge->GetDestNode()->SetDarkColor(Qt::white);
					edge->GetDestNode()->update();
				}
				else if (edge->GetDestNode() == this)
				{
					edge->SetColor(Qt::blue);
					edge->update();

					RecursiveColorChange(edge, Qt::darkGreen);
				}
			}
		}
	}
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
