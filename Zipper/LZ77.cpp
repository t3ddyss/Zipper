//
// Created by Fedor on 31.03.2020.
//

#include "LZ77.h"

LZ77::Node *LZ77::findMatching(const string& vocab_buffer, int vocab_buffer_end, string preview_buffer,
                               int preview_buffer_end)
{
    unsigned short position = 0, length = 0;
    unsigned short buffer_pos = 0;

    string buffer_reversed = vocab_buffer.substr(0, vocab_buffer_end + 1);
    string buffer ="";

    for (size_t i = 0; i < buffer_reversed.length(); i++)
    {
        buffer += buffer_reversed[buffer_reversed.length() - i - 1];
    }

    buffer += preview_buffer.substr(0, preview_buffer_end + 1);

    for (int i = 1; i <= preview_buffer_end + 1; i++)
    {
        buffer_pos = buffer.find(preview_buffer.substr(0, i));

        if (buffer_pos == vocab_buffer_end + 1) break;

        else
        {
            position = vocab_buffer_end - buffer_pos + 1;
            length = i;
        }
    }


    byte symbol;

    if (position + length == 0)
    {
        symbol = preview_buffer[0];
    }

    else
    {
        symbol = buffer[buffer_pos + length];
    }

    Node* node = new Node(position, length, symbol);

    return node;
}

vector<byte> LZ77::Encode(vector<byte> input, int VOCAB_BUFFER_SIZE, int PREVIEW_BUFFER_SIZE)
{
    string vocab_buffer;
    string preview_buffer;

    int vocab_buffer_end = -1;
    int preview_buffer_end = min(PREVIEW_BUFFER_SIZE, (int) input.size()) - 1;

    vector<Node*> nodes;

    int input_pointer = input.size() > PREVIEW_BUFFER_SIZE ? PREVIEW_BUFFER_SIZE : -1;

    for (int i = 0; i <= preview_buffer_end; i++)
    {
        preview_buffer.push_back(input[i]);
    }

    while (preview_buffer_end >= 0)
    {
        nodes.push_back(findMatching(vocab_buffer, vocab_buffer_end, preview_buffer, preview_buffer_end));

        for (int i = 0; i < nodes.back()->length + 1; i++)
        {
            if (vocab_buffer_end >= VOCAB_BUFFER_SIZE - 1)
            {
                vocab_buffer.pop_back();
                vocab_buffer_end--;
            }

            vocab_buffer.insert(vocab_buffer.begin(), preview_buffer[i]);
            vocab_buffer_end++;
        }

        preview_buffer.erase(0, nodes.back()->length + 1);
        preview_buffer_end -= nodes.back()->length + 1;

        while (preview_buffer_end < PREVIEW_BUFFER_SIZE - 1 && input_pointer > 0 && input_pointer < input.size())
        {
            preview_buffer.push_back(input[input_pointer]);

            preview_buffer_end++;
            input_pointer++;
        }
    }

    vector<byte>output;
    LongShortByteUnion longShortByteUnion;

    output.push_back(input.size() % 10);

    for (size_t i = 0; i < nodes.size(); i++)
    {
        // Position, 2 bytes
        longShortByteUnion.uShort = nodes[i]->position;

        output.push_back(longShortByteUnion.uBytes[1]);
        output.push_back(longShortByteUnion.uBytes[0]);

        // Length, 2 bytes
        longShortByteUnion.uShort = nodes[i]->length;

        output.push_back(longShortByteUnion.uBytes[1]);
        output.push_back(longShortByteUnion.uBytes[0]);

        // Symbol, 1 byte
        output.push_back(nodes[i]->symbol);
    }

    return output;
}

vector<byte> LZ77::Decode(vector<byte> input)
{
    LongShortByteUnion longShortByteUnion;

    vector<Node*> nodes;
    byte fileSizeLastDigit = input[0];
    input.erase(input.begin());

    for (unsigned long long i = 0; i < input.size(); i += SYMBOL_DATA_SIZE)
    {
        longShortByteUnion.uBytes[1] = input[i];
        longShortByteUnion.uBytes[0] = input[i + 1];

        unsigned short position = longShortByteUnion.uShort;

        longShortByteUnion.uBytes[1] = input[i + 2];
        longShortByteUnion.uBytes[0] = input[i + 3];

        unsigned short length = longShortByteUnion.uShort;

        nodes.push_back(new Node(position, length, input[i + 4]));
    }

    string result = "";

    for (size_t i = 0; i < nodes.size(); i++)
    {
        int str_length = result.length();
        int index = str_length - nodes[i]->position;
        int counter = 0;

        while (counter < nodes[i]->length)
        {
            if (index >= str_length) index = str_length - nodes[i]->position;

            result += result[index];

            index++;
            counter++;
        }

        result += nodes[i]->symbol;
    }

    vector<byte> output(result.begin(), result.size() % 10 == (fileSizeLastDigit + 1) % 10 ?
                        result.end() - 1 : result.end());

    return output;
}
