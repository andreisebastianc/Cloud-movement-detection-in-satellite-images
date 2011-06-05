#ifndef MOVEMENTSCENE_H
#define MOVEMENTSCENE_H

#include <QGraphicsView>
#include <QVarLengthArray>
#include <QWheelEvent>
#include <math.h>

class MovementScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MovementScene(QWidget *parent = 0);
    void setGridBlockSize(int size);
protected:
    void drawForeground(QPainter *painter, const QRectF &rect);
    void wheelEvent(QWheelEvent *event) { scaleBy(pow(4.0/3.0,(-event->delta()/240.0)));}

private:
    int gridInterval;
    bool gridIsVisible;
    void scaleBy(double factor) { scale(factor,factor);}

signals:

public slots:
    void setBlockGridVisible(bool visible=true);
    void zoomIn() { scaleBy(1.1); }
    void zoomOut() { scaleBy(1.0/1.1); }

};

#endif // MOVEMENTSCENE_H
