#include "UI/TECPhotos.h"
#include "Communication/DataManager.h"

#include <QApplication>

/**
 * @fn main int main(int argc, char *argv[])
 * @brief
 * @param argc
 * @param argv
 * @return QApplication::exec()
 * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
 * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
 * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
 * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
 */
int main(int argc, char *argv[]) {
    DataManager::getInstance()->connectToDB();
    QApplication application(argc, argv);
    TECPhotos tecPhotos;
    tecPhotos.show();
    return application.exec();
}
