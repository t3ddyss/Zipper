//
// Created by Fedor on 28.03.2020.
//

#include "Huffman.h"
#include <utility>

map<byte, tuple<string, double>> Huffman::symbolsTable;
//map<byte, double> Huffman::probabilitiesTable;

vector<byte> Huffman::Encode(vector<byte> input)
{
    this->bytes = std::move(input);

    buildProbabilityTable();

    HuffmanTree* tree = new HuffmanTree();
    tree->buildTree();

    vector<byte> output;

    DoubleLongShortByteUnion doubleLongShortByteUnion;
    doubleLongShortByteUnion.uShort = symbolsTable.size();

    // Size of header; 2 bytes
    output.push_back(doubleLongShortByteUnion.uBytes[1]);
    output.push_back(doubleLongShortByteUnion.uBytes[0]);

    string code = "";

    // Length of code; 8 bytes
    for (size_t i = 0; i < bytes.size(); i++)
    {
        code += get<0>(symbolsTable.find(bytes[i])->second);
    }

    unsigned long long codeLength = code.length();
    doubleLongShortByteUnion.uLong = codeLength;

    for (int j = 7; j >= 0; j--)
    {
        output.push_back(doubleLongShortByteUnion.uBytes[j]);
    }

    map<byte, tuple<string, double>>::iterator it;

    // Header; symbol = 1 byte, probability = 8 bytes;
    for (it = symbolsTable.begin(); it != symbolsTable.end(); it++)
    {
        output.push_back(it->first);

        doubleLongShortByteUnion.Double = get<1>(it->second);

        for (int j = 7; j >= 0; j--)
        {
            output.push_back(doubleLongShortByteUnion.uBytes[j]);
        }
    }


    // Code
    for (size_t i = 0; i < codeLength - 8; i += 8)
    {
        string bits8 = code.substr(i, 8);

        std::bitset<8> bitset8(bits8);
        byte byte1 = bitset8.to_ulong();

        output.push_back(byte1);
    }

    byte last = codeLength % 8 == 0 ? 8 : codeLength % 8;

    string bits8 = code.substr(code.length() - last, last);
    bits8 += string(8 - last, '0');

    std::bitset<8> bitset8(bits8);
    byte byte1 = bitset8.to_ulong();

    output.push_back(byte1);

    return output;
}

void Huffman::buildProbabilityTable()
{
    symbolsTable.clear();

    map<byte, tuple<string, double>>::iterator it;
    size_t size = bytes.size();

    for (size_t i = 0; i < size; i++)
    {
        it = symbolsTable.find(bytes[i]);

        if (it == symbolsTable.end())
        {
            symbolsTable.insert(std::make_pair(
                    bytes[i], std::make_tuple("", 1.0 / size)));
        }

        else
        {
            get<1>(it->second) += 1.0 / size;
        }
    }
}

vector<byte> Huffman::Decode(vector<byte> input)
{
    symbolsTable.clear();

    this->bytes = std::move(input);

    DoubleLongShortByteUnion doubleShortByteUnion;

    doubleShortByteUnion.uBytes[1] = bytes[0];
    doubleShortByteUnion.uBytes[0] = bytes[1];

    size_t symbolTableCount = doubleShortByteUnion.uShort;

    for (size_t i = 2; i < symbolTableCount * SYMBOL_DATA + 2; i += SYMBOL_DATA)
    {
        byte symbol = bytes[i];
        byte leadingZeros = bytes[i + 1];

        /*doubleShortByteUnion.uBytes[1] = bytes[i + 2];
        doubleShortByteUnion.uBytes[0] = bytes[i + 3];

        string code = std::bitset<16>(doubleShortByteUnion.uShort).to_string();*/

        for (int j = SYMBOL_DATA - 1; j >= 4; j--)
        {
            doubleShortByteUnion.uBytes[j] = bytes[i + SYMBOL_DATA - j];
        }





    }
}

Huffman::HuffmanTree::Node::Node(double probability, byte symbol)
{
    this->probability = probability;
    this->symbol = symbol;
    this->left = nullptr;
    this->right = nullptr;
}

bool Huffman::HuffmanTree::Node::operator>(const Huffman::HuffmanTree::Node &other) const
{
    return (probability > other.probability);
}

Huffman::HuffmanTree::HuffmanTree()
{
    map<byte, tuple<string, double>>::iterator it;

    for (it = symbolsTable.begin(); it != symbolsTable.end(); it++)
    {
        nodes.push_back(new Node(get<1>(it->second), it->first));
    }

    sort(nodes.begin(), nodes.end());
}

void Huffman::HuffmanTree::buildTree()
{
    while (nodes.size() != 1)
    {
        Node* leftNode = nodes.back();
        nodes.pop_back();
        Node* rightNode = nodes.back();
        nodes.pop_back();

        if (isLeaf(leftNode)) leaves.insert(leaves.begin(), leftNode);
        if (isLeaf(rightNode)) leaves.insert(leaves.begin(), rightNode);

        if (leftNode->probability <= rightNode->probability)
        {
            leftNode->code = "0";
            rightNode->code = "1";
        }

        else
        {
            leftNode->code = "1";
            rightNode->code = "0";
        }

        Node* parent = new Node(leftNode->probability + rightNode->probability, ' ');
        parent->left = leftNode;
        parent->right = rightNode;

        int i = 0;
        while (nodes[i]->probability >= parent->probability) i++;
        nodes.insert(nodes.begin() + i, parent);
    }

    createCodes(nodes[0]);

    for (int i = 0; i < leaves.size(); i++)
    {
        get<0>(symbolsTable.find(leaves[i]->symbol)->second) = leaves[i]->code;
    }

    deleteTree(nodes[0]);
}

bool Huffman::HuffmanTree::isLeaf(Huffman::HuffmanTree::Node *node)
{
    return (node->left == nullptr && node->right == nullptr);
}

void Huffman::HuffmanTree::deleteTree(Huffman::HuffmanTree::Node *root) {
    if (root == nullptr) return;

    deleteTree(root->left);
    deleteTree(root->right);

    free(root);
}

void Huffman::HuffmanTree::createCodes(Huffman::HuffmanTree::Node *root)
{
    if (root == nullptr) return;

    if (root->right != nullptr) root->right->code = (root->code + root->right->code);
    if (root->left != nullptr) root->left->code = (root->code + root->left->code);

    createCodes(root->right);
    createCodes(root->left);
}
