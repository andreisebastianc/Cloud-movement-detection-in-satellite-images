#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageshandler.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
#include <QList>
#include <QString>
#include <QListWidgetItem>

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

private:
    Ui::MainWindow *ui;
    ImagesHandler* imageHandler;
    void loadImages();
    QList<QString> imagesPath;
    QPixmap pixmapObject;
};

#endif // MAINWINDOW_H
