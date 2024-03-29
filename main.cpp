#include <iostream>
#include <vector>
#include <ios>
#include "Huffman.h"
#include "LZ77.h"
#include "ReadWriter.h"
#include <ctime>
#include <cmath>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::pair;
using std::make_pair;

typedef unsigned char byte;

enum BUFFER_SIZE
{
    KB_5, KB_10, KB_20, END
};

const int ITERATIONS_COUNT = 10;
string folder;
vector<pair<string, string>> files;

pair<double, double> EncodeHuffman(const string& filename);
double DecodeHuffman(const string& filename);
pair<double, double> EncodeLZ77(const string& filename, BUFFER_SIZE bs);
double DecodeLZ77(const string& filename, BUFFER_SIZE bs);

void HuffmanExperiment();
void LZ77Experiment();

void calculateEntropy();
void setFiles();

int main(int argc, char *argv[])
{
    folder = "./DATA/";
    setFiles();

//    calculateEntropy();

//    HuffmanExperiment();
//    LZ77Experiment();

    return 0;
}

void HuffmanExperiment()
{
    ofstream fout1(folder + "HuffmanCompression.csv");
    fout1 << "File; Average time; Compression ratio;" << std::endl;

    for (size_t i = 0; i < files.size(); i++)
    {
        double averageTime = 0;
        double compressionRatio = 0;

        fout1 << files[i].first + files[i].second << ';';

        for (int j = 0; j < ITERATIONS_COUNT + 1; j++)
        {
            pair<double, double> info = EncodeHuffman(files[i].first + files[i].second);

            if (j == 0) continue;

            averageTime += info.first;
            compressionRatio = info.second;
        }

        fout1 << averageTime / ITERATIONS_COUNT << ';' << compressionRatio << ';' << std::endl;

        std::cout << files[i].first << " compression completed!" << std::endl;
    }

    fout1.close();

    ofstream fout2(folder + "HuffmanDecompression.csv");
    fout2 << "File; Average time;" << std::endl;

    for (size_t i = 0; i < files.size(); i++)
    {
        double averageTime = 0;

        fout2 << files[i].first + files[i].second << ';';

        for (int j = 0; j < ITERATIONS_COUNT + 1; j++)
        {
            double info = DecodeHuffman(files[i].first);

            if (j == 0) continue;

            averageTime += info;
        }

        fout2 << averageTime / ITERATIONS_COUNT << ';' << std::endl;

        std::cout << files[i].first << " decompression completed!" << std::endl;
    }

    fout2.close();
}

void LZ77Experiment()
{

    for (int BUFFER = KB_5; BUFFER != END; BUFFER++)
    {
        BUFFER_SIZE bufferSize = static_cast<BUFFER_SIZE>(BUFFER);
        string bs = "5";

        switch(bufferSize)
        {
            case KB_5:
                bs = "5";
                break;
            case KB_10:
                bs = "10";
                break;
            case KB_20:
                bs = "20";
                break;
            case END:
                break;
        }

        ofstream fout1(folder + "LZ77" + bs + "Compression.csv");
        fout1 << "File; Average time; Compression ratio;" << std::endl;

        for (size_t i = 0; i < files.size(); i++)
        {

            double averageTime = 0;
            double compressionRatio = 0;

            fout1 << files[i].first + files[i].second << ';';

            for (int j = 0; j < ITERATIONS_COUNT + 1; j++)
            {
                pair<double, double> info = EncodeLZ77(files[i].first + files[i].second, bufferSize);

                if (j == 0) continue;

                averageTime += info.first;
                compressionRatio = info.second;
            }

            fout1 << averageTime / ITERATIONS_COUNT << ';' << compressionRatio << ';' << std::endl;

            std::cout << files[i].first << " compression completed!" << std::endl;
        }

        fout1.close();

        ofstream fout2(folder + "LZ77" + bs + "Decompression.csv");
        fout2 << "File; Average time;" << std::endl;

        for (size_t i = 0; i < files.size(); i++)
        {
            double averageTime = 0;

            fout2 << files[i].first + files[i].second << ';';

            for (int j = 0; j < ITERATIONS_COUNT + 1; j++)
            {
                double info = DecodeLZ77(files[i].first, bufferSize);

                if (j == 0) continue;

                averageTime += info;
            }

            fout2 << averageTime / ITERATIONS_COUNT << ';' << std::endl;

            std::cout << files[i].first << " " + bs + "" <<" decompression completed!" << std::endl;
        }

        fout2.close();
    }
}

pair<double, double> EncodeHuffman(const string& filename)
{
    ReadWriter readWriter;
    Huffman huffman;

    vector<byte> input = readWriter.ReadAllBytes(folder + "Original/" + filename);

    double start = (double) (clock());
    vector<byte> output = huffman.Encode(input);
    double finish = (double) (clock());

    readWriter.WriteAllBytes(folder + "Compressed/" +
    filename.substr(0, filename.find('.'))  + ".huff", output);

    return make_pair(finish - start, output.size() * 1.0 / input.size());
}

