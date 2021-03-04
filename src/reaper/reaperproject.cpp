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
#include "ReaperProject.h"
#include "ReaperNotificationStore.h"

namespace ultraschall { namespace reaper {

ReaperProject::ReaperProject() {}

ReaperProject::ReaperProject(ProjectReference nativeReference) : nativeReference_(nativeReference) {}

ReaperProject::~ReaperProject()
{
    nativeReference_ = nullptr;
}

ReaperProject::ReaperProject(const ReaperProject& rhs)
{
    *this = rhs;
}

ReaperProject& ReaperProject::operator=(const ReaperProject& rhs)
{
    if(this != &rhs)
    {
        nativeReference_ = rhs.nativeReference_;
    }

    return *this;
}

bool ReaperProject::IsValid() const
{
    return nativeReference_ != nullptr;
}

bool ReaperProject::IsValid(const ReaperProject& project)
{
    return project.IsValid();
}

ReaperProject ReaperProject::Current()
{
    return ReaperProject(ReaperGateway::CurrentProject());
}

runtime::String ReaperProject::PathName() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, runtime::String());
    return ReaperGateway::ProjectPath(nativeReference_);
}

runtime::String ReaperProject::FolderName() const
{
    runtime::String result;

    const runtime::String fullPath = PathName();
    if(fullPath.empty() == false)
    {
        const char                 pathSeparator  = framework::FileManager::PathSeparator();
        const runtime::StringArray pathComponents = runtime::StringTokenize(fullPath, pathSeparator);
        if(pathComponents.empty() == false)
        {
            for(size_t i = 0; i < pathComponents.size() - 1; i++)
            {
                result += pathComponents[i];
                if(i < pathComponents.size() - 2)
                {
                    result += pathSeparator;
                }
            }
        }
    }

    return result;
}

runtime::String ReaperProject::FileName() const
{
    runtime::String result;

    const runtime::String fullPath = PathName();
    if(fullPath.empty() == false)
    {
        const char                 pathSeparator  = framework::FileManager::PathSeparator();
        const runtime::StringArray pathComponents = runtime::StringTokenize(fullPath, pathSeparator);
        if(pathComponents.empty() == false)
        {
            result = pathComponents[pathComponents.size() - 1];
        }
    }

    return result;
}

runtime::String ReaperProject::Name() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, runtime::String());

    runtime::String result;

    const runtime::String file = FileName();
    if(file.empty() == false)
    {
        result = file.substr(0, file.rfind('.'));
    }

    return result;
}

runtime::StringArray ReaperProject::SanitizeNotes(const runtime::String& source)
{
    PRECONDITION_RETURN(source.empty() == false, runtime::StringArray());

    runtime::StringArray result;

    static const runtime::Char TOKEN_DELIMITER      = '\n';
    static const size_t        REQUIRED_TOKEN_COUNT = 6;

    runtime::StringArray sanitizedSource = runtime::StringTokenize(source, TOKEN_DELIMITER);
    if(sanitizedSource.size() <= REQUIRED_TOKEN_COUNT)
    {
        while(sanitizedSource.size() < REQUIRED_TOKEN_COUNT)
        {
            sanitizedSource.push_back("");
        }

        std::for_each(sanitizedSource.begin(), sanitizedSource.end(), [&](const runtime::String str) {
            if(str == "\n")
            {
                result.push_back("");
            }
            else
            {
                result.push_back(str);
            }
        });
    }

    return result;
}

runtime::String ReaperProject::ProjectMetaDataKey(const runtime::String& prefix, const runtime::String& name)
{
    PRECONDITION_RETURN(prefix.empty() == false, runtime::String());
    PRECONDITION_RETURN(name.empty() == false, runtime::String());

    return prefix + ":" + name;
}

runtime::StringDictionary ReaperProject::ProjectMetaData() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, runtime::StringDictionary());

    runtime::StringDictionary metaData;

    const runtime::String prefix("ID3");
    runtime::String       value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "TALB"));
    metaData.insert(std::pair<runtime::String, runtime::String>("podcast", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "TPE1"));
    metaData.insert(std::pair<runtime::String, runtime::String>("author", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "TIT2"));
    metaData.insert(std::pair<runtime::String, runtime::String>("episode", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "TYER"));
    metaData.insert(std::pair<runtime::String, runtime::String>("publicationDate", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "TCON"));
    metaData.insert(std::pair<runtime::String, runtime::String>("category", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "TLEN"));
    metaData.insert(std::pair<runtime::String, runtime::String>("duration", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "COMM"));
    metaData.insert(std::pair<runtime::String, runtime::String>("description", value));
    value = ReaperGateway::ProjectMetaData(nativeReference_, ProjectMetaDataKey(prefix, "APIC_FILE"));
    metaData.insert(std::pair<runtime::String, runtime::String>("coverImage", value));

    return metaData;
}

