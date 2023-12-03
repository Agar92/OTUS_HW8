#pragma once

#include <hash.h>

class Md5Hash: public Hash
{
public:
    std::vector<uint8_t> hash_block(std::vector<uint8_t> block) override;
};
