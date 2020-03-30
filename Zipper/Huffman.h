//
// Created by Fedor on 28.03.2020.
//
#include <vector>
#include <ios>
#include <map>
#include <algorithm>
#include <bitset>

#ifndef ZIPPER_HUFFMAN_H
#define ZIPPER_HUFFMAN_H

typedef unsigned char byte;
using std::vector;
using std::string;
using std::map;
using std::pair;
using std::sort;
using std::tuple;
using std::get;

class Huffman
{
public:
    vector<byte> Encode(vector<byte> input);

    vector<byte> Decode(vector<byte> input);

private:
    class HuffmanTree
    {
    public:
        HuffmanTree();
        void buildTree();
        vector<byte> findSymbols(string code, unsigned long long codeLength);

    private:
        struct Node
        {
            byte symbol;
            string code;
            double probability;
            struct Node* left;
            struct Node* right;

            explicit Node(double probability, byte symbol);

            bool operator > (const Node& other) const;
        };

        bool isLeaf(Node* node);

        void deleteTree(Node* root);

        void createCodes(Node* root);

        vector<Node*> nodes; // node[0] -- tree root after buildTree() execution

        Node* node;

    protected:
        vector<Node*> leaves;
    };

    void buildProbabilityTable();

    const int SYMBOL_DATA = 9;
    const int HEADER_BEGIN = 10;
    vector<byte> bytes;
    static map<byte, tuple<string, double>> symbolsTable; //<символ, <код, вероятность>>

};

union DoubleLongShortByteUnion
{
    double Double;
    unsigned long long uLong;
    unsigned short uShort;
    byte uBytes[8];
};


#endif //ZIPPER_HUFFMAN_H
