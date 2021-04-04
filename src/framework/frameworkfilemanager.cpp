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

#include "frameworkcommon.h"
#include "frameworkfilemanager.h"
#include "platform.h"

namespace ultraschall { namespace framework {

runtime::Char FileManager::PathSeparator()
{
    return platform::FileSystem::QueryPathSeparator();
}

runtime::String FileManager::AppendPath(const runtime::String& prefix, const runtime::String& appendix)
{
    return prefix + platform::FileSystem::QueryPathSeparator() + appendix;
}

runtime::String FileManager::StripPath(const runtime::String& path)
{
    PRECONDITION_RETURN(path.empty() == false, runtime::String());

    runtime::String shortName;

    if(path.empty() == false) {
        shortName = path;

        const runtime::String::size_type offset = path.rfind(FileManager::PathSeparator());
        if(offset != runtime::String::npos) {
            shortName = path.substr(offset + 1, path.size()); // skip separator
        }
    }

    return shortName;
}

runtime::StringArray FileManager::SplitPath(const runtime::String& path)
{
    return runtime::StringTokenize(path, PathSeparator());
}

bool FileManager::FileExists(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, false);

    bool fileExists = false;

    std::ifstream is(U2H(filename), std::ios::in | std::ios::binary);
    if(is.is_open() == true) {
        fileExists = true;
        is.close();
    }

    return fileExists;
}

size_t FileManager::FileExists(const runtime::StringArray& paths)
{
    PRECONDITION_RETURN(paths.empty() == false, -1);

    size_t offset = -1;

    for(size_t i = 0; (i < paths.size()) && (offset == -1); i++) {
        if(FileExists(paths[i]) == true) {
            offset = i;
        }
    }

    return offset;
}

runtime::String FileManager::QueryFileDirectory(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, runtime::String());

    runtime::String directory = ".";

    size_t offset = filename.find_last_of(platform::FileSystem::QueryPathSeparator());
    if(offset != std::string::npos) {
        directory = filename.substr(0, offset);
    }

    return directory;
}

size_t FileManager::QueryFileSize(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, -1);
    PRECONDITION_RETURN(FileExists(filename) == true, -1);

    size_t size = -1;

    std::ifstream file(U2H(filename), std::ios::in | std::ios::binary | std::ios::ate);
    if(file.is_open() == true) {
        size = file.tellg();
        file.close();
    }

    return size;
}

runtime::String FileManager::NormalizeFileName(const runtime::String& targetName)
{
    runtime::String firstStage  = targetName;
    runtime::String secondStage = runtime::StringCopyTrim(firstStage);
    return runtime::StringLowercase(secondStage);
}

FileManager::FILE_TYPE FileManager::QueryFileType(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, FILE_TYPE::UNKNOWN_FILE_TYPE);

    FILE_TYPE             type             = FILE_TYPE::UNKNOWN_FILE_TYPE;
    const runtime::String cookedTargetName = NormalizeFileName(filename);
    const size_t          extensionOffset  = cookedTargetName.rfind(".");
    if(extensionOffset != runtime::String::npos) {
        const runtime::String fileExtension =
            cookedTargetName.substr(extensionOffset + 1, cookedTargetName.length() - extensionOffset);
        if(fileExtension.empty() == false) {
            if((fileExtension == "txt") || (fileExtension == "mp4chaps")) {
                type = FILE_TYPE::MP4CHAPS;
            }
            else if(fileExtension == "mp3") {
                type = FILE_TYPE::MP3;
            }
            else if((fileExtension == "jpg") || (fileExtension == "jpeg")) {
                type = FILE_TYPE::JPEG;
            }
            else if(fileExtension == "png") {
                type = FILE_TYPE::PNG;
            }
            else {
                type = FILE_TYPE::UNKNOWN_FILE_TYPE;
            }
        }
    }

    return type;
}

bool FileManager::IsDiskSpaceAvailable(const runtime::String& filename, const size_t requiredBytes)
{
    PRECONDITION_RETURN(filename.empty() == false, false);
    PRECONDITION_RETURN(requiredBytes != -1, false);

    bool isAvailable = false;

    const size_t availableSpace = platform::FileSystem::QueryAvailableDiskSpace(QueryFileDirectory(filename));
    if(availableSpace != -1) {
        isAvailable = (requiredBytes <= availableSpace);
    }

    return isAvailable;
}

runtime::Stream* FileManager::ReadBinaryFile(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, nullptr);

    runtime::Stream* pStream = nullptr;

    const size_t fileSize = QueryFileSize(filename);
    if(fileSize != -1) {
        std::ifstream file(U2H(filename), std::ios::in | std::ios::binary);
        if(file.is_open() == true) {
            uint8_t* buffer = new uint8_t[fileSize];
            if(buffer != nullptr) {
                file.read(reinterpret_cast<char*>(buffer), fileSize);
                if(file) {
                    pStream = new runtime::Stream(fileSize);
                    if(pStream != nullptr) {
                        const bool succeeded = pStream->Write(0, buffer, fileSize);
                        if(succeeded == false) {
                            runtime::SafeRelease(pStream);
                        }
                    }
                }

                runtime::SafeDeleteArray(buffer);
            }

            file.close();
        }
    }

    return pStream;
}

runtime::StringArray FileManager::ReadTextFile(const runtime::String& filename)
{
    PRECONDITION_RETURN(filename.empty() == false, runtime::StringArray());
    PRECONDITION_RETURN(FileExists(filename) == true, runtime::StringArray());

    runtime::StringArray lines;

    std::ifstream   is(U2H(filename).c_str());
    runtime::String line;
    while(std::getline(is, line)) {
        lines.push_back(line);
    }

    return lines;
}

bool FileManager::WriteTextFile(const runtime::String& filename, const runtime::String& str)
{
    PRECONDITION_RETURN(filename.empty() == false, false);
    PRECONDITION_RETURN(IsDiskSpaceAvailable(filename, str.size()) == true, false);
    PRECONDITION_RETURN(str.empty() == false, false);

    bool status = false;

    std::ofstream os(U2H(filename).c_str());
    if(os.is_open() == true) {
        os << str;
        os.close();
        status = true;
    }

    return status;
}

}} // namespace ultraschall::framework
