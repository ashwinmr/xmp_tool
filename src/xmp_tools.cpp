#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>

// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string

// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1

// Ensure XMP templates are instantiated
#include "XMP.incl_cpp"

// Provide access to the API
#include "XMP.hpp"

namespace fs = boost::filesystem;

/**
 * Get tags from a file
 */
std::vector<std::string> GetTags(std::string full_file_path) {
    std::vector<std::string> tags;

    if (!SXMPMeta::Initialize()) {
        std::cout << "Could not initialize toolkit!";
        return tags;
    }
    XMP_OptionBits options = 0;
#if UNIX_ENV
    options |= kXMPFiles_ServerMode;
#endif
    // Must initialize SXMPFiles before we use it
    if (!SXMPFiles::Initialize(options)) {
        std::cout << "Could not initialize SXMPFiles.";
        return tags;
    }

    try {
        // Options to open the file with - read only and use a file handler
        XMP_OptionBits opts = kXMPFiles_OpenForRead | kXMPFiles_OpenUseSmartHandler;

        bool ok;
        SXMPFiles myFile;
        std::string status = "";

        // First we try and open the file
        ok = myFile.OpenFile(full_file_path, kXMP_UnknownFile, opts);
        if (!ok) {
            status += "No smart handler available for " + full_file_path + "\n";
            status += "Trying packet scanning.\n";

            // Now try using packet scanning
            opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
            ok = myFile.OpenFile(full_file_path, kXMP_UnknownFile, opts);
        }

        // If the file is open then read the metadata
        if (ok) {
            // Create the xmp object and get the xmp data
            SXMPMeta meta;
            myFile.GetXMP(&meta);

            // Get the the entire dc:subject array
            int n_tags = meta.CountArrayItems(kXMP_NS_DC, "subject");
            for (int i = 1; i <= n_tags; i++) {
                std::string tag;
                meta.GetArrayItem(kXMP_NS_DC, "subject", i, &tag, 0);
                tags.push_back(tag);
            }

            // Close the SXMPFile.  The resource file is already closed if it was
            // opened as read only but this call must still be made.
            myFile.CloseFile();
        } else {
            std::cout << "Unable to open " << full_file_path << std::endl;
        }
    } catch (XMP_Error &e) {
        std::cout << "ERROR: " << e.GetErrMsg() << std::endl;
    }

    // Terminate the toolkit
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();

    return tags;
}

/**
 * Get all file paths within a directory
 */
std::vector<std::string> GetFilePaths(std::string dir_path) {
    std::vector<std::string> paths;

    // Check if path is a directory
    if (!fs::is_directory(dir_path)) {
        std::cout << "Input path is not directory" << std::endl;
        return paths;
    }

    for (fs::directory_entry &de : fs::directory_iterator(dir_path)) {
        if(fs::is_regular_file(de.path())){
            paths.push_back(de.path().string());
        }
    }

    return paths;
}
