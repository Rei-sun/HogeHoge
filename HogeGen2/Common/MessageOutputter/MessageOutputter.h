#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace HogeGen2 {

class MessageOutputter {
    std::ofstream *ofs;
    std::ostream *output_stream;

public:
    MessageOutputter(std::string output_path):
        ofs(nullptr)
    {
        ofs = new std::ofstream(output_path);
        output_stream = ofs->fail() ? &std::cout : ofs;
    }

    MessageOutputter(std::ostream *output):
        ofs(nullptr),
        output_stream(output)
    {

    }

    virtual ~MessageOutputter() {
        if (ofs != nullptr) {
            ofs->close();
        }
    }

    void Message(std::string message) {
        std::stringstream ss;
        ss << message << "\n";
        (*output_stream) << ss.str();
    }
};

}
