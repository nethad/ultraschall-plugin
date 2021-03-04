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

#include "ReaperCommon.h"
#include "ReaperGateway.h"
#include "ReaperEntryPoints.h"
#include "ReaperGlobals.h"

namespace ultraschall { namespace reaper {

intptr_t ReaperGateway::View()
{
    return reinterpret_cast<intptr_t>(reaper_api::GetMainHwnd());
}

runtime::String ReaperGateway::ApplicationVersion()
{
    return H2U(reaper_api::GetAppVersion());
}

int32_t ReaperGateway::RegisterCustomAction(const runtime::String& name, void* infoStruct)
{
    PRECONDITION_RETURN(name.empty() == false, -1);

    return reaper_api::plugin_register(U2H(name).c_str(), infoStruct);
}

ProjectReference ReaperGateway::CurrentProject()
{
    return reinterpret_cast<ProjectReference>(reaper_api::EnumProjects(-1, 0, 0));
}

runtime::String ReaperGateway::CurrentProjectPath()
{
    runtime::String result;

    char             buffer[MAX_REAPER_STRING_BUFFER_SIZE] = {0};
    ProjectReference projectReference = reaper_api::EnumProjects(-1, buffer, MAX_REAPER_STRING_BUFFER_SIZE);
    if((projectReference != nullptr) && (strlen(buffer) > 0))
    {
        result = buffer;
    }

    return result;
}

runtime::String ReaperGateway::CurrentProjectFile()
{
    runtime::String result;

    const runtime::String projectPath = CurrentProjectPath();
    if(projectPath.empty() == false)
    {
        const runtime::StringArray pathComponents = framework::FileManager::SplitPath(projectPath);
        if(pathComponents.empty() == false)
        {
            result = pathComponents[pathComponents.size() - 1];
        }
    }

    return result;
}

runtime::String ReaperGateway::CurrentProjectDirectory()
{
    runtime::String result;

    const runtime::String projectPath = CurrentProjectPath();
    if(projectPath.empty() == false)
    {
        const runtime::StringArray pathComponents = framework::FileManager::SplitPath(projectPath);
        if(pathComponents.empty() == false)
        {
            for(size_t i = 0; i < pathComponents.size() - 1; i++)
            {
                result += pathComponents[i];
                if(i < pathComponents.size() - 2)
                {
                    result += framework::FileManager::PathSeparator();
                }
            }
        }
    }

    return result;
}

runtime::String ReaperGateway::CurrentProjectName()
{
    runtime::String result;

    const runtime::String projectFile = CurrentProjectFile();
    if(projectFile.empty() == false)
    {
        result = projectFile.substr(0, projectFile.find('.', 0));
    }

    return result;
}

struct Timestamp
{
    int hours;
    int minutes;
    int seconds;
    int milliSeconds;

    Timestamp() : hours(0), minutes(0), seconds(0), milliSeconds(0) {}

    static Timestamp FromString(const runtime::String& str)
    {
        runtime::StringArray items = runtime::StringTokenize(str, ':');
        std::reverse(items.begin(), items.end());

        Timestamp timestamp;

        runtime::StringArray buffer = runtime::StringTokenize(items[0], '.');
        for(size_t i = 0; i < buffer.size(); ++i)
        {
            switch(i)
            {
                case 0:
                    timestamp.seconds = std::atoi(buffer[0].c_str());
                    break;

                case 1:
                    timestamp.milliSeconds = std::atoi(buffer[1].c_str());
                    break;

                default:
                    break;
            }
        }

        for(size_t i = 0; i < items.size(); ++i)
        {
            switch(i)
            {
                case 1:
                    timestamp.minutes = std::atoi(items[1].c_str());
                    break;

                case 2:
                    timestamp.hours = std::atoi(items[2].c_str());
                    break;

                default:
                    break;
            }
        }

        return timestamp;
    }

    runtime::String ToString() const
    {
        std::stringstream buffer;
        buffer << std::setfill('0');
        buffer << std::setw(2) << hours << ":";
        buffer << std::setw(2) << minutes << ":";
        buffer << std::setw(2) << seconds << ".";
        buffer << std::setw(3) << milliSeconds;

        return buffer.str();
    }
};

runtime::String ReaperGateway::TimestampToString(const double timestamp)
{
    runtime::String result;

    char buffer[MAX_REAPER_STRING_BUFFER_SIZE] = {0};
    reaper_api::format_timestr_pos(timestamp, buffer, MAX_REAPER_STRING_BUFFER_SIZE, 0);
    if(strlen(buffer) > 0)
    {
        result = Timestamp::FromString(buffer).ToString();
    }

    return result;
}

double ReaperGateway::StringToTimestamp(const runtime::String& input)
{
    PRECONDITION_RETURN(input.empty() == false, -1);
    return reaper_api::parse_timestr(input.c_str());
}

runtime::String ReaperGateway::ProjectPath(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, runtime::String());

