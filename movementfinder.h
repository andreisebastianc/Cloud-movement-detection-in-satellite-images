#ifndef MOVEMENTFINDER_H
#define MOVEMENTFINDER_H

#include <QThread>
#include <QImage>
#include <QString>
#include <QPair>
#include <QDebug>
#include "math.h"

class MovementFinder : public QThread
{
    Q_OBJECT
public:
    explicit MovementFinder();
    ~MovementFinder();
    void start();
    void stop();
    void setConstraitmentSizes(int patternSize = 20,int windowSize = 60);
    void setFirstFrame(QString firstFrame);
    void setSecondFrame(QString secondFramePath);
    QList<QPair<QPoint,QPoint> > getWhatToDraw();

private:
    volatile bool stopped;
    int patternSize;
    int windowSize;
    bool firstFrameIsSet;
    bool secondFrameIsSet;
    QImage* firstFrame;
    QImage* secondFrame;
    QList<QPair<QPoint,QPoint> > toDraw;

protected:
    void run();

signals:
    void operationsComplete();

public slots:

};

#endif // MOVEMENTFINDER_H
