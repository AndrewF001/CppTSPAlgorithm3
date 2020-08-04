#include "CppTSPalgorithm.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CppTSPalgorithm w;
    w.show();
    return a.exec();
}