    runtime::String projectPath;

    ReaProject*         nativeReference                       = reinterpret_cast<ReaProject*>(projectReference);
    static const size_t MAX_REAPER_STRING_BUFFER_SIZE         = 4096;
    char                buffer[MAX_REAPER_STRING_BUFFER_SIZE] = {0};
    int                 index                                 = 0;
    ProjectReference    externalReference =
        (ProjectReference)reaper_api::EnumProjects(index++, buffer, MAX_REAPER_STRING_BUFFER_SIZE);
    while((externalReference != 0) && (projectPath.empty() == true))
    {
        if(externalReference == nativeReference)
        {
            projectPath = buffer;
        }

        externalReference = (ProjectReference)reaper_api::EnumProjects(index++, buffer, MAX_REAPER_STRING_BUFFER_SIZE);
    }

    return projectPath;
}

runtime::String ReaperGateway::ProjectNotes(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, runtime::String());

    runtime::String projectNotes;

    ReaProject*         nativeReference                = reinterpret_cast<ReaProject*>(projectReference);
    static const size_t MAX_PROJECT_NOTES_SIZE         = 4096;
    char                buffer[MAX_PROJECT_NOTES_SIZE] = {0};
    reaper_api::GetSetProjectNotes(nativeReference, false, buffer, (int)MAX_PROJECT_NOTES_SIZE);
    projectNotes = H2U(buffer);
    projectNotes.erase(std::remove(projectNotes.begin(), projectNotes.end(), '\r'), projectNotes.end());

    return projectNotes;
}

bool IsEditMarker(const runtime::String& name)
{
    PRECONDITION_RETURN(name.empty() == false, false);

    const runtime::String normalizedName = runtime::StringCopyTrimLeft(name);
    PRECONDITION_RETURN(normalizedName.empty() == false, false);

    return (normalizedName.size() >= 5) && (normalizedName.substr(0, 5) == "_Edit");
}

model::ChapterTagArray ReaperGateway::Markers(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, model::ChapterTagArray());

    model::ChapterTagArray allMarkers;

    bool        isRegion        = false;
    double      position        = 0;
    double      duration        = 0;
    const char* name            = 0;
    int         number          = 0;
    int         color           = 0;
    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    int         nextIndex =
        reaper_api::EnumProjectMarkers3(nativeReference, 0, &isRegion, &position, &duration, &name, &number, &color);
    while(nextIndex > 0)
    {
        if(IsEditMarker(name) == false) // remove edit markers
        {
            if(false == isRegion) // remove regions
            {
                allMarkers.push_back(model::ChapterTag(position, name));
            }
        }

        nextIndex = reaper_api::EnumProjectMarkers3(
            nativeReference, nextIndex, &isRegion, &position, &duration, &name, &number, &color);
    }

    return allMarkers;
}

size_t ReaperGateway::CountMarkers(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, false);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    int         numMarkers      = -1;
    reaper_api::CountProjectMarkers(nativeReference, &numMarkers, 0);
    return static_cast<size_t>(numMarkers);
}

bool ReaperGateway::ClearMarkers(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, false);

    ReaProject*  nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    const size_t numMarkers      = CountMarkers(projectReference);
    for(size_t i = 0; i < numMarkers; i++)
    {
        reaper_api::DeleteProjectMarkerByIndex(nativeReference, int(i));
    }

    return true;
}

bool ReaperGateway::InsertMarker(ProjectReference projectReference, const runtime::String& name, const double position)
{
    PRECONDITION_RETURN(projectReference != nullptr, false);
    PRECONDITION_RETURN(position >= 0, false);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    return reaper_api::AddProjectMarker2(nativeReference, false, position, 0, name.c_str(), -1, 0) != -1;
}

bool ReaperGateway::InsertMarker(ProjectReference projectReference, const model::ChapterTag& marker)
{
    PRECONDITION_RETURN(projectReference != nullptr, false);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    return reaper_api::AddProjectMarker2(
               nativeReference, false, marker.Position(), 0, marker.Title().c_str(), -1,
               Globals::DEFAULT_CHAPTER_MARKER_COLOR) != -1;
}

