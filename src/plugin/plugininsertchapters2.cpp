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
#include "PluginInsertChapters2.h"

namespace ultraschall { namespace plugin {

static reaper::DeclareCustomAction<InsertMediaPropertiesAction> action;

ServiceStatus InsertMediaPropertiesAction::Execute()
{
    PRECONDITION_RETURN(HasValidProject() == true, SERVICE_FAILURE);

    PRECONDITION_RETURN(ConfigureTargets() == true, SERVICE_FAILURE);
    PRECONDITION_RETURN(ConfigureSources() == true, SERVICE_FAILURE);

    ServiceStatus             status = SERVICE_FAILURE;
    reaper::NotificationStore notificationStore(UniqueId());
    size_t                    errorCount = 0;

    for(size_t i = 0; i < targets_.size(); i++)
    {
        framework::ITagWriter* pTagWriter = framework::TagWriterFactory::Create(targets_[i]);
        if(pTagWriter != 0)
        {
            if(pTagWriter->Start(targets_[i]) == true)
            {
                const runtime::StringArray missingMediaDataFields = FindMissingMediaData();
                const size_t               missingFieldCount      = missingMediaDataFields.size();
                static const size_t        ALL_MEDIA_DATA_FIELDS  = 7;
                if((missingFieldCount > 0) && (missingFieldCount < ALL_MEDIA_DATA_FIELDS))
                {
                    runtime::StringStream os;
                    os << "MP3 metadata is incomplete.";
                    notificationStore.RegisterWarning(os.str());
                }
                else if(missingFieldCount == ALL_MEDIA_DATA_FIELDS)
                {
                    runtime::StringStream os;
                    os << "MP3 metadata is missing";
                    notificationStore.RegisterWarning(os.str());
                }

                if(missingFieldCount < ALL_MEDIA_DATA_FIELDS)
                {
                    if(pTagWriter->InsertProperties(targets_[i], mediaData_) == false)
                    {
                        runtime::StringStream os;
                        os << "Failed to insert MP3 metadata into " << targets_[i] << ".";
                        notificationStore.RegisterError(os.str());
                        errorCount++;
                    }
                }

                if(coverImage_.empty() == false)
                {
                    if(pTagWriter->InsertCoverImage(targets_[i], coverImage_) == false)
                    {
                        runtime::StringStream os;
                        os << "Failed to insert cover image into " << targets_[i] << ".";
                        notificationStore.RegisterError(os.str());
                        errorCount++;
                    }
                }
                else
                {
                    runtime::StringStream os;
                    os << "The cover image is missing.";
                    notificationStore.RegisterWarning(os.str());
                }

                if(chapterMarkers_.empty() == true)
                {
                    runtime::StringStream os;
                    os << "The chapter markers are missing.";
                    notificationStore.RegisterWarning(os.str());
                }
                else if(AreChapterMarkersValid(chapterMarkers_) == false)
                {
                    runtime::StringStream os;
                    os << "One or more chapter markers are invalid.";
                    notificationStore.RegisterError(os.str());
                    errorCount++;
                }
                else
                {
                    if(pTagWriter->InsertChapterMarkers(targets_[i], chapterMarkers_) == false)
                    {
                        runtime::StringStream os;
                        os << "Failed to insert chapter markers into " << targets_[i] << ".";
                        notificationStore.RegisterError(os.str());
                        errorCount++;
                    }
                }

                pTagWriter->Stop(0 == errorCount);
            }

            SafeRelease(pTagWriter);
        }
    }

    if(0 == errorCount)
    {
        for(size_t i = 0; i < targets_.size(); i++)
        {
            runtime::StringStream os;
            os << targets_[i] << " has been updated successfully.";
            notificationStore.RegisterSuccess(os.str());
        }

        status = SERVICE_SUCCESS;
    }

    return status;
}

bool InsertMediaPropertiesAction::ConfigureSources()
{
    bool result = true;

    mediaData_.clear();
    coverImage_.clear();
    chapterMarkers_.clear();

    mediaData_      = reaper::ReaperProject::Current().ProjectMetaData();
    coverImage_     = FindCoverImage();
    chapterMarkers_ = CurrentProject().ChapterMarkers();
    if(chapterMarkers_.empty() == false)
    {
        bool errorFound = false;
        std::for_each(chapterMarkers_.begin(), chapterMarkers_.end(), [&](const model::ChapterTag& chapterMarker) {
            if(chapterMarker.Title().length() > model::Globals::MAX_CHAPTER_TITLE_LENGTH)
            {
                runtime::StringStream os;
                os << "The chapter marker title '" << chapterMarker.Title() << "' is too long. "
                   << "Make sure that is does not exceed " << model::Globals::MAX_CHAPTER_TITLE_LENGTH
                   << " characters.";
                reaper::NotificationStore notificationStore(UniqueId());
                notificationStore.RegisterError(os.str());
                errorFound = true;
            }
        });

        result = (false == errorFound);
    }

    return result;
}

bool InsertMediaPropertiesAction::ConfigureTargets()
{
    reaper::NotificationStore supervisor(UniqueId());

    targets_.clear();

    static const runtime::StringArray fileExtensions = {".mp3"};
    for(size_t i = 0; i < fileExtensions.size(); i++)
    {
        runtime::String targetName = CreateProjectPath(fileExtensions[i]);
        if(framework::FileManager::FileExists(targetName) != false)
        {
            targets_.push_back(targetName);
        }
    }

    if(targets_.empty() == true)
    {
        platform::FileDialog  fileDialog("Select audio file");
        const runtime::String target = fileDialog.SelectAudioFile();
        if(target.empty() == false)
        {
            targets_.push_back(target);
        }
    }

    return targets_.empty() == false;
}

runtime::String InsertMediaPropertiesAction::FindCoverImage()
{
    runtime::String coverImage = mediaData_["coverImage"];
    if(coverImage.empty() == true)
    {
        runtime::StringArray       files;
        const runtime::StringArray extensions {".jpg", ".jpeg", ".png"};
        for(size_t i = 0; i < extensions.size(); i++)
        {
            files.push_back(framework::FileManager::AppendPath(CurrentProjectDirectory(), "cover") + extensions[i]);
            files.push_back(framework::FileManager::AppendPath(CurrentProjectDirectory(), "Cover") + extensions[i]);
            files.push_back(framework::FileManager::AppendPath(CurrentProjectDirectory(), "COVER") + extensions[i]);
            files.push_back(
                framework::FileManager::AppendPath(CurrentProjectDirectory(), CurrentProjectName()) + extensions[i]);
        }

        const size_t imageIndex = framework::FileManager::FileExists(files);
        if(imageIndex != -1)
        {
            coverImage = files[imageIndex];
        }
    }

    return coverImage;
}

runtime::StringArray InsertMediaPropertiesAction::FindMissingMediaData()
{
    runtime::StringArray missingMediaDataFields;

    static const runtime::StringArray mediaDataKeys = {"podcast",         "author",      "episode",   "category",
                                                       "publicationDate", "description", "coverImage"};
    std::for_each(mediaDataKeys.begin(), mediaDataKeys.end(), [&](const runtime::String& mediaDataKey) {
        const runtime::StringDictionary::const_iterator mediaDataIterator = mediaData_.find(mediaDataKey);
        if(mediaDataIterator != mediaData_.end())
        {
            const runtime::String mediaDataField = runtime::StringCopyTrimLeft(mediaDataIterator->second);
            if(mediaDataField.empty() == true)
            {
                missingMediaDataFields.push_back(mediaDataKey);
            }
            else if(mediaDataField[0] == '\n')
            {
                missingMediaDataFields.push_back(mediaDataKey);
            }
        }
        else
        {
            missingMediaDataFields.push_back(mediaDataKey);
        }
    });

    return missingMediaDataFields;
}

}} // namespace ultraschall::plugin
