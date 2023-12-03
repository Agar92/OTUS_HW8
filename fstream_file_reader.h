#pragma once

#include <fstream>
#include <string>

#include <file_reader.h>

class IfstreamFileReader : public FileReader
{
public:
    IfstreamFileReader(std::string path, size_t block_size) : 
                       mFileStream(path), mBlockSize(block_size) {}

    size_t maximum_block_size() const override {return mBlockSize;}

    bool is_EOF() const override {return mFileStream.eof();}

    size_t read_block(std::vector<uint8_t>& buffer) override
    {
        mFileStream.read((char*)buffer.data(), mBlockSize);
        return (size_t)mFileStream.gcount();
    }

private:
    std::ifstream mFileStream;
    size_t mBlockSize; 
};