bool ReaperGateway::UndoMarker(ProjectReference projectReference, const double position)
{
    PRECONDITION_RETURN(projectReference != nullptr, false);
    PRECONDITION_RETURN(position >= 0, false);

    bool        undone          = false;
    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    int         markerIndex     = -1;
    reaper_api::GetLastMarkerAndCurRegion(nativeReference, position, &markerIndex, 0);
    if(markerIndex != -1)
    {
        undone = reaper_api::DeleteProjectMarkerByIndex(nativeReference, markerIndex);
    }

    return undone;
}

int ReaperGateway::PlayState(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, -1);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    return reaper_api::GetPlayStateEx(nativeReference);
}

double ReaperGateway::CursorPosition(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, -1);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    return reaper_api::GetCursorPositionEx(nativeReference);
}

double ReaperGateway::PlayPosition(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, -1);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    return reaper_api::GetPlayPositionEx(nativeReference);
}

double ReaperGateway::MinPosition(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, -1);

    double minPosition = MaxPosition(projectReference);
    if(minPosition != model::Globals::INVALID_MARKER_POSITION)
    {
        ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
        int         i               = 0;
        MediaItem*  mediaItem       = reaper_api::GetMediaItem(nativeReference, i++);
        while(mediaItem != 0)
        {
            double startPosition = reaper_api::GetMediaItemInfo_Value(mediaItem, "D_POSITION");
            if(startPosition < minPosition)
            {
                minPosition = startPosition;
            }
            mediaItem = reaper_api::GetMediaItem(nativeReference, i++);
        }
    }

    return minPosition;
}

double ReaperGateway::MaxPosition(ProjectReference projectReference)
{
    PRECONDITION_RETURN(projectReference != nullptr, -1);

    double      maxPosition     = model::Globals::INVALID_MARKER_POSITION;
    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    int         i               = 0;
    MediaItem*  mediaItem       = reaper_api::GetMediaItem(nativeReference, i++);
    while(mediaItem != 0)
    {
        const double endPosition = reaper_api::GetMediaItemInfo_Value(mediaItem, "D_POSITION") +
                                   reaper_api::GetMediaItemInfo_Value(mediaItem, "D_LENGTH");
        if((endPosition >= 0) && (endPosition > maxPosition))
        {
            maxPosition = endPosition;
        }

        mediaItem = reaper_api::GetMediaItem(nativeReference, i++);
    }

    return maxPosition;
}

bool ReaperGateway::HasSystemValue(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(section.empty() == false, false);
    PRECONDITION_RETURN(key.empty() == false, false);

    return reaper_api::HasExtState(U2H(section).c_str(), U2H(key).c_str());
}

runtime::String ReaperGateway::SystemValue(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(section.empty() == false, runtime::String());
    PRECONDITION_RETURN(key.empty() == false, runtime::String());

    return H2U(reaper_api::GetExtState(U2H(section).c_str(), U2H(key).c_str()));
}

void ReaperGateway::SetSystemValue(
    const runtime::String& section, const runtime::String& key, const runtime::String& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);
    PRECONDITION(value.empty() == false);

    reaper_api::SetExtState(U2H(section).c_str(), U2H(key).c_str(), U2H(value).c_str(), false);
}

void ReaperGateway::SaveSystemValue(
    const runtime::String& section, const runtime::String& key, const runtime::String& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);
    PRECONDITION(value.empty() == false);

    reaper_api::SetExtState(U2H(section).c_str(), U2H(key).c_str(), U2H(value).c_str(), true);
}

void ReaperGateway::ClearSystemValue(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    reaper_api::DeleteExtState(U2H(section).c_str(), U2H(key).c_str(), false);
}

void ReaperGateway::DeleteSystemValue(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    reaper_api::DeleteExtState(U2H(section).c_str(), U2H(key).c_str(), true);
}

bool ReaperGateway::HasProjectValue(
    ProjectReference projectReference, const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(projectReference != nullptr, false);
    PRECONDITION_RETURN(section.empty() == false, false);
    PRECONDITION_RETURN(key.empty() == false, false);

    ReaProject*         nativeReference                = reinterpret_cast<ReaProject*>(projectReference);
    static const size_t MAX_PROJECT_VALUE_SIZE         = 4096;
    char                buffer[MAX_PROJECT_VALUE_SIZE] = {0};
    const int           valueSize                      = reaper_api::GetProjExtState(
        nativeReference, U2H(section).c_str(), U2H(key).c_str(), buffer, (int)MAX_PROJECT_VALUE_SIZE);
    return valueSize > 0;
}

