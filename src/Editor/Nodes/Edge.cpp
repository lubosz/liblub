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

#include <QPainter>
#include <QGraphicsSceneMouseEvent>

#include "Edge.h"
#include "GraphNode.h"
#include "System/Logger.h"
#include <math.h>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

Edge::Edge(GraphNode *sourceGraphNode, GraphNode *destGraphNode, string name)
    : arrowSize(10), name(name)
{
//    setAcceptedMouseButtons(0);
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    source = sourceGraphNode;
    dest = destGraphNode;
    source->addEdge(this);
    dest->addEdge(this);
    centerPoint = QPointF();
    adjust();
}

GraphNode *Edge::sourceGraphNode() const
{
    return source;
}

GraphNode *Edge::destGraphNode() const
{
    return dest;
}

void Edge::adjust()
{
    if (!source || !dest)
        return;

    QLineF line(mapFromItem(source, 100, 10), mapFromItem(dest, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        sourcePoint = line.p1() + edgeOffset;
        destPoint = line.p2() - edgeOffset;
    } else {
        sourcePoint = destPoint = line.p1();
    }

    centerPoint = (sourcePoint + destPoint) / 2;
    centerPoint.setY(centerPoint.y() + 20 * source->getEdgeNumberTo(this));
}

QRectF Edge::boundingRect() const
{
    if (!source || !dest)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    QPainterPath path = updatePath();
    QRectF lineBox =  path.boundingRect().adjusted(-extra, -extra, extra, extra);
    return lineBox.unite(textRect);
}

QPainterPath Edge::shape() const {
    QPainterPath path;
    path.addRect(textRect);
    return path;
}

QPainterPath Edge::updatePath() const {
    QPainterPath path(sourcePoint);
    QPointF c1 = (sourcePoint + centerPoint) / 2.0;
    QPointF c2 = (centerPoint + destPoint) / 2.0;

    QLineF foo(sourcePoint, centerPoint);
    QLineF normal = foo.normalVector();

    QLineF foo2(centerPoint, destPoint);
    QLineF normal2 = foo2.normalVector();

    float scale = -0.1;

    path.quadTo((normal.p2() - sourcePoint) * scale + c1, centerPoint);
    path.quadTo((normal2.p2() - centerPoint) * scale + c2, destPoint);

    return path;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (!source || !dest)
        return;

    QLineF line(sourcePoint, destPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QPainterPath path = updatePath();
    painter->drawPath(path);

    // Draw the arrows
    QLineF line1(sourcePoint, centerPoint);
    QLineF line2(centerPoint, destPoint);

    double angle1 = ::acos(line1.dx() / line1.length());
    if (line1.dy() >= 0)
        angle1 = TwoPi - angle1;

    double angle2 = ::acos(line2.dx() / line2.length());
    if (line2.dy() >= 0)
        angle2 = TwoPi - angle2;

    QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle1 + Pi / 3) * arrowSize,
                                                  cos(angle1 + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle1 + Pi - Pi / 3) * arrowSize,
                                                  cos(angle1 + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = destPoint + QPointF(sin(angle2 - Pi / 3) * arrowSize,
                                              cos(angle2 - Pi / 3) * arrowSize);
    QPointF destArrowP2 = destPoint + QPointF(sin(angle2 - Pi + Pi / 3) * arrowSize,
                                              cos(angle2 - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

    // Text
    QFont font = painter->font();
    font.setBold(true);
    font.setPointSize(8);
    painter->setFont(font);
    painter->setPen(Qt::black);

    QFontMetrics fm(font);
    int pixelsWide = fm.width(QString::fromStdString(name));
    int pixelsHigh = fm.height();

     textRect = QRectF(centerPoint.x(), centerPoint.y(), pixelsWide, pixelsHigh);

    painter->drawText(textRect, QString::fromStdString(name));
}


void Edge::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    prepareGeometryChange();
    centerPoint = event->scenePos();
    update();
}
