#include "fullsearch.h"

FullSearch::FullSearch(QList<QPair<QPoint, QVector<QPoint> > > *CBFPoints,OperationType type)
{
    this->stopped = false;
    this->blockSize = 7;
    this->searchWindowSize = 35;
    this->firstFrame = new QImage();
    this->secondFrame = new QImage();
    this->duplicatesCounter = 1;
    this->operationType = type;
    this->localCandidateBestFits = new QVector<QPair<QPoint,int> >(this->cbfSize-1);
    this->candidatePoints = CBFPoints;
}

FullSearch::~FullSearch(){
}


void FullSearch::run(){
    if(this->firstFrameIsSet && this->secondFrameIsSet){
        qDebug() << "thread" <<this->toDraw.size();
        this->toDraw = QList<QPair<QPoint,QPoint> > ();
        qDebug() << "thread-empty" <<this->toDraw.size();

        int blocksInWindow = this->searchWindowSize - this->blockSize;
        int numberOfBlocks_X = this->firstFrame->width() / this->blockSize;
        int numberOfBlocks_Y = this->firstFrame->height() / this->blockSize;

        long sum;
        int currentBlockPreparedVals[this->blockSize][this->blockSize], valuesInTestedBlock;
        int currentBlock_x, currentBlock_y,blockTested_x, blockTested_y;
        int searchWindow_x, searchWindow_y;
        long prevSSD;
        bool mantainsPosition;
        int conflictCounter;
        QPoint drawFrom, drawTo;

        // goes through each block of the first image
        for(int firstFrame_i=0;firstFrame_i<numberOfBlocks_X;firstFrame_i++){
            for(int firstFrame_j=0;firstFrame_j<numberOfBlocks_Y;firstFrame_j++){

                this->localCandidateBestFits->clear();

                prevSSD = 65000;
                conflictCounter = 0;
                mantainsPosition = true;
                sum = 0;

                currentBlock_x = firstFrame_i * this->blockSize;
                currentBlock_y = firstFrame_j * this->blockSize;

                // sets the real coordinates of the top left point of the block in the first image
                drawFrom.setX(currentBlock_x);
                drawFrom.setY(currentBlock_y);

                // fills an array of int with the values of each pixel in block,
                // basically calculates the gray values of the current block
                // into an array so it's not calculated every time
                for(int block_i = 0 ; block_i < this->blockSize ; block_i++){
                    for(int block_j = 0 ; block_j < this->blockSize ; block_j++){
                        currentBlockPreparedVals[block_i][block_j] = qGray(this->firstFrame->pixel(block_i+currentBlock_x,block_j+currentBlock_y));
                    }
                }

                //position the search window with the block inside, at center
                searchWindow_x = currentBlock_x - (this->searchWindowSize-this->blockSize)/2;
                searchWindow_y = currentBlock_y - (this->searchWindowSize-this->blockSize)/2;


                for(int i=0;i<blocksInWindow;i++){
                    for(int j=0;j<blocksInWindow;j++){
                        blockTested_x = i  + searchWindow_x;
                        blockTested_y = j  + searchWindow_y;
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

                        // if two or more blocks are possible results
                        // it might be that there are false positive, so a counter is used to flag the
                        // possible errors so behavior can be altered via a parameter set in the ui
                        if(prevSSD == sum){
                            conflictCounter++;
                        }

                        //checks if a block mantains position
                        if(blockTested_x==currentBlock_x && blockTested_y==currentBlock_y){
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
                                drawTo.setX(blockTested_x);
                                drawTo.setY(blockTested_y);
                            }
                            //get the candidate best fits
                            else{
                                if(this->operationType == ImprovedOperation){
                                    if(!this->localCandidateBestFits->isEmpty()){
                                        if(this->localCandidateBestFits->at(0).second > sum){
                                            this->localCandidateBestFits->remove(0);
                                            this->localCandidateBestFits->insert(0,QPair<QPoint,int>(QPoint(blockTested_x,blockTested_y),sum));
                                        }
                                        else{
                                            if(this->localCandidateBestFits->at(1).second > sum){
                                                this->localCandidateBestFits->remove(1);
                                                this->localCandidateBestFits->insert(1,QPair<QPoint,int>(QPoint(blockTested_x,blockTested_y),sum));
                                            }
                                        }
                                    }
                                    else{
                                        QPair<QPoint,int> tempPoint = QPair<QPoint,int>(QPoint(blockTested_x,blockTested_y),sum);
                                        for(i=0;i<this->cbfSize-1;i++){
                                            this->localCandidateBestFits->insert(0,tempPoint);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                //depending on the allowed duplicates and if a block doesn't mantain position in
                //the second image, the points are returned as a result of the operation
                if(!mantainsPosition && conflictCounter<=this->duplicatesCounter){
                    QPair<QPoint,QPoint> drawPoints;
                    drawPoints.first = drawFrom;
                    drawPoints.second = drawTo;
                    this->toDraw.append(drawPoints);
                    if(this->operationType == ImprovedOperation){
                        QVector<QPoint> cBF(3,QPoint());
                        cBF[0] = drawTo;
                        for(int i=1;i<this->cbfSize;i++){
                            cBF[i]= this->localCandidateBestFits->at(i).first;
                        }
                        this->candidatePoints->append(QPair<QPoint,QVector<QPoint> >(drawPoints.first,cBF));
                    }
                }
            }
        }
        emit operationsComplete();
    }
}


void FullSearch::start(){
    this->stopped = false;
    run();
}

void FullSearch::stop(){
    this->stopped = true;
}
