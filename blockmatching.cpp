#include "blockmatching.h"

BlockMatching::BlockMatching() : QThread()
{
    this->cbfSize = 3;
}

BlockMatching::~BlockMatching(){
    delete this->firstFrame;
    delete this->secondFrame;
    delete this->candidatePoints;
    delete this->localCandidateBestFits;
}


/**
  *
  */
void BlockMatching::setFirstFrame(QString firstFramePath){
    this->firstFrameIsSet = true;
    this->firstFrameIsSet = this->firstFrame->load(firstFramePath);
}

void BlockMatching::setFirstFrame(QImage *firstFrame){
    this->firstFrameIsSet = true;
    this->firstFrame = firstFrame;
}

/**
  *
  */
void BlockMatching::setSecondFrame(QString secondFramePath){
    this->secondFrameIsSet = true;
    this->secondFrameIsSet = this->secondFrame->load(secondFramePath);
}

void BlockMatching::setSecondFrame(QImage *secondFrame){
    this->secondFrameIsSet = true;
    this->secondFrame = secondFrame;
}

/**
  *
  */
void BlockMatching::setConstraitmentSizes(int patternSize, int windowSize){
    this->blockSize = patternSize;
    this->searchWindowSize = windowSize;
}

QList<QPair<QPoint,QPoint> > BlockMatching::getWhatToDraw(){
    return this->toDraw;
}

void BlockMatching::setCoeficient(int value){
    this->duplicatesCounter = value;
    qDebug() << this->duplicatesCounter;
}

bool BlockMatching::pointInSearchWindow(QPoint point, QPoint searchWindow_Top_Left_Coord){
    if(point.x() < searchWindow_Top_Left_Coord.x() || point.x() > searchWindow_Top_Left_Coord.x()+this->searchWindowSize)
        return false;
    if(point.y() < searchWindow_Top_Left_Coord.y() || point.x() > searchWindow_Top_Left_Coord.y()+this->searchWindowSize)
        return false;
    return true;
}

void BlockMatching::setHistorySize(int value){
    this->historySize = value;
}

void BlockMatching::setOperationType(OperationType type){
    this->operationType = type;
}
