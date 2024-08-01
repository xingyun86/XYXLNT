#pragma once

#include <array>
#include <fstream>
#include <string>
#include <sstream>

#include <xyxlnt/utils/exceptions.hpp>
#include <xyxlnt/utils/path.hpp>

#define STRING_LITERAL2(a) #a
#define LSTRING_LITERAL2(a) L#a
#define U8STRING_LITERAL2(a) u8#a
#define STRING_LITERAL(a) STRING_LITERAL2(a)
#define LSTRING_LITERAL(a) STRING_LITERAL2(a)
#define U8STRING_LITERAL(a) STRING_LITERAL2(a)

#ifndef XYXLNT_BENCHMARK_DATA_DIR
#define XYXLNT_BENCHMARK_DATA_DIR ""
#endif

#ifndef XYXLNT_SAMPLE_DATA_DIR
#define XYXLNT_SAMPLE_DATA_DIR ""
#endif

class path_helper
{
public:
    static xyxlnt::path test_data_directory(const std::string &append = "")
    {
        static const std::string data_dir = STRING_LITERAL(XYXLNT_TEST_DATA_DIR);
        return xyxlnt::path(data_dir);
    }

    static xyxlnt::path test_file(const std::string &filename)
    {
        return test_data_directory().append(xyxlnt::path(filename));
    }

    static xyxlnt::path benchmark_data_directory(const std::string &append = "")
    {
        static const std::string data_dir = STRING_LITERAL(XYXLNT_BENCHMARK_DATA_DIR);
        return xyxlnt::path(data_dir);
    }

    static xyxlnt::path benchmark_file(const std::string &filename)
    {
        return benchmark_data_directory().append(xyxlnt::path(filename));
    }

    static xyxlnt::path sample_data_directory(const std::string &append = "")
    {
        static const std::string data_dir = STRING_LITERAL(XYXLNT_SAMPLE_DATA_DIR);
        return xyxlnt::path(data_dir);
    }

    static xyxlnt::path sample_file(const std::string &filename)
    {
        return sample_data_directory().append(xyxlnt::path(filename));
    }

    static void copy_file(const xyxlnt::path &source, const xyxlnt::path &destination, bool overwrite)
    {
        if(!overwrite && destination.exists())
        {
            throw xyxlnt::exception("destination file already exists and overwrite==false");
        }

        std::ifstream src(source.string(), std::ios::binary);
        std::ofstream dst(destination.string(), std::ios::binary);

        dst << src.rdbuf();
    }

    static void delete_file(const xyxlnt::path &path)
    {
      std::remove(path.string().c_str());
    }
};