double DecodeHuffman(const string& filename)
{
    ReadWriter readWriter;
    Huffman huffman;

    vector<byte> input = readWriter.ReadAllBytes(folder + "Compressed/" + filename + ".huff");

    double start = (double) (clock());
    vector<byte> output = huffman.Decode(input);
    double finish = (double) (clock());

    readWriter.WriteAllBytes(folder + "Decompressed/" + filename + ".unhuff", output);

    return finish - start;
}

pair<double, double> EncodeLZ77(const string& filename, BUFFER_SIZE bs)
{
    ReadWriter readWriter;
    LZ77 lz77;

    vector<byte> input = readWriter.ReadAllBytes(folder + "Original/" + filename);

    int VOCAB_BUFFER_SIZE = LZ77::KB_4, PREVIEW_BUFFER_SIZE = LZ77::KB_1;
    string extension = ".lz775";

    switch(bs)
    {
        case KB_5:
            VOCAB_BUFFER_SIZE = LZ77::KB_4;
            PREVIEW_BUFFER_SIZE = LZ77::KB_1;
            extension = ".lz775";
            break;
        case KB_10:
            VOCAB_BUFFER_SIZE = LZ77::KB_8;
            PREVIEW_BUFFER_SIZE = LZ77::KB_2;
            extension = ".lz7710";
            break;
        case KB_20:
            VOCAB_BUFFER_SIZE = LZ77::KB_16;
            PREVIEW_BUFFER_SIZE = LZ77::KB_4;
            extension = ".lz7720";
            break;
    }

    double start = (double) (clock());
    vector<byte> output = lz77.Encode(input, VOCAB_BUFFER_SIZE, PREVIEW_BUFFER_SIZE);
    double finish = (double) (clock());

    readWriter.WriteAllBytes(folder + "Compressed/" +
                             filename.substr(0, filename.find('.')) + extension, output);

    return make_pair(finish - start, output.size() * 1.0 / input.size());
}

double DecodeLZ77(const string& filename, BUFFER_SIZE bs)
{
    ReadWriter readWriter;
    LZ77 lz77;

    string extension = ".lz775";

    switch(bs)
    {
        case KB_5:
            extension = ".lz775";
            break;
        case KB_10:
            extension = ".lz7710";
            break;
        case KB_20:
            extension = ".lz7720";
            break;
    }

    vector<byte> input = readWriter.ReadAllBytes(folder + "Compressed/" + filename + extension);

    double start = (double) (clock());
    vector<byte> output = lz77.Decode(input);
    double finish = (double) (clock());

    readWriter.WriteAllBytes(folder + "Decompressed/"
    + filename + extension.insert(1, "un"), output);

    return finish - start;
}

void calculateEntropy()
{
    ReadWriter readWriter;

    ofstream fout(folder + "entropy.csv");
    fout << "File;";

    for (int i = 1; i <= 9; i++)
    {
        fout << i << ';';
    }

    fout << std::endl;

    vector<map<byte, double>> maps;

    for (size_t i = 0; i < files.size(); i++)
    {
        vector<byte> input = readWriter.ReadAllBytes(folder + "Original/" + files[i].first + files[i].second);

        map<byte, double> symbolsTable;
        map<byte, double>::iterator it;
        size_t size = input.size();

        for (size_t j = 0; j < size; j++)
        {
            it = symbolsTable.find(input[j]);

            if (it == symbolsTable.end())
            {
                symbolsTable.insert(std::make_pair(input[j], 1.0 / size));
            }

            else
            {
                it->second += 1.0 / size;
            }
        }

        maps.push_back(symbolsTable);
    }

    for (size_t i = 0; i < 256; i++)
    {
        fout << i << ';';

        for (size_t j = 0; j < maps.size(); j++)
        {

            map<byte, double>::iterator it = maps[j].find((byte) i);

            if (it != maps[j].end())
            {
                fout << it->second << ';';
            }

            else
            {
                fout << 0 << ';';
            }
        }

        fout << std::endl;
    }

    fout << "H;";

    for (size_t j = 0; j < maps.size(); j++)
    {
        map<byte, double>::iterator it;

        double entropy = 0;

        for (it = maps[j].begin(); it != maps[j].end(); it++)
        {
            entropy -= (it->second * std::log2(it->second));
        }

        entropy /= 8;

        fout << entropy << ';';
    }

    fout << std::endl;
    fout.close();
}

void setFiles()
{
    files.emplace_back("1", ".txt");
    files.emplace_back("2", ".doc");
    files.emplace_back("3", ".ppt");
    files.emplace_back("4", ".pdf");
    files.emplace_back("5", ".dll");
    files.emplace_back("6", ".jpg");
    files.emplace_back("7", ".jpg");
    files.emplace_back("8", ".bmp");
    files.emplace_back("9", ".bmp");
}

