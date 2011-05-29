#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <QListWidgetItem>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "imageshandler.h"
#include "movementfinder.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addImagesButton_released();
    void on_filesListWidget_itemSelectionChanged();
    void on_blockSizeSlider_valueChanged(int value);
    void on_searchWindowSlider_valueChanged(int value);
    void on_actionRun_triggered();
    void drawMovementLines();

private:
    int blockSize;
    int windowSize;

    Ui::MainWindow *ui;
    //ImagesHandler* imageHandler;
    MovementFinder* finder;
    QList<QString> imagesPath;
    QPixmap pixmapObject;
    QGraphicsScene displayScene;
    QGraphicsItem *backgroundImage;

    void loadImages();
};

#endif // MAINWINDOW_H
