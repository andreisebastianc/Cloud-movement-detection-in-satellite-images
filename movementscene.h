#ifndef MOVEMENTSCENE_H
#define MOVEMENTSCENE_H

#include <QGraphicsView>
#include <QVarLengthArray>

class MovementScene : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MovementScene(QWidget *parent = 0);
    void setGridBlockSize(int size);

protected:
    void drawForeground(QPainter *painter, const QRectF &rect);

private:
    int gridInterval;

signals:

public slots:

};

#endif // MOVEMENTSCENE_H
