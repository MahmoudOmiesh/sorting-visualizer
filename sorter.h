#ifndef SORTER_H
#define SORTER_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QThread>

QT_BEGIN_NAMESPACE

namespace Ui {
    class Sorter;
}

QT_END_NAMESPACE

class Sorter : public QMainWindow
{
    Q_OBJECT

public:
    Sorter(QWidget *parent = nullptr);
    ~Sorter();


private slots:
    void on_sortBtn_clicked();
    void on_columnCount_valueChanged(int count);
    void on_shuffleBtn_clicked();
    void on_delay_valueChanged(int d);
    void on_chosenAlgorithm_currentIndexChanged(int index);

    void swapColumns(int i, int j);
    void slideColumn(int i, int j);
    void highlight(int i, Qt::GlobalColor clr);
    void unhighlight(int i);
    void setUI(bool v);

    void on_stopBtn_clicked();

signals:
    void startVisualize(int sortingAlgorithmIdx, int delay, std::vector<int> columnHeights);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::Sorter *ui;

    QGraphicsScene* scene;
    QThread visualizerThread;

    bool isGraphicsViewShown;
    int scenePadding;
    int columnCount;
    int delay;
    int sortingAlgorithmIdx;
    std::vector<QGraphicsRectItem*> columns;

    void prepareScene(int padding, int sceneWidth, int sceneHeight);
    void drawColumns(double columnWidth, int columnHeightMax);
    void generateColumns();
    std::vector<int> getColumnHeights();
    bool inRange(int i);
};

#endif // SORTER_H

