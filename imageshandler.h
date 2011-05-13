#ifndef IMAGESHANDLER_H
#define IMAGESHANDLER_H

#include <QObject>
#include <QList>
#include <QImage>
#include <QRgb>
#include <QDebug>
#include <QList>

class ImagesHandler : public QObject
{
    Q_OBJECT
public:
    explicit ImagesHandler(QObject *parent = 0,int blockSize = 0);
    void setImages(QImage startImage, QImage compareImage);
    void setBlockSize(int newBlockSize);
    void calculateHashForBlocks();

private:
    QImage startImage;
    QImage compareImage;
    int blockSize;

signals:

public slots:

};

#endif // IMAGESHANDLER_H
