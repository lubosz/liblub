/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "GraphWidget.h"
#include "Edge.h"
#include "GraphNode.h"

#include <QtGui>

#include <math.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0) {
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.8), qreal(0.8));
    setMinimumSize(400, 400);
    setWindowTitle(tr("Elastic GraphNodes"));

    GraphNode *GraphNode1 = new GraphNode(this);
    GraphNode *GraphNode2 = new GraphNode(this);
    GraphNode *GraphNode3 = new GraphNode(this);
    GraphNode *GraphNode4 = new GraphNode(this);
    centerGraphNode = new GraphNode(this);
    GraphNode *GraphNode6 = new GraphNode(this);
    GraphNode *GraphNode7 = new GraphNode(this);
    GraphNode *GraphNode8 = new GraphNode(this);
    GraphNode *GraphNode9 = new GraphNode(this);
    scene->addItem(GraphNode1);
    scene->addItem(GraphNode2);
    scene->addItem(GraphNode3);
    scene->addItem(GraphNode4);
    scene->addItem(centerGraphNode);
    scene->addItem(GraphNode6);
    scene->addItem(GraphNode7);
    scene->addItem(GraphNode8);
    scene->addItem(GraphNode9);
    scene->addItem(new Edge(GraphNode1, GraphNode2));
    scene->addItem(new Edge(GraphNode2, GraphNode3));
    scene->addItem(new Edge(GraphNode2, centerGraphNode));
    scene->addItem(new Edge(GraphNode3, GraphNode6));
    scene->addItem(new Edge(GraphNode4, GraphNode1));
    scene->addItem(new Edge(GraphNode4, centerGraphNode));
    scene->addItem(new Edge(centerGraphNode, GraphNode6));
    scene->addItem(new Edge(centerGraphNode, GraphNode8));
    scene->addItem(new Edge(GraphNode6, GraphNode9));
    scene->addItem(new Edge(GraphNode7, GraphNode4));
    scene->addItem(new Edge(GraphNode8, GraphNode7));
    scene->addItem(new Edge(GraphNode9, GraphNode8));

    GraphNode1->setPos(-50, -50);
    GraphNode2->setPos(0, -50);
    GraphNode3->setPos(50, -50);
    GraphNode4->setPos(-50, 0);
    centerGraphNode->setPos(0, 0);
    GraphNode6->setPos(50, 0);
    GraphNode7->setPos(-50, 50);
    GraphNode8->setPos(0, 50);
    GraphNode9->setPos(50, 50);
}

void GraphWidget::itemMoved() {
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Up:
        centerGraphNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        centerGraphNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        centerGraphNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        centerGraphNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    case Qt::Key_Space:
    case Qt::Key_Enter:
        foreach (QGraphicsItem *item, scene()->items()) {
            if (qgraphicsitem_cast<GraphNode *>(item))
                item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
        }
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::timerEvent(QTimerEvent *event) {
    Q_UNUSED(event);

    QList<GraphNode *> graphNodes;
    foreach (QGraphicsItem *item, scene()->items()) {
        if (GraphNode *graphNode = qgraphicsitem_cast<GraphNode *>(item))
            graphNodes << graphNode;
    }

    foreach (GraphNode *graphNode, graphNodes)
        graphNode->calculateForces();

    bool itemsMoved = false;
    foreach (GraphNode *graphNode, graphNodes) {
        if (graphNode->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
	painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
	painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect(sceneRect.left() + 4, sceneRect.top() + 4,
                    sceneRect.width() - 4, sceneRect.height() - 4);
    QString message(tr("Click and drag the GraphNodes around, and zoom with the mouse "
                       "wheel or the '+' and '-' keys"));

    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}

void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
