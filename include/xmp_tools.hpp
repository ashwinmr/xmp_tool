#ifndef XMP_TOOLS_H_
#define XMP_TOOLS_H_

#include <vector>

/**
 * Get tags from a file
 */
std::vector<std::string> GetTags(std::string full_file_path);

/**
 * Get all file paths within a directory
 */
std::vector<std::string> GetFilePaths(std::string dir_path);

#endif // XMP_TOOLS_H_
