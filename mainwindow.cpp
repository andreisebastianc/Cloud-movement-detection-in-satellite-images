#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->displayScene.setBackgroundBrush(QBrush(QColor(230,230,230)));
    ui->imageDisplayer->setScene(&this->displayScene);
    this->blockSize = ui->blockSizeSlider->value();
    this->windowSize = ui->searchWindowSlider->value();
    this->ui->filesListWidget->setSortingEnabled(true);
    this->ui->coeficientInput->setText(QString("1"));
    this->drawSearchWindow(0,0);

    this->fullFinder = new FullSearch();
    this->hexFinder = new HexagonalSearch();
    //set display flags
    this->setFlags(MovementLines|SearchWindow|Image);

    //external actions
    connect(ui->actionGrid,SIGNAL(toggled(bool)),ui->imageDisplayer,SLOT(setBlockGridVisible(bool)));
    connect(ui->imageDisplayer,SIGNAL(redrawBlockAfterPoint(int,int)),this,SLOT(drawSearchWindow(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete this->imageHandler;
    delete this->fullFinder;
    while(!this->images.empty()){
        delete this->images.takeFirst();
    }
}

/**set the global display flags
  *
  *@param flags the flags for display
  */
void MainWindow::setFlags(int flags){
    this->flags = flags;
}

/**redisplays the elements taking taking into consideration the flags
  *
  */
void MainWindow::updateDisplay(){
    if(this->flags&Image){
        this->redisplayImage();
    }
    if(this->flags&MovementLines){
        this->drawLines();
    }
    if(this->flags&MovementDots){
        this->drawDots();
    }
    if(this->flags&SearchWindow){
        this->drawSearchWindow(10,10);
    }
}

/**special redisplay of the scene taking into consideration local flags
  *doesn't set the view flags
  *
  */
void MainWindow::updateDisplay(int flags){
    if(flags&Image){
        this->redisplayImage();
    }
    if(flags&MovementLines){
        this->drawLines();
    }
    if(flags&MovementDots){
        this->drawDots();
    }
    if(flags&SearchWindow){
        this->drawSearchWindow(10,10);
    }
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
            this->images << new QImage(files.at(i));
        }
        // displays only the first image in a sequence
        this->pixmapObject = QPixmap::fromImage(*images.at(0));
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
    short pos = ui->filesListWidget->currentRow();
    this->pixmapObject = QPixmap::fromImage(*images.at(pos));
    this->ui->searchWindowSlider->setMaximum(this->displayScene.width());
    if(pos%2==0){
        this->updateDisplay(MovementDots|SearchWindow|Image);
    }
    else{
        this->updateDisplay();
    }
}

/** the default run application call
  *
  */
void MainWindow::on_actionRun_triggered(){
//    this->fullFinder->setFirstFrame(new QImage(this->imagesPath.at(0)));
//    this->fullFinder->setSecondFrame(this->imagesPath.at(1));
//    connect(this->fullFinder,SIGNAL(operationsComplete()),this,SLOT(getMovementLines()));
//    this->fullFinder->start();
    this->hexFinder->setFirstFrame(new QImage(this->imagesPath.at(0)));
    this->hexFinder->setSecondFrame(this->imagesPath.at(1));
    connect(this->hexFinder,SIGNAL(operationsComplete()),this,SLOT(getMovementLines()));
    this->hexFinder->start();
}

void MainWindow::on_actionClear_Vectors_triggered(){
    this->clearVectors();
}

/**
  *
  */
void MainWindow::getMovementLines(){
    this->draw = this->fullFinder->getWhatToDraw();
    this->updateDisplay(MovementDots|SearchWindow|Image);
}

/**
  *
  */
void MainWindow::drawDots(){
    this->drawSearchWindow(0,0);
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
    this->fullFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
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
    this->fullFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
    ui->searchWindowLabel->setText("Search window size: "+QString::number(value));
    this->drawSearchWindow(0,0);
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
    this->fullFinder->setCoeficient(ui->coeficientInput->text().toInt());
}


// redisplays the image
void MainWindow::redisplayImage(){
    this->displayScene.clear();
    this->displayScene.addPixmap(this->pixmapObject);
}

void MainWindow::clearVectors(){
    this->displayScene.clear();
}

/**displays the search window for a block
  *
  *@param x int coordinate
  *@param y int coordinate
  *
  *@todo fix the bug with the scene being bigger than the image
  */
void MainWindow::drawSearchWindow(int x, int y){
    //only if flag is set
    if(this->flags&SearchWindow){
        //calculate the coordinates of the search window
        int horSide = (ui->imageDisplayer->width() -ui->imageDisplayer->scene()->width()) / 2;
        int vertSide = (ui->imageDisplayer->height() -ui->imageDisplayer->scene()->height()) / 2;
        x = x- horSide;
        y = y- vertSide;
        if(x > 0 && x < ui->imageDisplayer->scene()->width() && y > 0 && y < ui->imageDisplayer->scene()->height()){
            x = ( x / this->blockSize ) * this->blockSize - (this->windowSize-this->blockSize)/2;
            y = ( y / this->blockSize ) * this->blockSize - (this->windowSize-this->blockSize)/2;
            //clear and redisplay image
            this->redisplayImage();
            //build and draw the search window
            QGraphicsRectItem* theSearchwindow = new QGraphicsRectItem(
                        x,
                        y,
                        this->windowSize,
                        this->windowSize
                        );
            theSearchwindow->setPen(QPen(Qt::green));
            theSearchwindow->setBrush(QBrush(Qt::black,Qt::Dense7Pattern));
            this->displayScene.addItem(theSearchwindow);
            theSearchwindow->setZValue(100);
        }
    }
}
