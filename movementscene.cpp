#include "movementscene.h"

MovementScene::MovementScene(QWidget *parent) :
    QGraphicsView(parent)
{
    this->gridInterval = 20;
}

void MovementScene::setGridBlockSize(int size){
    this->gridInterval = size;
    this->scene()->update(this->sceneRect());
}

/**
  *
  */
void MovementScene::drawForeground(QPainter *painter, const QRectF &rect){
    painter->setWorldMatrixEnabled(true);

    qreal left = int(rect.left()) - (int(rect.left()) % gridInterval );
    qreal top = int(rect.top()) - (int(rect.top()) % gridInterval );

    QVarLengthArray<QLineF, 100> linesX;
    for (qreal x = left; x < rect.right(); x += gridInterval )
        linesX.append(QLineF(x, rect.top(), x, rect.bottom()));

    QVarLengthArray<QLineF, 100> linesY;
    for (qreal y = top; y < rect.bottom(); y += gridInterval )
        linesY.append(QLineF(rect.left(), y, rect.right(), y));

    painter->setPen(QColor(196,220,255));
    painter->drawLines(linesX.data(), linesX.size());
    painter->drawLines(linesY.data(), linesY.size());
}
