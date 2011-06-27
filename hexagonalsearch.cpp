#include "hexagonalsearch.h"

HexagonalSearch::HexagonalSearch()
{
    this->bigHexagonSpread = 2;
    this->smallHexagonSpread = 1;
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

        int numberOfBlocks_X = this->firstFrame->width() / this->blockSize;
        int numberOfBlocks_Y = this->firstFrame->height() / this->blockSize;
        long sum;
        int currentBlockPreparedVals[this->blockSize][this->blockSize], valuesInTestedBlock;
        int currentBlock_x, currentBlock_y,blockTested_x, blockTested_y;
        int currentBlock_centX, currentBlock_centY;
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
                hexagon.center = drawFrom;
                hexagon.centerSSD = -1;

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
                    //build the big rhombus by getting the points that make it
                    //get big hexagon points
                    QList<QPoint> newHexa = this->getRhombusPoints(hexagon.center);
                    //remove points from the QHash that don't represent the hexagon anymore
                    QHash<QPoint,int> newPoints;
                    //insert the new points with SSD -1 to be calculated at next run
                    for(int i=0;i<newHexa.size();i++){
                        //check that the point is inside the search window, otherwise don't use it
                        if(pointInSearchWindow(newHexa.at(i),QPoint(searchWindow_x,searchWindow_y))){
                            if(!hexagon.points.contains(newHexa.at(i))){
                                newPoints.insert(newHexa.at(i),-1);
                            }
                            else{
                                newPoints.insert(newHexa.at(i),this->hexagon.points.value(newHexa.at(i)));
                            }
                        }
                    }
                    this->hexagon.points.clear();
                    this->hexagon.points = newPoints;

                    //and calculate ssd for each point to store in the hexagon data structure

                    //ssd for the center point
                    //only if the value is not set
                    if(hexagon.centerSSD == -1){
                        blockTested_x = hexagon.center.x() - this->blockSize/2;
                        blockTested_y = hexagon.center.y() - this->blockSize/2;
                        for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                            for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                                if(blockTested_x > 0 && blockTested_x < this->secondFrame->width()-this->blockSize
                                        && blockTested_y>0 && blockTested_y < this->secondFrame->height()-this->blockSize){
                                    valuesInTestedBlock = currentBlockPreparedVals[block_i][block_j] -qGray(this->secondFrame->pixel(block_i+blockTested_x,block_j+blockTested_y));
                                    sum += valuesInTestedBlock * valuesInTestedBlock;
                                }
                            }
                        }
                        hexagon.centerSSD = sum;
                        prevSSD = hexagon.centerSSD;
                    }

                    //ssd for the points stored in the QHash
                    /**
                      *@todo functie din calculul ssd
                      */
                    QHash<QPoint, int>::iterator i = hexagon.points.begin();
                    while (i != hexagon.points.end()) {
                        //first search through the entire hash and for new points calculate the SSD
                        if(i.value() == -1){
                            //position the coordinates of the block at the top left corner
                            //for easier calculations
                            blockTested_x = i.key().x() - this->blockSize/2;
                            blockTested_y = i.key().y() - this->blockSize/2;
                            //set the sum so ssd returns valid results
                            sum = 0;
                            //calculate ssd for the current block
                            for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                                for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                                    //check if block is out of image bonds
                                    if(blockTested_x > 0 && blockTested_x < this->secondFrame->width()-this->blockSize
                                            && blockTested_y>0 && blockTested_y < this->secondFrame->height()-this->blockSize){
                                        //if block is valid calculate the ssd
                                        valuesInTestedBlock = currentBlockPreparedVals[block_i][block_j] -qGray(this->secondFrame->pixel(block_i+blockTested_x,block_j+blockTested_y));
                                        sum += valuesInTestedBlock * valuesInTestedBlock;
                                    }
                                }
                            }
                            //update the sum value for the current key
                            hexagon.points.insert(i.key(),sum);
                        }
                        ++i;
                    }

                    //check the closest value - the smallest stored sum
                    //from the QHash values
                    i = hexagon.points.begin();
                    while (i != hexagon.points.end()) {
                        if(prevSSD > i.value()){
                            prevSSD = i.value();
                            drawTo = i.key();
                        }
                        ++i;
                    }

                    //set so it prefers staying at center
                    //small rhombus
                    if(hexagon.centerSSD <= prevSSD){
                        drawTo = hexagon.center;
                        //if the one at center, build small hexagon
                        QList<QPoint> newSmallRhombus = this->getRhombusPoints(hexagon.center,SmallRhombus);
                        //compare and give result the best fit of the small hexagon points
                        for(int j=0;j<newSmallRhombus.size();j++){
                            //again set the coordinates for the block
                            blockTested_x = newSmallRhombus.at(j).x()- this->blockSize/2;
                            blockTested_y = newSmallRhombus.at(j).y() - this->blockSize/2;
                            sum = 0;
                            for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                                for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                                    if(blockTested_x > 0 && blockTested_x < this->secondFrame->width()-this->blockSize
                                            && blockTested_y>0 && blockTested_y < this->secondFrame->height()-this->blockSize){
                                        valuesInTestedBlock = currentBlockPreparedVals[block_i][block_j] -qGray(this->secondFrame->pixel(block_i+blockTested_x,block_j+blockTested_y));
                                        sum += valuesInTestedBlock * valuesInTestedBlock;
                                    }
                                }
                            }
                            if(hexagon.centerSSD > sum){
                                drawTo.setX(blockTested_x);
                                drawTo.setY(blockTested_y);
                                hexagon.centerSSD = sum;
                            }
                        }
                        resultFound = true;
                    }
                    else{ //center has bigger SSD than a side => rebuild the hexagon
                        //add the old center in the QHash
                        hexagon.points.insert(hexagon.center,hexagon.centerSSD);

                        //new center - gets the ssd value and removed from the hexagon
                        hexagon.center = drawTo;
                        hexagon.centerSSD = hexagon.points.value(drawTo);
                        prevSSD = hexagon.centerSSD;
                        hexagon.points.remove(drawTo);
                    }
                }

                if(drawFrom.x() != drawTo.x() && drawFrom.y() != drawTo.y()){
                    QPair<QPoint,QPoint> drawPoints;
                    drawPoints.first = drawFrom;
                    drawPoints.second = drawTo;
                    this->toDraw.append(drawPoints);
                }
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

