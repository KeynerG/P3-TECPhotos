#include "DataManager.h"

DataManager *DataManager::instance = nullptr;

DataManager *DataManager::getInstance() {
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return instance;
}

void DataManager::createCollection(string collectionName) {
    db.create_collection(collectionName);
}

void DataManager::login(string username, string password) {
    mongocxx::cursor cursor = usersCollection.find(make_document(kvp("username", username), kvp("password", password)));
    for (auto &&doc: cursor) {
        bsoncxx::document::element usernameBD = doc["username"];
        this->currentUsername = usernameBD.get_utf8().value.to_string();
        querryUserInformation();
        cout << "DATABASE LOG - " << this->currentUsername << " LOGGED IN." << endl << endl;
    }
    if (this->currentUsername.empty()) {
        cerr << "ERROR - INVALID USERNAME OR PASSWORD." << endl << endl;
    }
}

void DataManager::signUp(string username, string password) {
    bool exists = false;
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
        cout << "DATABASE LOG - " << this->currentUsername << " SIGNED UP." << endl << endl;
    } else {
        cerr << "ERROR - UNAVAILABLE USERNAME" << endl << endl;
    }
}

void DataManager::querryUserInformation() {
    currentUserMap.clear();
    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("username", this->currentUsername)));
    for (auto &&doc: cursor) {
        bsoncxx::document::element albumName = doc["albumName"];
        bsoncxx::document::element imageId = doc["imageId"];
        if (!currentUserMap.contains(albumName.get_utf8().value.to_string())) {
            currentUserMap.insert(albumName.get_utf8().value.to_string(), QVector<string>());
            QVector<string> v = currentUserMap.value(albumName.get_utf8().value.to_string());
            v.append(imageId.get_utf8().value.to_string());
            currentUserMap[albumName.get_utf8().value.to_string()] = v;
        } else {
            QVector<string> v = currentUserMap.value(albumName.get_utf8().value.to_string());
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
    cout << "DATABASE LOG - IMAGE SAVED [ID: " << imageId << ", ALBUM: " << albumName << ", IMAGE NAME: " << imageName
         << "]." << endl << endl;
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
            cerr << "ERROR: LOST IMAGE METADATA." << endl << endl;
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
            cout << "DATABASE LOG - IMAGE LOADED [ID: " << this->currentImageId << ", ALBUM: " << this->currentAlbumName
                 << ", IMAGE NAME: " << this->currentImageName << "]." << endl << endl;
        }
    }
}

void DataManager::printInfo() {
    cout << "Info:" << endl << endl;
    cout << "Username: " << currentUsername << endl;
    cout << "imageId: " << currentImageId << endl;
    cout << "albumName: " << currentAlbumName << endl;
    cout << "imageAuthor: " << currentImageAuthor << endl;
    cout << "imageName: " << currentImageName << endl;
    cout << "imageCreationDate: " << currentImageCreationDate << endl;
    cout << "imageSize: " << currentImageSize << endl;
    cout << "imageWidthX: " << currentImageWidthX << endl;
    cout << "imageHeightY: " << currentImageHeightY << endl;
    cout << "imageDescription: " << currentImageDescription << endl;
    cout << "Albums: [";
    QMapIterator<string, QVector<string>> j(currentUserMap);
    while (j.hasNext()) {
        j.next();
        cout << j.key() << ": ";
        QVector<string> v = j.value();
        for (int i = 0; i < v.size(); ++i) {
            cout << v[i] << ", ";
        }
        cout << " - ";
    }
    cout << "]." << endl;
}

void DataManager::saveImage(QImage &image, string imageName, string imageAlbumName, string imageDescription, string imageAuthor,
                            string imageSize, string imageWidthX, string imageHeightY, string imageDate) {
    int extraCeros = 0;
    int imageId = 0;
    // Create a QList of the image
    QList<QRgb> pixelsList;
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
    pair<string, QMap<char, string>> compressResult = huffman.compress(pixelsListString.toStdString());
    // Checking if the code generated is module of 3 before sending to the raid
    while ((compressResult.first.length() % 3) != 0) {
        compressResult.first.push_back('0');
        extraCeros++;
    }
    // Save the code in the raid
    imageId = raid.saveData(compressResult.first);
    // Save the dictionary of the image code in the Dictionary.xml
    saveXML(imageId, compressResult.second, extraCeros);
    // Send image metadata to database
    sendImageMetadata(to_string(imageId), imageAlbumName, imageAuthor, imageName, imageDate, imageSize, imageWidthX,
                      imageHeightY, imageDescription);
}

