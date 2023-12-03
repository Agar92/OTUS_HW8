#pragma once

#include <hash.h>

class Crc32Hash: public Hash
{
public:
    std::vector<uint8_t> hash_block(std::vector<uint8_t> block) override;
};
