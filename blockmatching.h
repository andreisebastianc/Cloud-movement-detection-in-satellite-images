#ifndef BLOCKMATCHING_H
#define BLOCKMATCHING_H

#include <QString>
#include <QList>
#include <QImage>
#include <QPair>
#include <QDebug>

inline uint qHash(const QPoint& p)
{
    return qHash(p.x()+p.y());
}

class BlockMatching
{
public:
    BlockMatching();
    virtual ~BlockMatching();
    void setConstraitmentSizes(int blockSize = 20,int searchWindowSize = 60);
    void setFirstFrame(QString firstFramePath);
    void setSecondFrame(QString secondFramePath);
    void setFirstFrame(QImage* firstFrame);
    void setSecondFrame(QImage* secondFrame);
    void setCoeficient(int value=0);
    QList<QPair<QPoint,QPoint> > getWhatToDraw();

protected:
    int blockSize; // the size of the blocks
    int searchWindowSize; // the size of the window where to search for matching blocks
    bool firstFrameIsSet;
    bool secondFrameIsSet;
    int duplicatesCounter;
    QImage* secondFrame;
    QImage* firstFrame;
    QList<QPair<QPoint,QPoint> > toDraw;
};

#endif // BLOCKMATCHING_H
