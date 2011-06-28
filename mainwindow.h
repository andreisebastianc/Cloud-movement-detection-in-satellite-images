#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QVector>
#include <QString>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QHash>
#include <QTime>
//#include "imageshandler.h"
#include "fullsearch.h"
#include "hexagonalsearch.h"
#include "rhombussearch.h"
#include "improvesearch.h"

namespace Ui {
    class MainWindow;
}

enum SetDisplay{
    NoItemFlags = 0,
    Image = 1,
    MovementDots = 2,
    MovementLines = 4,
    SearchWindow = 8
};

struct Movement{
    QPair<int,int> bestFit;
    QVector<QPair<int,int> > candidateFits;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setFlags(int flags);
    void updateDisplay();
    void updateDisplay(int flags);

private slots:
    void on_addImagesButton_released();
    //void on_filesListWidget_itemSelectionChanged();
    void on_blockSizeSlider_valueChanged(int value);
    void on_searchWindowSlider_valueChanged(int value);
    void on_diamondBigSize_valueChanged(int value);
    void on_diamondSmallSize_valueChanged(int value);
    void on_actionRun_triggered();
    void on_actionClear_Vectors_triggered();
    void getMovementLines();
    void drawSearchWindow(int x, int y);

    void on_actionExport_view_as_image_triggered();

    void on_useImportantCheckBox_toggled(bool checked);

    void on_filesListWidget_currentRowChanged(int currentRow);


private:
    short blockSize;
    short windowSize;
    short flags;
    short currentFrame;
    bool operationComplete;
    QVector<QList<QPair<QPoint,QPoint> > >* draw;
    QList<QPair<QPoint,QVector<QPoint> > >* cbfPoints;

    Ui::MainWindow *ui;
    QList<QString> imagesPath;
    QList<QImage*> images;
    QPixmap pixmapObject;
    QGraphicsScene displayScene;
    QGraphicsItem *backgroundImage;

    FullSearch* fullFinder;
    HexagonalSearch* hexFinder;
    RhombusSearch* rhombusFinder;
    ImproveSearch* improvedFinder;

    void clearVectors();
    void loadImages();
    void drawLines();
    void drawDots();
    void redisplayImage();
    void exportResults();
};

#endif // MAINWINDOW_H
