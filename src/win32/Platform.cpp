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

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <shlobj.h>
#include <windows.h>

#include "FileManager.h"
#include "Platform.h"

#include "Malloc.h"

namespace ultraschall { namespace reaper {

const UnicodeString Platform::THEME_PATH("\\REAPER\\ColorThemes\\Ultraschall_4.0.ReaperThemeZip");
const UnicodeString Platform::SOUNDBOARD_PATH("\\Steinberg\\VstPlugins\\Soundboard64.dll");
const UnicodeString Platform::SWS_PATH("\\REAPER\\UserPlugins\\reaper_sws64.dll");
const UnicodeString Platform::PLUGIN_PATH("\\REAPER\\UserPlugins\\reaper_ultraschall.dll");
const UnicodeString Platform::STUDIO_LINK_PATH("\\Steinberg\\VstPlugins\\studio-link.dll");
const UnicodeString Platform::STUDIO_LINK_ONAIR_PATH("\\Steinberg\\VstPlugins\\studio-link-onair.dll");

UnicodeString Platform::QueryProgramFilesDirectory()
{
    UnicodeString directory;

    WideUnicodeChar* unicodeString = nullptr;
    const HRESULT    hr            = SHGetKnownFolderPath(FOLDERID_ProgramFilesX64, 0, nullptr, (PWSTR*)&unicodeString);
    if(SUCCEEDED(hr))
    {
        directory = WideUnicodeStringToUnicodeString(unicodeString);
        CoTaskMemFree(unicodeString);
    }

    return directory;
}

UnicodeString Platform::QueryUserDataDirectory()
{
    UnicodeString directory;

    WideUnicodeChar* unicodeString = nullptr;
    const HRESULT    hr            = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, (PWSTR*)&unicodeString);
    if(SUCCEEDED(hr))
    {
        directory = WideUnicodeStringToUnicodeString(unicodeString);
        CoTaskMemFree(unicodeString);
        unicodeString = nullptr;
    }

    return directory;
}

UnicodeChar Platform::QueryPathSeparator()
{
    return '\\';
}

bool Platform::QueryFileExists(const UnicodeString& path)
{
    PRECONDITION_RETURN(path.empty() == false, false);

    bool fileExists = false;

    HANDLE fileHandle = CreateFileA(
        path.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if(INVALID_HANDLE_VALUE != fileHandle)
    {
        fileExists = true;
        CloseHandle(fileHandle);
    }

    return fileExists;
}

UnicodeString Platform::AppendPath(const UnicodeString& prefix, const UnicodeString& appendix)
{
    return prefix + QueryPathSeparator() + appendix;
}

UnicodeString Platform::ReadFileVersion(const UnicodeString& path)
{
    PRECONDITION_RETURN(path.empty() == false, UnicodeString());

    UnicodeString version;

    DWORD       fileVersionInfoHandle = 0;
    const DWORD fileVersionInfoSize   = GetFileVersionInfoSize(path.c_str(), &fileVersionInfoHandle);
    if(fileVersionInfoSize > 0)
    {
        uint8_t* fileVersionInfo = new uint8_t[fileVersionInfoSize];
        if(fileVersionInfo != 0)
        {
            memset(fileVersionInfo, 0, fileVersionInfoSize * sizeof(uint8_t));
            if(GetFileVersionInfoA(path.c_str(), fileVersionInfoHandle, fileVersionInfoSize, fileVersionInfo))
            {
                uint8_t* versionDataPtr  = 0;
                uint32_t versionDataSize = 0;
                if(VerQueryValueA(fileVersionInfo, "\\", (void**)&versionDataPtr, &versionDataSize))
                {
                    if(versionDataSize > 0)
                    {
                        const VS_FIXEDFILEINFO* fileInfo = reinterpret_cast<VS_FIXEDFILEINFO*>(versionDataPtr);
                        if(fileInfo->dwSignature == 0xfeef04bd)
                        {
                            std::stringstream str;
                            str << ((fileInfo->dwFileVersionMS >> 16) & 0xffff) << ".";
                            str << ((fileInfo->dwFileVersionMS >> 0) & 0xffff) << ".";
                            str << ((fileInfo->dwFileVersionLS >> 16) & 0xffff) << ".";
                            str << ((fileInfo->dwFileVersionLS >> 0) & 0xffff);
                            version = str.str();
                        }
                    }
                }
            }

            SafeDeleteArray(fileVersionInfo);
        }
    }

    return version;
}

UnicodeString FindUltraschallPluginDirectory()
{
    UnicodeString pluginDirectory;

    HMODULE moduleHandle = GetModuleHandleA("reaper_ultraschall.dll");
    if(moduleHandle != 0)
    {
        CHAR        dllPath[_MAX_PATH] = {0};
        const DWORD charCount          = GetModuleFileNameA(moduleHandle, dllPath, _MAX_PATH);
        if(charCount > 0)
        {
            pluginDirectory = dllPath;
            if(pluginDirectory.empty() == false)
            {
                const size_t offset = pluginDirectory.rfind('\\');
                if(offset != std::string::npos)
                {
                    pluginDirectory = pluginDirectory.substr(0, offset);
                }
                else
                {
                    pluginDirectory.clear();
                }
            }
        }
    }

    return pluginDirectory;
}

size_t Platform::QueryAvailableDiskSpace(const UnicodeString& directory)
{
    PRECONDITION_RETURN(directory.empty() == false, -1);

    size_t availableSpace = -1;

    ULARGE_INTEGER freeBytesAvailableToUser = {0};
    if(GetDiskFreeSpaceEx(directory.c_str(), &freeBytesAvailableToUser, nullptr, nullptr) != FALSE)
    {
        availableSpace = freeBytesAvailableToUser.QuadPart;
    }

    return availableSpace;
}

}} // namespace ultraschall::reaper

#endif // #ifdef _WIN32
