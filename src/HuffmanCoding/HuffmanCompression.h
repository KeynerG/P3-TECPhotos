#ifndef P3_TECPHOTOS_HUFFMANCOMPRESSION_H
#define P3_TECPHOTOS_HUFFMANCOMPRESSION_H

#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <QList>
#include <QMap>

using namespace std;

/**
 * @class Class for compress and decompress strings
 * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
 */
class HuffmanCompression {

public:

    /**
     * @brief Node struct to generate the codes in the huffman tree
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    struct Node {
        char character;
        int freq;
        Node *left, *right;
    };

    /**
     * @brief Comparison object to be used to order the heap
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    struct comparison {
        bool operator()(Node *l, Node *r) {
            // highest priority item has lowest frequency
            return l->freq > r->freq;
        }
    };

    /**
     * @brief Function to allocate a new tree node
     * @param character
     * @param freq
     * @param left
     * @param right
     * @return
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static Node *getNode(char character, int freq, Node *left, Node *right);

    /**
     * @brief traverse the Huffman Tree and store Huffman Codes in a map.
     * @param root
     * @param str
     * @param huffmanCode
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static void encode(Node *root, const string &str, unordered_map<char, string> &huffmanCode);

    /**
     * @brief Traverse the Huffman Tree and decode the encoded string
     * @param root
     * @param index
     * @param str
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static void decode(Node *root, int &index, string str);

    /**
     * @brief Builds a Huffman Tree and decode given input text
     * @param text
     * @return
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static pair<string, QMap<char, string>> compress(const string &text);

    /**
     * @brief Builds the original text from the code and the map of characters
     * @param str
     * @param huffmanList
     * @return
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static string decompress(string str, QMap<char, string> huffmanList);

    /**
     * @brief
     * @param scripts
     * @param key
     * @return
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static bool isChar(vector<string> scripts, string key);

    /**
     * @brief
     * @param scripts
     * @param pos
     * @return
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static int getKey(vector<string> scripts, string pos);
};

#endif //P3_TECPHOTOS_HUFFMANCOMPRESSION_H