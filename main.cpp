#include "sorter.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sorter w;
    w.show();
    return a.exec();
}
