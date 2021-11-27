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
 * @brief Database controller.
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
    mongocxx::database db = conn["TECPhotos"]; /**< . */

    mongocxx::collection imagesCollection = db["Images"]; /**< . */
    mongocxx::collection usersCollection = db["Users"]; /**< . */
    mongocxx::collection connectivityAuthentication = db["Connection"]; /**< . */

    string currentUsername; /**< . */
    string currentAlbumName; /**< . */
    int currentAlbumIndex; /**< . */
    QMap <string, QVector<string>> currentUserMap; /**< . */

    string currentImageId; /**< . */
    string currentImageAuthor; /**< . */
    string currentImageName; /**< . */
    string currentImageCreationDate; /**< . */
    string currentImageSize; /**< . */
    string currentImageWidthX; /**< . */
    string currentImageHeightY; /**< . */
    string currentImageDescription; /**< . */

public:
    DataManager() = default; /**< DataManager class constructor. */

    /**
     * @fn void createCollection(string collectionName)
     * @brief Creates a new collection in the database.
     * @param collectionName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void createCollection(string collectionName);

    /**
     * @fn int querryNextImageId();
     * @return
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    int querryNextImageId();

    /**
     * @fn void querryUserInformation()
     * @brief Gets user information from the database.
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void querryUserInformation();

    /**
     * @fn void sendImageMetadata(string imageId, string albumName, string author, string imageName, string creationDate, string size, string widthX, string heightY, string description)
     * @brief Uploads new image metadata to the database.
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
     * @brief Gets image metadata from the database.
     * @param imageId
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void querryImageMetadata(string imageId);

    /**
     * @fn void saveXML(int id, QMap<char, string> dictionary, int ceros);
     * @brief Creates and saves an XML file.
     * @param id
     * @param dictionary
     * @param ceros
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void saveXML(int id, QMap<char, string> dictionary, int ceros);

    /**
     * @fn pair<QMap<char, string>, int> loadXML(string id)
     * @brief Loads an XML file.
     * @param id
     * @return pair<QMap<char, string>, int>
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    pair<QMap<char, string>, int> loadXML(string id);

    /**
     * @fn static DataManager *getInstance()
     * @brief Gets the DataManager singleton instance.
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
     * @fn const string &getCurrentUsername() const
     * @brief Gets the current user's username.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentUsername() const;

    /**
     * @fn void setCurrentUsername(const string &newCurrentUsername)
     * @brief Sets the current username.
     * @param newCurrentUsername
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentUsername(const string &newCurrentUsername);

    /**
     * @fn const string &getCurrentAlbumName() const
     * @brief Gets the current album's name.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentAlbumName() const;

    /**
     * @fn void setCurrentAlbumName(const string &newCurrentAlbumName)
     * @brief Sets the current album's name.
     * @param newCurrentAlbumName
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentAlbumName(const string &newCurrentAlbumName);

    /**
     * @fn const string &getCurrentImageAuthor() const
     * @brief Gets the current selected image's author.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageAuthor() const;

    /**
     * @fn void setCurrentImageAuthor(const string &newCurrentImageAuthor)
     * @brief Sets the current selected image's author.
     * @param newCurrentImageAuthor
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageAuthor(const string &newCurrentImageAuthor);

    /**
     * @fn const string &getCurrentImageName() const
     * @brief Gets the current selected image's name.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageName() const;

    /**
     * @fn void setCurrentImageName(const string &newCurrentImageName)
     * @brief Sets the current selected image's name.
     * @param newCurrentImageName
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageName(const string &newCurrentImageName);

    /**
     * @fn const string &getCurrentImageCreationDate() const
     * @brief Gets the current selected image's creation date.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageCreationDate() const;

    /**
     * @fn void setCurrentImageCreationDate(const string &newCurrentImageCreationDate)
     * @brief Sets the current selected image's creation date.
     * @param newCurrentImageCreationDate
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageCreationDate(const string &newCurrentImageCreationDate);

    /**
     * @fn const string &getCurrentImageSize() const
     * @brief Gets the current selected image's file size.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageSize() const;

    /**
     * @fn void setCurrentImageSize(const string &newCurrentImageSize)
     * @brief Sets the current selected image's file size.
     * @param newCurrentImageSize
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageSize(const string &newCurrentImageSize);

    /**
     * @fn const string &getCurrentImageWidthX() const
     * @brief Gets the current selected image's width.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageWidthX() const;

    /**
     * @fn void setCurrentImageWidthX(const string &newCurrentImageWidthX)
     * @brief Sets the current selected image's width.
     * @param newCurrentImageWidthX
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageWidthX(const string &newCurrentImageWidthX);

    /**
     * @fn const string &getCurrentImageHeightY() const
     * @brief Gets the current selected image's height.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageHeightY() const;

    /**
     * @fn void setCurrentImageHeightY(const string &newCurrentImageHeightY)
     * @brief Sets the current selected image's height.
     * @param newCurrentImageHeightY
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageHeightY(const string &newCurrentImageHeightY);

    /**
     * @fn const string &getCurrentImageDescription() const
     * @brief Gets the current selected image's description.
     * @return string
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    const string &getCurrentImageDescription() const;

    /**
     * @fn void setCurrentImageDescription(const string &newCurrentImageDescription)
     * @brief Sets the current selected image's description.
     * @param newCurrentImageDescription
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentImageDescription(const string &newCurrentImageDescription);

    /**
     * @fn QMap<string, QVector<string>> &getCurrentUserMap()
     * @brief Gets the current User Map.
     * @return QMap<string, QVector<string>>
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    QMap <string, QVector<string>> &getCurrentUserMap();

    /**
     * @fn void setCurrentUserMap(const QMap<string, QVector<string>> &newCurrentUserMap)
     * @brief Sets the current User Map.
     * @param newCurrentUserMap
     * @author <a href="https://github.com/valeriehernandez-7">Valerie M. Hernández Fernández</a>
     */
    void setCurrentUserMap(const QMap <string, QVector<string>> &newCurrentUserMap);

    /**
     * @fn void connectToDB()
     * @brief Creates connection to the Database.
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void connectToDB();

    /**
     * @fn bool login(string username, string password)
     * @brief Checks that the username and password entered corresponds to the ones in the database.
     * @param username
     * @param password
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    bool login(string username, string password);

    /**
     * @fn bool signUp(string username, string password)
     * @brief Creates a new user in the database.
     * @param username
     * @param password
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    bool signUp(string username, string password);

    /**
     * @fn bool saveImage(QImage &image, string imageName, string imageAlbumName, string imageDescription, string imageAuthor, string imageSize, string imageWidthX, string imageHeightY, string imageDate)
     * @brief Saves a new image.
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
    bool saveImage(QImage image, string imageName, string imageAlbumName, string imageDescription, string imageAuthor,
                   string imageSize, string imageWidthX, string imageHeightY, string imageDate);

    /**
     * @fn QPair<QImage, bool> loadImage(string id)
     * @brief Loads an image.
     * @param id
     * @return QPair<QImage, bool>
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    QPair<QImage, bool> loadImage(string id);

    /**
     * @fn QPair<QImage, bool> loadImage(int indexChange)
     * @brief Loads an image.
     * @param indexChange
     * @return QPair<QImage, bool>
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    QPair<QImage, bool> loadImage(int indexChange);

    /**
     * @fn void deleteImageMetadata()
     * @brief Deletes image metadata in the database.
     * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
     */
    void deleteImageMetadata();

    /**
     * @fn void deleteImageMetadata(string imageId)
     * @brief Deletes image metadata in the database.
     * @param imageId
     * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
     */
    void deleteImageMetadata(string imageId);

    /**
     * @fn bool deleteAlbum(string albumName)
     * @brief Deletes an album.
     * @param albumName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    bool deleteAlbum(string albumName);

    /**
     * @fn bool updateImageMetadata(string imageId, string imageName, string imageDesc, string imageAuthor, string imageDate)
     * @brief Updates the metadata in the database.
     * @param imageId
     * @param imageName
     * @param imageDesc
     * @param imageAuthor
     * @param imageDate
     * @author <a href="https://github.com/danyazunigab">Daniel A. Zúñiga Barahona</a>
     */
    bool updateImageMetadata(string imageName, string imageDesc, string imageAuthor, string imageDate);

    /**
     * @fn void printInfo()
     * @brief Console log for the current image's info.
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void printInfo();

    /**
     * @fn void openAlbum(string album)
     * @brief Opens an album.
     * @param album
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void openAlbum(string album);
};

#endif //P3_TECPHOTOS_DATAMANAGER_H