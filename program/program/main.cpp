#include "program.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    program w;
    w.show();
    return a.exec();
}
