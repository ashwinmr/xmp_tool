#ifndef XMP_TOOLS_H_
#define XMP_TOOLS_H_

#include <vector>

/**
 * Get tags from a file
 */
std::vector<std::string> GetFileTags(std::string full_file_path);

/**
 * Get all file paths within a directory
 */
std::vector<std::string> GetFilePaths(std::string dir_path, bool recurse);

/**
 * Get and store tags for all paths in a vector into a database
 */
void GetAndStoreTags(std::vector<std::string> paths, std::string db_path);

/**
 * Select rows
 */
void SelectRows(std::string db_path, std::string tag);

#endif // XMP_TOOLS_H_
