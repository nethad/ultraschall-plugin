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

#ifndef __ULTRASCHALL_RUNTIME_STRING_UTILITIES_H_INCL__
#define __ULTRASCHALL_RUNTIME_STRING_UTILITIES_H_INCL__

#include "runtimestring.h"

namespace ultraschall { namespace runtime {

#if 0
    #define _MAKE_TEXT(str) #str
    #define MAKE_TEXT(str)  _MAKE_TEXT(str)

StringArray StringTokenize(const String& input, const char delimiter);

void StringTrimLeft(String& str);
void StringTrimRight(String& str);
void StringTrim(String& str);

String StringCopyTrimLeft(String str);
String StringCopyTrimRight(String str);
String StringCopyTrim(String str);

void StringReplace(String& str, const String& source, const String& target);

int    StringToInt(const String& str);
String StringFromInt(const int value);

String StringLowercase(const String& str);
String StringUppercase(const String& str);

String   MillisecondsToString(const uint32_t milliseconds, const bool roundSeconds = false);
uint32_t StringToMilliseconds(const String& str);
String   SecondsToString(const double seconds, const bool roundSeconds = false);
double   StringToSeconds(const String& str);

#endif

}} // namespace ultraschall::runtime

#endif // #ifndef __ULTRASCHALL_RUNTIME_STRING_UTILITIES_H_INCL__
