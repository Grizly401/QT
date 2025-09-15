#ifndef POINTSCOLLECTION_H
#define POINTSCOLLECTION_H

#include <QObject>
#include <QPointF>
#include <QVector>

class PointsCollection : public QObject
{
    Q_OBJECT
public:
    explicit PointsCollection(QObject *parent = nullptr);
    const QVector<QPointF> & getPoints() const { return _points; }
    void setPoints(QVector<QPointF> & points);

signals:
    void collectionChanged();

public slots:
    void addPoint(double x, double y);
    void addPoint(const QPointF & point);
    void clear();

private:
    QVector<QPointF> _points;
};

#endif // POINTSCOLLECTION_H
