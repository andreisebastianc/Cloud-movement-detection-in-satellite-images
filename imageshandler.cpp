#include "imageshandler.h"

ImagesHandler::ImagesHandler(QObject *parent,int blockSize) :
    QObject(parent)
{
    this->blockSize = blockSize;
}

void ImagesHandler::setImages(QImage startImage,QImage compareImage)
{
    this->startImage = startImage;
    this->compareImage = compareImage;
}

void ImagesHandler::setBlockSize(int newBlockSize)
{
    this->blockSize = newBlockSize;
}

void ImagesHandler::calculateHashForBlocks()
{
    int blocksX = this->startImage.width()/this->blockSize;
    int blocksY = this->startImage.height()/this->blockSize;
    int pixelsInBlock[this->blockSize][this->blockSize];
    int pixelOnI, pixelOnY;
    QRgb pixel;
    for(int i=0;i<blocksX;i++)
        for(int j=0;j<blocksY;j++){
            //builds a matrix of block size containing all the gray values
            for(pixelOnI = 0;pixelOnI<this->blockSize;pixelOnI++)
                for(pixelOnY = 0;pixelOnY<this->blockSize;pixelOnY++){
                    pixel = this->startImage.pixel(pixelOnI,pixelOnY);
                    pixelsInBlock[pixelOnI][pixelOnY] = qGray(pixel);
                }
        }
}
