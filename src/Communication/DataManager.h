#ifndef P3_TECPHOTOS_DATAMANAGER_H
#define P3_TECPHOTOS_DATAMANAGER_H

#include "../HuffmanCoding/HuffmanCompression.h"
#include "../RAID5/RAID.h"

#include <QFile>
#include <QImage>
#include <QString>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <iostream>
#include <sstream>

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
    QString dictionaryPath = "../src/RAID5/Drives/dictionaries/";

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
     * @fn void saveXML(int id, QMap<char, string> dictionary, int ceros);
     * @brief
     * @param id
     * @param dictionary
     * @param ceros
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void saveXML(int id, QMap<char, string> dictionary, int ceros);

    /**
     * @fn
     * @brief
     * @param id
     * @return
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    pair<QMap<char, string>, int> loadXML(int id);

    /**
     * @fn void saveImage(QImage &image)
     * @brief
     * @param image
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    void saveImage(QImage &image);

    /**
     * @fn QImage loadImage(QString id)
     * @brief
     * @param id
     * @return QImage
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    QImage loadImage(int id);
};

#endif //P3_TECPHOTOS_DATAMANAGER_H