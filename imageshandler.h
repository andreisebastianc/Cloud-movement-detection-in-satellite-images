#ifndef IMAGESHANDLER_H
#define IMAGESHANDLER_H

#include <QObject>
#include <QList>
#include <QImage>

class ImagesHandler : public QObject
{
    Q_OBJECT
public:
    explicit ImagesHandler(QObject *parent = 0);

private:
    QList<QImage*> allImages; //stores all the images selected in the visual cue

signals:

public slots:

};

#endif // IMAGESHANDLER_H