QImage DataManager::loadImage(string id) {
    QImage image(QString::fromStdString(id));
    QList<QRgb> imageQList;

    pair<QMap<char, string>, int> xmlData = loadXML(id);
    string raidData = raid.loadData(id);
    if (xmlData.second != 0) {
        for (int i = 0; i < xmlData.second; ++i) {
            raidData.pop_back();
        }
    }
    string imageQListString = huffman.decompress(raidData, xmlData.first);

    QRgb pixel;
    stringstream ss(imageQListString);
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        pixel = stoi(substr);
        imageQList.append(pixel);
    }

    //QList<QRgb> imageQList to QImage image

    querryImageMetadata(id);

    return image;
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
    QFile dictionaryFile(dictionaryPath + QString::fromStdString(id) + ".xml");
    QMap<char, string> dictionary;
    if (dictionaryFile.exists()) {
        dictionaryFile.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlReader(&dictionaryFile);
        xmlReader.setDevice(&dictionaryFile);
        // reading from file
        while (xmlReader.readNextStartElement()) {
            if (xmlReader.name() == "Image_" + QString(QString::fromStdString(id))) {
                // dictionary append
            } else {
                xmlReader.skipCurrentElement();
            }
        }
        if (xmlReader.hasError()) {
            qDebug() << "ERROR: XML " << xmlReader.errorString().data();
        }
    } else {
        qDebug() << "ERROR: The dictionary of image " << QString(QString::fromStdString(id))
                 << " was not successfully read.";
    }
    return result;
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
        cout << "DATABASE LOG - IMAGE DELETED [ID: " << imageId << ", ALBUM: " << albumName.get_utf8().value.to_string()
             << ", IMAGE NAME: " << imageName.get_utf8().value.to_string() << "]." << endl << endl;
    } else {
        cerr << "ERROR - COULD NOT DELETE THE IMAGE." << endl << endl;
    }
}

void DataManager::deleteAlbum(string albumName) {
    querryUserInformation();
    QMapIterator<string, QVector<string>> j(currentUserMap);
    while (j.hasNext()) {
        j.next();
        if (j.key() == albumName) {
            QVector<string> v = j.value();
            for (int i = 0; i < v.size(); ++i) {
                deleteImageMetadata(v[i]);
            }
            cout << "DATABASE LOG - ALBUM DELETED [ALBUM NAME: " << albumName << "]." << endl << endl;
        }
    }
    querryUserInformation();
}

void DataManager::updateImageMetadata(string imageId, string imageName, string imageDesc, string imageAuthor,
                                      string imageDate) {
    bool exists = false;
    bsoncxx::document::element albumName;

    mongocxx::cursor cursor = imagesCollection.find(make_document(kvp("imageId", imageId)));
    for (auto &&doc: cursor) {
        albumName = doc["albumName"];
        exists = true;
    }
    if (exists) {
        imagesCollection.update_one(document{} << "imageId" << imageId << bsoncxx::builder::stream::finalize,
                                    document{} << "$set" << bsoncxx::builder::stream::open_document <<
                                               "imageName" << imageName <<
                                               "imageDescription" << imageDesc <<
                                               "imageAuthor" << imageAuthor <<
                                               "imageCreationDate" << imageDate
                                               << bsoncxx::builder::stream::close_document << finalize);
        cout << "DATABASE LOG - IMAGE UPDATED [ID: " << imageId << ", ALBUM: " << albumName.get_utf8().value.to_string()
             << ", IMAGE NAME: " << imageName << "]." << endl << endl;

    } else {
        cerr << "ERROR - COULD NOT UPDATE THE IMAGE." << endl << endl;
    }
}