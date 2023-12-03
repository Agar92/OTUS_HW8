#pragma once

#include <scan_depth.h>

#include <array>
#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class CMDSettings
{
public:
    static CMDSettings parse_from_arguments(int argc, const char *argv[]);
    
    bool mShowHelp;
    std::string mHelpText;
    std::vector<std::string> mIncudeDirs;
    std::vector<std::string> mExcludeDirs;
    ScanDepth mScanDepth;
    unsigned int mMinSize;
    std::vector<std::string> mFileMasks;
    unsigned int mBlockSize;
    std::string mHashAlgorithm;

    static std::string CRC32_STR;
    static std::string MD5_STR;
    
private:
    CMDSettings(){};

    template <typename T>
    static void check_value_is_supported(const T& value, const std::initializer_list<T>& supported_values, std::string option_name)
    {
        for(const auto& supported : supported_values) if (value == supported) return;
        throw po::validation_error(po::validation_error::invalid_option_value, option_name);
    }

    template <typename T>
    static void check_value_is_non_negative(const T& value, std::string option_name)
    {
        if(value < 0) throw po::validation_error(po::validation_error::invalid_option_value, option_name);
    }
};
