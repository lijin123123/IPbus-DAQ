#include "mywindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MYWindow w;
    w.show();

    return a.exec();
}
