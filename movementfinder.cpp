#include "movementfinder.h"

MovementFinder::MovementFinder()
{
    this->stopped = false;
    this->patternSize = 20;
    this->windowSize = 60;
    this->firstFrame = new QImage();
    this->secondFrame = new QImage();
    this->corelationToUse = Corelations(0);
    this->duplicatesCounter = 1;
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
            QPoint drawFrom, drawTo;
            long sum;
            int valueFF[this->patternSize][this->patternSize], valueSF;
            int ffModifierX, ffModifierY,sfModifierX, sfModifierY;
            int prevSSD;
            bool mantainsPosition;
            int conflictCounter;

            // goes through each block of the first image
            for(int fframe_i=0;fframe_i<totalNumberOfBlocks;fframe_i++){
                for(int fframe_j=0;fframe_j<totalNumberOfBlocks;fframe_j++){

                    // sets the draw starting point

                    prevSSD = 60000;
                    conflictCounter = 0;
                    mantainsPosition = true;
                    sum = 0;

                    // sets the real coordinates of the top left point of the block in the first image
                    ffModifierX = fframe_i * this->patternSize;
                    ffModifierY = fframe_j * this->patternSize;

                    drawFrom.setX(ffModifierX);
                    drawFrom.setY(ffModifierY);

                    // fills an array of int with the values of each pixel in block, basically copies a block into an array
                    for(int block_i = 0 ; block_i < this->patternSize ; block_i++){
                        for(int block_j = 0 ; block_j < this->patternSize ; block_j++){
                            valueFF[block_i][block_j] = qGray(this->firstFrame->pixel(block_i+ffModifierX,block_j+ffModifierY));
                        }
                    }

                    // positions the search window X coordinate so it doesn't jump out of the search into image
                    int windowX = ffModifierX-(this->windowSize-this->patternSize)/2;
                    if(windowX<0){
                        windowX = 0;
                    }
                    int xBl = this->secondFrame->width()-(ffModifierX+this->patternSize+(this->windowSize-this->patternSize)/2);
                    if(xBl<0){
                        windowX += xBl;
                    }

                    // positions the search window Y coordinate so it doesn't jump out of the search into image
                    int windowY = ffModifierY-(this->windowSize-this->patternSize)/2;
                    if(windowY<0){
                        windowY = 0;
                    }
                    xBl = this->secondFrame->height()-(ffModifierY+this->patternSize+(this->windowSize-this->patternSize)/2);
                    if(xBl<0){
                        windowY += xBl;
                    }

                    for(int i=0;i<blocksInWindow;i++){
                        for(int j=0;j<blocksInWindow;j++){
                            sfModifierX = i * this->patternSize + windowX;
                            sfModifierY = j * this->patternSize + windowY;
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
                                    drawTo.setX(sfModifierX);
                                    drawTo.setY(sfModifierY);
                                    //qDebug() << "[" << i <<","<<j<<"]= "<< sum;
                                }
                            }
                        }
                    }


                    if(!mantainsPosition && conflictCounter<=this->duplicatesCounter){
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

void MovementFinder::setCoeficient(int value){
    this->duplicatesCounter = value;
    qDebug() << this->duplicatesCounter;
}
