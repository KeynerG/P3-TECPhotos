#include "RAID.h"

RAID::RAID() {
    checkDirectoriesIntegrity();
    checkPartitionsIntegrity();
}

bool RAID::checkFileExistance(int partition, string fileName) {
    ifstream ifile;
    switch (partition) {
        case 1:
            ifile.open(partitions1Directory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
            break;
        case 2:
            ifile.open(partitions2Directory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
            break;
        case 3:
            ifile.open(partitions3Directory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
            break;
        default:
            ifile.open(parityPartitionsDirectory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
            break;

    }
}

void RAID::generateParityPartition(string fileName) {
    ifstream fileManagerR;
    fileManagerR.open(partitions1Directory + fileName);
    string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),
                             (std::istreambuf_iterator<char>()));
    fileManagerR.close();

    fileManagerR.open(partitions2Directory + fileName);
    string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),
                             (std::istreambuf_iterator<char>()));
    fileManagerR.close();

    fileManagerR.open(partitions3Directory + fileName);
    string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),
                             (std::istreambuf_iterator<char>()));
    fileManagerR.close();

    ofstream fileManagerW;

    string parityPartition;
    for (int i = 0; i < partition1Content.length(); ++i) {
        int addition =
                (partition1Content.at(i) - '0') + (partition2Content.at(i) - '0') + (partition3Content.at(i) - '0');
        if (addition == 0 or addition == 2) {
            parityPartition.push_back('0');
        } else {
            parityPartition.push_back('1');
        }
    }

    fileManagerW.open(parityPartitionsDirectory + fileName);
    fileManagerW << parityPartition;
    fileManagerW.close();
}

void RAID::checkPartitionsIntegrity() {
    int fileId = 0;
    for (int i = 0;
         checkFileExistance(1, to_string(fileId) + ".txt") or
         checkFileExistance(2, to_string(fileId) + ".txt") or
         checkFileExistance(3, to_string(fileId) + ".txt") or
         checkFileExistance(4, to_string(fileId) + ".txt"); ++i) {

        if (!checkFileExistance(1, to_string(fileId) + ".txt") or
            !checkFileExistance(2, to_string(fileId) + ".txt") or
            !checkFileExistance(3, to_string(fileId) + ".txt") or
            !checkFileExistance(4, to_string(fileId) + ".txt")) {

            restoreFilePartition(to_string(fileId) + ".txt");

        }

        fileId = i + 1;
    }
}

void RAID::checkDirectoriesIntegrity() {
    QDir DrivesDirectory("../src/RAID5/Drives/");

    cout << "Drives directory exists: " << DrivesDirectory.exists() << endl;

    if (!DrivesDirectory.exists()) {
        DrivesDirectory.mkpath(".");
        DrivesDirectory.mkpath("drive-1");
        DrivesDirectory.mkpath("drive-2");
        DrivesDirectory.mkpath("drive-3");
        DrivesDirectory.mkpath("parity-drive");

    } else {
        int missingPartitions = 0;

        QDir disk1Directory(QString::fromStdString(partitions1Directory));
        if (!disk1Directory.exists()) {
            disk1Directory.mkpath(".");
            missingPartitions++;
        }

        QDir disk2Directory(QString::fromStdString(partitions2Directory));
        if (!disk2Directory.exists()) {
            disk2Directory.mkpath(".");
            missingPartitions++;
        }

        QDir disk3Directory(QString::fromStdString(partitions3Directory));
        if (!disk3Directory.exists()) {
            disk3Directory.mkpath(".");
            missingPartitions++;
        }

        QDir diskParityDirectory(QString::fromStdString(parityPartitionsDirectory));
        if (!diskParityDirectory.exists()) {
            diskParityDirectory.mkpath(".");
            missingPartitions++;
        }

        switch (missingPartitions) {
            case 0:
                break;
            case 1:
                checkPartitionsIntegrity();
                break;
            default:
                cerr << endl << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Error ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
                     << endl;
                cerr << "There are more than one missing partition for all files. Nothing to do." << endl << endl;
                cerr << "    Missing files partitions: " << to_string(missingPartitions) << ".";
                cerr << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl
                     << endl;
                break;
        }
    }
}

