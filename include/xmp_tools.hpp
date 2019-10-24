#ifndef XMP_TOOLS_H_
#define XMP_TOOLS_H_

#include <vector>

/**
 * Get tags from a file
 */
std::vector<std::string> GetTagsFromFile(std::string full_file_path);

/**
 * Get tags from a file
 */
void ReadXmpFromFile(std::string full_file_path);

/**
 * Add tags to file
 */
bool AddTagsToFile(std::string full_file_path, std::vector<std::string>& tags);

/**
 * Add tags to files
 */
void AddTagsToFiles(std::vector<std::string>& paths, std::vector<std::string>& tags);

/**
 * Remove tags from file
 */
bool RemoveTagsFromFile(std::string full_file_path, std::vector<std::string>& tags, bool remove_all);

/**
 * Remove tags from files
 */
void RemoveTagsFromFiles(std::vector<std::string>& paths, std::vector<std::string>& tags, bool remove_all);

/**
 * Get all file paths within a directory
 */
std::vector<std::string> GetFilePaths(std::string dir_path, bool recurse);

/**
 * Get and store tags for all paths in a vector into a database
 */
void GetAndStoreTags(std::vector<std::string>& paths, std::string db_path);

/**
 * Get all paths that have a tag 
 */
void PrintPathsForTagQuery(std::string db_path, std::string tag);

#endif // XMP_TOOLS_H_
