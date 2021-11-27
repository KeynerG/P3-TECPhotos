#include "DataManager.h"

DataManager *DataManager::instance = nullptr;

DataManager *DataManager::getInstance() {
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return instance;
}

const string &DataManager::getCurrentUsername() const {
    return currentUsername;
}

void DataManager::setCurrentUsername(const string &newCurrentUsername) {
    currentUsername = newCurrentUsername;
}

QMap <string, QVector<string>> &DataManager::getCurrentUserMap() {
    return currentUserMap;
}

void DataManager::setCurrentUserMap(const QMap <string, QVector<string>> &newCurrentUserMap) {
    currentUserMap = newCurrentUserMap;
}

const string &DataManager::getCurrentAlbumName() const {
    return currentAlbumName;
}

void DataManager::setCurrentAlbumName(const string &newCurrentAlbumName) {
    currentAlbumName = newCurrentAlbumName;
}

const string &DataManager::getCurrentImageAuthor() const {
    return currentImageAuthor;
}

void DataManager::setCurrentImageAuthor(const string &newCurrentImageAuthor) {
    currentImageAuthor = newCurrentImageAuthor;
}

const string &DataManager::getCurrentImageName() const {
    return currentImageName;
}

void DataManager::setCurrentImageName(const string &newCurrentImageName) {
    currentImageName = newCurrentImageName;
}

const string &DataManager::getCurrentImageCreationDate() const {
    return currentImageCreationDate;
}

void DataManager::setCurrentImageCreationDate(const string &newCurrentImageCreationDate) {
    currentImageCreationDate = newCurrentImageCreationDate;
}

const string &DataManager::getCurrentImageSize() const {
    return currentImageSize;
}

void DataManager::setCurrentImageSize(const string &newCurrentImageSize) {
    currentImageSize = newCurrentImageSize;
}

const string &DataManager::getCurrentImageWidthX() const {
    return currentImageWidthX;
}

void DataManager::setCurrentImageWidthX(const string &newCurrentImageWidthX) {
    currentImageWidthX = newCurrentImageWidthX;
}

const string &DataManager::getCurrentImageHeightY() const {
    return currentImageHeightY;
}

void DataManager::setCurrentImageHeightY(const string &newCurrentImageHeightY) {
    currentImageHeightY = newCurrentImageHeightY;
}

const string &DataManager::getCurrentImageDescription() const {
    return currentImageDescription;
}

void DataManager::setCurrentImageDescription(const string &newCurrentImageDescription) {
    currentImageDescription = newCurrentImageDescription;
}

void DataManager::createCollection(string collectionName) {
    db.create_collection(collectionName);
}

int DataManager::querryNextImageId() {
    bool exists = true;
    bsoncxx::document::element imageIdFounded;
    string imageIdFoundedStr;
    int possibleId;

    while (exists) {
        possibleId = rand() % 1000000000;
        imageIdFoundedStr.clear();

        mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("imageId", to_string(possibleId))));
        for (auto &&doc: cursor) {
            imageIdFounded = doc["imageId"];
            imageIdFoundedStr = imageIdFounded.get_utf8().value.to_string();
        }

        if (imageIdFoundedStr.empty()) {
            exists = false;
        }
    }

    return possibleId;
}

void DataManager::connectToDB() {
    cout << "DATABASE LOG - CONNECTING TO DATABASE..." << endl << endl;
    mongocxx::cursor cursor = connectivityAuthentication.find(make_document(kvp("connection", "true")));
    for (auto &&doc: cursor) {
        bsoncxx::document::element connectionResult = doc["connection"];
        if (connectionResult.get_utf8().value.to_string() == "true") {
            cout << "DATABASE LOG - SUCCESSFULLY CONNECTED TO DATABASE." << endl << endl;
        } else {
            cerr << "DATABASE LOG - ERROR: UNABLE TO CONNECT TO DATABASE." << endl << endl;
        }
    }
}

