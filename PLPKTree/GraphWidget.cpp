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

	this->tree_level_height = 100;
	this->tree_nodes_width = 80;
}

void GraphWidget::SetTree(std::vector<std::vector<Expression*>> tree)
{
	QGraphicsScene *scene = new QGraphicsScene(this);
	scene->setItemIndexMethod(QGraphicsScene::NoIndex);

	// set background width based on how many level the tree has to the power of max leafs of a binary tree
	int scene_width = std::pow(tree.size() + 1, 2) * 10;
	scene->setSceneRect(-scene_width, -(tree.size() * this->tree_level_height), scene_width, tree.size() * this->tree_level_height);
	setScene(scene);

	// set visual effects/relative positions of window/widget
	setCacheMode(CacheBackground);
	setViewportUpdateMode(BoundingRectViewportUpdate);
	setRenderHint(QPainter::Antialiasing);
	setTransformationAnchor(AnchorUnderMouse);
	scale(qreal(0.8), qreal(0.8));

	setMinimumSize(this->screen_width, this->screen_height);
	setWindowTitle(tr("PLP KTree"));

	// set root node
	this->root_node = new Node(this, QString(tree[0][0]->GetExpressionName()), Qt::red, Qt::darkRed);
	this->nodes.emplace_back(this->root_node);

	// add nodes in nodes vector
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
				scene->addItem(edges.back()); // add edges before nodes - they will get the nodes position which will be set up later
			}
			current_node++;
		}
	}

	// set all nodes in the middle of the background in x axis and on a tree_level_width * level/background_size on y axis
	this->root_node->setPos(QPointF(-scene_width / 2, -scene->height() + 40)); // 40 - drop it a bit lower than the edge of the background
	QPointF source_pos;
	for (auto node : nodes)
	{
		int count = 0; // how many time is node as a source node in edges vector?
		for (auto edge : edges)
		{
			if (edge->SourceNode() == node)
			{
				count++;
			}
		}

		if (count != 0) // if node is not leaf it should be at least once in the edges vector as source node
		{
			for (int index = edges.size() - 1; index >= 0; index--)
			{
				if (edges[index]->SourceNode() == node)
				{
						source_pos = edges[index]->SourceNode()->GetPos();
						// set all nodes with a small offset (tree_nodes_width) which will center them later taking root as a fixed middle point on y axis for all child nodes
						edges[index]->DestNode()->setPos(-scene_width / 2 + tree_nodes_width / 2, source_pos.y() + this->tree_level_height);	
				}
			}
		}
	}

	// add nodes in scene
	for (auto node : nodes)
	{
		scene->addItem(node);
	}

	// balance nodes position
	int nodes_moved = true;			
	std::vector<int> indexes;
	int balance_position_index;
	while (nodes_moved)	// there are still too closer nodes
	{
		nodes_moved = false;
		for (int index01 = 0; index01 < nodes.size(); index01++)
		{
			indexes.emplace_back(index01);
			for (int index02 = index01 + 1; index02 < nodes.size(); index02++)
			{
				qreal difference = abs(nodes[index01]->GetPos().x() - nodes[index02]->GetPos().x());
				if (nodes[index01]->GetPos().y() == nodes[index02]->GetPos().y() &&
					difference < tree_nodes_width) // if 2 nodes are on the same y axis but too close on x axis
				{
					indexes.emplace_back(index02); // mark and center them later
					nodes_moved = true;
				}
			}

			if (indexes.size() == 1) // if this node doesn't have any other node too close on x axis
			{
				indexes.clear();
				continue;
			}

			balance_position_index = - indexes.size() / 2; // upper offset useful here while centering all nodes
			for (auto index : indexes)
			{
				nodes[index]->setPos(nodes[index]->GetPos().x() + tree_nodes_width * balance_position_index, nodes[index]->GetPos().y());
				balance_position_index++;
			}
			indexes.clear();
		}
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
