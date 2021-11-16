#ifndef P3_TECPHOTOS_DATAMANAGER_H
#define P3_TECPHOTOS_DATAMANAGER_H

#include "../HuffmanCoding/HuffmanCompression.h"
#include "../RAID5/RAID.h"

#include <QImage>
#include <QString>
#include <iostream>

using namespace std;

/**
 * @class DataManager
 * @brief
 * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
 */
class DataManager {
private:
    HuffmanCompression huffman;
    RAID raid;
    static DataManager *instance;

protected:
    DataManager() {};

    static DataManager *getInstance();

    void saveImage(QImage image);

    QImage loadImage(QString id);

    void operator=(const DataManager &) = delete;
};

#endif //P3_TECPHOTOS_DATAMANAGER_H