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
        cout << root->character;
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

    cout << "Huffman Codes are :\n" << '\n';
    QMap<char, string> huffmanResult;
    for (const auto &pair: huffmanCode) {
        cout << pair.first << " " << pair.second << '\n';
        huffmanResult.insert(pair.first, pair.second);
    }
    cout << "\nOriginal string was :\n" << text << '\n';
    string str;
    for (char character: text) {
        str += huffmanCode[character];
    }
    cout << "\nEncoded string is :\n" << str << '\n';

    int index = -1;
    cout << "\nDecoded string is: \n";
    while (index < (int) str.size() - 2) {
        decode(root, index, str);
    }
    pair <string, QMap<char, string>> result;
    result.first = str;
    result.second = huffmanResult;
    cout << endl;
    return result;
}

string HuffmanCompression::decompress(string str, QMap<char, string> huffmanList) {
    vector<char> characters;
    vector<string> scripts;
    string result, curr;
    int max_chars = 0;
    for (auto e: huffmanList.keys()) {
        characters.push_back(e);
        scripts.push_back(huffmanList.value(e));
    }
    while (max_chars < str.size()) {
        curr.push_back(str[max_chars]);
        if (isChar(scripts, curr)) {
            result.push_back(characters[getKey(scripts, curr)]);
            curr.clear();
        }
        max_chars++;
    }
    return result;
}

bool HuffmanCompression::isChar(vector<string> scripts, string key) {
    for (int i = 0; i < scripts.size(); ++i) {
        if (scripts[i] == key) { return true; }
    }
}

int HuffmanCompression::getKey(vector<string> scripts, string pos) {
    for (int i = 0; i < scripts.size(); ++i) {
        if (scripts[i] == pos) { return i; }
    }
}