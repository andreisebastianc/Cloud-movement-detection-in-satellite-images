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

enum RhombusType{
    BigRhombus,
    SmallRhombus
};

struct Rhombus{
    QPoint center;
    int centerSSD;
    QHash<QPoint,int> points;
};

class RhombusSearch : public QThread, public BlockMatching
{
    Q_OBJECT
public:
    explicit RhombusSearch();
    ~RhombusSearch();
    void setSize(int newSize,RhombusType type=BigRhombus);
    void start();
    void stop();

private:
    short bigRhombusSpread;
    short smallRhombusSpread;
    volatile bool stopped;
    Rhombus rhombus;
    QList<QPoint> getRhombusPoints(QPoint center,RhombusType type=BigRhombus);

protected:
    void run();

signals:
    void operationsComplete();

public slots:

};


#endif // RhombusSearch_H
