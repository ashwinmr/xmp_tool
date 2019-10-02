#include "args.hpp"
#include "xmp_tools.hpp"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

using namespace std;

int main(int argc, const char *argv[]) {
    // Parse args
    Args args(argc,argv);
    if (!args.valid) {
        return 0;
    }

    // Store path
    string path = args.path;

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
