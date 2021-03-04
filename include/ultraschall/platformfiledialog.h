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

#ifndef __ULTRASCHALL_PLATFORM_FILE_DIALOG_H_INCL__
#define __ULTRASCHALL_PLATFORM_FILE_DIALOG_H_INCL__

#include "PlatformClass.h"

namespace ultraschall { namespace platform {

class FileDialog
{
public:
    FileDialog(
        const runtime::String& caption, const runtime::String& initialDirectory = "",
        const runtime::String& initialFile = "");
    virtual ~FileDialog();

    runtime::String SelectChaptersFile();
    runtime::String SelectAudioFile();
    runtime::String SelectPictureFile();

    runtime::String ChooseChaptersFileName();

private:
    runtime::String caption_;
    runtime::String initialDirectory_;
    runtime::String initialFile_;
};

}} // namespace ultraschall::platform

#endif // #ifndef __ULTRASCHALL_PLATFORM_FILE_DIALOG_H_INCL__
