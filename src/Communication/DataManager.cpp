#include "DataManager.h"

DataManager *DataManager::instance = nullptr;

DataManager *DataManager::getInstance() {
    if (instance == nullptr) {
        instance = new DataManager();
    }
    return instance;
}

void DataManager::saveImage(QImage &image) {
    // Comprimir imagen con Huffman
    // Guardar con RAID
}

QImage DataManager::loadImage(QString &id) {
    QImage image;
    // Cargar con RAID
    // Descomprimir con Huffman
    return image;
}