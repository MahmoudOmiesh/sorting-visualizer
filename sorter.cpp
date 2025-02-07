#include "sorter.h"
#include "ui_sorter.h"
#include "visualizer.h"

Sorter::Sorter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Sorter)
{
    ui->setupUi(this);

    //initialize scene
    scenePadding = 10;
    isGraphicsViewShown = false;
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    //initialize column count
    columnCount = 100;
    ui->columnCount->setValue(columnCount);

    //initialize delay count
    delay = 200;
    ui->delay->setValue(delay);

    //initialize sorting algorithm
    sortingAlgorithmIdx = 0;
    ui->chosenAlgorithm->setCurrentIndex(sortingAlgorithmIdx);

    //initialize buttons
    ui->stopBtn->setEnabled(false);

    //initialize visualizer thread
    VisualizerWorker* visualizerWorker = new VisualizerWorker();
    visualizerWorker->moveToThread(&visualizerThread);

    connect(this, &Sorter::startVisualize, visualizerWorker, &VisualizerWorker::visualize);
    connect(visualizerWorker, &VisualizerWorker::swapColumns, this, &Sorter::swapColumns);
    connect(visualizerWorker, &VisualizerWorker::slideColumn, this, &Sorter::slideColumn);
    connect(visualizerWorker, &VisualizerWorker::highlight, this, &Sorter::highlight);
    connect(visualizerWorker, &VisualizerWorker::unhighlight, this, &Sorter::unhighlight);
    connect(visualizerWorker, &VisualizerWorker::setUI, this, &Sorter::setUI);

}

Sorter::~Sorter()
{
    visualizerThread.quit();
    visualizerThread.wait();
    delete ui;
}


void Sorter::prepareScene(int scenePadding, int sceneWidth, int sceneHeight) {
    columns.clear();
    scene->clear();

    ui->graphicsView->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    scene->setSceneRect(-scenePadding , 0, sceneWidth, sceneHeight);
}

void Sorter::drawColumns(double columnWidth, int columnHeightMax) {
    QPen pen(Qt::black);
    QBrush brush(Qt::cyan);

    for(int i = 0; i < columnCount; ++i) {
        int columnHeight = std::rand() % columnHeightMax;
        QGraphicsRectItem* column = new QGraphicsRectItem(i*columnWidth, 0, columnWidth, columnHeight);

        column->setPen(pen);
        column->setBrush(brush);

        columns.push_back(column);
        scene->addItem(column);
    }
}

void Sorter::generateColumns() {
    if(!isGraphicsViewShown) return;

    int graphicsViewWidth = ui->graphicsView->width();
    int graphicsViewHeight = ui->graphicsView->height();
    int sceneWidth = graphicsViewWidth - 2*scenePadding;

    double columnWidth = (sceneWidth) / (double) columnCount;
    int columnHeightMax = graphicsViewHeight - 5;

    prepareScene(scenePadding, sceneWidth, graphicsViewHeight);
    drawColumns(columnWidth, columnHeightMax);
}

void Sorter::swapColumns(int i, int j) {
    if(!inRange(i) || !inRange(j)) return;

    auto col1 = columns[i]->rect();
    auto col2 = columns[j]->rect();
    auto col1Left = col1.left();

    col1.moveLeft(col2.left());
    col2.moveLeft(col1Left);

    columns[i]->setRect(col1);
    columns[j]->setRect(col2);

    std::swap(columns[i], columns[j]);
}

void Sorter::slideColumn(int columnIdx, int slideToIdx) {
    if(!inRange(columnIdx) || !inRange(slideToIdx)) return;

    int step = slideToIdx > columnIdx  ? 1 : -1;
    int len = std::abs(slideToIdx - columnIdx);

    for(int i = 0; i < len; ++i) {
        swapColumns(columnIdx, columnIdx + step);
        columnIdx += step;
    }
}

void Sorter::highlight(int i, Qt::GlobalColor clr) {
    if(!inRange(i)) return;

    QGraphicsRectItem* column = columns[i];
    QBrush brush(clr);
    column->setBrush(brush);
}

void Sorter::unhighlight(int i) {
    highlight(i, Qt::cyan);
}

void Sorter::setUI(bool v) {
    ui->sortBtn->setEnabled(v);
    ui->shuffleBtn->setEnabled(v);
    ui->chosenAlgorithm->setEnabled(v);
    ui->columnCount->setEnabled(v);
    ui->delay->setEnabled(v);
    ui->stopBtn->setEnabled(!v);
}

std::vector<int> Sorter::getColumnHeights() {
    std::vector<int> columnHeights(columns.size());
    for(int i = 0; i < (int) columns.size(); ++i) {
        columnHeights[i] = columns[i]->rect().height();
    }
    return columnHeights;
}

bool Sorter::inRange(int i) {
    return i >= 0 && i < (int) columns.size();
}

void Sorter::showEvent(QShowEvent *event) {
    QMainWindow::showEvent(event);
    isGraphicsViewShown = true;
    generateColumns();
}

void Sorter::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    if(visualizerThread.isRunning()) {
        ui->graphicsView->setAlignment(Qt::AlignTop);
    } else {
        generateColumns();
    }
}

void Sorter::on_sortBtn_clicked()
{
    setUI(false);

    if(visualizerThread.isFinished() || !visualizerThread.isRunning()) {
        visualizerThread.start();
    }

    emit startVisualize(sortingAlgorithmIdx, delay, getColumnHeights());
}


void Sorter::on_columnCount_valueChanged(int count)
{
    columnCount = count;
    generateColumns();
    ui->sortBtn->setEnabled(true);
}


void Sorter::on_shuffleBtn_clicked()
{
    generateColumns();
}

void Sorter::on_stopBtn_clicked()
{
    ui->stopBtn->setEnabled(false);

    visualizerThread.quit();
    if(visualizerThread.isRunning()) {
        visualizerThread.requestInterruption();
        visualizerThread.wait();
    }

    setUI(true);
}


void Sorter::on_delay_valueChanged(int d)
{
    delay = d;
}


void Sorter::on_chosenAlgorithm_currentIndexChanged(int index)
{
    sortingAlgorithmIdx = index;
}



