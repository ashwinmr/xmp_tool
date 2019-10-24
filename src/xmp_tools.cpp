#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "db_tools.hpp"

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
std::vector<std::string> GetTagsFromFile(std::string full_file_path) {
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
        std::cout << "Error opening " + full_file_path + ":\n\t" << e.GetErrMsg() << std::endl;
    }

    // Terminate the toolkit
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();

    return tags;
}

/**
 * Add tags to file
 */
bool AddTagsToFile(std::string full_file_path, std::vector<std::string> &tags) {
    bool success = false;

    // If not tags provided return success
    if (tags.empty()) {
        success = true;
        return success;
    }

    if (!SXMPMeta::Initialize()) {
        std::cout << "Could not initialize toolkit!";
        return success;
    }

    XMP_OptionBits options = 0;
#if UNIX_ENV
    options |= kXMPFiles_ServerMode;
#endif

    // Must initialize SXMPFiles before we use it
    if (!SXMPFiles::Initialize(options)) {
        std::cout << "Could not initialize SXMPFiles.";
        return success;
    }

    try {
        // Options to open the file with - open for editing and use a smart handler
        XMP_OptionBits opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler;

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

        // If the file is open then read get the XMP data
        if (ok) {
            // Create the XMP object and get the XMP data
            SXMPMeta meta;
            myFile.GetXMP(&meta);

            // Now modify the XMP
            for (auto &tag : tags) {
                // Note the options used, kXMP_PropArrayIsOrdered, if the array does not exist it will be created
                meta.AppendArrayItem(kXMP_NS_DC, "subject", kXMP_PropArrayIsOrdered, tag, 0);
            }

            // Check we can put the XMP packet back into the file
            if (myFile.CanPutXMP(meta)) {
                // If so then update the file with the modified XMP
                myFile.PutXMP(meta);

                success = true;
            }

            // Close the SXMPFile.  This *must* be called.  The XMP is not
            // actually written and the disk file is not closed until this call is made.
            myFile.CloseFile();
        } else {
            std::cout << "Unable to open " << full_file_path << std::endl;
        }
    } catch (XMP_Error &e) {
        std::cout << "Error opening " + full_file_path + ":\n\t" << e.GetErrMsg() << std::endl;
    }

    // Terminate the toolkit
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();

    return success;
}

/**
 * Add tags to files
 */
void AddTagsToFiles(std::vector<std::string>& paths, std::vector<std::string>& tags){

    for(auto& path: paths){
        if(!AddTagsToFile(path,tags)){
            std::cout << "Warning: Failed to add tags to " << path << std::endl;
        } 
    }
}

/**
 * Remove tags from file
 */
bool RemoveTagsFromFile(std::string full_file_path, std::vector<std::string> &tags, bool remove_all) {
    bool success = false;

    if (!SXMPMeta::Initialize()) {
        std::cout << "Could not initialize toolkit!";
        return success;
    }

    XMP_OptionBits options = 0;
#if UNIX_ENV
    options |= kXMPFiles_ServerMode;
#endif

    // Must initialize SXMPFiles before we use it
    if (!SXMPFiles::Initialize(options)) {
        std::cout << "Could not initialize SXMPFiles.";
        return success;
    }

    try {
        // Options to open the file with - open for editing and use a smart handler
        XMP_OptionBits opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUseSmartHandler;

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

        // If the file is open then read get the XMP data
        if (ok) {
            // Create the XMP object and get the XMP data
            SXMPMeta meta;
            myFile.GetXMP(&meta);

            // Now modify the XMP
            int n_tags = meta.CountArrayItems(kXMP_NS_DC, "subject");

            // Create tag set
            std::set<std::string> tag_set(tags.begin(),tags.end());

            // Create list of indexes to be removed
            std::vector<int> rem_list;

            // Get indexes for removal
            for (int i = 1; i <= n_tags; i++) {
                if(remove_all){
                    rem_list.push_back(i);
                }
                else{
                    std::string tag;
                    meta.GetArrayItem(kXMP_NS_DC, "subject", i, &tag, 0);

                    if(tag_set.find(tag) != tag_set.end()){
                        rem_list.push_back(i);
                    }
                }
            }

            // Remove tags
            for(auto& i: rem_list){
                meta.DeleteArrayItem(kXMP_NS_DC, "subject", i);
            }

            // Check we can put the XMP packet back into the file
            if (myFile.CanPutXMP(meta)) {
                // If so then update the file with the modified XMP
                myFile.PutXMP(meta);

                success = true;
            }

            // Close the SXMPFile.  This *must* be called.  The XMP is not
            // actually written and the disk file is not closed until this call is made.
            myFile.CloseFile();
        } else {
            std::cout << "Unable to open " << full_file_path << std::endl;
        }
    } catch (XMP_Error &e) {
        std::cout << "Error opening " + full_file_path + ":\n\t" << e.GetErrMsg() << std::endl;
    }

    // Terminate the toolkit
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();

    return success;
}

/**
 * Remove tags from files
 */
void RemoveTagsFromFiles(std::vector<std::string>& paths, std::vector<std::string>& tags, bool remove_all){

    for(auto& path: paths){
        if(!RemoveTagsFromFile(path,tags,remove_all)){
            std::cout << "Warning: Failed to remove tags from " << path << std::endl;
        } 
    }
}

/**
 * Get all file paths within a directory
 */
std::vector<std::string> GetFilePaths(std::string dir_path, bool recurse) {
    std::vector<std::string> paths;

    // Check if path is a directory
    if (!fs::is_directory(dir_path)) {
        std::cout << "Input path is not directory" << std::endl;
        return paths;
    }

    if (recurse) {
        for (fs::directory_entry &de : fs::recursive_directory_iterator(dir_path)) {
            if (fs::is_regular_file(de.path())) {
                paths.push_back(de.path().string());
            }
        }
    } else {
        for (fs::directory_entry &de : fs::directory_iterator(dir_path)) {
            if (fs::is_regular_file(de.path())) {
                paths.push_back(de.path().string());
            }
        }
    }

    return paths;
}

/**
 * Get and store tags for all paths in a vector into a database
 */
void GetAndStoreTags(std::vector<std::string>& paths, std::string db_path) {
    // Check if database already exists
    if (fs::is_regular_file(db_path)) {
        std::cout << "Database already exists" << std::endl;
        return;
    }

    // Create and open database
    Db db(db_path);
    if (!db.IsOpen()) {
        return;
    }

    // Insert into database
    for (auto &path : paths) {
        std::vector<std::string> tags = GetTagsFromFile(path);
        if (!tags.empty()) {
            for (auto &tag : tags) {
                db.InsertRow(path, tag);
            }
        }
    }
}

/**
 * Select rows
 */
void PrintPathsForTagQuery(std::string db_path, std::string tag_query) {
    // Check if database doesnt exist
    if (!fs::is_regular_file(db_path)) {
        std::cout << "Database doesn't exist" << std::endl;
        return;
    }

    // Open database
    Db db(db_path);
    if (!db.IsOpen()) {
        return;
    }

    // Select from database
    std::vector<std::string> paths = db.SelectTagQuery(tag_query);

    // Print paths
    for (auto &path : paths) {
        std::cout << path << std::endl;
    }
}