bool DataManager::login(string username, string password) {
    this->currentUsername = "";
    bool successful = false;
    mongocxx::cursor cursor = usersCollection.find(make_document(kvp("username", username), kvp("password", password)));
    for (auto &&doc: cursor) {
        bsoncxx::document::element usernameBD = doc["username"];
        this->currentUsername = usernameBD.get_utf8().value.to_string();
        querryUserInformation();
        cout << "DATABASE LOG - " << this->currentUsername << " LOGGED IN." << endl << endl;
        successful = true;
    }
    if (this->currentUsername.empty()) {
        cerr << "DATABASE LOG - ERROR: INVALID USERNAME OR PASSWORD." << endl << endl;
        successful = false;
    }
    return successful;
}

bool DataManager::signUp(string username, string password) {
    bool exists = false;
    bool successful = false;
    mongocxx::cursor cursor = usersCollection.find(make_document(kvp("username", username)));
    for (auto &&doc: cursor) {
        exists = true;
    }
    if (!exists) {
        auto builder = bsoncxx::builder::stream::document{};
        bsoncxx::document::value doc_value = builder
                << "username" << username
                << "password" << password
                << bsoncxx::builder::stream::finalize;

        bsoncxx::document::view document = doc_value.view();

        usersCollection.insert_one(document);
        this->currentUsername = username;
        successful = true;
        cout << "DATABASE LOG - " << this->currentUsername << " SIGNED UP." << endl << endl;
    } else {
        cerr << "DATABASE LOG - ERROR: UNAVAILABLE USERNAME" << endl << endl;
    }
    return successful;
}

void DataManager::querryUserInformation() {
    currentUserMap.clear();
    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("username", this->currentUsername)));
    for (auto &&doc: cursor) {
        bsoncxx::document::element albumName = doc["albumName"];
        bsoncxx::document::element imageId = doc["imageId"];
        if (!currentUserMap.contains(albumName.get_utf8().value.to_string())) {
            currentUserMap.insert(albumName.get_utf8().value.to_string(), QVector<string>());
            QVector <string> v = currentUserMap.value(albumName.get_utf8().value.to_string());
            v.append(imageId.get_utf8().value.to_string());
            currentUserMap[albumName.get_utf8().value.to_string()] = v;
        } else {
            QVector <string> v = currentUserMap.value(albumName.get_utf8().value.to_string());
            v.append(imageId.get_utf8().value.to_string());
            currentUserMap[albumName.get_utf8().value.to_string()] = v;
        }
    }

    cout << "DATABASE LOG - LOADED " << this->currentUsername << "'S INFORMATION." << endl << endl;
}

void
DataManager::sendImageMetadata(string imageId, string albumName, string author, string imageName, string creationDate,
                               string size, string widthX, string heightY, string description) {

    auto builder = bsoncxx::builder::stream::document{};
    bsoncxx::document::value doc_value = builder
            << "username" << this->currentUsername
            << "imageId" << imageId
            << "albumName" << albumName
            << "author" << author
            << "imageName" << imageName
            << "creationDate" << creationDate
            << "size" << size
            << "widthX" << widthX
            << "heightY" << heightY
            << "description" << description
            << bsoncxx::builder::stream::finalize;

    bsoncxx::document::view document = doc_value.view();

    imagesCollection.insert_one(document);
    querryUserInformation();
    cout << "DATABASE LOG - IMAGE " << imageId << " SAVED." << endl << endl;
}

