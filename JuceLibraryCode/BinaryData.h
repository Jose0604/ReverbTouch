/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   logo2_png;
    const int            logo2_pngSize = 13806;

    extern const char*   stevejohnsondVRD8E3XUGsunsplash_jpg;
    const int            stevejohnsondVRD8E3XUGsunsplash_jpgSize = 67004;

    extern const char*   Donald_Duck_png;
    const int            Donald_Duck_pngSize = 443279;

    extern const char*   Felix_Orange_png;
    const int            Felix_Orange_pngSize = 315030;

    extern const char*   knob_png;
    const int            knob_pngSize = 1010797;

    extern const char*   Royal_Flush_png;
    const int            Royal_Flush_pngSize = 312135;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 6;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
