#include "simpleTK.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    simpleTK w;
    w.show();
    return a.exec();
}
