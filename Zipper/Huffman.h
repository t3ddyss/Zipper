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

    const int SYMBOL_DATA = 12; //9
    vector<byte> bytes;
    static map<byte, tuple<string, double>> symbolsTable; //<символ, <код, вероятность>>
    //static map<byte, double> probabilitiesTable; //<символ, вероятность>

};

union DoubleLongShortByteUnion
{
    double Double;
    unsigned long long uLong;
    unsigned short uShort;
    byte uBytes[8];
};


#endif //ZIPPER_HUFFMAN_H
