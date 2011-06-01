#ifndef MOVEMENTFINDER_H
#define MOVEMENTFINDER_H

#include <QThread>
#include <QImage>
#include <QString>
#include <QPair>
#include <QDebug>
#include "math.h"

enum Corelations {SSD = 0};

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
    void setCoeficient(int value=0);
    QList<QPair<QPoint,QPoint> > getWhatToDraw();

private:
    volatile bool stopped;
    int patternSize; // the size of the blocks
    int windowSize; // the size of the window where to search for matching blocks
    bool firstFrameIsSet;
    bool secondFrameIsSet;
    int duplicatesCounter;
    Corelations corelationToUse;
    QImage* secondFrame;
    QImage* firstFrame;
    QList<QPair<QPoint,QPoint> > toDraw;


protected:
    void run();

signals:
    void operationsComplete();

public slots:

};

#endif // MOVEMENTFINDER_H
