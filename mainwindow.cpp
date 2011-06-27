#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->displayScene.setBackgroundBrush(QBrush(QColor(230,230,230)));
    this->displayScene.setItemIndexMethod(QGraphicsScene::NoIndex);
    ui->imageDisplayer->setScene(&this->displayScene);
    this->blockSize = ui->blockSizeSlider->value();
    this->windowSize = ui->searchWindowSlider->value();
    this->ui->filesListWidget->setSortingEnabled(true);
    this->drawSearchWindow(0,0);

    this->draw = new QVector<QList<QPair<QPoint,QPoint> > >();
    this->fullFinder = new FullSearch(this->cbfPoints);
    this->hexFinder = new HexagonalSearch();
    this->rhombusFinder = new RhombusSearch();
    this->improvedFinder = new ImproveSearch(this->draw,this->cbfPoints);

    //set display flags
    this->setFlags(MovementLines|SearchWindow|Image);
    this->operationComplete = false;

    //external actions
    connect(ui->actionGrid,SIGNAL(toggled(bool)),ui->imageDisplayer,SLOT(setBlockGridVisible(bool)));
    connect(ui->imageDisplayer,SIGNAL(redrawBlockAfterPoint(int,int)),this,SLOT(drawSearchWindow(int,int)));
}

MainWindow::~MainWindow()
{
    delete ui;
    //delete this->imageHandler;
    delete this->fullFinder;
    delete this->rhombusFinder;
    delete this->hexFinder;
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
    this->displayScene.clear();
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
    this->displayScene.clear();
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
  * REMOVED WHEN UPDATED WITH currentRowChanged(int currentRow) slot
  */
//void MainWindow::on_filesListWidget_itemSelectionChanged()
//{
//    short pos = ui->filesListWidget->currentRow();
//    this->pixmapObject = QPixmap::fromImage(*images.at(pos));
//    this->ui->searchWindowSlider->setMaximum(this->displayScene.width());
//    if(pos%2==0){
//        this->updateDisplay(MovementDots|SearchWindow|Image);
//    }
//    else{
//        this->updateDisplay();
//    }
//}

/** the default run application call
  *
  */
void MainWindow::on_actionRun_triggered(){
    this->draw->clear();
    this->cbfPoints->clear();
    switch(this->ui->searchTypeSelect->currentIndex()){
    default:
        break;
    case 0://fullsearch
        qDebug() << "full search";
        for(int i = 0; i<this->imagesPath.count()-1;i++){
            this->fullFinder->setFirstFrame(new QImage(this->imagesPath.at(i)));
            this->fullFinder->setSecondFrame(new QImage(this->imagesPath.at(i+1)));
            connect(this->fullFinder,SIGNAL(operationsComplete()),this,SLOT(getMovementLines()));
            this->fullFinder->start();
        }
        break;
    case 1://diamond(rhombus) search
        qDebug() << "diamond search";
        for(int i = 0; i<this->imagesPath.count()-1;i++){
            this->rhombusFinder->setFirstFrame(new QImage(this->imagesPath.at(i)));
            this->rhombusFinder->setSecondFrame(new QImage(this->imagesPath.at(i+1)));
            connect(this->rhombusFinder,SIGNAL(operationsComplete()),this,SLOT(getMovementLines()));
            this->rhombusFinder->start();
        }
        break;
    case 2://hexagonal
        qDebug() << "hex search";
        for(int i = 0; i<this->imagesPath.count()-1;i++){
            this->hexFinder->setFirstFrame(new QImage(this->imagesPath.at(i)));
            this->hexFinder->setSecondFrame(new QImage(this->imagesPath.at(i+1)));
            connect(this->hexFinder,SIGNAL(operationsComplete()),this,SLOT(getMovementLines()));
            this->hexFinder->start();
        }
        break;
    }
}

void MainWindow::on_actionClear_Vectors_triggered(){
    this->clearVectors();
}

/**
  *
  */
void MainWindow::getMovementLines(){
    switch(this->ui->searchTypeSelect->currentIndex()){
    case 0:
        this->draw->append(this->fullFinder->getWhatToDraw());
        break;
    case 1:
        this->draw->append(this->rhombusFinder->getWhatToDraw());
        break;
    case 2:
        this->draw->append(this->hexFinder->getWhatToDraw());
        break;
    }

    this->updateDisplay(MovementDots|SearchWindow|Image);
    this->operationComplete = true;
    if(this->fullFinder->getOperationType() == ImprovedOperation){
        this->improvedFinder->run();
    }
}

/**
  *
  */
void MainWindow::drawDots(){
    this->drawSearchWindow(0,0);
    for(int i=0;i<this->draw->at(this->currentFrame).size();i++){
        QGraphicsRectItem *dot = new QGraphicsRectItem(
                    this->draw->at(this->currentFrame).at(i).first.x()+this->blockSize/2,
                    this->draw->at(this->currentFrame).at(i).first.y()+this->blockSize/2,
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
    for(int i=0;i<this->draw->at(this->currentFrame).size();i++){
        QGraphicsLineItem *line = new QGraphicsLineItem(
                    this->draw->at(this->currentFrame).at(i).first.x()+this->blockSize/2,
                    this->draw->at(this->currentFrame).at(i).first.y()+this->blockSize/2,
                    this->draw->at(this->currentFrame).at(i).second.x()+this->blockSize/2,
                    this->draw->at(this->currentFrame).at(i).second.y()+this->blockSize/2
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
    this->rhombusFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
    this->hexFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
    this->ui->imageDisplayer->setGridBlockSize(value);
    ui->blockSizeLabel->setText("Block size: "+QString::number(value));
}

/** updates the size of the window
  * sets the value in the worker thread
  *
  */
void MainWindow::on_searchWindowSlider_valueChanged(int value){
    this->windowSize = value;
    this->fullFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
    this->rhombusFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
    this->hexFinder->setConstraitmentSizes(this->blockSize,this->windowSize);
    ui->searchWindowLabel->setText("Search window size: "+QString::number(value));
    this->drawSearchWindow(0,0);
}

/** sets size
  *
  */
void MainWindow::on_diamondBigSize_valueChanged(int value){
    this->rhombusFinder->setSize(value,BigRhombus);
    ui->diamondBigSizeLabel->setText("Search window size: "+QString::number(value));
}

/** sets size
  *
  */
void MainWindow::on_diamondSmallSize_valueChanged(int value){
    this->rhombusFinder->setSize(value,BigRhombus);
    ui->diamondSmallSizeLabel->setText("Search window size: "+QString::number(value));
}


/** redisplays the image
  *
  */
void MainWindow::redisplayImage(){
    this->displayScene.addPixmap(this->pixmapObject);
}

/** clears the display
  *
  */
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
        QPolygonF scenePolygon = ui->imageDisplayer->mapToScene(ui->imageDisplayer->viewport()->rect());
        x = x + scenePolygon.at(0).x();
        y = y + scenePolygon.at(0).y();
        int point_x = ( x / this->blockSize ) * this->blockSize;
        int point_y = ( y / this->blockSize ) * this->blockSize;
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
            theSearchwindow->setPen(QPen(Qt::red));
            theSearchwindow->setBrush(QBrush(Qt::black,Qt::NoBrush));
            this->displayScene.addItem(theSearchwindow);
            theSearchwindow->setZValue(100);

            if(this->operationComplete){
                for(int i=0;i<this->draw[this->currentFrame].size();i++){
                    if(this->draw->at(this->currentFrame).at(i).first.x() == point_x &&
                            this->draw->at(this->currentFrame).at(i).first.y() == point_y){
                        QGraphicsLineItem *line = new QGraphicsLineItem(
                                    this->draw->at(this->currentFrame).at(i).first.x()+this->blockSize/2,
                                    this->draw->at(this->currentFrame).at(i).first.y()+this->blockSize/2,
                                    this->draw->at(this->currentFrame).at(i).second.x()+this->blockSize/2,
                                    this->draw->at(this->currentFrame).at(i).second.y()+this->blockSize/2
                                    );
                        line->setPen(QPen(Qt::red));
                        this->displayScene.addItem(line);
                        line->setZValue(100);
                    }
                }
            }
        }
    }
}

/**
  *
  */
void MainWindow::on_actionExport_view_as_image_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Save Current View"),
            QDir::currentPath(),
            tr("Images (*.png,*.jpg)") );
        if( !filename.isNull() )
        {
            QImage img(this->displayScene.width(),this->displayScene.height(),QImage::Format_ARGB32_Premultiplied);
            QPainter p(&img);
            this->displayScene.render(&p);
            p.end();
            img.save(filename);
        }
}

/**
  *
  */
void MainWindow::on_useImportantCheckBox_toggled(bool checked)
{
    if(checked){
        this->fullFinder->setOperationType(ImprovedOperation);
    }
}

void MainWindow::exportResults(){
}

void MainWindow::on_filesListWidget_currentRowChanged(int currentRow){
    this->pixmapObject = QPixmap::fromImage(*images.at(currentRow));
    this->ui->searchWindowSlider->setMaximum(this->displayScene.width());
    currentRow == 0 ? this->currentFrame = currentRow : this->currentFrame = currentRow -1 ;
    if(this->operationComplete){
        if(currentRow==0){
            this->updateDisplay(MovementDots|SearchWindow|Image);
        }
        else{
            this->updateDisplay(MovementLines|Image);
        }
    }
    else{
        this->updateDisplay(Image);
    }
}
