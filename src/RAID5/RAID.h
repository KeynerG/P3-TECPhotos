#ifndef P3_TECPHOTOS_RAID_H
#define P3_TECPHOTOS_RAID_H

#include <QDir>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * @class RAID
 * @brief Class for the RAID-partitioning and save process.
 * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
 */
class RAID {

private:
    string partitions1Directory = "../src/RAID5/Drives/drive-1/"; /**< RAID 's drive 1 path reference. */
    string partitions2Directory = "../src/RAID5/Drives/drive-2/"; /**< RAID 's drive 2 path reference. */
    string partitions3Directory = "../src/RAID5/Drives/drive-3/"; /**< RAID 's drive 3 path reference. */
    string parityPartitionsDirectory = "../src/RAID5/Drives/parity-drive/"; /**< RAID 's parity drive path reference. */
    string imagesDictionariesDirectory = "../src/RAID5/Drives/dictionaries/"; /**< Images Huffman Tree path reference. */

public:
    RAID(); /**< RAID class constructor. */

    /**
     * @fn bool checkFileExistance(int partition, string fileName)
     * @brief Function that verifies if an specific file partition exists or not.
     * @param partition
     * @param fileName
     * @return boolean
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    bool checkFileExistance(int partition, string fileName);

    /**
     * @fn void generateParityPartition(string fileName)
     * @brief Function that generates the parity partition by XOR methodology.
     * @param fileName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void generateParityPartition(string fileName);

    /**
     * @fn void checkDirectoriesIntegrity()
     * @brief Function that certifies if the important directories already exist.
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void checkDirectoriesIntegrity();

    /**
     * @fn void restoreFilePartition(string fileName)
     * @brief Function that rebuilds a lost partition of an specific file.
     * @param fileName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void restoreFilePartition(string fileName);

    /**
     * @fn void saveData(string data, int imageId)
     * @brief Function that saves a binary-data into the different RAID partition drives.
     * @param data
     * @param imageId
     * @return int
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void saveData(string data, int imageId);

    /**
     * @fn string loadData(string &fileName)
     * @brief Function that loads and specific file from the RAID partition drives.
     * @param fileName
     * @return string
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    string loadData(string &fileName);

    /**
     * @fn void deleteData(string &fileId)
     * @brief Function that deletes a file from all the partition drives.
     * @param fileId
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void deleteData(string &fileId);
};

#endif //P3_TECPHOTOS_RAID_H