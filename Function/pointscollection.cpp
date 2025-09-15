#include "pointscollection.h"

PointsCollection::PointsCollection(QObject *parent)
    : QObject{parent}
{}


void PointsCollection::addPoint(double x, double y)
{
    addPoint(QPointF(x, y));
}

void PointsCollection::addPoint(const QPointF & point)
{
    _points.append(point);

    emit collectionChanged();
}

void PointsCollection::setPoints(QVector<QPointF> & points)
{
    _points = points;

    emit collectionChanged();
}

void PointsCollection::clear()
{
    _points.clear();
}