runtime::String ReaperGateway::QueryProjectValue(
    ProjectReference projectReference, const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(projectReference != nullptr, runtime::String());
    PRECONDITION_RETURN(section.empty() == false, runtime::String());
    PRECONDITION_RETURN(key.empty() == false, runtime::String());

    runtime::String projectValue;

    ReaProject*         nativeReference                = reinterpret_cast<ReaProject*>(projectReference);
    static const size_t MAX_PROJECT_VALUE_SIZE         = 4096;
    char                buffer[MAX_PROJECT_VALUE_SIZE] = {0};
    const int           valueSize                      = reaper_api::GetProjExtState(
        nativeReference, U2H(section).c_str(), U2H(key).c_str(), buffer, (int)MAX_PROJECT_VALUE_SIZE);
    if(valueSize > 0)
    {
        projectValue = H2U(buffer);
    }

    return projectValue;
}

void ReaperGateway::SetProjectValue(
    ProjectReference projectReference, const runtime::String& section, const runtime::String& key,
    const runtime::String& value)
{
    PRECONDITION(projectReference != nullptr);
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);
    PRECONDITION(value.empty() == false);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    reaper_api::SetProjExtState(nativeReference, U2H(section).c_str(), U2H(key).c_str(), U2H(value).c_str());
}

void ReaperGateway::ClearProjectValue(
    ProjectReference projectReference, const runtime::String& section, const runtime::String& key)
{
    PRECONDITION(projectReference != nullptr);
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    reaper_api::SetProjExtState(nativeReference, U2H(section).c_str(), U2H(key).c_str(), nullptr);
}

void ReaperGateway::ClearProjectValues(ProjectReference projectReference, const runtime::String& section)
{
    PRECONDITION(projectReference != nullptr);
    PRECONDITION(section.empty() == false);

    ReaProject* nativeReference = reinterpret_cast<ReaProject*>(projectReference);
    reaper_api::SetProjExtState(nativeReference, U2H(section).c_str(), nullptr, nullptr);
}

runtime::StringDictionary ReaperGateway::QueryProjectValues(
    ProjectReference projectReference, const runtime::String& section)
{
    PRECONDITION_RETURN(projectReference != nullptr, runtime::StringDictionary());
    PRECONDITION_RETURN(section.empty() == false, runtime::StringDictionary());

    ReaProject*         nativeReference        = reinterpret_cast<ReaProject*>(projectReference);
    static const size_t MAX_BUFFER_SIZE        = 4096;
    char                key[MAX_BUFFER_SIZE]   = {0};
    char                value[MAX_BUFFER_SIZE] = {0};

    runtime::StringDictionary values;

    int  i        = 0;
    bool hasValue = reaper_api::EnumProjExtState(
        nativeReference, U2H(section).c_str(), i++, key, MAX_BUFFER_SIZE, value, MAX_BUFFER_SIZE);
    while(true == hasValue)
    {
        values.insert(std::pair<runtime::String, runtime::String>(key, value));

        memset(key, 0, MAX_BUFFER_SIZE * sizeof(char));
        memset(value, 0, MAX_BUFFER_SIZE * sizeof(char));
        hasValue = reaper_api::EnumProjExtState(
            nativeReference, U2H(section).c_str(), i++, key, MAX_BUFFER_SIZE, value, MAX_BUFFER_SIZE);
    }

    return values;
}

runtime::String ReaperGateway::ProjectMetaData(ProjectReference projectReference, const runtime::String& key)
{
    PRECONDITION_RETURN(projectReference != nullptr, runtime::String());
    PRECONDITION_RETURN(key.empty() == false, runtime::String());

    runtime::String data;

    ReaProject*         nativeReference        = reinterpret_cast<ReaProject*>(projectReference);
    static const size_t MAX_BUFFER_SIZE        = 4096;
    char                value[MAX_BUFFER_SIZE] = {0};

    strncpy(value, key.c_str(), key.length());
    if(reaper_api::GetSetProjectInfo_String(nativeReference, "RENDER_METADATA", value, false))
    {
        data = H2U(value);
    }

    return data;
}

}} // namespace ultraschall::reaper
