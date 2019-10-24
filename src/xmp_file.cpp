#include "xmp_file.hpp"
#include <iostream>

/**
 * Constructor initializes an Xmp file
 */
XmpFile::XmpFile(std::string full_file_path, bool read, bool write){

    if (!SXMPMeta::Initialize()) {
        std::cout << "Could not initialize toolkit!";
        return;
    }
    XMP_OptionBits options = 0;
#if UNIX_ENV
    options |= kXMPFiles_ServerMode;
#endif
    // Must initialize SXMPFiles before we use it
    if (!SXMPFiles::Initialize(options)) {
        std::cout << "Could not initialize SXMPFiles.";
        return;
    }

    try {
        // Options to open the file with
        XMP_OptionBits opts = kXMPFiles_OpenUseSmartHandler;
        if(read){
            opts |= kXMPFiles_OpenForRead;
        }
        if(write){
            opts |= kXMPFiles_OpenForUpdate;
        }

        bool ok;
        std::string status = "";

        // First we try and open the file
        ok = this->xmp_file.OpenFile(full_file_path, kXMP_UnknownFile, opts);
        if (!ok) {
            status += "No smart handler available for " + full_file_path + "\n";
            status += "Trying packet scanning.\n";

            // Now try using packet scanning
            opts = kXMPFiles_OpenForUpdate | kXMPFiles_OpenUsePacketScanning;
            ok = this->xmp_file.OpenFile(full_file_path, kXMP_UnknownFile, opts);
        }

        // If the file is open then read the metadata
        if (ok) {
            this->valid = true;
        } else {
            std::cout << "Unable to open " << full_file_path << std::endl;
        }
    } catch (XMP_Error &e) {
        std::cout << "Error opening " + full_file_path + ":\n\t" << e.GetErrMsg() << std::endl;
    }
    
    return;
}

/**
 * Destructor closes the Xmp file
 */
XmpFile::~XmpFile(){

    // Close the SXMPFile.  The resource file is already closed if it was
    // opened as read only but this call must still be made.
    if(this->valid){
        this->xmp_file.CloseFile();
    }

    // Terminate the toolkit
    SXMPFiles::Terminate();
    SXMPMeta::Terminate();
}

/**
 * Get xmp meta
 */
SXMPMeta XmpFile::GetMeta(){
    // Create the xmp object and get the xmp data
    SXMPMeta meta;
    if(this->valid){
        this->xmp_file.GetXMP(&meta);
    }
    return meta;
}

/**
 * Put meta
 */
bool XmpFile::PutMeta(SXMPMeta& meta){
    // Check we can put the XMP packet back into the file
    if(this->valid && this->xmp_file.CanPutXMP(meta)){
        this->xmp_file.PutXMP(meta);
        return true;
    }
    return false;
}
