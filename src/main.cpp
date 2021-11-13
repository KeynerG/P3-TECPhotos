#include "src/UI/PhotoTEC.h"

#include <QApplication>

/**
 * @fn main int main(int argc, char *argv[])
 * @brief
 * @param argc
 * @param argv
 * @return QApplication::exec()
 */
int main(int argc, char *argv[]) {
    QApplication application(argc, argv);
    PhotoTEC photoTEC;
    photoTEC.show();
    return application.exec();
}