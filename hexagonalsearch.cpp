#include "hexagonalsearch.h"

HexagonalSearch::HexagonalSearch()
{
    this->bigHexagonSpread = 2;
    this->stopped = false;
    this->blockSize = 20;
    this->searchWindowSize = 60;
    this->firstFrame = new QImage();
    this->secondFrame = new QImage();
    this->duplicatesCounter = 1;
}

HexagonalSearch::~HexagonalSearch(){
}


void HexagonalSearch::run(){
    if(this->firstFrameIsSet && this->secondFrameIsSet){


        int blocksInWindow = this->searchWindowSize/this->blockSize;
        int numberOfBlocks_X = this->firstFrame->width() / this->blockSize;
        int numberOfBlocks_Y = this->firstFrame->height() / this->blockSize;

        long sum;
        int currentBlockPreparedVals[this->blockSize][this->blockSize], valuesInTestedBlock;
        int currentBlock_x, currentBlock_y,blockTested_x, blockTested_y;
        int currentBlock_centX, currentBlock_centY, blockTested_centX, blockTested_centY;
        int searchWindow_x, searchWindow_y;
        long prevSSD;
        bool mantainsPosition;
        int conflictCounter;
        QPoint drawFrom, drawTo;

        // goes through each block of the first image
        for(int firstFrame_i=0;firstFrame_i<numberOfBlocks_X;firstFrame_i++){
            for(int firstFrame_j=0;firstFrame_j<numberOfBlocks_Y;firstFrame_j++){

                prevSSD = 65000;
                conflictCounter = 0;
                mantainsPosition = true;
                sum = 0;
                bool resultFound = false;

                currentBlock_x = firstFrame_i * this->blockSize;
                currentBlock_y = firstFrame_j * this->blockSize;
                //get the center points of the current block
                currentBlock_centX = currentBlock_x + this->blockSize/2;
                currentBlock_centY = currentBlock_y + this->blockSize/2;

                // sets the real coordinates of the top left point of the block in the first image
                drawFrom.setX(currentBlock_centX);
                drawFrom.setY(currentBlock_centY);
                //set the center of the big hexagon
                bigHexagon.center = drawFrom;

                // fills an array of int with the values of each pixel in block,
                // basically calculates the gray values of the current block
                // into an array so it's not calculated every time
                for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                    for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                        currentBlockPreparedVals[block_i][block_j] = qGray(this->firstFrame->pixel(block_i+currentBlock_x,block_j+currentBlock_y));
                    }
                }

                //position the search window with the block inside, centered
                searchWindow_x = currentBlock_centX - (this->searchWindowSize-this->blockSize)/2;
                searchWindow_y = currentBlock_centY - (this->searchWindowSize-this->blockSize)/2;

                //hexagonal search in the search window for matching block
                while(!resultFound){
                    sum = 0;
                    //build the big hexagon by getting the points that make it
                    //get big hexagon points
                    QList<QPoint> newHexa = this->getBigHexagonPoints(bigHexagon.center);
                    //remove points from the QHash that don't represent the hexagon anymore
                    QHash<QPoint,int> newPoints;
                    //insert the new points with SSD -1 to be calculated at next run
                    for(int i=0;i<newHexa.size();i++){
                        if(!bigHexagon.points.contains(newHexa.at(i))){
                            newPoints.insert(newHexa.at(i),-1);
                        }
                        else{
                            newPoints.insert(newHexa.at(i),bigHexagon.points.value(newHexa.at(i)));
                        }
                    }
                    this->bigHexagon.points = newPoints;

                    //and calculate ssd for each point to store in the hexagon data structure
                    //ssd for the center
                    blockTested_x = bigHexagon.center.x() - this->blockSize/2;
                    blockTested_y = bigHexagon.center.y() - this->blockSize/2;
                    for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                        for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                            if(blockTested_x > 0 && blockTested_x < this->secondFrame->width()-this->blockSize
                                    && blockTested_y>0 && blockTested_y < this->secondFrame->height()-this->blockSize){
                                valuesInTestedBlock = currentBlockPreparedVals[block_i][block_j] -qGray(this->secondFrame->pixel(block_i+blockTested_x,block_j+blockTested_y));
                                sum += valuesInTestedBlock * valuesInTestedBlock;
                            }
                        }
                    }
                    bigHexagon.centerSSD = sum;

                    //ssd for the points stored in the QHash
                    /**
                      *@todo functie din calculul ssd
                      *@todo functie din construirea hexagoanelor
                      */
                    QHash<QPoint,int>::const_iterator i = bigHexagon.points.constBegin();
                    while (i != bigHexagon.points.constEnd()) {
                        if(i.value() == -1){
                            blockTested_x = i.key().x() - this->blockSize/2;
                            blockTested_y = i.key().y() - this->blockSize/2;
                            for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                                for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                                    if(blockTested_x > 0 && blockTested_x < this->secondFrame->width()-this->blockSize
                                            && blockTested_y>0 && blockTested_y < this->secondFrame->height()-this->blockSize){
                                        valuesInTestedBlock = currentBlockPreparedVals[block_i][block_j] -qGray(this->secondFrame->pixel(block_i+blockTested_x,block_j+blockTested_y));
                                        sum += valuesInTestedBlock * valuesInTestedBlock;
                                    }
                                }
                            }
                            bigHexagon.points.insert(i.key(),sum);
                        }
                        ++i;
                    }

                    //check the closest value - the smallest stored sum
                    //from the QHash values
                    while (i != bigHexagon.points.constEnd()) {
                        if(prevSSD > i.value()){
                            prevSSD = i.value();
                            drawTo = i.key();
                        }
                    }
                    //for the center
                    //set so it prefers staying at center
                    if(bigHexagon.centerSSD <= prevSSD){
                        //if the one at center, build small hexagon
                        //compare and give result the best fit of the small hexagon points
                        //else move the center of the hexagon at the smallest difference
                        resultFound = true;
                    }
                    else{ //center has bigger SSD than a side => rebuild the hexagon
                        //add the old center in the QHash
                        bigHexagon.points.insert(bigHexagon.center,bigHexagon.centerSSD);


                        //new center - gets the ssd value and removed from the hexagon
                        bigHexagon.center = drawTo;
                        bigHexagon.centerSSD = bigHexagon.points.value(drawTo);
                        bigHexagon.points.remove(drawTo);
                    }
                }


                    QPair<QPoint,QPoint> drawPoints;
                    drawPoints.first = drawFrom;
                    drawPoints.second = drawTo;
                    this->toDraw.append(drawPoints);

            }
        }
    }
    emit operationsComplete();
}

QList<QPoint> HexagonalSearch::getBigHexagonPoints(QPoint center){
    QList<QPoint> toReturn;
    toReturn << QPoint(center.x()-this->bigHexagonSpread*this->blockSize,center.y());
    toReturn << QPoint(center.x()+this->bigHexagonSpread*this->blockSize,center.y());
    toReturn << QPoint(center.x()-(this->bigHexagonSpread*this->blockSize)/2,center.y()-this->bigHexagonSpread*this->blockSize);
    toReturn << QPoint(center.x()+(this->bigHexagonSpread*this->blockSize)/2,center.y()-this->bigHexagonSpread*this->blockSize);
    toReturn << QPoint(center.x()-(this->bigHexagonSpread*this->blockSize)/2,center.y()+this->bigHexagonSpread*this->blockSize);
    toReturn << QPoint(center.x()+(this->bigHexagonSpread*this->blockSize)/2,center.y()+this->bigHexagonSpread*this->blockSize);
    return toReturn;
}

void HexagonalSearch::start(){
    this->stopped = false;
    run();
}

void HexagonalSearch::stop(){
    this->stopped = true;
}
