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
  * @todo add all the files selected to the images handler class
  *
  */
void MainWindow::on_addImagesButton_released()
{
    QStringList files = QFileDialog::getOpenFileNames(
                                    this, tr("QFileDialog::getOpenFileNames()"),
                                    tr("Image Files (*.png *.jpg *.bmp)"));
    // if a user selected valid images, go through all and add the details of the selected images
    // to the logic structures
    if (files.count()) {
        QFileInfo theFileInfo;
        for(int i=0;i<files.count();i++){
            theFileInfo.setFile(files[i]);
            ui->filesListWidget->addItem(theFileInfo.fileName());
        }
        // display only the first image in a sequence
        QPixmap pixmapObject(files[0]);
        pixmapObject = pixmapObject.scaled(ui->imageDisplayer->size(),Qt::KeepAspectRatio, Qt::FastTransformation );
        ui->imageDisplayer->setPixmap(pixmapObject);
    }
}
