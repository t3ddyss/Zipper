#include <iostream>
#include <vector>
#include <ios>
#include <fstream>
#include "Huffman.h"
#include "LZ77.h"
#include "ReadWriter.h"
#include <time.h>

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios;

typedef unsigned char byte;

ReadWriter* readWriter;
Huffman* huffman;
LZ77* lz77;

enum BUFFER_SIZE
{
    KB_5, KB_10, KB_20
};

string folder = "./DATA/";

void EncodeHuffman(const string& filename, const string& format);
void DecodeHuffman(const string& filename, const string& format);
void EncodeLZ77(const string& filename, const string& format, BUFFER_SIZE bs);
void DecodeLZ77(const string& filename, const string& format, BUFFER_SIZE bs);

int main(int argc, char *argv[])
{
    readWriter = new ReadWriter();
    huffman = new Huffman();
    lz77 = new LZ77();

    string filename = "book";
    string format = ".doc";


    for (int i = 0; i < 10; i++)
    {
        double start = (double) (clock());

//        EncodeHuffman(filename, format);
//        DecodeHuffman(filename, format);
//
//
//        EncodeLZ77(filename, format, KB_5);
        DecodeLZ77(filename, format, KB_5);

        double end = (double) (clock());

        std::cout << "File was written, process took " << end - start << " ms" << std::endl;
    }

    return 0;
}


void EncodeHuffman(const string& filename, const string& format)
{
    vector<byte> bytes = readWriter->ReadAllBytes(folder + "Original/" + filename + format);

    vector<byte> result = huffman->Encode(bytes);

    readWriter->WriteAllBytes(folder + "Compressed/" + filename + ".huff", result);
}

void DecodeHuffman(const string& filename, const string& format)
{
    vector<byte> bytes = readWriter->ReadAllBytes(folder + "Compressed/" + filename + ".huff");

    vector<byte> result = huffman->Decode(bytes);

    readWriter->WriteAllBytes(folder + "Decompressed/" + filename + format, result);
}

void EncodeLZ77(const string& filename, const string& format, BUFFER_SIZE bs)
{
    vector<byte> bytes = readWriter->ReadAllBytes(folder + "Original/" + filename + format);

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

    vector<byte> result = lz77->Encode(bytes, VOCAB_BUFFER_SIZE, PREVIEW_BUFFER_SIZE);

    readWriter->WriteAllBytes(folder + "Compressed/" + filename + extension, result);
}

void DecodeLZ77(const string& filename, const string& format, BUFFER_SIZE bs)
{
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

    vector<byte> bytes = readWriter->ReadAllBytes(folder + "Compressed/" + filename + extension);

    vector<byte> result = lz77->Decode(bytes);

    readWriter->WriteAllBytes(folder + "Decompressed/" + filename + format, result);
}

