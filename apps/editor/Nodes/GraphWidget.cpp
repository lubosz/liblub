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

#include "Renderer/DeferredRenderer.h"

#include <QtGui>

#include <math.h>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent) {
    middleButton = false;
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-1000, -1000, 2000, 2000);
    setScene(scene);
    setCacheMode(CacheBackground);
//    setViewportUpdateMode(BoundingRectViewportUpdate);
    setViewportUpdateMode(FullViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    setMinimumSize(400, 400);

    lastMousePosition = QPointF(0,0);
    qreal xpos = -900;
    qreal ypos = -50;
    foreach(DrawThing * pass, DeferredRenderer::Instance().drawPasses) {
        GraphNode *graphNode = new GraphNode(this, QString::fromStdString(pass->typeName));

        DrawPass * passCheck = dynamic_cast<DrawPass*>(pass);
        if (passCheck != nullptr) {
            graphNode->setShaderName(passCheck->shader->getName());
        }

        SourcePass * sourceCheck = dynamic_cast<SourcePass*>(pass);
        if (sourceCheck != nullptr) {
            foreach(Texture * target, sourceCheck->targets) {
                targets.insert(target->name, graphNode);
            }
        }

        InOutPass * inOutCheck = dynamic_cast<InOutPass*>(pass);
        if (inOutCheck != nullptr) {
            foreach(Texture * source, inOutCheck->sources) {
                sources.insert(source->name, graphNode);
            }
        }

        SinkPass * sinkCheck = dynamic_cast<SinkPass*>(pass);
        if (sinkCheck != nullptr) {
            foreach(DebugPlane * plane, sinkCheck->debugPlanes) {
                sources.insert(plane->targetName, graphNode);
            }
        }

        if (sourceCheck && !inOutCheck) {
            // Stack source passes
            ypos -= 80;
        } else {
            xpos += 200;
        }

        graphNode->setPos(xpos,ypos);
        scene->addItem(graphNode);
    }

    foreach (string foo, targets.keys()) {
        GraphNode *sourceNode = targets[foo];

         QList<GraphNode *>targetNodes = sources.values(foo);

         foreach (GraphNode * targetNode, targetNodes) {
             scene->addItem(new Edge(sourceNode, targetNode, foo));
         }
    }
}

void GraphWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
    case Qt::Key_Plus:
        scaleView(qreal(1.2));
        break;
    case Qt::Key_Minus:
        scaleView(1 / qreal(1.2));
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void GraphWidget::wheelEvent(QWheelEvent *event) {
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        middleButton = true;
        setTransformationAnchor(NoAnchor);
    }
    lastMousePosition.setX(event->x());
    lastMousePosition.setY(event->y());
    QGraphicsView::mousePressEvent(event);
}

void GraphWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::MiddleButton) {
        middleButton = false;
        setTransformationAnchor(AnchorUnderMouse);
    }
    lastMousePosition.setX(event->x());
    lastMousePosition.setY(event->y());
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphWidget::mouseMoveEvent(QMouseEvent *event) {
    if (middleButton) {
        translate(event->x() - lastMousePosition.x(), event->y() - lastMousePosition.y());
    }
    lastMousePosition.setX(event->x());
    lastMousePosition.setY(event->y());
    QGraphicsView::mouseMoveEvent(event);
}

/*
void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect) {
    Q_UNUSED(rect);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersect(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}
*/
void GraphWidget::scaleView(qreal scaleFactor) {
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}
