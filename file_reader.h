#pragma once

#include <cstddef>//size_t
#include <cstdint>//uint8_t
#include <vector>

class FileReader
{
public:
    virtual size_t maximum_block_size() const = 0;
    virtual bool is_EOF() const = 0;
    virtual size_t read_block(std::vector<uint8_t>& buffer) = 0;
};
