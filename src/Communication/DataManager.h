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
 * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
 */
class DataManager {
private:
    static DataManager *instance; /**< DataManager class instance. */
    HuffmanCompression huffman; /**< HuffmanCompression class reference. */
    RAID raid; /**< RAID class reference. */

protected:
    DataManager() = default; /**< DataManager class constructor. */
    
public:
    /**
     * @fn static DataManager *getInstance()
     * @brief
     * @return instance
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    static DataManager *getInstance();

    /**
     * @fn void operator=(const DataManager &) = delete
     * @brief
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void operator=(const DataManager &) = delete;

    /**
     * @fn void saveImage(QImage image)
     * @brief
     * @param image
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void saveImage(QImage &image);

    /**
     * @fn QImage loadImage(QString id)
     * @brief
     * @param id
     * @return QImage
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    QImage loadImage(QString &id);
};

#endif //P3_TECPHOTOS_DATAMANAGER_H