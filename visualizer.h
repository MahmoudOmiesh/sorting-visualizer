#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGraphicsRectItem>

class VisualizerWorker : public QObject
{
    Q_OBJECT

public slots:
    void visualize(int sortingAlgorithmIdx, int delay, std::vector<int> arr) ;

signals:
    void swapColumns(int i, int j);
    void slideColumn(int i, int j);
    void highlight(int i, Qt::GlobalColor clr);
    void unhighlight(int i);
    void setUI(bool v);

private:
    int delay;
    std::vector<int> arr;

    void bubbleSort();
    void selectionSort();
    void insertionSort();
    void merge(int s, int m, int e);
    void mergeSort(int s, int e);
    void quickSort(int s, int e);
    void heapify(int parent, int n);
    void heapSort();

    void compare(int i, int j);
    void swap(int i, int j);
    void slide(int i, int j);
    void sortDone();
};

#endif // VISUALIZER_H
