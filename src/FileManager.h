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

#ifndef __ULTRASCHALL_REAPER_FILE_MANAGER_H_INCL__
#define __ULTRASCHALL_REAPER_FILE_MANAGER_H_INCL__

#include "BinaryStream.h"
#include "Common.h"

namespace ultraschall { namespace reaper {

class FileManager
{
public:
    static UnicodeChar PathSeparator();

    static UnicodeString      AppendPath(const UnicodeString& prefix, const UnicodeString& appendix);
    static UnicodeString      StripPath(const UnicodeString& path);
    static UnicodeStringArray SplitPath(const UnicodeString& path);

    static bool   FileExists(const UnicodeString& path);
    static size_t FileExists(const UnicodeStringArray& paths);

    static UnicodeString QueryFileDirectory(const UnicodeString& filename);

    static size_t QueryFileSize(const UnicodeString& filename);
    static bool   IsDiskSpaceAvailable(const UnicodeString& filename);

    static BinaryStream*      ReadBinaryFile(const UnicodeString& filename);
    static UnicodeStringArray ReadTextFile(const UnicodeString& filename);

    static bool WriteBinaryFile(const UnicodeString& filename, const BinaryStream* pStream);
    static bool WriteTextFile(const UnicodeString& filename, const UnicodeString& str);
    static bool WriteTextFile(const UnicodeString& filename, const UnicodeStringArray& lines);

    static bool CreateFileBackup(const UnicodeString& filename);

private:
    static bool backupInProgress_;

    static void BeginBackup();
    static void EndBackup();
    static bool BackupInProgress();
};

}} // namespace ultraschall::reaper

#endif // #ifndef __ULTRASCHALL_REAPER_FILE_MANAGER_H_INCL__
