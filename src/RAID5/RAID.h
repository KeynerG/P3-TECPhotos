#ifndef P3_TECPHOTOS_RAID_H
#define P3_TECPHOTOS_RAID_H

#include <QDir>
#include <fstream>
#include <iostream>

using namespace std;

/**
 * @class RAID
 * @brief
 * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
 */
class RAID {

private:
    string partitions1Directory = "../src/RAID5/Drives/drive-1/"; /**< RAID 's drive 1 path reference. */
    string partitions2Directory = "../src/RAID5/Drives/drive-2/"; /**< RAID 's drive 2 path reference. */
    string partitions3Directory = "../src/RAID5/Drives/drive-3/"; /**< RAID 's drive 3 path reference. */
    string parityPartitionsDirectory = "../src/RAID5/Drives/parity-drive/"; /**< RAID 's parity drive path reference. */

public:
    RAID(); /**< RAID class constructor. */

    /**
     * @fn bool checkFileExistance(int partition, string fileName)
     * @brief
     * @param partition
     * @param fileName
     * @return boolean
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    bool checkFileExistance(int partition, string fileName);

    /**
     * @fn void generateParityPartition(string fileName)
     * @brief
     * @param fileName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void generateParityPartition(string fileName);

    /**
     * @fn void checkPartitionsIntegrity()
     * @brief
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void checkPartitionsIntegrity();

    /**
     * @fn void checkDirectoriesIntegrity()
     * @brief
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void checkDirectoriesIntegrity();

    /**
     * @fn void restoreFilePartition(string fileName)
     * @brief
     * @param fileName
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    void restoreFilePartition(string fileName);

    /**
     * @fn void saveData(string data)
     * @brief
     * @param data
     * @return int
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    int saveData(string data);

    /**
     * @fn string loadData(string fileName)
     * @brief
     * @param fileName
     * @return string
     * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
     */
    string loadData(string fileName);

};

#endif //P3_TECPHOTOS_RAID_H