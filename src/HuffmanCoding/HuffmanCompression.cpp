#include "HuffmanCompression.h"

HuffmanCompression::Node *
HuffmanCompression::getNode(char character, int freq, HuffmanCompression::Node *left, HuffmanCompression::Node *right) {
    Node *node = new Node();

    node->character = character;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

void HuffmanCompression::encode(HuffmanCompression::Node *root, const string &str,
                                unordered_map<char, string> &huffmanCode) {
    if (root == nullptr) {
        return;
    }
    if (!root->left && !root->right) {
        huffmanCode[root->character] = str;
    }
    encode(root->left, str + "0", huffmanCode);
    encode(root->right, str + "1", huffmanCode);
}

void HuffmanCompression::decode(HuffmanCompression::Node *root, int &index, string str) {
    if (root == nullptr) {
        return;
    }
    if (!root->left && !root->right) {
        return;
    }
    index++;

    if (str[index] == '0') {
        decode(root->left, index, str);
    } else {
        decode(root->right, index, str);
    }
}

pair<string, QMap<char, string>> HuffmanCompression::compress(const string &text) {
    // count frequency of appearance of each character and store it in a map
    unordered_map<char, int> freq;
    for (char character: text) {
        freq[character]++;
    }

    // Create a priority queue to store live nodes of Huffman tree;
    priority_queue<Node *, vector<Node *>, comparison> pq;

    // Create a leaf node for each character and add it to the priority queue.
    for (auto pair: freq) {
        pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
    }

    // do till there is more than one node in the queue
    while (pq.size() != 1) {
        // Remove the two nodes of highest priority (lowest frequency) from the queue
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        // Create a new internal node with these two nodes as children and with frequency equal to the sum
        // of the two nodes' frequencies. Add the new node to the priority queue.
        int sum = left->freq + right->freq;
        pq.push(getNode('\0', sum, left, right));
    }

    // root stores pointer to root of Huffman Tree
    Node *root = pq.top();

    // traverse the Huffman Tree and store Huffman Codes in a map. Also prints them
    unordered_map<char, string> huffmanCode;
    encode(root, "", huffmanCode);

    QMap<char, string> huffmanResult;
    for (const auto &pair: huffmanCode) {
        huffmanResult.insert(pair.first, pair.second);
    }
    string str;
    for (char character: text) {
        str += huffmanCode[character];
    }

//    int index = -1;
//    while (index < (int) str.size() - 2) {
//        decode(root, index, str);
//    }
    pair <string, QMap<char, string>> result;
    result.first = str;
    result.second = huffmanResult;

    cout << "HUFFMAN LOG - COMPRESSION SUCCESSFUL.\n" << endl;

    return result;
}

string HuffmanCompression::decompress(string str, QMap<char, string> huffmanList) {
    QList<char> characters = huffmanList.keys();
    QList<string> scripts = huffmanList.values();
    string result , curr;
    int curr_char = 0;

    while(curr_char < str.size())
    {
        curr.push_back(str[curr_char]);
        if(scripts.contains(curr)){
            result.push_back(characters[scripts.indexOf(curr)]);
            curr.clear();
        }
        curr_char++;
    }

    cout << "HUFFMAN LOG - DECOMPRESSION SUCCESSFUL.\n" << endl;

    return result;
}
