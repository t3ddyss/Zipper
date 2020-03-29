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
    vector<byte> Encode(vector<byte> bytes);

    vector<byte> Decode(vector<byte> bytes);

private:
    class HuffmanTree
    {
    public:
        HuffmanTree();
        map<byte, string> buildTree();

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

        vector<Node*> nodes;

    protected:
        vector<Node*> leaves;
    };

    void buildProbabilityTable();

    void buildCodesTable(const map<byte, string>& codeTable);

    vector<byte> bytes;
    static map<byte, tuple<byte, unsigned short, double>> codesTable; //<символ, <кол-во ведущих нулей, код, вероятность>>
    static map<byte, double> probabilitiesTable; //<символ, вероятность>

};

union DoubleShortByteUnion
{
    double Double;
    unsigned short uShort;
    byte uBytes[8];
};


#endif //ZIPPER_HUFFMAN_H
