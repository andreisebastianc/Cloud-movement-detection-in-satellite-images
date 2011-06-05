#include "movementscene.h"

MovementScene::MovementScene(QWidget *parent) :
    QGraphicsView(parent)
{
    this->gridInterval = 20;
    this->gridIsVisible = true;
    setDragMode(RubberBandDrag);
    setRenderHints(QPainter::Antialiasing);
}

void MovementScene::setGridBlockSize(int size){
    this->gridInterval = size;
    this->scene()->update(this->sceneRect());
}

/**
  *
  */
void MovementScene::drawForeground(QPainter *painter, const QRectF &rect){
    if(gridIsVisible){
        painter->setWorldMatrixEnabled(true);
        QVarLengthArray<QLineF, 100> linesX;
        QVarLengthArray<QLineF, 100> linesY;

        qreal left = int(rect.left()) - (int(rect.left()) % gridInterval );
        qreal top = int(rect.top()) - (int(rect.top()) % gridInterval );

        for (qreal x = left; x < rect.right(); x += gridInterval )
            linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

        for (qreal y = top; y < rect.bottom(); y += gridInterval )
            linesY.append(QLineF(rect.left(), y, rect.right(), y));

        painter->setPen(QColor(196,220,255));
        painter->drawLines(linesX.data(), linesX.size());
        painter->drawLines(linesY.data(), linesY.size());
    }
}

void MovementScene::setBlockGridVisible(bool visible){
    this->gridIsVisible = visible;
    this->scene()->invalidate();
}
