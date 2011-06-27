#ifndef RhombusSearch_H
#define RhombusSearch_H

#include <QThread>
#include <QImage>
#include <QString>
#include <QPair>
#include <QHash>
#include <QPoint>
#include "math.h"
#include "blockmatching.h"


struct Rhombus{
    QPoint center;
    int centerSSD;
    QHash<QPoint,int> points;
};

class RhombusSearch : public BlockMatching
{
    Q_OBJECT

public:
    explicit RhombusSearch();
    ~RhombusSearch();
    void setSize(int newSize,ObjectType type=BigRhombus);
    void start();
    void stop();

private:
    short bigRhombusSpread;
    short smallRhombusSpread;
    volatile bool stopped;
    Rhombus rhombus;
    QList<QPoint> getRhombusPoints(QPoint center,ObjectType type=BigRhombus);

protected:
    void run();

signals:
    void operationsComplete();

public slots:

};


#endif // RhombusSearch_H
