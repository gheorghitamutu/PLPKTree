#include "Graphwidget.h"
#include "Edge.h"
#include "Node.h"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>
#include <qguiapplication.h>
#include <qscreen.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timer_id(0)
{
	QScreen *screen = QGuiApplication::primaryScreen();
	QRect  screen_geometry = screen->geometry();
	this->screen_height = screen_geometry.height();
	this->screen_width = screen_geometry.width();

	this->tree_level_height = 400;
}

void GraphWidget::SetTree(std::vector<std::vector<Expression*>> tree)
{
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);
	int scene_width = std::pow(tree.size() + 1, 2) * 40;
	scene->setSceneRect(-scene_width, -(tree.size() * this->tree_level_height), scene_width, tree.size() * this->tree_level_height);
	setScene(scene);

	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	scale(qreal(0.8), qreal(0.8));

	setMinimumSize(this->screen_width, this->screen_height);
	setWindowTitle(tr("PLP KTree"));

	this->root_node = new Node(this, QString(tree[0][0]->GetExpressionName()), Qt::red, Qt::darkRed);
	this->nodes.emplace_back(this->root_node);

	for (int index = 1; index < tree.size(); index++)
	{
		for (auto node : tree[index])
		{		
			this->nodes.emplace_back(new Node(this, QString(node->GetExpressionName())));
		}
	}

	// add edges
	int total_children = 0;
	int current_node = 0;
	for (auto nodes_v : tree)
	{
		for (auto node : nodes_v)
		{
			for (int index = 0; index < node->GetChildren().size() && node->GetChildren().size() != 0; index++)
			{
				total_children++;
				this->edges.emplace_back(new Edge(this->nodes[current_node], this->nodes[total_children]));
				scene->addItem(edges.back()); // draw edges before nodes - they will get the nodes position which will be set up later
			}
			current_node++;
		}
	}

	bool root_children = true;
	float scene_width_temp = scene_width;
	this->root_node->setPos(QPointF(-scene_width_temp / 2, -scene->height() + 40));
	current_node = 0;
	int level = 0;
	for (auto node : nodes)
	{
		int count = 0; // how many time is source node in edges vector?
		for (auto edge : edges)
		{
			if (edge->SourceNode() == node)
			{
				count++;
			}
		}

		if (count != 0)
		{


			int count_children = 1;
			for (int index = edges.size() - 1; index >= 0; index--)
			{
				if (edges[index]->SourceNode() == node)
				{
					if (root_children == false)
					{
						QPointF source_pos = edges[index]->SourceNode()->GetPos();
						edges[index]->DestNode()->setPos(source_pos.x() - scene_width_temp / (count + 2)*count_children + scene_width_temp / 2, source_pos.y() + this->tree_level_height);
						count_children += 2;
					}
					else
					{
						QPointF source_pos = root_node->GetPos();
						edges[index]->DestNode()->setPos(-scene_width_temp / (count + 1)*count_children, source_pos.y() + this->tree_level_height);
						count_children += 1;
					}
				}
			}
			root_children = false;
		}
		current_node++;
		int count_nodes = 0;
		for (auto nodes : tree)
		{
			count_nodes += nodes.size();
			if (count_nodes == current_node)
			{
				scene_width_temp /= (1.8 + 1/(level+1));
				level++;
				break;
			}
		}
		
	}

	for (auto node : nodes)
	{
		scene->addItem(node);
	}

	
}

void GraphWidget::ItemMoved()
{
	if (!timer_id)
	{
		timer_id = startTimer(1000 / 25);
	}
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
	{
    case Qt::Key_Plus:
        ZoomIn();
        break;
    case Qt::Key_Minus:
        ZoomOut();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent * event)
{
    Q_UNUSED(event);

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene()->items())
	{
		if (Node *node = qgraphicsitem_cast<Node *>(item))
		{
			nodes << node;
		}
    }

	foreach(Node *node, nodes)
	{
		node->CalculateForces();
	}

    bool itemsMoved = false;
    foreach (Node *node, nodes)
	{
		if (node->Advance())
		{
			itemsMoved = true;
		}
    }

    if (!itemsMoved)
	{
        killTimer(timer_id);
        timer_id = 0;
    }
}

#if QT_CONFIG(wheelevent)
void GraphWidget::wheelEvent(QWheelEvent *event)
{
    ScaleView(pow((double)2, -event->delta() / 240.0));
}
#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
	if (rightShadow.intersects(rect) || rightShadow.contains(rect))
	{
		painter->fillRect(rightShadow, Qt::darkGray);
	}
	if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
	{
		painter->fillRect(bottomShadow, Qt::darkGray);
	}

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void GraphWidget::ScaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::ZoomIn()
{
    ScaleView(qreal(1.2));
}

void GraphWidget::ZoomOut()
{
    ScaleView(1 / qreal(1.2));
}
