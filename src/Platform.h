////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) The Ultraschall Project (http://ultraschall.fm)
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

#ifndef __ULTRASCHALL_REAPER_PLATFORM_H_INCL__
#define __ULTRASCHALL_REAPER_PLATFORM_H_INCL__

#include "Common.h"

namespace ultraschall { namespace reaper {

class Platform
{
public:
    static const UnicodeString THEME_PATH;
    static const UnicodeString SOUNDBOARD_PATH;
    static const UnicodeString SWS_PATH;
    static const UnicodeString PLUGIN_PATH;
    static const UnicodeString STUDIO_LINK_PATH;
    static const UnicodeString STUDIO_LINK_ONAIR_PATH;

    static UnicodeString QueryProgramFilesDirectory();
    static UnicodeString QueryUserDataDirectory();

    static UnicodeChar QueryPathSeparator();
    static size_t      QueryAvailableDiskSpace(const UnicodeString& directory);

    static bool          QueryFileExists(const UnicodeString& path);
    static UnicodeString AppendPath(const UnicodeString& prefix, const UnicodeString& appendix);

    static UnicodeString ReadFileVersion(const UnicodeString& path);
};

}} // namespace ultraschall::reaper

#endif // #ifndef __ULTRASCHALL_REAPER_PLATFORM_H_INCL__
