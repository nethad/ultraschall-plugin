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

#include "PluginCommon.h"
#include "PluginInsertChapters.h"

namespace ultraschall { namespace plugin {

static reaper::DeclareCustomAction<InsertChapterMarkersAction> action;

ServiceStatus InsertChapterMarkersAction::Execute()
{
    PRECONDITION_RETURN(HasValidProject() == true, SERVICE_FAILURE);

    PRECONDITION_RETURN(ConfigureSources() == true, SERVICE_FAILURE);
    PRECONDITION_RETURN(ConfigureTargets() == true, SERVICE_FAILURE);

    PRECONDITION_RETURN(AreChapterMarkersValid(chapterMarkers_) == true, SERVICE_FAILURE);

    ServiceStatus             status = SERVICE_FAILURE;
    reaper::NotificationStore supervisor(UniqueId());

    reaper::ReaperProject currentProject = reaper::ReaperProject::Current();
    size_t                addedTags      = 0;
    for(size_t i = 0; i < chapterMarkers_.size(); i++)
    {
        if(currentProject.InsertChapterMarker(chapterMarkers_[i].Title(), chapterMarkers_[i].Position()) == true)
        {
            addedTags++;
        }
        else
        {
            runtime::StringStream os;
            os << "Chapter marker '" << chapterMarkers_[i].Title() << "' at position '"
               << runtime::SecondsToString(chapterMarkers_[i].Position()) << "' could not be added.";
            supervisor.RegisterError(os.str());
        }
    }

    if(chapterMarkers_.size() != addedTags)
    {
        runtime::StringStream os;
        os << "Not all chapter markers were added.";
        supervisor.RegisterError(os.str());
    }

    return status;
}

bool InsertChapterMarkersAction::ConfigureTargets()
{
    reaper::NotificationStore supervisor(UniqueId());
    model::ChapterTagArray    chapterMarkers;

    framework::FileManager::FILE_TYPE mediaType = framework::FileManager::QueryFileType(source_);
    switch(mediaType)
    {
        case framework::FileManager::FILE_TYPE::MP4CHAPS:
            chapterMarkers = ReadTextFile(source_);
            break;
        case framework::FileManager::FILE_TYPE::MP3:
            // chapterMarkers = ReadMP3File(source_);
            break;
        default:
            break;
    }

    if(chapterMarkers.empty() == false)
    {
        chapterMarkers_ = chapterMarkers;
    }

    return chapterMarkers.empty() == false;
}

bool InsertChapterMarkersAction::ConfigureSources()
{
    source_.clear();

    platform::FileDialog fileDialog("Import chapter markers");
    source_ = fileDialog.SelectChaptersFile();
    return source_.empty() == false;
}

model::ChapterTagArray InsertChapterMarkersAction::ReadTextFile(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, model::ChapterTagArray());

    reaper::NotificationStore supervisor(UniqueId());
    model::ChapterTagArray    chapterMarkers;

    const runtime::StringArray lines = framework::FileManager::ReadTextFile(filename);
    if(lines.empty() == false)
    {
        for(size_t i = 0; i < lines.size(); i++)
        {
            const runtime::String normalizedLine = runtime::StringCopyTrimLeft(lines[i]);
            if(normalizedLine.empty() == false)
            {
                if(normalizedLine.size() >= reaper::Globals::MIN_CHAPTER_MARKER_LINE_LENGTH)
                {
                    const runtime::StringArray items = runtime::StringTokenize(normalizedLine, ' ');
                    if(items.empty() == false)
                    {
                        const double position = runtime::StringToSeconds(items[0]);
                        if(position >= 0)
                        {
                            runtime::String title;
                            for(size_t j = 1; j < items.size(); j++)
                            {
                                if(items[j].empty() == false)
                                {
                                    if(title.empty() == false)
                                    {
                                        title += ' ';
                                    }

                                    title += items[j];
                                }
                            }

                            chapterMarkers.push_back(model::ChapterTag(position, title));
                        }
                        else
                        {
                            runtime::StringStream os;
                            os << "Line " << (i + 1) << ": Invalid timestamp in '" << lines[i] << "'.";
                            supervisor.RegisterError(os.str());
                        }
                    }
                }
                else
                {
                    runtime::StringStream os;
                    os << "Line " << (i + 1) << ": Invalid format in '" << lines[i] << "'.";
                    supervisor.RegisterError(os.str());
                }
            }
        }
    }
    else
    {
        runtime::StringStream os;
        os << "The file '" << filename << "' does not contain chapter markers";
        supervisor.RegisterWarning(os.str());
    }

    return chapterMarkers;
}

}} // namespace ultraschall::plugin
