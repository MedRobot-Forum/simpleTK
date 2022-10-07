#include "simpleTK.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
	vtkObject::GlobalWarningDisplayOff();
	application.setWindowIcon(QIcon(iconapp));
    simpleTK w;
    w.show();
    return application.exec();
}
