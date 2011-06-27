#include "improvesearch.h"


void ImproveSearch::run(){
    //go through all of the points from the full search
//    QVector<QList<QPair<QPoint,QPoint> > >::iterator i;
//    for (i = this->m_draw->begin(); i != this->m_draw->end(); ++i)
//        qDebug() << *i;
    QList<QPair<QPoint,QVector<QPoint> > >::iterator i;
    for (i = this->m_candidatePoints->begin(); i != this->m_candidatePoints->end(); ++i)
        qDebug() << *i;
    //build the line for the current bf with the starting point
    //for each cbf build a line with the starting point
    //for each point (bf+cbf) get the candidate best fits from the next frame
    //for each result (3 points) draw the line with the point that was matched
    //compare each line with the line of the starting point and tested point to get angle
    //lowest difference in angle is the result
}
