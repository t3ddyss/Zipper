//
// Created by Fedor on 28.03.2020.
//

#include "Huffman.h"
#include <utility>

map<byte, pair<byte, unsigned short>> Huffman::codesTable;
map<byte, double> Huffman::probabilitiesTable;

vector<byte> Huffman::Encode()
{
    buildProbabilityTable();

    HuffmanTree* tree = new HuffmanTree();
    map<byte, string> codes = tree->buildTree();

    buildCodesTable(codes);

    vector<byte> result;

    DoubleShortByteUnion doubleShortByteUnion;
    doubleShortByteUnion.uShort = codesTable.size();

    //auto headerSize = (unsigned short) codesTable.size();

    /*byte* code = new byte[2]; //= (byte*)(headerSize);

    code[0] = headerSize; // вторые 8
    code[1] = (headerSize >> 8); // первые 8

    result.push_back(code[1]);
    result.push_back(code[0]);*/

    //byte* code = reinterpret_cast<byte*>(headerSize);

    result.push_back(shortByteUnion.uBytes[1]);
    result.push_back(shortByteUnion.uBytes[0]);

    map<byte, pair<byte, unsigned short>>::iterator it;

    for (it = codesTable.begin(); it != codesTable.end(); it++)
    {
        result.push_back(it->first);
        result.push_back(it->second.first);

        byte* code = (byte*)&it->second.second;

        result.push_back(code[0]);
        result.push_back(code[1]);
    }

    for (size_t i = 0; i < bytes.size(); i++)
    {
        it = codesTable.find(bytes[i]);

        byte* code = (byte*)&it->second.second;

        result.push_back(code[0]);
        result.push_back(code[1]);
    }

    return result;
}

void Huffman::buildProbabilityTable()
{
    map<byte, double>::iterator it;
    size_t size = bytes.size();

    for (size_t i = 0; i < size; i++)
    {
        it = probabilitiesTable.find(bytes[i]);

        if (it == probabilitiesTable.end())
        {
            probabilitiesTable.insert(pair<byte, double>(bytes[i], 1.0 / size));
        }

        else
        {
            it->second += 1.0 / size;
        }
    }
}

void Huffman::buildCodesTable(const map<byte, string>& codeTable)
{
    map<byte, string>::const_iterator it;

    for (it = codeTable.begin(); it != codeTable.end(); it++)
    {
        size_t found = it->second.find('1');
        byte leadingZerosCount = found == string::npos ? it->second.length() : found;

        unsigned short code = std::stoi(it->second, nullptr, 2);
        codesTable.insert(std::make_pair(it->first, std::make_pair(leadingZerosCount, code)));
    }

}

Huffman::Huffman(vector<byte> bytes)
{
    this->bytes = std::move(bytes);
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
    map<byte, double>::iterator it;

    for (it = probabilitiesTable.begin(); it != probabilitiesTable.end(); it++)
    {
        nodes.push_back(new Node(it->second, it->first));
    }

    sort(nodes.begin(), nodes.end());
}

map<byte, string> Huffman::HuffmanTree::buildTree()
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

    map<byte, string> codes;

    for (int i = 0; i < leaves.size(); i++)
    {

        codes.insert(pair<byte, string>(leaves[i]->symbol, leaves[i]->code));
    }

    return codes;
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
