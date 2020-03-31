//
// Created by Fedor on 31.03.2020.
//

#include <iostream>
#include <vector>
#include <ios>
#include <fstream>

#ifndef ZIPPER_READWRITER_H
#define ZIPPER_READWRITER_H

typedef unsigned char byte;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::ios;

class ReadWriter
{
public:
    vector<byte> ReadAllBytes(const string& filename);

    void WriteAllBytes(const string& filename, vector<byte> bytes);
};


#endif //ZIPPER_READWRITER_H