void DataManager::querryImageMetadata(string imageId) {
    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("imageId", imageId)));
    for (auto &&doc: cursor) {
        bsoncxx::document::element albumName = doc["albumName"];
        bsoncxx::document::element author = doc["author"];
        bsoncxx::document::element imageName = doc["imageName"];
        bsoncxx::document::element creationDate = doc["creationDate"];
        bsoncxx::document::element size = doc["size"];
        bsoncxx::document::element widthX = doc["widthX"];
        bsoncxx::document::element heightY = doc["heightY"];
        bsoncxx::document::element description = doc["description"];

        if (albumName.type() != bsoncxx::type::k_utf8 or author.type() != bsoncxx::type::k_utf8 or
            imageName.type() != bsoncxx::type::k_utf8 or creationDate.type() != bsoncxx::type::k_utf8 or
            size.type() != bsoncxx::type::k_utf8 or description.type() != bsoncxx::type::k_utf8) {
            cerr << "DATABASE LOG - ERROR: LOST IMAGE METADATA." << endl << endl;
        } else {
            this->currentImageId = imageId;
            this->currentAlbumName = albumName.get_utf8().value.to_string();
            this->currentImageAuthor = author.get_utf8().value.to_string();
            this->currentImageName = imageName.get_utf8().value.to_string();
            this->currentImageCreationDate = creationDate.get_utf8().value.to_string();
            this->currentImageSize = size.get_utf8().value.to_string();
            this->currentImageWidthX = widthX.get_utf8().value.to_string();
            this->currentImageHeightY = heightY.get_utf8().value.to_string();
            this->currentImageDescription = description.get_utf8().value.to_string();
            cout << "DATABASE LOG - IMAGE " << this->currentImageId << " LOADED." << endl << endl;
        }
    }
}

bool DataManager::saveImage(QImage image, string imageName, string imageAlbumName, string imageDescription,
                            string imageAuthor,
                            string imageSize, string imageWidthX, string imageHeightY, string imageDate) {
    bool imageSaved = false;
    int extraCeros = 0;
    int imageId = querryNextImageId();
    // Create a QList of the image
    QList <QRgb> pixelsList;
    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QRgb &rgb = line[x];
            pixelsList.append(rgb);
        }
    }
    // Cast QList to QString
    QString pixelsListString;
    for (int i = 0; i < pixelsList.size(); i++) {
        pixelsListString += QString::number(pixelsList[i]);
        if (i < pixelsList.size() - 1)
            pixelsListString += ",";
    }
    // compressing the image data
    pair <string, QMap<char, string>> compressResult = huffman.compress(pixelsListString.toStdString());
    // Checking if the code generated is module of 3 before sending to the raid
    while ((compressResult.first.length() % 3) != 0) {
        compressResult.first.push_back('0');
        extraCeros++;
    }
    // Save the code in the raid
    raid.saveData(compressResult.first, imageId);
    // Save the dictionary of the image code in the Dictionary.xml
    saveXML(imageId, compressResult.second, extraCeros);
    // Send image metadata to database
    sendImageMetadata(to_string(imageId), imageAlbumName, imageAuthor, imageName, imageDate, imageSize, imageWidthX,
                      imageHeightY, imageDescription);
    imageSaved = true;
    return imageSaved;
}

QPair<QImage, bool> DataManager::loadImage(string id) {
    querryImageMetadata(id);

    QPair<QImage, bool> imageObj;
    bool exists = false;

    int width = stoi(this->currentImageWidthX);
    int height = stoi(this->currentImageHeightY);

    QImage image(width, height, QImage::Format_ARGB32);

    if (raid.checkFileExistance(1, id + ".txt") or raid.checkFileExistance(2, id + ".txt") or
        raid.checkFileExistance(3, id + ".txt") or raid.checkFileExistance(4, id + ".txt")) {
        QList <QRgb> imageQList;

        pair<QMap<char, string>, int> xmlData = loadXML(id);


        string raidData = raid.loadData(id);
        if (xmlData.second != 0) {
            for (int i = 0; i < xmlData.second; ++i) {
                raidData.pop_back();
            }
        }
        string imageQListString = huffman.decompress(raidData, xmlData.first);

        stringstream ss(imageQListString);
        long int pixelColor;
        while (ss.good()) {
            string substr;
            getline(ss, substr, ',');

            pixelColor = stol(substr);

            imageQList.append(pixelColor);
        }

        //QList<QRgb> imageQList to QImage image
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image.setPixel(x, y, imageQList.first());
                imageQList.removeFirst();
            }
        }
        exists = true;
    } else {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image.setPixel(x, y, 000000);
            }
        }
        exists = false;
        cerr << "RAID LOG - UNABLE TO LOAD IMAGE " << id << "." << endl << endl;
    }

    imageObj.first = image;
    imageObj.second = exists;

    return imageObj;
}

