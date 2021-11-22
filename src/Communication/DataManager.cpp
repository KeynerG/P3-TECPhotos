#include "DataManager.h"

DataManager *DataManager::instance = nullptr;

DataManager *DataManager::getInstance() {
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return instance;
}

void DataManager::saveImage(QImage &image) {
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
}

QImage DataManager::loadImage(int id) {
    QImage image(QString::fromStdString(to_string(id)));
    QList<QRgb> imageQList;

    pair<QMap<char, string>, int> xmlData = loadXML(id);
    string raidData = raid.loadData(to_string(id));
    if(xmlData.second != 0){
        for (int i = 0; i < xmlData.second; ++i) {
            raidData.pop_back();
        }
    }
    string imageQListString = huffman.decompress(raidData,xmlData.first);

    QRgb pixel;
    stringstream ss(imageQListString);
    while (ss.good()) {
        string substr;
        getline(ss, substr, ',');
        pixel = stoi(substr);
        imageQList.append(pixel);
    }

    //QList<QRgb> imageQList to QImage image

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
            if(m.key() == ','){
                dictionaryData += QString(QString::fromStdString(std::to_string(m.key()))) + ":" + QString(QString::fromStdString(m.value()));
            }else{
                dictionaryData += QString(QString::fromStdString(std::to_string(m.key()-'0'))) + ":" + QString(QString::fromStdString(m.value()));
            }
            if(m.hasNext()){dictionaryData += ";";}
        }
        xmlWriter.writeTextElement("Huffman_Tree", dictionaryData);

        xmlWriter.writeTextElement("Extra_Ceros", QString::fromStdString(to_string(ceros)));

        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        dictionaryFile.close();
    } else {
        qDebug() << "ERROR: The dictionary of image " << QString(QString::fromStdString(std::to_string(id))) << " was not successfully saved.";
    }
}

pair<QMap<char, string>, int> DataManager::loadXML(int id) {
    pair<QMap<char, string>, int> result;
    QFile dictionaryFile(dictionaryPath + QString::fromStdString(to_string(id)) + ".xml");
    QMap<char, string> dictionary;
    if (dictionaryFile.exists()) {
        dictionaryFile.open(QIODevice::ReadOnly);
        QXmlStreamReader xmlReader(&dictionaryFile);
        xmlReader.setDevice(&dictionaryFile);
        // reading from file
        while (xmlReader.readNextStartElement()) {
            if (xmlReader.name() == "Image_" + QString(QString::fromStdString(std::to_string(id)))) {
                // dictionary append
            } else {
                xmlReader.skipCurrentElement();
            }
        }
        if (xmlReader.hasError()) {
            qDebug() << "ERROR: XML " << xmlReader.errorString().data();
        }
    } else {
        qDebug() << "ERROR: The dictionary of image " << QString(QString::fromStdString(std::to_string(id))) << " was not successfully read.";
    }
    return result;
}