bool ReaperProject::InsertChapterMarker(const runtime::String& name, const double position)
{
    PRECONDITION_RETURN(nativeReference_ != 0, false);
    PRECONDITION_RETURN(name.empty() == false, false);

    double actualPosition = (position != model::Globals::INVALID_MARKER_POSITION) ? position : CurrentPosition();

    return ReaperGateway::InsertMarker(nativeReference_, name, actualPosition);
}

double ReaperProject::CurrentPosition() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, model::Globals::INVALID_MARKER_POSITION);

    double    currentPosition = model::Globals::INVALID_MARKER_POSITION;
    const int playState       = ReaperGateway::PlayState(nativeReference_);
    if((playState == 0) || (playState == 2))
    {
        currentPosition = ReaperGateway::CursorPosition(nativeReference_);
    }
    else
    {
        currentPosition = ReaperGateway::PlayPosition(nativeReference_);
    }

    return currentPosition;
}

double ReaperProject::MinPosition() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, model::Globals::INVALID_MARKER_POSITION);

    return ReaperGateway::MinPosition(nativeReference_);
}

double ReaperProject::MaxPosition() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, model::Globals::INVALID_MARKER_POSITION);

    return ReaperGateway::MaxPosition(nativeReference_);
}

bool ReaperProject::IsValidPosition(const double position)
{
    PRECONDITION_RETURN(nativeReference_ != 0, false);

    return (position >= 0) && (position <= MaxPosition());
}

model::ChapterTagArray ReaperProject::ChapterMarkers() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, model::ChapterTagArray());

    model::ChapterTagArray markers = ReaperGateway::Markers(nativeReference_);
    if(markers.empty() == false)
    {
        model::ChapterImageArray images = ChapterImages();
        model::ChapterUrlArray   urls   = ChapterUrls();
        MapImagesAndUrlsToChapters(images, urls, markers);
    }

    return markers;
}

model::ChapterImageArray ReaperProject::ChapterImages() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, model::ChapterImageArray());

    model::ChapterImageArray images;

    runtime::StringDictionary locatedImages = ReaperGateway::QueryProjectValues(nativeReference_, "chapterimages");
    if(locatedImages.empty() == false)
    {
        std::for_each(
            locatedImages.begin(), locatedImages.end(), [&](const std::pair<runtime::String, runtime::String>& item) {
                const double          position = std::stod(item.first);
                const runtime::String uri      = item.second;
                images.push_back(model::ChapterImage(position, uri));
            });
    }

    runtime::StringDictionary unlocatedImages = ReaperGateway::QueryProjectValues(nativeReference_, "lostimages");
    if(unlocatedImages.empty() == false)
    {
        std::for_each(
            unlocatedImages.begin(), unlocatedImages.end(),
            [&](const std::pair<runtime::String, runtime::String>& item) {
                const double          position = std::stod(item.first);
                const runtime::String uri      = item.second;
                images.push_back(model::ChapterImage(position, uri));
            });
    }

    return images;
}

model::ChapterUrlArray ReaperProject::ChapterUrls() const
{
    PRECONDITION_RETURN(nativeReference_ != 0, model::ChapterUrlArray());

    model::ChapterUrlArray urls;

    runtime::StringDictionary urlDictionary = ReaperGateway::QueryProjectValues(nativeReference_, "chapterurls");
    if(urlDictionary.empty() == false)
    {
        std::for_each(
            urlDictionary.begin(), urlDictionary.end(), [&](const std::pair<runtime::String, runtime::String>& item) {
                const double          position = std::stod(item.first);
                const runtime::String uri      = runtime::StringCopyTrim(item.second);
                urls.push_back(model::ChapterUrl(position, uri));
            });
    }

    return urls;
}

void ReaperProject::MapImagesAndUrlsToChapters(
    const model::ChapterImageArray& images, const model::ChapterUrlArray& urls, model::ChapterTagArray& chapters)
{
    PRECONDITION(chapters.empty() == false);

    static const double POSITION_DEAD_BAND = 2.0;

    std::for_each(images.begin(), images.end(), [&](const model::ChapterImage& image) {
        std::for_each(chapters.begin(), chapters.end(), [&](model::ChapterTag& chapter) {
            if(std::fabs(image.Position() - chapter.Position()) < POSITION_DEAD_BAND)
            {
                chapter.SetImage(image.Uri());
            }
        });
    });

    std::for_each(urls.begin(), urls.end(), [&](const model::ChapterUrl& url) {
        std::for_each(chapters.begin(), chapters.end(), [&](model::ChapterTag& chapter) {
            if(std::fabs(url.Position() - chapter.Position()) < POSITION_DEAD_BAND)
            {
                // chapter.SetUrl(HttpClient::EncodeUrl(url.Uri()));
                chapter.SetUrl(url.Uri());
            }
        });
    });
}

}} // namespace ultraschall::reaper
