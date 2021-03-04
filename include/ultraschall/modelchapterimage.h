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

#ifndef __ULTRASCHALL_MODEL_CHAPTER_IMAGE_H_INCL__
#define __ULTRASCHALL_MODEL_CHAPTER_IMAGE_H_INCL__

#include "ModelClass.h"
#include "ModelGlobals.h"

namespace ultraschall { namespace model {

class ChapterImage
{
public:
    ChapterImage() : position_(Globals::INVALID_MARKER_POSITION) {}

    ChapterImage(const double position, const runtime::String& uri) : position_(position), uri_(uri) {}

    double Position() const
    {
        return position_;
    }

    const runtime::String& Uri() const
    {
        return uri_;
    }

private:
    double          position_;
    runtime::String uri_;
};

typedef std::vector<ChapterImage> ChapterImageArray;

}} // namespace ultraschall::model

#endif // #ifndef __ULTRASCHALL_MODEL_CHAPTER_IMAGE_H_INCL__
