#ifndef OSMPBF_H
#define OSMPBF_H

#include "BlobHeader.h"
#include "Headerblock.h"
#include "PrimitiveBlock.h"

// this describes the low-level blob storage
//#include "fileformat.pb.h"

// this describes the high-level OSM objects
//#include "osmformat.pb.h"

namespace OSMPBF {

    // library version
    //static const char *version = "1.2.1";

    // the maximum size of a blob header in bytes
    static const int max_blob_header_size = 64 * 1024; // 64 kB

    // the maximum size of an uncompressed blob in bytes
    static const int max_uncompressed_blob_size = 32 * 1024 * 1024; // 32 MB

    // resolution for longitude/latitude used for conversion
    // between representation as double and as int
    static const int lonlat_resolution = 1000 * 1000 * 1000;

}

#endif // OSMPBF_H
