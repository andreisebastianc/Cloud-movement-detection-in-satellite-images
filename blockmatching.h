#ifndef BLOCKMATCHING_H
#define BLOCKMATCHING_H

#include <QString>
#include <QList>
#include <QImage>
#include <QPair>
#include <QDebug>
#include <QThread>

inline uint qHash(const QPoint& p)
{
    return qHash(p.x()+p.y());
}

enum OperationType{
    NormalOperation,
    ImprovedOperation
};

enum ObjectType{
    BigRhombus,
    SmallRhombus,
    BigHexagon,
    SmallHexagon
};

class BlockMatching :public QThread
{
    Q_OBJECT

public:
    BlockMatching();
    virtual ~BlockMatching();
    void setConstraitmentSizes(int blockSize = 7,int searchWindowSize = 35);
    void setFirstFrame(QString firstFramePath);
    void setSecondFrame(QString secondFramePath);
    void setFirstFrame(QImage* firstFrame);
    void setSecondFrame(QImage* secondFrame);
    void setCoeficient(int value=0);
    void setHistorySize(int value = 3);
    void setOperationType(OperationType type = NormalOperation);
    OperationType getOperationType() {return this->operationType;}
    QList<QPair<QPoint,QPoint> > getWhatToDraw();

protected:
    void run() = 0;
    int blockSize; // the size of the blocks
    int searchWindowSize; // the size of the window where to search for matching blocks
    bool firstFrameIsSet;
    bool secondFrameIsSet;
    int duplicatesCounter;
    short cbfSize;
    bool pointInSearchWindow(QPoint point,QPoint searchWindow_Top_Left_Coord);
    QImage* secondFrame;
    QImage* firstFrame;
    QList<QPair<QPoint,QPoint> > toDraw;
    QList<QPair<QPoint,QVector<QPoint> > >* candidatePoints;
    OperationType operationType;
    short historySize;
    QVector<QPair<QPoint,int> >* localCandidateBestFits;
};

#endif // BLOCKMATCHING_H
