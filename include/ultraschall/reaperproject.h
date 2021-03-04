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

#ifndef __ULTRASCHALL_REAPER_PROJECT_H_INCL__
#define __ULTRASCHALL_REAPER_PROJECT_H_INCL__

#include "ReaperClass.h"
#include "ReaperGateway.h"

namespace ultraschall { namespace reaper {

class ReaperProject
{
public:
    ReaperProject();
    ReaperProject(ProjectReference nativeReference);
    virtual ~ReaperProject();

    ReaperProject(const ReaperProject& rhs);
    ReaperProject& operator=(const ReaperProject& rhs);

    static bool IsValid(const ReaperProject& project);
    bool        IsValid() const;

    static ReaperProject Current();

    runtime::String PathName() const;
    runtime::String FolderName() const;
    runtime::String FileName() const;
    runtime::String Name() const;

    double CurrentPosition() const;
    double MinPosition() const;
    double MaxPosition() const;
    bool   IsValidPosition(const double position);

    bool InsertChapterMarker(
        const runtime::String& name, const double position = model::Globals::INVALID_MARKER_POSITION);

    model::ChapterTagArray ChapterMarkers() const;

    runtime::StringDictionary ProjectMetaData() const;

private:
    ProjectReference nativeReference_ = nullptr;

    static runtime::StringArray SanitizeNotes(const runtime::String& notes);

    model::ChapterImageArray ChapterImages() const;
    model::ChapterUrlArray   ChapterUrls() const;

    static void MapImagesAndUrlsToChapters(
        const model::ChapterImageArray& images, const model::ChapterUrlArray& urls, model::ChapterTagArray& chapters);

    static runtime::String ProjectMetaDataKey(const runtime::String& prefix, const runtime::String& name);
};

}} // namespace ultraschall::reaper

#endif // #ifndef __ULTRASCHALL_REAPER_PROJECT_H_INCL__
