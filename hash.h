#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class Hash
{
public:
    virtual std::vector<uint8_t> hash_block(std::vector<uint8_t> block) = 0;
};
