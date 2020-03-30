#include <iostream>
#include <vector>
#include <ios>
#include <fstream>
#include "Huffman.h"

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

void Decode(const string& filename, const string& format)
{
    vector<byte> bytes = ReadAllBytes("./DATA/Compressed/" + filename + ".bin");

    Huffman* huffman = new Huffman();
    vector<byte> result = huffman->Decode(bytes);

    WriteAllBytes("./DATA/Decompressed/" + filename + format, result);
}

void Encode(const string& filename, const string& format)
{
    vector<byte> bytes = ReadAllBytes("./DATA/Original/" + filename + format);

    Huffman* huffman = new Huffman();
    vector<byte> result = huffman->Encode(bytes);

    WriteAllBytes("./DATA/Compressed/" + filename + ".bin", result);
}

int main(int argc, char *argv[])
{
    /*string filename = "./";
    filename += argv[1];*/

    //std::cout << bytes.size() << std::endl;
    //std::cout << "File was read!" << std::endl;


//    Encode("image", ".png");
//    Decode("image", ".png");


    std::cout << "File was written!" << std::endl;

    return 0;
}

