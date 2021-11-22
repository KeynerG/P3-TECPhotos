#include "UI/TECPhotos.h"
#include "Communication/DataManager.h"

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
    TECPhotos tecPhotos;
    tecPhotos.show();
    return application.exec();
}