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

#ifndef __ULTRASCHALL_FRAMEWORK_ID3V2_H_INCL__
#define __ULTRASCHALL_FRAMEWORK_ID3V2_H_INCL__

#include "FrameworkClass.h"
#include "FrameworkID3V2Context.h"

namespace ultraschall { namespace framework {

ID3V2Context* ID3V2StartTransaction(const runtime::String& targetName);
bool          ID3V2CommitTransaction(ID3V2Context*& context);
void          ID3V2AbortTransaction(ID3V2Context*& context);

void ID3V2RemoveAllFrames(ID3V2Context*);

bool ID3V2InsertTextFrame(
    ID3V2Context* context, const runtime::String& id, const runtime::String& text,
    const runtime::CHAR_ENCODING encoding);

bool ID3V2InsertCommentsFrame(ID3V2Context* context, const runtime::String& text);

bool ID3V2InsertChapterFrame(
    ID3V2Context* context, const runtime::String& id, const runtime::String& text, const uint32_t startTime,
    const uint32_t endTime);

bool ID3V2InsertChapterFrame(
    ID3V2Context* context, const runtime::String& id, const runtime::String& text, const uint32_t startTime,
    const uint32_t endTime, const runtime::String& image, const runtime::String& url);

bool ID3V2InsertTableOfContentsFrame(ID3V2Context* context, const runtime::StringArray& tableOfContentsItems);

bool ID3V2InsertCoverPictureFrame(ID3V2Context* context, const runtime::String& image);

}} // namespace ultraschall::framework

#endif // #ifndef __ULTRASCHALL_FRAMEWORK_ID3V2_H_INCL__
