#include "RAID.h"

RAID::RAID() {
    checkDirectoriesIntegrity();
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
        case 2:
            ifile.open(partitions2Directory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
        case 3:
            ifile.open(partitions3Directory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
        default:
            ifile.open(parityPartitionsDirectory + fileName);
            if (ifile) {
                return true;
            } else {
                return false;
            }
    }
}

void RAID::generateParityPartition(string fileName) {
    ifstream fileManagerR;
    fileManagerR.open(partitions1Directory + fileName);
    string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
    fileManagerR.close();

    fileManagerR.open(partitions2Directory + fileName);
    string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
    fileManagerR.close();

    fileManagerR.open(partitions3Directory + fileName);
    string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
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

void RAID::checkDirectoriesIntegrity() {
    QDir DrivesDirectory("../src/RAID5/Drives/");

    if (!DrivesDirectory.exists()) {
        DrivesDirectory.mkpath(".");
        DrivesDirectory.mkpath("drive-1");
        DrivesDirectory.mkpath("drive-2");
        DrivesDirectory.mkpath("drive-3");
        DrivesDirectory.mkpath("parity-drive");
        DrivesDirectory.mkpath("dictionaries");
    } else {

        QDir disk1Directory(QString::fromStdString(partitions1Directory));
        if (!disk1Directory.exists()) {
            disk1Directory.mkpath(".");
        }

        QDir disk2Directory(QString::fromStdString(partitions2Directory));
        if (!disk2Directory.exists()) {
            disk2Directory.mkpath(".");
        }

        QDir disk3Directory(QString::fromStdString(partitions3Directory));
        if (!disk3Directory.exists()) {
            disk3Directory.mkpath(".");
        }

        QDir diskParityDirectory(QString::fromStdString(parityPartitionsDirectory));
        if (!diskParityDirectory.exists()) {
            diskParityDirectory.mkpath(".");
        }

        QDir dictionariesDirectory(QString::fromStdString(imagesDictionariesDirectory));
        if (!dictionariesDirectory.exists()) {
            dictionariesDirectory.mkpath(".");
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
                string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(partitions3Directory + fileName);
                string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(parityPartitionsDirectory + fileName);
                string parityPartitionContent((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
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
                string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(partitions3Directory + fileName);
                string partition3Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(parityPartitionsDirectory + fileName);
                string parityPartitionContent((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                ofstream fileManagerW;

                string partition2Content;
                for (int i = 0; i < partition1Content.length(); ++i) {
                    int addition = (partition1Content.at(i) - '0') + (partition3Content.at(i) - '0') + (parityPartitionContent.at(i) - '0');
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
                string partition1Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(partitions2Directory + fileName);
                string partition2Content((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                fileManagerR.open(parityPartitionsDirectory + fileName);
                string parityPartitionContent((std::istreambuf_iterator<char>(fileManagerR)),(std::istreambuf_iterator<char>()));
                fileManagerR.close();

                ofstream fileManagerW;

                string partition3Content;
                for (int i = 0; i < partition1Content.length(); ++i) {
                    int addition = (partition1Content.at(i) - '0') + (partition2Content.at(i) - '0') + (parityPartitionContent.at(i) - '0');
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

void RAID::saveData(string data, int imageId) {
    cout << "Data to save: " << data << "\n" << endl;

    checkDirectoriesIntegrity();

    // Separates the data in 3 parts.
    string partition1, partition2, partition3;

    int charactersByPartition = data.length() / 3;

    partition1 = data.substr(0, charactersByPartition);
    partition2 = data.substr(charactersByPartition, charactersByPartition);
    partition3 = data.substr(charactersByPartition*2);

//    // Determinate the name of the file to write
//    int fileId = 0;
//    for (int i = 0;
//         checkFileExistance(1, to_string(fileId) + ".txt") or
//         checkFileExistance(2, to_string(fileId) + ".txt") or
//         checkFileExistance(3, to_string(fileId) + ".txt"); ++i) {
//        fileId = i;
//    }

    // Writes the content of the .txt files.
    ofstream fileManager;

    // Write the content of the partition 1
    fileManager.open(partitions1Directory + to_string(imageId) + ".txt");
    fileManager << partition1;
    fileManager.close();

    // Write the content of the partition 2
    fileManager.open(partitions2Directory + to_string(imageId) + ".txt");
    fileManager << partition2;
    fileManager.close();

    // Write the content of the partition 3
    fileManager.open(partitions3Directory + to_string(imageId) + ".txt");
    fileManager << partition3;
    fileManager.close();

    generateParityPartition(to_string(imageId) + ".txt");
}

string RAID::loadData(string &imageID) {

    string fileName = imageID + ".txt";

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
            restoreFilePartition(fileName);

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
        }
    }
}

void RAID::deleteData(string &fileId) {
    QDir partition1(QString::fromStdString(partitions1Directory));
    QDir partition2(QString::fromStdString(partitions2Directory));
    QDir partition3(QString::fromStdString(partitions3Directory));
    QDir parityDrive(QString::fromStdString(parityPartitionsDirectory));
    partition1.remove(QString::fromStdString(fileId + ".txt"));
    partition2.remove(QString::fromStdString(fileId + ".txt"));
    partition3.remove(QString::fromStdString(fileId + ".txt"));
    parityDrive.remove(QString::fromStdString(fileId + ".txt"));

    QDir dictionaries(QString::fromStdString(imagesDictionariesDirectory));
    dictionaries.remove(QString::fromStdString(fileId + ".xml"));
}