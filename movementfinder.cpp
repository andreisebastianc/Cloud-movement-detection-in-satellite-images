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
            long sum;
            int valueFF[this->patternSize][this->patternSize], valueSF;
            int ffModifierX, ffModifierY, sfModifierX, sfModifierY;
            int prevSSD = 65532;

            sum = 0;
            for(int fframe_i=0;fframe_i<blocksInWindow;fframe_i++){
                for(int fframe_j=0;fframe_j<blocksInWindow;fframe_j++){

                    QPoint drawFrom, drawTo;
                    drawFrom.setX(fframe_i);
                    drawFrom.setY(fframe_j);


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

                            for(int block_i = 0 ; block_i < this->patternSize ; block_i++){
                                for(int block_j = 0 ; block_j < this->patternSize ; block_j++){
                                    valueSF = valueFF[block_i][block_j];
                                    valueSF -= qGray(this->secondFrame->pixel(block_i+sfModifierX,block_j+sfModifierY));
                                    sum += valueSF * valueSF;
                                }
                            }
                            //qDebug() << "[" << i <<","<<j<<"]= "<< sum;
                            if(prevSSD > sum){
                                prevSSD = sum;
                                drawTo.setX(i);
                                drawTo.setY(j);
                            }
                            sum = 0;
                        }
                    }

                    QPair<QPoint,QPoint> drawPoints;
                    drawPoints.first = drawFrom;
                    drawPoints.second = drawTo;
                    this->toDraw.append(drawPoints);
                    //qDebug() << drawFrom << " to " << drawTo;



//                    for(int block_i = 0 ; block_i < this->patternSize ; block_i++){
//                        for(int block_j = 0 ; block_j < this->patternSize ; block_j++){
//                            value = qGray(this->firstFrame->pixel(block_i+xtemp,block_j+ytemp));
//                            sum += value;
//                            sqrsum += value * value;
//                        }
//                    }

//                    mean = sum/(this->patternSize*this->patternSize);
//                    var2template = sqrsum - (sum * sumOfSquaredDiffs)/(this->patternSize*this->patternSize);

//                    for(int i=0;i<blocksInWindow;i++){
//                        for(int j=0;j<blocksInWindow;j++){
//                            sum = 0;
//                            sqrsum = 0;
//                            prodsum = 0;
//                            for(int block_i = 0 ; block_i < this->patternSize ; block_i++){
//                                for(int block_j = 0 ; block_j < this->patternSize ; block_j++){
//                                    value = qGray(this->secondFrame->pixel(block_i + i*this->patternSize,block_j + j*this->patternSize));
//                                    sum += value;
//                                    sqrsum += value*value;
//                                    prodsum += value*qGray(this->firstFrame->pixel(block_i+xtemp,block_j+ytemp));
//                                }
//                            }
//                            var2image = sqrsum-(sum*sumOfSquaredDiffs)/(this->patternSize*this->patternSize);
//                            qDebug() << "[" << i <<","<<j<<"]= "<< (prodsum - (mean*sum)) / sqrt(var2image * var2template);
//                            koef[i][j] = (prodsum - (mean*sum)) / sqrt(var2image * var2template);
//                        }
//                    }

                    // get the movements
//                    for(int i=0;i<blocksInWindow;i++){
//                        for(int j=0;j<blocksInWindow;j++){
//                        }
//                    }
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