void RAID::restoreFilePartition(string fileName) {
    int missingFiles = 0;
    int filePartition;

    if (!checkFileExistance(1, fileName)) {
        missingFiles++;
        filePartition = 1;
    }
    if (!checkFileExistance(2, fileName)) {
        missingFiles++;
        filePartition = 2;
    }
    if (!checkFileExistance(3, fileName)) {
        missingFiles++;
        filePartition = 3;
    }
    if (!checkFileExistance(4, fileName)) {
        missingFiles++;
        filePartition = 4;
    }

    if (missingFiles == 1) {
        switch (filePartition) {
            case 1: {
                ifstream fileManagerR;
                fileManagerR.open(partitions2Directory + fileName);
                string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),
                                         (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(partitions3Directory + fileName);
                string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),
                                         (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(parityPartitionsDirectory + fileName);
                string parityPartitionContent((std::istreambuf_iterator<char>(fileManagerR)),
                                              (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                ofstream fileManagerW;

                string partition1Content;
                for (int i = 0; i < partition2Content.length(); ++i) {
                    int addition =
                            (partition2Content.at(i) - '0') + (partition3Content.at(i) - '0') +
                            (parityPartitionContent.at(i) - '0');
                    if (addition == 0 or addition == 2) {
                        partition1Content.push_back('0');
                    } else {
                        partition1Content.push_back('1');
                    }
                }

                fileManagerW.open(partitions1Directory + fileName);
                fileManagerW << partition1Content;
                fileManagerW.close();
                break;
            }
            case 2: {
                ifstream fileManagerR;
                fileManagerR.open(partitions1Directory + fileName);
                string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),
                                         (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(partitions3Directory + fileName);
                string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),
                                         (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(parityPartitionsDirectory + fileName);
                string parityPartitionContent((std::istreambuf_iterator<char>(fileManagerR)),
                                              (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                ofstream fileManagerW;

                string partition2Content;
                for (int i = 0; i < partition1Content.length(); ++i) {
                    int addition =
                            (partition1Content.at(i) - '0') + (partition3Content.at(i) - '0') +
                            (parityPartitionContent.at(i) - '0');
                    if (addition == 0 or addition == 2) {
                        partition2Content.push_back('0');
                    } else {
                        partition2Content.push_back('1');
                    }
                }

                fileManagerW.open(partitions2Directory + fileName);
                fileManagerW << partition2Content;
                fileManagerW.close();
                break;
            }
            case 3: {
                ifstream fileManagerR;
                fileManagerR.open(partitions1Directory + fileName);
                string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),
                                         (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(partitions2Directory + fileName);
                string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),
                                         (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(parityPartitionsDirectory + fileName);
                string parityPartitionContent((std::istreambuf_iterator<char>(fileManagerR)),
                                              (std::istreambuf_iterator<char>()));
                fileManagerR.close();

                ofstream fileManagerW;

                string partition3Content;
                for (int i = 0; i < partition1Content.length(); ++i) {
                    int addition =
                            (partition1Content.at(i) - '0') + (partition2Content.at(i) - '0') +
                            (parityPartitionContent.at(i) - '0');
                    if (addition == 0 or addition == 2) {
                        partition3Content.push_back('0');
                    } else {
                        partition3Content.push_back('1');
                    }
                }

                fileManagerW.open(partitions3Directory + fileName);
                fileManagerW << partition3Content;
                fileManagerW.close();
                break;
            }
            default: {
                generateParityPartition(fileName);
                break;
            }
        }
    } else {
        cerr << endl << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Error ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cerr << "There are more than one missing partition for the file: " << fileName << "." << endl << endl;
        cerr << "    Missing file partitions: " << to_string(missingFiles) << ".";
        cerr << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
    }
}

int RAID::saveData(string data) {
    cout << "Data to save: " << data << "\n" << endl;

    checkDirectoriesIntegrity();
    checkPartitionsIntegrity();

    // Separates the data in 3 parts.
    string partition1, partition2, partition3;

    while ((data.length() % 3) != 0) {
        data.push_back('0');
    }

    int charactersByPartition = data.length() / 3;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < charactersByPartition; ++j) {
            switch (i) {
                case 0:
                    partition1.push_back(data.at(0));
                    data.erase(0, 1);
                    break;
                case 1:
                    partition2.push_back(data.at(0));
                    data.erase(0, 1);
                    break;
                case 2:
                    partition3.push_back(data.at(0));
                    data.erase(0, 1);
                    break;
            }
        }
    }

    // Determinate the name of the file to write
    int fileId = 0;
    for (int i = 0;
         checkFileExistance(1, to_string(fileId) + ".txt") or
         checkFileExistance(2, to_string(fileId) + ".txt") or
         checkFileExistance(3, to_string(fileId) + ".txt"); ++i) {
        fileId = i;
    }

    // Writes the content of the .txt files.
    ofstream fileManager;

    // Write the content of the partition 1
    fileManager.open(partitions1Directory + to_string(fileId) + ".txt");
    fileManager << partition1;
    fileManager.close();

    // Write the content of the partition 2
    fileManager.open(partitions2Directory + to_string(fileId) + ".txt");
    fileManager << partition2;
    fileManager.close();

    // Write the content of the partition 3
    fileManager.open(partitions3Directory + to_string(fileId) + ".txt");
    fileManager << partition3;
    fileManager.close();

    generateParityPartition(to_string(fileId) + ".txt");

    return fileId;
}

string RAID::loadData(string fileName) {
    if (checkFileExistance(1, fileName) and
        checkFileExistance(2, fileName) and
        checkFileExistance(3, fileName) and
        checkFileExistance(4, fileName)) {

        ifstream fileManagerR;
        fileManagerR.open(partitions1Directory + fileName);
        string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),
                                 (std::istreambuf_iterator<char>()));
        fileManagerR.close();

        fileManagerR.open(partitions2Directory + fileName);
        string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),
                                 (std::istreambuf_iterator<char>()));
        fileManagerR.close();

        fileManagerR.open(partitions3Directory + fileName);
        string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),
                                 (std::istreambuf_iterator<char>()));
        fileManagerR.close();

        ofstream fileManagerW;

        string filecontent;
        filecontent += partition1Content + partition2Content + partition3Content;

        cout << "Data to load: " << filecontent << "\n" << endl;

        return filecontent;
    } else {
        checkDirectoriesIntegrity();
        checkPartitionsIntegrity();

        if (checkFileExistance(1, fileName) and
            checkFileExistance(2, fileName) and
            checkFileExistance(3, fileName) and
            checkFileExistance(4, fileName)) {

            ifstream fileManagerR;
            fileManagerR.open(partitions1Directory + fileName);
            string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),
                                     (std::istreambuf_iterator<char>()));
            fileManagerR.close();

            fileManagerR.open(partitions2Directory + fileName);
            string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),
                                     (std::istreambuf_iterator<char>()));
            fileManagerR.close();

            fileManagerR.open(partitions3Directory + fileName);
            string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),
                                     (std::istreambuf_iterator<char>()));
            fileManagerR.close();

            ofstream fileManagerW;

            string filecontent;
            filecontent += partition1Content + partition2Content + partition3Content;

            cout << "Data to load: " << filecontent << "\n" << endl;

            return filecontent;
        } else {
            return "";
        }
    }
}