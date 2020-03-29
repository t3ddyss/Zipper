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

void WriteAllBytes(vector<byte> bytes)
{
    ofstream ofs("output.bin", ios::out | ios::binary);
    ofs.write(reinterpret_cast<const char *>(bytes.data()), bytes.size() * sizeof(byte));
}

int main(int argc, char *argv[])
{
    string filename = "./";
    filename += argv[1];

    vector<byte> bytes = ReadAllBytes(filename);

    std::cout << bytes.size() << std::endl;
    std::cout << "File was read!" << std::endl;

    Huffman* huffman = new Huffman();
    vector<byte> result = huffman->Encode(bytes);

    WriteAllBytes(result);
    std::cout << "File was written!" << std::endl;

    //std::cout << "Hello, World!" << std::endl;
    return 0;
}

