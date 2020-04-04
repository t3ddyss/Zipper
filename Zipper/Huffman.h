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
    /**
     * Метод кодирования исходного файла
     * @param input последовательность байт исходного файла
     * @return последовательность байт закодированного файла
     */
    vector<byte> Encode(vector<byte> input);

    /**
     * Метод декодирования ранее закодированного файла
     * @param input последовательность байт ранее закодированного файла
     * @return последовательность байт декодированного файла
     */
    vector<byte> Decode(vector<byte> input);

private:
    class HuffmanTree
    {
    public:
        HuffmanTree();

        ~HuffmanTree();

        /**
         * Метод построения кодового дерева
         */
        void buildTree();

        /**
         * Фактически декодирует закодированную строку
         * @param code закодированная строка
         * @param lastByteUsedBitsCount число используемых битов в последнем байте
         * @return последовательность байт декодированного файла
         */
        vector<byte> findSymbols(string code, byte lastByteUsedBitsCount);

    private:
        struct Node
        {
            byte symbol;
            string code;
            double probability;
            struct Node *left;
            struct Node *right;

            explicit Node(double probability, byte symbol);
        };

        struct NodesComparator
        {
            inline bool operator() (const Node* node1, const Node* node2)
            {
                return (node1->probability > node2->probability);
            }
        };

        /**
         * Проверяет, является ли узел листом
         * @param node узел
         * @return да или нет
         */
        bool isLeaf(Node* node);

        /**
         * Удаляет поддерево
         * @param root корень поддерева
         */
        void deleteTree(Node* root);

        /**
         * Проходится по поддереву от корня до листьев и сохраняет в них код
         * @param root корень поддерева
         */
        void createCodes(Node* root);

        vector<Node*> nodes; // node[0] -- tree root after buildTree() execution

        Node* node;

    protected:
        vector<Node*> leaves;
    };

    /**
     * Составляет таблицу вероятности появления символа (байта) в исходном файле
     */
    void buildProbabilityTable();

    static const int SYMBOL_DATA_SIZE = 9;
    vector<byte> bytes;
    static map<byte, tuple<string, double>> symbolsTable; //<byte, <code, probability>>

};

union DoubleLongShortByteUnion
{
    double Double;
    unsigned short uShort;
    byte uBytes[8];
};


#endif //ZIPPER_HUFFMAN_H
