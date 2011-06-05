#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->displayScene.setBackgroundBrush(QBrush(QColor(230,230,230)));
    ui->imageDisplayer->setScene(&this->displayScene);
    this->finder = new MovementFinder();
    this->blockSize = ui->blockSizeSlider->value();
    this->windowSize = ui->searchWindowSlider->value();
    this->ui->filesListWidget->setSortingEnabled(true);
    this->ui->coeficientInput->setText(QString("1"));
    this->drawSearchWindow();
    this->whomToAddWindowDisplay = QPoint(0,0);

    connect(ui->actionGrid,SIGNAL(toggled(bool)),ui->imageDisplayer,SLOT(setBlockGridVisible(bool)));
    connect(ui->imageDisplayer,SIGNAL(redrawBlockAfterPoint(int,int)),this,SLOT(getDisplayPointForWindow(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete this->imageHandler;
    delete this->finder;
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
            QListWidgetItem* item = new QListWidgetItem(theFileInfo.fileName(),ui->filesListWidget);
            item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            item->setCheckState(Qt::Unchecked);
            ui->filesListWidget->addItem(item);
            this->imagesPath << files.at(i);
        }
        // displays only the first image in a sequence
        this->pixmapObject = QPixmap(files.at(0));
        //qDebug() << this->ui->imageDisplayer->size() <<' '<< pixmapObject.size();
        //pixmapObject = pixmapObject.scaled(ui->imageDisplayer->size(),Qt::KeepAspectRatioByExpanding, Qt::FastTransformation );
        // set image in graphics view
        this->displayScene.addPixmap(pixmapObject);
        // qDebug() << this->imagesPath;
    }
}

/** updates the image view based on the selected item from the images view
  *
  */
void MainWindow::on_filesListWidget_itemSelectionChanged()
{
    this->pixmapObject = QPixmap(this->imagesPath.at(ui->filesListWidget->currentRow()));
    this->ui->searchWindowSlider->setMaximum(this->displayScene.width());
    //qDebug() << this->ui->imageDisplayer->size() <<' '<< pixmapObject.size();
    //pixmapObject = pixmapObject.scaled(ui->imageDisplayer->size(),Qt::KeepAspectRatioByExpanding, Qt::FastTransformation );
    //this->drawLines();
    //set image in graphics view
    this->drawDots();
}

/** the default run application call
  *
  */
void MainWindow::on_actionRun_triggered(){
    this->finder->setFirstFrame(this->imagesPath.at(0));
    this->finder->setSecondFrame(this->imagesPath.at(1));
    connect(this->finder,SIGNAL(operationsComplete()),this,SLOT(getMovementLines()));
    this->finder->start();
}

void MainWindow::on_actionClear_Vectors_triggered(){
    this->clearVectors();
}

void MainWindow::getMovementLines(){
    this->draw = this->finder->getWhatToDraw();
    //drawLines();
    this->drawDots();
}

/**
  *
  */
void MainWindow::drawDots(){
    this->displayScene.clear();
    this->redisplayImage();
    this->drawSearchWindow();
    for(int i=0;i<this->draw.size();i++){
        QGraphicsRectItem *dot = new QGraphicsRectItem(
                    this->draw.at(i).first.x()+this->blockSize/2,
                    this->draw.at(i).first.y()+this->blockSize/2,
                    1,
                    1
                    );
        dot->setPen(QPen(Qt::red));
        this->displayScene.addItem(dot);
        dot->setZValue(100);
    }
}

/**
  *
  */
void MainWindow::drawLines(){
    this->displayScene.clear();
    this->redisplayImage();
    this->drawSearchWindow();

    for(int i=0;i<this->draw.size();i++){
        QGraphicsLineItem *line = new QGraphicsLineItem(
                    this->draw.at(i).first.x()+this->blockSize/2,
                    this->draw.at(i).first.y()+this->blockSize/2,
                    this->draw.at(i).second.x()+this->blockSize/2,
                    this->draw.at(i).second.y()+this->blockSize/2
                    );
        line->setPen(QPen(Qt::red));
        this->displayScene.addItem(line);
        line->setZValue(100);
    }
}

/** updates the size of the block
  * sets the value in the worker thread
  *
  */
void MainWindow::on_blockSizeSlider_valueChanged(int value)
{
    this->blockSize = value;
    this->finder->setConstraitmentSizes(this->blockSize,this->windowSize);
    this->ui->imageDisplayer->setGridBlockSize(value);
    ui->blockSizeLabel->setText("Block size: "+QString::number(value));
}

/** updates the size of the window
  * sets the value in the worker thread
  *
  */
void MainWindow::on_searchWindowSlider_valueChanged(int value)
{
    this->windowSize = value;
    this->finder->setConstraitmentSizes(this->blockSize,this->windowSize);
    ui->searchWindowLabel->setText("Search window size: "+QString::number(value));
    this->drawSearchWindow();
}

void MainWindow::on_coeficientCheck_toggled(bool checked)
{
    if(checked == true){
        this->ui->coeficientInput->setEnabled(true);
    }
    else{
        this->ui->coeficientInput->setEnabled(false);
    }
}

void MainWindow::on_coeficientInput_returnPressed()
{
    this->finder->setCoeficient(ui->coeficientInput->text().toInt());
}

/** @todo for point get block and display scene with block as center
  * @todo see if possible to remove an item without redrawing them all
  * @todo maybe just moving the search window would be better than redrawing it
  *
  */
void MainWindow::drawSearchWindow(){
    qDebug() << this->whomToAddWindowDisplay;
    this->displayScene.clear();
    this->redisplayImage();
    QGraphicsRectItem* theSearchwindow = new QGraphicsRectItem(
                this->whomToAddWindowDisplay.x(),
                this->whomToAddWindowDisplay.y(),
                this->windowSize,
                this->windowSize
                );
    theSearchwindow->setPen(QPen(Qt::green));
    theSearchwindow->setBrush(QBrush(Qt::black,Qt::Dense7Pattern));
    this->displayScene.addItem(theSearchwindow);
    theSearchwindow->setZValue(100);
}

void MainWindow::redisplayImage(){
    this->displayScene.addPixmap(this->pixmapObject);
}

void MainWindow::clearVectors(){
    this->displayScene.clear();
}

void MainWindow::getDisplayPointForWindow(int x, int y){
    this->whomToAddWindowDisplay = QPoint(x,y);
    int horSide = (ui->imageDisplayer->width() -ui->imageDisplayer->scene()->width()) / 2;
    int vertSide = (ui->imageDisplayer->height() -ui->imageDisplayer->scene()->height()) / 2;
    x = x- horSide;
    y = y- vertSide;
    if(x > 0 && x < ui->imageDisplayer->scene()->width() && y > 0 && y < ui->imageDisplayer->scene()->height()){
        this->whomToAddWindowDisplay.setX(x);
        this->whomToAddWindowDisplay.setY(y);
        this->drawSearchWindow();
    }
}
