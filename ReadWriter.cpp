//
// Created by Fedor on 31.03.2020.
//

#include "ReadWriter.h"

vector<byte> ReadWriter::ReadAllBytes(const string &filename)
{
    ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    vector<byte> bytes(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(reinterpret_cast<char *>(bytes.data()), pos);
    ifs.close();

    return bytes;
}

void ReadWriter::WriteAllBytes(const string &filename, vector<byte> bytes)
{
    ofstream ofs(filename, ios::out | ios::binary);
    ofs.write(reinterpret_cast<const char *>(bytes.data()), bytes.size() * sizeof(byte));
    ofs.close();
}
