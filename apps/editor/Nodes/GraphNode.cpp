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
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

#include "Edge.h"
#include "GraphNode.h"
#include "GraphWidget.h"
#include "System/Timer.h"


GraphNode::GraphNode(GraphWidget *graphWidget, QString name)
    : graph(graphWidget), name(name)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    size = QSize(120, 50);
}

void GraphNode::setShaderName(string shaderName) {
    this->shaderName = shaderName;
}

void GraphNode::addEdge(Edge *edge)
{
    edgeList << edge;
    edge->adjust();
}

unsigned GraphNode::getConnectionsTo(GraphNode * node) {
    unsigned connections = 0;
    foreach (Edge * edge, edgeList) {
        if (edge->source == this && edge->dest == node
                || edge->source == node && edge->dest == this)
            connections++;

    }
    return connections;
}

unsigned GraphNode::getEdgeNumber(Edge * edge) {
    for (unsigned i = 0; i < edgeList.size(); i++)
        if (edgeList[i] == edge)
            return i;
}

unsigned GraphNode::getEdgeNumberTo(Edge * edge) {
    GraphNode * destination;

    if (edge->source == this)
        destination = edge->dest;
    else
        destination = edge->source;

    unsigned i = 0;

    foreach (Edge * someEdge, edgeList) {
        if (someEdge->dest == destination || someEdge->source == destination)
            i++;

        if (someEdge == edge)
            return i;
    }

}

QList<Edge *> GraphNode::edges() const
{
    return edgeList;
}

bool GraphNode::advance()
{
    if (newPos == pos())
        return false;
    setPos(newPos);
    return true;
}

QRectF GraphNode::boundingRect() const
{
    qreal adjust = 2;
    return QRectF(-10 - adjust, -10 - adjust,
                  size.width() + 3 + adjust, size.height() + 3 + adjust);
}

QPainterPath GraphNode::shape() const
{
    QPainterPath path;
    path.addEllipse(-10, -10, size.width(), size.height());
    return path;
}

void GraphNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) {
    QLinearGradient gradient(size.width()/2, 0, size.width()/2, size.height());
    if (option->state & QStyle::State_Sunken) {
        gradient.setColorAt(0.8, Qt::white);
        gradient.setColorAt(0, Qt::blue);
    } else {
        gradient.setColorAt(0.8, Qt::white);
        gradient.setColorAt(0, Qt::darkBlue);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));
    painter->drawRoundedRect(QRect(-10, -10, size.width(), size.height()), 5, 5);

    // Text
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(10);
    painter->setFont(font);
    painter->setPen(Qt::black);

    QFontMetrics fm(font);
    int pixelsWide = fm.width(name);
    int pixelsHigh = fm.height();
    QRectF textRect(size.width()/2.0 - pixelsWide/2.0 - 10.0, -10, pixelsWide, pixelsHigh);
    painter->drawText(textRect, name);

    font.setBold(false);
    font.setPointSize(8);
    painter->setFont(font);

    fm = QFontMetrics(font);
    pixelsWide = fm.width(QString::fromStdString(shaderName));
    pixelsHigh = fm.height();
    textRect = QRectF(-5, 5, pixelsWide, pixelsHigh);
    painter->drawText(textRect, QString::fromStdString(shaderName));


    // draw frametime
    float frameTime = drawPass->frameTime.tv_nsec / 1000000.0;
    QString fps = QString::number(float(BILLION)/float(drawPass->frameTime.tv_nsec)) + "fps";
    QString miliSecounds = QString::number(frameTime) + "ms";
//    QString timeString = miliSecounds + " " + fps;
    pixelsWide = fm.width(fps);
    textRect = QRectF(-5, 12, pixelsWide, pixelsHigh);
    painter->drawText(textRect, fps);

    pixelsWide = fm.width(miliSecounds);
    textRect = QRectF(-5, 20, pixelsWide, pixelsHigh);
    painter->drawText(textRect, miliSecounds);
}

QVariant GraphNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (Edge *edge, edgeList)
            edge->adjust();
        break;
    default:
        break;
    };
    return QGraphicsItem::itemChange(change, value);
}

void GraphNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mousePressEvent(event);
}

void GraphNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}