QList<QPoint> HexagonalSearch::getSmallHexagonPoints(QPoint center){
    QList<QPoint> toReturn;
    toReturn << QPoint(center.x()-(this->bigHexagonSpread*this->blockSize)/2,center.y());
    toReturn << QPoint(center.x()+(this->bigHexagonSpread*this->blockSize)/2,center.y());
    toReturn << QPoint(center.x(),center.y()-(this->bigHexagonSpread*this->blockSize)/2);
    toReturn << QPoint(center.x(),center.y()-(this->bigHexagonSpread*this->blockSize)/2);
    return toReturn;
}

void HexagonalSearch::start(){
    this->stopped = false;
    run();
}

void HexagonalSearch::stop(){
    this->stopped = true;
}

QList<QPoint> HexagonalSearch::getRhombusPoints(QPoint center,ObjectType type){
    QList<QPoint> toReturn;
    switch(type){
        case BigRhombus:
            toReturn << QPoint(center.x()-this->bigHexagonSpread,center.y());
            toReturn << QPoint(center.x()+this->bigHexagonSpread,center.y());

            toReturn << QPoint(center.x()+this->bigHexagonSpread/2,center.y()+this->bigHexagonSpread/2);
            toReturn << QPoint(center.x()-this->bigHexagonSpread/2,center.y()-this->bigHexagonSpread/2);
            toReturn << QPoint(center.x()+this->bigHexagonSpread/2,center.y()-this->bigHexagonSpread/2);
            toReturn << QPoint(center.x()-this->bigHexagonSpread/2,center.y()+this->bigHexagonSpread/2);
            break;
        case SmallRhombus:
            toReturn << QPoint(center.x()-this->smallHexagonSpread,center.y());
            toReturn << QPoint(center.x()+this->smallHexagonSpread,center.y());
            toReturn << QPoint(center.x(),center.y()-this->smallHexagonSpread);
            toReturn << QPoint(center.x(),center.y()+this->smallHexagonSpread);
            break;
        default:
            break;

    }
    return toReturn;
}
void HexagonalSearch::setSize(int newSize, ObjectType type){
    switch(type){
    case BigRhombus:
        this->bigHexagonSpread = newSize;
        break;
    case SmallRhombus:
        this->smallHexagonSpread = newSize;
        break;
    default:
        break;
    }
}
