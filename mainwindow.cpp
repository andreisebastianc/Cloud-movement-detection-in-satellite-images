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
  * gets all the images from the dialog and adds them to a list widget and stores the addresses in a QList
  * so that they can be used later
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
            theFileInfo.setFile(files.at(i));
            ui->filesListWidget->addItem(theFileInfo.fileName());
            this->imagesPath << files.at(i);
        }
        // displays only the first image in a sequence
        this->pixmapObject = QPixmap(files.at(0));
        pixmapObject = pixmapObject.scaled(ui->imageDisplayer->size(),Qt::KeepAspectRatio, Qt::FastTransformation );
        ui->imageDisplayer->setPixmap(pixmapObject);

        // qDebug() << this->imagesPath;
    }
}

/** updates the image view based on the selected item from the images view
  *
  */
void MainWindow::on_filesListWidget_itemSelectionChanged()
{
    this->pixmapObject = QPixmap(this->imagesPath.at(ui->filesListWidget->currentRow()));
    pixmapObject = pixmapObject.scaled(ui->imageDisplayer->size(),Qt::KeepAspectRatio, Qt::FastTransformation );
    ui->imageDisplayer->setPixmap(pixmapObject);
}

/** the default run application call
  *
  */
void MainWindow::on_actionRun_triggered(){
    ImagesHandler ih(0,10);
    ih.setImages(QImage(this->imagesPath.at(0)),QImage(this->imagesPath.at(0)));
    ih.calculateHashForBlocks();
}

void MainWindow::on_blockSizeSlider_valueChanged(int value)
{
    ui->blockSizeLabel->setText("Block size: "+QString::number(value));
}

void MainWindow::on_searchWindowSlider_valueChanged(int value)
{
    ui->searchWindowLabel->setText("Search window size: "+QString::number(value));
}
