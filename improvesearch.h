#ifndef IMPROVESEARCH_H
#define IMPROVESEARCH_H

#include <QThread>
#include <QPoint>

class ImproveSearch : public QThread
{
public:
    ImproveSearch();
    void run();
private:
    void getCandidatePoints(QPoint pointToMatch);
    void setData();
};

#endif // IMPROVESEARCH_H
