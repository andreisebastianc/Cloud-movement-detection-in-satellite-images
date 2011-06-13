#ifndef FULLSEARCH_H
#define FULLSEARCH_H

#include <QThread>
#include <QImage>
#include <QString>
#include <QPair>
#include <QDebug>
#include "math.h"
#include "blockmatching.h"

class FullSearch : public QThread, public BlockMatching
{
    Q_OBJECT
public:
    explicit FullSearch();
    ~FullSearch();
    void start();
    void stop();

private:
    volatile bool stopped;

protected:
    void run();

signals:
    void operationsComplete();

public slots:

};

#endif // FULLSEARCH_H
