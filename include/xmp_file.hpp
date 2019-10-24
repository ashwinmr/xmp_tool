#ifndef XMP_FILE_H_
#define XMP_FILE_H_

#include <string>

// Must be defined to instantiate template classes
#define TXMP_STRING_TYPE std::string

// Must be defined to give access to XMPFiles
#define XMP_INCLUDE_XMPFILES 1

// Ensure XMP templates are instantiated
#include "XMP.incl_cpp"

// Provide access to the API
#include "XMP.hpp"

class XmpFile{
    public:
    bool valid = false;

    /**
     * Constructor initializes an Xmp file
     */
    XmpFile(std::string full_file_path, bool read, bool write);

    /**
     * Destructor closes the Xmp file
     */
    ~XmpFile();

    /**
     * Get xmp meta
     */
    SXMPMeta GetMeta();

    private:
    SXMPFiles xmp_file;

};

#endif // XMP_FILE_H_
