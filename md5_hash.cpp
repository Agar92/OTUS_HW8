#include <boost/uuid/detail/md5.hpp>
#include <md5_hash.h>

using namespace boost::uuids::detail;

std::vector<uint8_t> Md5Hash::hash_block(std::vector<uint8_t> block)
{
    md5 hash;
    md5::digest_type digest;
    hash.process_bytes(block.data(), block.size());
    hash.get_digest(digest);
    uint8_t* byte_array = reinterpret_cast<uint8_t*>(digest);
    return std::vector<uint8_t>(byte_array, byte_array + sizeof(digest));
}


