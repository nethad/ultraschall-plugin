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

#ifndef __ULTRASCHALL_MODEL_CHAPTER_TAG_H_INCL__
#define __ULTRASCHALL_MODEL_CHAPTER_TAG_H_INCL__

#include "ModelClass.h"
#include "ModelGlobals.h"

namespace ultraschall { namespace model {

class ChapterTag
{
public:
    ChapterTag() : position_(Globals::INVALID_MARKER_POSITION) {}

    ChapterTag(const double position, const runtime::String& title) : position_(position), title_(title) {}

    ChapterTag(
        const double position, const runtime::String& title, const runtime::String& image, const runtime::String& url) :
        position_(position),
        title_(title), image_(image), url_(url)
    {}

    double Position() const
    {
        return position_;
    }

    const runtime::String& Title() const
    {
        return title_;
    }

    const runtime::String& Image() const
    {
        return image_;
    }

    void SetImage(const runtime::String& image)
    {
        image_ = image;
    }

    const runtime::String& Url() const
    {
        return url_;
    }

    void SetUrl(const runtime::String& url)
    {
        url_ = url;
    }

private:
    double          position_;
    runtime::String title_;
    runtime::String image_;
    runtime::String url_;
};

typedef std::vector<ChapterTag> ChapterTagArray;

}} // namespace ultraschall::model

#endif // #ifndef __ULTRASCHALL_MODEL_CHAPTER_TAG_H_INCL__
