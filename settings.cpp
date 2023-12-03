#include <settings.h>

#include <boost/any.hpp>
#include <sstream>
#include <iostream>

using namespace std;

std::string CMDSettings::CRC32_STR = std::string("crc32");
std::string CMDSettings::MD5_STR = std::string("md5");

CMDSettings CMDSettings::parse_from_arguments(int argc, const char *argv[])
{
    CMDSettings settings;
    po::options_description desc("Utility to search duplicate files. Command line options:");
    desc.add_options()
        ("help,h", "help message")
        ("include-dirs,i", 
            po::value<vector<string>>()->default_value(vector<string>{"."}, ".")->multitoken()->composing(),
            "Scanned directories, can be multiple. Default is current folder.")
        ("exclude-dirs,e",
            po::value<vector<string>>()->multitoken()->composing(),
            "Excluded directories, can be multiple.")
        ("depth,d", 
            po::value<unsigned short>()->default_value(0)->notifier([](const unsigned short& value)
            {
                auto supported_depths = 
                {
                    static_cast<unsigned short>(ScanDepth::SCAN_CURRENT),
                    static_cast<unsigned short>(ScanDepth::SCAN_ALL)
                };
                check_value_is_supported(value, supported_depths, "depth");
            }),
            "Scan depth, 1 - all directories, 0 - current folder only. Default value is 0.")
        ("min_size,m",
            po::value<int>()->default_value(1)->notifier([](const int& value)
            {
                check_value_is_non_negative(value, "min_size");
            }), 
            "Minimum size of the processed file in bytes. Default value is 1.")
        ("file_masks,f",
            po::value<vector<string>>()->multitoken()->composing(), 
            "Masks for the files participating in comparison, case-insensitive.")
        ("block_size,b",
            po::value<int>()->default_value(256)->notifier([](const int& value)
            {
                check_value_is_non_negative(value, "block_size");
            }), 
            "The size in bytes of the block to read the files with. Default value is 256.")
        ("algorithm,a",
            po::value<string>()->default_value("crc32")->notifier([](const string& value)
            {
                check_value_is_supported(value, {CRC32_STR, MD5_STR}, "algorithm");
            }),
            "Hashing algorithm to hash file blocks. Default value is crc32.")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    notify(vm);

    stringstream help;
    help << desc << endl;
    settings.mHelpText = help.str();
    settings.mShowHelp = vm.count("help");
    settings.mIncudeDirs = vm["include-dirs"].as<vector<string>>();
    if (vm.count("exclude-dirs"))
    {
        settings.mExcludeDirs = vm["exclude-dirs"].as<vector<string>>();
    }
    settings.mScanDepth = static_cast<ScanDepth>(vm["depth"].as<unsigned short>());
    settings.mMinSize = static_cast<unsigned int>(vm["min_size"].as<int>());
    if (vm.count("file_masks"))
    {
        settings.mFileMasks = vm["file_masks"].as<vector<string>>();
        std::cout<<"File masks size="<<settings.mFileMasks.size()<<" mask="<<settings.mFileMasks[0]<<std::endl;
    }
    settings.mBlockSize = static_cast<unsigned int>(vm["block_size"].as<int>());
    settings.mHashAlgorithm = vm["algorithm"].as<string>();

    return settings;
}



