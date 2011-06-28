#ifndef HEXAGONALSEARCH_H
#define HEXAGONALSEARCH_H

#include <QThread>
#include <QImage>
#include <QString>
#include <QPair>
#include <QHash>
#include <QPoint>
#include "math.h"
#include "blockmatching.h"

struct Hexagon{
    QPoint center;
    int centerSSD;
    QHash<QPoint,int> points;
};

class HexagonalSearch : public BlockMatching
{
    Q_OBJECT

public:
    explicit HexagonalSearch();
    ~HexagonalSearch();
    void start();
    void setSize(int newSize,ObjectType type=BigRhombus);
    void stop();

private:
    short bigHexagonSpread;
    short smallHexagonSpread;
    volatile bool stopped;
    Hexagon hexagon;
    QList<QPoint> getBigHexagonPoints(QPoint center);
    QList<QPoint> getSmallHexagonPoints(QPoint center);
    QList<QPoint> getHexagonPoints(QPoint center,ObjectType type=BigRhombus);


protected:
    void run();

signals:
    void operationsComplete();

public slots:

};


#endif // HEXAGONALSEARCH_H
