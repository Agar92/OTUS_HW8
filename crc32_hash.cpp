#include <boost/crc.hpp> 
#include <crc32_hash.h>

using namespace boost;

std::vector<uint8_t> Crc32Hash::hash_block(std::vector<uint8_t> block)
{
    crc_32_type crc32;
    crc32.process_bytes(block.data(), block.size());
    unsigned int checksum = crc32.checksum();
    uint8_t * byte_array = reinterpret_cast<uint8_t *>(&checksum);
    return std::vector<uint8_t>(byte_array, byte_array + sizeof(checksum));
}