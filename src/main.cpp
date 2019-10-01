#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include "xmp_tools.hpp"

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

/**
 * Parse command line arguments
 */
bool ParseArgs(int argc, const char **argv, po::variables_map &args) {
    try {
        // Declare supported options
        po::options_description desc("Allowed options:");
        desc.add_options()("help,h", "Help message")("path,p", po::value<std::string>()->required(), "Path to file or directory");

        // Make options positional
        po::positional_options_description desc_p;
        desc_p.add("path", 1);

        // Store options
        po::store(po::command_line_parser(argc, argv).options(desc).positional(desc_p).run(), args);

        // Handle help before checking for errors
        if (args.count("help") || (argc < 2)) {
            std::cout << desc << std::endl;
            return false;
        }

        // Check inputs for errors
        po::notify(args);

        // Parsing successful
        return true;
    } catch (po::error &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return false;
    }
}

int main(int argc, const char *argv[]) {
    // Parse args
    po::variables_map args;
    if (!ParseArgs(argc, argv, args)) {
        return 0;
    }

    // Store path
    string path = args["path"].as<string>();

    // Check if path is a file
    if (!fs::is_directory(path)) {
        for (auto &tag : GetTags(path)) {
            cout << tag << endl;
        }
    } else {
        for (fs::directory_entry &de : fs::directory_iterator(path)) {
            vector<string> tags = GetTags(de.path().string());
            if(tags.empty()){
                continue;
            }
            for (auto &tag : tags) {
                cout << tag << "\t";
            }
            cout << endl;
        }
    }

    return 0;
}
