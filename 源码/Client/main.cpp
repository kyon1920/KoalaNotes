#include "koalanotes.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    KoalaNotes w;
    w.show();
    return a.exec();
}
