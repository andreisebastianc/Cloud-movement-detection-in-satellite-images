#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "imageshandler.h"
#include <QFileDialog>
#include <QDebug>

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

private:
    Ui::MainWindow *ui;
    ImagesHandler* imageHandler;
    void loadImages();
};

#endif // MAINWINDOW_H
