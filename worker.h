#ifndef WORKER_H
#define WORKER_H

#include <QRunnable>
#include <QList>
#include <QImage>
#include <QString>
#include <QStringList>
#include <QPair>

enum Search{
    Full,
    Diamond,
    Hexagonal
};

class Worker : public QRunnable
{
public:
    explicit Worker(Search searchType,volatile bool *stopped,const QStringList &images,QVector<QList<QPair<QPoint,QPoint> > > &draw)
        :m_search(searchType),m_stopped(stopped),m_images(images),m_draw(draw){}
    void run();
private:
    Search m_search;
    volatile bool *m_stopped;
    const QStringList &m_images;
    QVector<QList<QPair<QPoint,QPoint> > > &m_draw;
};

#endif // WORKER_H
