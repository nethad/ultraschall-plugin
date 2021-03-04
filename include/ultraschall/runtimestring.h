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

#ifndef __ULTRASCHALL_RUNTIME_STRING_H_INCL__
#define __ULTRASCHALL_RUNTIME_STRING_H_INCL__

#include <map>
#include <string>
#include <sstream>
#include <vector>

namespace ultraschall { namespace runtime {

typedef char     Char;
typedef char16_t WideChar;

typedef std::string    String;
typedef std::u16string WideString;

typedef std::stringstream                                                                       StringStream;
typedef std::basic_stringstream<char16_t, std::char_traits<char16_t>, std::allocator<char16_t>> WideStringStream;

typedef std::vector<String>      StringArray;
typedef std::map<String, String> StringDictionary;

typedef std::vector<WideString>          WideStringArray;
typedef std::map<WideString, WideString> WideStringDictionary;

typedef enum
{
    UTF8,
    UTF16
} CHAR_ENCODING;

typedef enum
{
    NO_UTF16_BOM      = 0,
    WITH_UTF16_BOM_LE = 1,
    WITH_UTF16_BOM_BE = 2
} UTF16_BOM_SPEC;
static const char16_t* UTF16_BOM_LE = u"\xfffe"; // intel
static const char16_t* UTF16_BOM_BE = u"\xfeff";

WideString StringToWideString(const String& str, const UTF16_BOM_SPEC bomSpec = NO_UTF16_BOM);

typedef enum
{
    NO_UTF8_BOM   = 0,
    WITH_UTF8_BOM = 1
} UTF8_BOM_SPEC;
static const char* UTF8_BOM = "\xef\xbb\xbf";

String WideStringToString(const WideString& wideStr, const UTF8_BOM_SPEC bomSpec = NO_UTF8_BOM);

String HostStringToString(const std::string& hostStr, const UTF8_BOM_SPEC bomSpec = NO_UTF8_BOM);

WideString HostStringToWideString(const std::string& hostStr, const UTF16_BOM_SPEC bomSpec = NO_UTF16_BOM);

std::string StringToHostString(const String& str);
std::string WideStringToHostString(const WideString& wideStr);

}} // namespace ultraschall::runtime

#define H2U(a)  ultraschall::runtime::HostStringToString((a))
#define H2WU(a) ultraschall::runtime::HostStringToWideString((a))

#define U2H(a)  ultraschall::runtime::StringToHostString((a))
#define WU2H(a) ultraschall::runtime::WideStringToHostString((a))

#define U2WU(a) ultraschall::runtime::StringToWideString((a))
#define WU2U(a) ultraschall::runtime::WideStringToString((a))

#endif // #ifndef __ULTRASCHALL_RUNTIME_STRING_H_INCL__
