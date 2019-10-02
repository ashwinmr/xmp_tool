#include "args.hpp"
#include "db_tools.hpp"
#include "xmp_tools.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, const char *argv[]) {

    OpenDB();

    // Parse args
    Args args(argc, argv);
    if (!args.valid) {
        return 0;
    }

    // Store path
    std::string path = args.path;

    // Check if path exists
    if (fs::exists(path)) {
        if (fs::is_regular_file(path)) {
            for (auto &tag : GetFileTags(path)) {
                std::cout << tag << std::endl;
            }
        } else if (fs::is_directory(path)) {
            std::vector<std::string> paths = GetFilePaths(path, true);
            for (auto &path : paths) {
                std::vector<std::string> tags = GetFileTags(path);
                if (tags.empty()) {
                    continue;
                }
                for (auto &tag : tags) {
                    std::cout << tag << "\t";
                }
                std::cout << std::endl;
            }
        } else {
            std::cout << "Input path exists, but is not a file or directory" << std::endl;
        }
    } else {
        std::cout << "Input path does not exist" << std::endl;
    }
    return 0;
}
