////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) The Ultraschall Project (https://ultraschall.fm)
//
// The MIT License (MIT)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __ULTRASCHALL_FRAMEWORK_FILE_MANAGER_H_INCL__
#define __ULTRASCHALL_FRAMEWORK_FILE_MANAGER_H_INCL__

#include "frameworkclass.h"

namespace ultraschall { namespace framework {

class FileManager
{
public:
    static runtime::Char PathSeparator();

    static runtime::String      AppendPath(const runtime::String& prefix, const runtime::String& appendix);
    static runtime::String      StripPath(const runtime::String& path);
    static runtime::StringArray SplitPath(const runtime::String& path);

    static bool   FileExists(const runtime::String& path);
    static size_t FileExists(const runtime::StringArray& paths);

    static runtime::String QueryFileDirectory(const runtime::String& filename);

    enum class FILE_TYPE
    {
        MP4CHAPS,
        MP3,
        JPEG,
        PNG,
        UNKNOWN_FILE_TYPE,
        MAX_FILE_TYPE = UNKNOWN_FILE_TYPE
    };
    static FILE_TYPE QueryFileType(const runtime::String& filename);

    static size_t QueryFileSize(const runtime::String& filename);
    static bool   IsDiskSpaceAvailable(const runtime::String& filename, const size_t requiredBytes);

    static runtime::Stream*     ReadBinaryFile(const runtime::String& filename);
    static runtime::StringArray ReadTextFile(const runtime::String& filename);

    static bool WriteTextFile(const runtime::String& filename, const runtime::String& str);

private:
    static runtime::String NormalizeFileName(const runtime::String& targetName);
};

}} // namespace ultraschall::framework

#endif // #ifndef __ULTRASCHALL_FRAMEWORK_FILE_MANAGER_H_INCL__
