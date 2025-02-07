#include "visualizer.h"


void VisualizerWorker::visualize(int sortingAlgorithmIdx, int d, std::vector<int> a) {
    delay = d;
    arr = a;

    switch(sortingAlgorithmIdx) {
    case 0:
        bubbleSort();
        break;
    case 1:
        selectionSort();
        break;
    case 2:
        insertionSort();
        break;
    case 3:
        mergeSort(0, arr.size() - 1);
        break;
    case 4:
        quickSort(0, arr.size() - 1);
        break;
    case 5:
        heapSort();
        break;
    }

    sortDone();
}

void VisualizerWorker::bubbleSort() {
    int n = arr.size();

    for(int i = 0; i < n-1; ++i) {
        for(int j = 0; j < n-i-1; ++j) {
            compare(j, j+1);
            if(arr[j] > arr[j+1]) {
                swap(j, j+1);
            }


            if(QThread::currentThread()->isInterruptionRequested()) {
                QThread::currentThread()->quit();
                return;
            }
        }

    }
}

void VisualizerWorker::selectionSort() {
    int n = arr.size();
    int min_idx = -1;

    for(int i = 0; i < n-1; ++i) {
        min_idx = i;
        emit highlight(i, Qt::darkMagenta);

        for(int j = i+1; j < n; ++j) {
            if(min_idx != i) compare(j, min_idx);

            if(arr[j] < arr[min_idx])  min_idx = j;

            if(QThread::currentThread()->isInterruptionRequested()) {
                QThread::currentThread()->quit();
                emit unhighlight(i);
                return;
            }
        }

        swap(i, min_idx);
        emit unhighlight(i);
    }
}

void VisualizerWorker::insertionSort() {
    int n = arr.size();

    for(int i = 1; i < n; ++i) {
        int j = i-1;

        while(j >= 0 && arr[j] > arr[j+1]) {
            compare(j, j+1);
            swap(j, j+1);
            --j;

            if(QThread::currentThread()->isInterruptionRequested()) {
                QThread::currentThread()->quit();
                return;
            }
        }
    }
}

void VisualizerWorker::merge(int start, int mid, int end) {
    //arr1 [l, m]
    //arr2 [m+1, e]

    int start1 = start;
    int start2 = mid+1;

    if(arr[mid] <= arr[start2]) return;

    while(start1 <= mid && start2 <= end) {
        compare(start1, start2);

        if(arr[start1] <= arr[start2]) {
            start1++;
        } else {
            slide(start2, start1);

            ++start1;
            ++mid;
            ++start2;
        }

        if(QThread::currentThread()->isInterruptionRequested()) {
            QThread::currentThread()->quit();
            return;
        }
    }
}

void VisualizerWorker::mergeSort(int s, int e) {
    if(QThread::currentThread()->isInterruptionRequested()) {
        QThread::currentThread()->quit();
        return;
    }

    if(e - s + 1 <= 1) return;

    int m = s + (e - s) / 2;
    mergeSort(s, m);
    mergeSort(m+1, e);
    merge(s, m, e);
}

void VisualizerWorker::quickSort(int s, int e) {
    if(e - s + 1 <= 1) return;

    int left = s;
    int pivot = arr[e];
    emit highlight(e, Qt::darkMagenta);

    for(int i = s; i < e; ++i) {
        compare(left, i);

        if(arr[i] < pivot) {
            swap(left, i);
            ++left;
        }

        if(QThread::currentThread()->isInterruptionRequested()) {
            QThread::currentThread()->quit();
            emit unhighlight(e);
            return;
        }
    }

    emit unhighlight(e);
    swap(left, e);

    quickSort(s, left-1);
    quickSort(left+1, e);
}

void VisualizerWorker::heapify(int parent, int n) {
    if(QThread::currentThread()->isInterruptionRequested()) {
        QThread::currentThread()->quit();
        return;
    }

    int largest = parent;
    int left = 2*parent + 1;
    int right = 2*parent + 2;

    emit highlight(parent, Qt::darkMagenta);
    emit highlight(left, Qt::darkMagenta);
    emit highlight(right, Qt::darkMagenta);

    QThread::msleep(delay);

    if(left < n && arr[left] > arr[largest])
        largest = left;

    if(right < n && arr[right] > arr[largest])
        largest = right;

    emit unhighlight(parent);
    emit unhighlight(left);
    emit unhighlight(right);

    if(largest != parent) {
        swap(parent, largest);
        heapify(largest, n);
    }
}

void VisualizerWorker::heapSort() {
    int n = arr.size();

    //build max heap;
    for(int i = n/2-1; i >= 0; --i)
        heapify(i, n);

    for(int i = n-1; i >= 0; --i) {
        swap(0, i);
        heapify(0, i);
    }
}

void VisualizerWorker::compare(int i, int j) {
    if(i == j) return;

    emit highlight(i, Qt::green);
    emit highlight(j, Qt::green);

    QThread::msleep(delay);

    emit unhighlight(i);
    emit unhighlight(j);
}

void VisualizerWorker::swap(int i, int j) {
    if(i == j) return;

    emit highlight(i, Qt::red);
    emit highlight(j, Qt::red);
    QThread::msleep(delay);

    emit swapColumns(i, j);
    std::swap(arr[i], arr[j]);
    QThread::msleep(delay);

    emit highlight(i, Qt::green);
    emit highlight(j, Qt::green);
    QThread::msleep(delay);

    emit unhighlight(i);
    emit unhighlight(j);
}

void VisualizerWorker::slide(int i, int j) {
    if(i == j) return;

    emit highlight(i, Qt::red);
    emit highlight(j, Qt::red);
    QThread::msleep(delay);

    emit slideColumn(i, j);
    while(i != j) {
        std::swap(arr[i], arr[i-1]);
        --i;
    }
    QThread::msleep(delay);

    emit highlight(j, Qt::green);
    emit highlight(j+1, Qt::green);
    QThread::msleep(delay);

    emit unhighlight(j);
    emit unhighlight(j+1);
}

void VisualizerWorker::sortDone() {
    if(QThread::currentThread()->isInterruptionRequested()) {
        QThread::currentThread()->quit();
        return;
    }

    int n = arr.size();

    for(int i = 0; i < n-1; ++i) {
        if(arr[i] > arr[i+1]) {
            qDebug() << "no";
            break;
        }

        qDebug() << "yes";
    }

    for(int i = 0; i < n; ++i) {
        emit highlight(i, Qt::green);
        QThread::msleep(50);
        emit unhighlight(i);
    }

    emit setUI(true);
}
