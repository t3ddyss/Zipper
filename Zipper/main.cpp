#include <iostream>
#include <vector>
#include <ios>
#include <fstream>
#include "Huffman.h"
#include "LZ77.h"

using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios;

typedef unsigned char byte;

vector<byte> ReadAllBytes(const string& filename)
{
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    vector<byte> bytes(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(reinterpret_cast<char *>(bytes.data()), pos);

    return bytes;
}

void WriteAllBytes(const string& filename, vector<byte> bytes)
{
    ofstream ofs(filename, ios::out | ios::binary);
    ofs.write(reinterpret_cast<const char *>(bytes.data()), bytes.size() * sizeof(byte));
}

void DecodeHuffman(const string& filename, const string& format)
{
    vector<byte> bytes = ReadAllBytes("./DATA/Compressed/" + filename + ".bin");

    Huffman* huffman = new Huffman();
    vector<byte> result = huffman->Decode(bytes);

    WriteAllBytes("./DATA/Decompressed/" + filename + format, result);
}

void EncodeHuffman(const string& filename, const string& format)
{
    vector<byte> bytes = ReadAllBytes("./DATA/Original/" + filename + format);

    Huffman* huffman = new Huffman();
    vector<byte> result = huffman->Encode(bytes);

    WriteAllBytes("./DATA/Compressed/" + filename + ".huff", result);
}

void EncodeLZ77(const string& filename, const string& format)
{
    vector<byte> bytes = ReadAllBytes("./DATA/Original/" + filename + format);

    LZ77* lz77 = new LZ77();
    vector<byte> result = lz77->Encode(bytes, LZ77::KB_16, LZ77::KB_4);

    WriteAllBytes("./DATA/Compressed/" + filename + ".lz77", result);
}

void DecodeLZ77(const string& filename, const string& format)
{
    vector<byte> bytes = ReadAllBytes("./DATA/Compressed/" + filename + ".lz77");

    LZ77* lz77 = new LZ77();
    vector<byte> result = lz77->Decode(bytes);

    WriteAllBytes("./DATA/Decompressed/" + filename + format, result);
}

int main(int argc, char *argv[])
{
    //std::cout << bytes.size() << std::endl;
    //std::cout << "File was read!" << std::endl;

    string filename = "book";
    string format = ".doc";


//    EncodeHuffman(filaname, format);
//    DecodeHuffman(filename, format);

//    EncodeLZ77(filename, format);
//    DecodeLZ77(filename, format);

    std::cout << "File was written!" << std::endl;

    return 0;
}