QPair<QImage, bool> DataManager::loadImage(int indexChange) {
    switch (indexChange) {
        case -1:
            if (currentAlbumIndex != 0) {
                currentAlbumIndex += indexChange;
            } else {
                currentAlbumIndex = currentUserMap.value(currentAlbumName).size() - 1;
            }
            break;
        case 1:
            if (currentAlbumIndex != currentUserMap.value(currentAlbumName).size() - 1 &&
                currentUserMap.value(currentAlbumName).size() != 1) {
                currentAlbumIndex += indexChange;
            } else {
                currentAlbumIndex = 0;
            }
            break;
        default:
            break;
    }
    QVector <string> albumElements = currentUserMap.value(this->currentAlbumName);
    return loadImage(albumElements[currentAlbumIndex]);
}

void DataManager::saveXML(int id, QMap<char, string> dictionary, int ceros) {
    QFile dictionaryFile(dictionaryPath + QString::fromStdString(to_string(id)) + ".xml");
    dictionaryFile.open(QIODevice::WriteOnly);
    if (dictionaryFile.exists()) {
        QXmlStreamWriter xmlWriter(&dictionaryFile);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeStartElement("Image_" + QString(QString::fromStdString(std::to_string(id))));

        QString dictionaryData;
        QMapIterator<char, string> m(dictionary);
        while (m.hasNext()) {
            m.next();
            if (m.key() == ',') {
                dictionaryData += QString(QString::fromStdString(std::to_string(m.key()))) + ":" +
                                  QString(QString::fromStdString(m.value()));
            } else {
                dictionaryData += QString(QString::fromStdString(std::to_string(m.key() - '0'))) + ":" +
                                  QString(QString::fromStdString(m.value()));
            }
            if (m.hasNext()) { dictionaryData += ";"; }
        }
        xmlWriter.writeTextElement("Huffman_Tree", dictionaryData);

        xmlWriter.writeTextElement("Extra_Ceros", QString::fromStdString(to_string(ceros)));

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        dictionaryFile.close();
    } else {
        qDebug() << "ERROR: The dictionary of image " << QString(QString::fromStdString(std::to_string(id)))
                 << " was not successfully saved.";
    }
}

pair<QMap<char, string>, int> DataManager::loadXML(string id) {
    pair<QMap<char, string>, int> result;
    QMap<char, string> dictionary;
    QString mapString;
    QList <string> mapElements;
    QList <string> keyValueElements;
    QString cerosString;

    // read the xml file
    QFile dictionaryFile(dictionaryPath + QString::fromStdString(id) + ".xml");
    if (dictionaryFile.exists()) {
        dictionaryFile.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlReader(&dictionaryFile);
        xmlReader.setDevice(&dictionaryFile);
        // reading from file
        if (xmlReader.readNextStartElement()) {
            if (xmlReader.name() == "Image_" + QString(QString::fromStdString(id))) {
                while (xmlReader.readNextStartElement()) {
                    if (xmlReader.name() == QString::fromStdString("Huffman_Tree")) {
                        mapString = xmlReader.readElementText();
                    }
                    if (xmlReader.name() == QString::fromStdString("Extra_Ceros")) {
                        cerosString = xmlReader.readElementText();
                    }
                }
            }
        }
        if (xmlReader.hasError()) {
            qDebug() << "ERROR: XML " << xmlReader.errorString().data();
        }
    } else {
        qDebug() << "ERROR: The dictionary of image " << QString(QString::fromStdString(id))
                 << " was not successfully read.";
    }

    // separate the pairs of key and value from the mapString
    stringstream ss(mapString.toStdString());
    while (ss.good()) {
        string substr;
        getline(ss, substr, ';');
        keyValueElements.push_back(substr);
    }

    for (int i = 0; i < keyValueElements.size(); ++i) {
        stringstream ss(keyValueElements[i]);
        while (ss.good()) {
            string substr;
            getline(ss, substr, ':');
            mapElements.push_back(substr);
        }
    }

    for (int i = 0; i < mapElements.size(); i = i + 2) {
        if (mapElements[i] == "44") {
            dictionary.insert(',', mapElements[i + 1]);
        } else {
            dictionary.insert(mapElements[i][0], mapElements[i + 1]);
        }
    }

    result.first = dictionary;
    result.second = stoi(cerosString.toStdString());

    return result;
}

