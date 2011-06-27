#ifndef IMPROVESEARCH_H
#define IMPROVESEARCH_H

#include <QThread>
#include <QPoint>
#include <QVector>
#include <QPair>
#include <QDebug>

class ImproveSearch : public QThread
{
public:
    ImproveSearch(QVector<QList<QPair<QPoint,QPoint> > >* draw,QList<QPair<QPoint,QVector<QPoint> > >* cbfPoints)
        :m_draw(draw),m_candidatePoints(cbfPoints) {}
    void run();
private:
    void getCandidatePoints(QPoint pointToMatch);
    void setData();
    QVector<QList<QPair<QPoint,QPoint> > >* m_draw;
    QList<QPair<QPoint,QVector<QPoint> > >* m_candidatePoints;
};

#endif // IMPROVESEARCH_H
