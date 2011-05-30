#include "movementfinder.h"

MovementFinder::MovementFinder()
{
    this->stopped = false;
    this->patternSize = 20;
    this->windowSize = 60;
    this->firstFrame = new QImage();
}

MovementFinder::~MovementFinder(){
    delete this->firstFrame;
    delete this->secondFrame;
}

/**
  *
  */
void MovementFinder::setFirstFrame(QString firstFramePath){
    this->firstFrameIsSet = this->firstFrame->load(firstFramePath);
}

/**
  *
  */
void MovementFinder::setSecondFrame(QString secondFramePath){
    this->secondFrameIsSet = this->secondFrame->load(secondFramePath);
}

/**
  *
  */
void MovementFinder::setConstraitmentSizes(int patternSize, int windowSize){
    this->patternSize = patternSize;
    this->windowSize = windowSize;
}

/**
  *
  */
void MovementFinder::run(){

        if(this->firstFrameIsSet){

            //set the middle of search window close to the block as possible
//            int searchWindowXPos = 0 + this->patternSize / 2;
//            int searchWindowYPos = 0 + this->patternSize / 2;

            int blocksInWindow = this->windowSize/this->patternSize;
            int totalNumberOfBlocks = this->firstFrame->height()/this->patternSize;
            long sum;
            int valueFF[this->patternSize][this->patternSize], valueSF;
            int ffModifierX, ffModifierY, sfModifierX, sfModifierY;
            int prevSSD;
            QPoint drawFrom, drawTo;
            bool mantainsPosition, conflicted;
            int conflictCounter;

            for(int fframe_i=0;fframe_i<totalNumberOfBlocks;fframe_i++){
                for(int fframe_j=0;fframe_j<totalNumberOfBlocks;fframe_j++){

                    drawFrom.setX(fframe_i);
                    drawFrom.setY(fframe_j);
                    mantainsPosition = true;
                    conflicted = false;
                    conflictCounter = 0;
                    prevSSD = 60000;


                    //qDebug() << "blocul: "<<fframe_i<<", "<<fframe_j;

                    ffModifierX = fframe_i * this->patternSize;
                    ffModifierY = fframe_j * this->patternSize;
                    for(int block_i = 0 ; block_i < this->patternSize ; block_i++){
                        for(int block_j = 0 ; block_j < this->patternSize ; block_j++){
                            valueFF[block_i][block_j] = qGray(this->firstFrame->pixel(block_i+ffModifierX,block_j+ffModifierY));
                        }
                    }

                    for(int i=0;i<blocksInWindow;i++){
                        for(int j=0;j<blocksInWindow;j++){
                            sfModifierX = i * this->patternSize;
                            sfModifierY = j * this->patternSize;
                            sum = 0;

                            for(int block_i = 0 ; block_i < this->patternSize ; block_i++){
                                for(int block_j = 0 ; block_j < this->patternSize ; block_j++){
                                    valueSF = valueFF[block_i][block_j];
                                    valueSF -= qGray(this->secondFrame->pixel(block_i+sfModifierX,block_j+sfModifierY));
                                    sum += valueSF * valueSF;
                                }
                            }

                            //qDebug() << "[" << i <<","<<j<<"]= "<< sum;

                            if(prevSSD == sum){
                                conflictCounter++;
                            }

                            // the block that is matched
                            if(i==fframe_i && j==fframe_j){
                                if(prevSSD >= sum){
                                    prevSSD = sum;
                                    mantainsPosition = true;
                                }
                            }
                            else{
                                if(prevSSD > sum){
                                    mantainsPosition = false;
                                    conflictCounter = 0;
                                    prevSSD = sum;
                                    drawTo.setX(i);
                                    drawTo.setY(j);
                                    //qDebug() << "[" << i <<","<<j<<"]= "<< sum;
                                }
                            }
                        }
                    }

                    if(!mantainsPosition && conflictCounter==0){
                        QPair<QPoint,QPoint> drawPoints;
                        drawPoints.first = drawFrom;
                        drawPoints.second = drawTo;
                        this->toDraw.append(drawPoints);

                        qDebug() << drawFrom << " to " << drawTo;
                    }

                }
            }

        }

        emit operationsComplete();
}

QList<QPair<QPoint,QPoint> > MovementFinder::getWhatToDraw(){
    return this->toDraw;
}

void MovementFinder::start(){
    this->stopped = false;
    run();
}

void MovementFinder::stop(){
    this->stopped = true;
}
