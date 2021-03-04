
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

#ifndef __ULTRASCHALL_REAPER_GATEWAY_H_INCL__
#define __ULTRASCHALL_REAPER_GATEWAY_H_INCL__

#include "ReaperClass.h"

namespace ultraschall { namespace reaper {

typedef void* ProjectReference;

class ReaperGateway
{
public:
    static intptr_t View();

    static runtime::String ApplicationVersion();
    static int32_t         RegisterCustomAction(const runtime::String& name, void* infoStruct);

    static runtime::String CurrentProjectPath();
    static runtime::String CurrentProjectFile();
    static runtime::String CurrentProjectDirectory();
    static runtime::String CurrentProjectName();

    static runtime::String TimestampToString(const double timestamp);
    static double          StringToTimestamp(const runtime::String& input);

    static ProjectReference CurrentProject();
    static runtime::String  ProjectPath(ProjectReference projectReference);
    static runtime::String  ProjectNotes(ProjectReference projectReference);

    static model::ChapterTagArray Markers(ProjectReference projectReference);

    static size_t CountMarkers(ProjectReference projectReference);
    static bool   ClearMarkers(ProjectReference projectReference);

    static bool InsertMarker(ProjectReference projectReference, const runtime::String& name, const double position);
    static bool InsertMarker(ProjectReference projectReference, const model::ChapterTag& marker);
    static bool UndoMarker(ProjectReference projectReference, const double position);

    static int    PlayState(ProjectReference projectReference);
    static double CursorPosition(ProjectReference projectReference);
    static double PlayPosition(ProjectReference projectReference);
    static double MinPosition(ProjectReference projectReference);
    static double MaxPosition(ProjectReference projectReference);

    static bool            HasSystemValue(const runtime::String& section, const runtime::String& key);
    static runtime::String SystemValue(const runtime::String& section, const runtime::String& key);
    static void            SetSystemValue(
                   const runtime::String& section, const runtime::String& key, const runtime::String& value);
    static void SaveSystemValue(
        const runtime::String& section, const runtime::String& key, const runtime::String& value);
    static void ClearSystemValue(const runtime::String& section, const runtime::String& key);
    static void DeleteSystemValue(const runtime::String& section, const runtime::String& key);

    static bool HasProjectValue(
        ProjectReference projectReference, const runtime::String& section, const runtime::String& key);
    static runtime::String QueryProjectValue(
        ProjectReference projectReference, const runtime::String& section, const runtime::String& key);
    static void SetProjectValue(
        ProjectReference projectReference, const runtime::String& section, const runtime::String& key,
        const runtime::String& value);
    static void ClearProjectValue(
        ProjectReference projectReference, const runtime::String& section, const runtime::String& key);
    static void ClearProjectValues(ProjectReference projectReference, const runtime::String& section);
    static runtime::StringDictionary QueryProjectValues(
        ProjectReference projectReference, const runtime::String& section);

    static runtime::String ProjectMetaData(ProjectReference projectReference, const runtime::String& key);

private:
    static const size_t MAX_REAPER_STRING_BUFFER_SIZE = 4096;
};

}} // namespace ultraschall::reaper

#endif // #ifndef __ULTRASCHALL_REAPER_GATEWAY_H_INCL__
