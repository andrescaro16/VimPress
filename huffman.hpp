#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <queue>
#include <bitset>

class HuffmanCoding {
private:
    struct HeapNode {
        char data;
        unsigned int freq;
        HeapNode* left;
        HeapNode* right;
    };

    struct Compare {
        bool operator()(HeapNode* lhs, HeapNode* rhs) const {
            return lhs->freq > rhs->freq;
        }
    };

    std::priority_queue<HeapNode*, std::vector<HeapNode*>, Compare> heap;
    std::unordered_map<char, std::string> codes;
    std::unordered_map<std::string, char> reverseMapping;
    std::string text;
    const std::string HEADER_END = "HEADER_END";

    std::unordered_map<char, unsigned int> makeFrequencyDict(std::string text);
    void makeHeap(const std::unordered_map<char, unsigned int>& frequency);
    void mergeNodes();
    void makeCodesHelper(HeapNode* root, std::string currentCode);
    void makeCodes();
    void reconstructReverseMapping(const std::string& mappingString);
    std::string getEncodedText(const std::string& text);
    std::string padEncodedText(const std::string& encodedText);
    std::vector<unsigned char> getByteArray(const std::string& paddedEncodedText);
    std::string removePadding(const std::string& paddedEncodedText);
    std::string decodeText(const std::string& encodedText);
    HeapNode* buildHuffmanTree();

public:
    HuffmanCoding() = default;
    std::vector<unsigned char> compress(const std::string& text);
    std::string decompress(const std::vector<unsigned char>& compressedData);
};

#endif // HUFFMAN_HPP
