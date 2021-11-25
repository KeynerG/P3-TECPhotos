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
 * @class HuffmanCompression
 * @brief For compress and decompress strings.
 * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
 */
class HuffmanCompression {

public:

    /**
     * @struct Node
     * @brief Node struct to generate the codes in the huffman tree.
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    struct Node {
        char character;
        int freq;
        Node *left, *right;
    };

    /**
     * @Struct comparison
     * @brief Comparison object to be used to order the heap.
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    struct comparison {
        bool operator()(Node *l, Node *r) {
            // highest priority item has lowest frequency
            return l->freq > r->freq;
        }
    };

    /**
     * @fn static Node *getNode(char character, int freq, Node *left, Node *right)
     * @brief Function to allocate a new tree node.
     * @param character
     * @param freq
     * @param left
     * @param right
     * @return Node
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static Node *getNode(char character, int freq, Node *left, Node *right);

    /**
     * @fn static void encode(Node *root, const string &str, unordered_map<char, string> &huffmanCode)
     * @brief Traverse the Huffman Tree and store Huffman Codes in a map.
     * @param root
     * @param str
     * @param huffmanCode
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static void encode(Node *root, const string &str, unordered_map<char, string> &huffmanCode);

    /**
     * @fn static void decode(Node *root, int &index, string str)
     * @brief Traverse the Huffman Tree and decode the encoded string.s
     * @param root
     * @param index
     * @param str
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static void decode(Node *root, int &index, string str);

    /**
     * @fn static pair<string, QMap<char, string>> compress(const string &text)
     * @brief Builds a Huffman Tree and decode given input text.
     * @param text
     * @return pair
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static pair<string, QMap<char, string>> compress(const string &text);

    /**
     * @fn static string decompress(string str, QMap<char, string> huffmanList)
     * @brief Builds the original text from the code and the map of characters.
     * @param str
     * @param huffmanList
     * @return string
     * @author <a href="https://github.com/KeynerG">Keyner S. Gómez Pana</a>
     */
    static string decompress(string str, QMap<char, string> huffmanList);
};

#endif //P3_TECPHOTOS_HUFFMANCOMPRESSION_H