void DataManager::deleteImageMetadata() {
    bool exists = false;
    bsoncxx::document::element albumName;
    bsoncxx::document::element imageName;

    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("imageId", currentImageId)));
    for (auto &&doc: cursor) {
        albumName = doc["albumName"];
        imageName = doc["imageName"];
        exists = true;
    }
    if (exists) {
        imagesCollection.delete_one(document{} << "imageId" << currentImageId << bsoncxx::builder::stream::finalize);
        querryUserInformation();
        raid.deleteData(currentImageId);
        cout << "DATABASE LOG - IMAGE " << currentImageId << " DELETED." << endl << endl;
    } else {
        cerr << "DATABASE LOG - ERROR: UNABLE TO DELETE IMAGE " << currentImageId << "." << endl << endl;
    }
}

void DataManager::deleteImageMetadata(string imageId) {
    bool exists = false;
    bsoncxx::document::element albumName;
    bsoncxx::document::element imageName;

    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("imageId", imageId)));
    for (auto &&doc: cursor) {
        albumName = doc["albumName"];
        imageName = doc["imageName"];
        exists = true;
    }
    if (exists) {
        imagesCollection.delete_one(document{} << "imageId" << imageId << bsoncxx::builder::stream::finalize);
        querryUserInformation();
        raid.deleteData(imageId);
        cout << "DATABASE LOG - IMAGE " << imageId << " DELETED." << endl << endl;
    } else {
        cerr << "DATABASE LOG - ERROR: UNABLE TO DELETE IMAGE " << imageId << "." << endl << endl;
    }
}

bool DataManager::deleteAlbum(string albumName) {
    bool successful = false;
    querryUserInformation();
    QMapIterator <string, QVector<string>> j(currentUserMap);
    while (j.hasNext()) {
        j.next();
        if (j.key() == albumName) {
            QVector <string> v = j.value();
            for (int i = 0; i < v.size(); ++i) {
                deleteImageMetadata(v[i]);
            }
            successful = true;
            cout << "DATABASE LOG - ALBUM " << albumName << " DELETED." << endl << endl;
        }
    }
    querryUserInformation();
    return successful;
}

bool DataManager::updateImageMetadata(string imageName, string imageDesc, string imageAuthor, string imageDate) {
    bool successful = false;
    bool exists = false;
    bsoncxx::document::element albumName;

    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("imageId", currentImageId)));
    for (auto &&doc: cursor) {
        albumName = doc["albumName"];
        exists = true;
    }
    if (exists) {
        imagesCollection.update_one(document{} << "imageId" << currentImageId << bsoncxx::builder::stream::finalize,
                                    document{} << "$set" << bsoncxx::builder::stream::open_document <<
                                               "imageName" << imageName <<
                                               "description" << imageDesc <<
                                               "author" << imageAuthor <<
                                               "creationDate" << imageDate
                                               << bsoncxx::builder::stream::close_document << finalize);
        successful = true;
        this->currentImageName = imageName;
        this-> currentImageDescription = imageDesc;
        this->currentImageAuthor = imageAuthor;
        this->currentImageCreationDate = imageDate;
        cout << "DATABASE LOG - IMAGE " << currentImageId << " UPDATED." << endl << endl;
    } else {
        successful = false;
        cerr << "DATABASE LOG - ERROR: UNABLE TO UPDATE IMAGE " << currentImageId << "." << endl << endl;
    }
    return successful;
}

void DataManager::openAlbum(string album) {
    this->currentAlbumName = album;
    this->currentAlbumIndex = 0;
}