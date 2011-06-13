#include "blockmatching.h"

BlockMatching::BlockMatching()
{
}

BlockMatching::~BlockMatching(){
    delete this->firstFrame;
    delete this->secondFrame;
}

/**
  *
  */
void BlockMatching::setFirstFrame(QString firstFramePath){
    this->firstFrameIsSet = this->firstFrame->load(firstFramePath);
}

void BlockMatching::setFirstFrame(QImage *firstFrame){
    this->firstFrame = firstFrame;
    this->firstFrameIsSet = true;
}

/**
  *
  */
void BlockMatching::setSecondFrame(QString secondFramePath){
    this->secondFrameIsSet = this->secondFrame->load(secondFramePath);
    this->secondFrameIsSet = true;
}

void BlockMatching::setSecondFrame(QImage *secondFrame){
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
