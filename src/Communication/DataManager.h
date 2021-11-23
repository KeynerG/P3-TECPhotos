#ifndef P3_TECPHOTOS_DATAMANAGER_H
#define P3_TECPHOTOS_DATAMANAGER_H

#include "../HuffmanCoding/HuffmanCompression.h"
#include "../RAID5/RAID.h"

#include <bsoncxx/json.hpp>
#include <bsoncxx/builder/stream/helpers.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/builder/stream/array.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>

#include <QDebug>
#include <QFile>
#include <QImage>
#include <QMap>
#include <QString>
#include <QVector>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>

#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>

using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;

using namespace std;

/**
 * @class DataManager
 * @brief
 * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
 * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
 * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
 * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
 */
class DataManager {
private:
    static DataManager *instance; /**< DataManager class instance. */
    HuffmanCompression huffman; /**< HuffmanCompression class reference. */
    RAID raid; /**< RAID class reference. */
    QString dictionaryPath = "../src/RAID5/Drives/dictionaries/"; /**< . */

    mongocxx::instance inst{}; /**< . */
    mongocxx::uri uri = mongocxx::uri{"mongodb+srv://admin:0000@tecphotos.2uv6a.mongodb.net/TECPhotos?retryWrites=true&w=majority"}; /**< . */
    mongocxx::client conn{uri}; /**< . */
    mongocxx::database db = conn["test"]; /**< . */

    mongocxx::collection imagesCollection = db["Images"]; /**< . */
    mongocxx::collection usersCollection = db["Users"]; /**< . */

    string currentUsername; /**< . */
    string currentAlbumName; /**< . */
    QMap<string, QVector<string>> currentUserMap; /**< . */

    string currentImageId; /**< . */
    string currentImageAuthor; /**< . */
    string currentImageName; /**< . */
    string currentImageCreationDate; /**< . */
    string currentImageSize; /**< . */
    string currentImageWidthX; /**< . */
    string currentImageHeightY; /**< . */
    string currentImageDescription; /**< . */

protected:
    DataManager() = default; /**< DataManager class constructor. */

    /**
     * @fn void createCollection(string collectionName)
     * @brief
     * @param collectionName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void createCollection(string collectionName);

    /**
     * @fn void querryUserInformation()
     * @brief
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void querryUserInformation();

    /**
     * @fn void sendImageMetadata(string imageId, string albumName, string author, string imageName, string creationDate, string size, string widthX, string heightY, string description)
     * @brief
     * @param imageId
     * @param albumName
     * @param author
     * @param imageName
     * @param creationDate
     * @param size
     * @param widthX
     * @param heightY
     * @param description
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void sendImageMetadata(string imageId, string albumName, string author, string imageName, string creationDate,
                           string size, string widthX, string heightY, string description);

    /**
     * @fn void querryImageMetadata(string imageId)
     * @brief
     * @param imageId
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void querryImageMetadata(string imageId);

    /**
     * @fn void printInfo()
     * @brief
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void printInfo();

    /**
     * @fn void saveXML(int id, QMap<char, string> dictionary, int ceros);
     * @brief
     * @param id
     * @param dictionary
     * @param ceros
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void saveXML(int id, QMap<char, string> dictionary, int ceros);

    /**
     * @fn pair<QMap<char, string>, int> loadXML(string id)
     * @brief
     * @param id
     * @return pair<QMap<char, string>, int>
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    pair<QMap<char, string>, int> loadXML(string id);

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
     * @fn void login(string username, string password)
     * @brief
     * @param username
     * @param password
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void login(string username, string password);

    /**
     * @fn void signUp(string username, string password)
     * @brief
     * @param username
     * @param password
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void signUp(string username, string password);

     /**
      * @fn void saveImage(QImage &image, string imageName, string imageAlbumName, string imageDescription, string imageAuthor, string imageSize, string imageWidthX, string imageHeightY, string imageDate)
      * @brief
      * @param image
      * @param imageName
      * @param imageAlbumName
      * @param imageDescription
      * @param imageAuthor
      * @param imageSize
      * @param imageWidthX
      * @param imageHeightY
      * @param imageDate
      * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
      */
    void saveImage(QImage &image, string imageName, string imageAlbumName, string imageDescription, string imageAuthor,
                   string imageSize, string imageWidthX, string imageHeightY, string imageDate);

    /**
     * @fn QImage loadImage(QString id)
     * @brief
     * @param id
     * @return QImage
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    QImage loadImage(string id);

    /**
     * @fn void deleteImageMetadata(string imageId)
     * @brief
     * @param imageId
     * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
     */
    void deleteImageMetadata(string imageId);

    /**
     * @fn void deleteAlbum(string albumName)
     * @brief
     * @param albumName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void deleteAlbum(string albumName);

    /**
     * @fn void updateImageMetadata(string imageId, string imageName, string imageDesc, string imageAuthor, string imageDate)
     * @brief
     * @param imageId
     * @param imageName
     * @param imageDesc
     * @param imageAuthor
     * @param imageDate
     * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
     */
    void updateImageMetadata(string imageId, string imageName, string imageDesc, string imageAuthor, string imageDate);
};

#endif //P3_TECPHOTOS_DATAMANAGER_H