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
    // Create a QList if the image
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
    for(int i=0; i < pixelsList.size(); i++)
    {
        pixelsListString += QString::number(pixelsList[i]);
        if( i < pixelsList.size()-1)
            pixelsListString += "," ;
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
    //saveXML(imageId, compressResult.second, extraCeros);
}

void DataManager::saveXML(int id, QMap<char, string> dictionary, int ceros) {
    QFile DictionaryFile(dictionaryPath);
    //editar XML
    DictionaryFile.open(QIODevice::WriteOnly);
    DictionaryFile.close();

}

QImage DataManager::loadImage(QString &id) {
    QImage image;
    // Cargar con RAID
    // Descomprimir con Huffman
    return image;
}