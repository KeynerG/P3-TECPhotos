#ifndef P3_TECPHOTOS_RAID_H
#define P3_TECPHOTOS_RAID_H

#include <fstream>
#include <QDir>
#include <iostream>

using namespace std;

/**
 * @class RAID
 * @brief
 * @author <a href="https://github.com/JoseAndres216">Jose A. Rodríguez Rojas</a>
 */
class RAID {

private:
    string partitions1Directory = "../src/RAID5/RAIDisks/disk-1/";
    string partitions2Directory = "../src/RAID5/RAIDisks/disk-2/";
    string partitions3Directory = "../src/RAID5/RAIDisks/disk-3/";
    string parityPartitionsDirectory = "../src/RAID5/RAIDisks/disk-parity/";

public:
    RAID();

    bool checkFileExistance(int partition, string fileName);

    void generateParityPartition(string fileName);

    void checkPartitionsIntegrity();

    void checkDirectoriesIntegrity();

    void restoreFilePartition(string fileName);

    void saveData(string data);

    string loadData(string fileName);

};

#endif //P3_TECPHOTOS_RAID_H