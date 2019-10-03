#include "args.hpp"
#include "db_tools.hpp"
#include "xmp_tools.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, const char *argv[]) {

    // Parse args
    Args args(argc, argv);
    if (!args.valid) {
        return 0;
    }

    // Store path
    std::string path = args.path;
    std::string db_path = args.db_path;

    // Check if path exists
    if (fs::exists(path)) {
        if (fs::is_regular_file(path)) {
            std::vector<std::string> paths;
            paths.push_back(path);
            GetAndStoreTags(paths,db_path);
        } else if (fs::is_directory(path)) {
            std::vector<std::string> paths = GetFilePaths(path, true);
            GetAndStoreTags(paths,db_path);
        } else {
            std::cout << "Input path exists, but is not a file or directory" << std::endl;
        }
    } else {
        std::cout << "Input path does not exist" << std::endl;
    }
    return 0;
}
