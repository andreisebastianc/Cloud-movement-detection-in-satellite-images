#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->imageHandler;
}

/** opens a select dialog that allows multiple files to be selected
  *
  * @todo add all the files selected to the list view
  * @todo add all the files selected to the images handler class
  *
  */
void MainWindow::on_addImagesButton_released()
{
    QStringList files = QFileDialog::getOpenFileNames(
                                    this, tr("QFileDialog::getOpenFileNames()"),
                                    tr("All Files (*);;Text Files (*.txt)"));
    if (files.count()) {
        qDebug() << files[0];
    }
}
