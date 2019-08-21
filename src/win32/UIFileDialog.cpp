////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) The Ultraschall Project (http://ultraschall.fm)
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

#include "ReaperGateway.h"
#include "StringUtilities.h"
#include "UIFileDialog.h"

namespace ultraschall { namespace reaper {

UIFileDialog::UIFileDialog(const UnicodeString& caption, const UnicodeString& initialDirectory) {}

UIFileDialog::~UIFileDialog() {}

UnicodeString UIFileDialog::BrowseForChapters()
{
    return BrowseForFile("MP4 chapters|*.chapters.txt|MP4 chapters|*.mp4chaps");
}

UnicodeString UIFileDialog::BrowseForAudio()
{
    return BrowseForFile("MP3 file|*.mp3|MP4 file|*.mp4|M4A file|*.m4a");
}

UnicodeString UIFileDialog::BrowseForPicture()
{
    return BrowseForFile("JPG file|*.jpg|PNG file|*.png");
}

UnicodeString UIFileDialog::BrowseForDirectory()
{
    WideUnicodeString result;

    IFileOpenDialog* pfod = nullptr;
    HRESULT          hr   = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&pfod));
    if(SUCCEEDED(hr))
    {
        pfod->SetTitle(reinterpret_cast<LPCWSTR>(U2WU(caption_).c_str()));

        if(initialDirectory_.empty() == false)
        {
            IShellItem* psi = 0;
            hr              = SHCreateItemFromParsingName(
                reinterpret_cast<LPCWSTR>(U2WU(initialDirectory_).c_str()), nullptr, IID_PPV_ARGS(&psi));
            if(SUCCEEDED(hr))
            {
                pfod->SetFolder(psi);
                SafeRelease(psi);
            }
        }

        if(SUCCEEDED(hr))
        {
            FILEOPENDIALOGOPTIONS fos = FOS_PICKFOLDERS | FOS_PATHMUSTEXIST;
            pfod->SetOptions(fos);

            hr = pfod->Show(reinterpret_cast<HWND>(ReaperGateway::View()));
            if(SUCCEEDED(hr))
            {
                IShellItem* psi = 0;
                hr              = pfod->GetResult(&psi);
                if(SUCCEEDED(hr))
                {
                    LPWSTR fileSystemPath = 0;
                    hr                    = psi->GetDisplayName(SIGDN_FILESYSPATH, &fileSystemPath);
                    if(SUCCEEDED(hr) && (0 != fileSystemPath))
                    {
                        result = reinterpret_cast<WideUnicodeChar*>(fileSystemPath);
                        CoTaskMemFree(fileSystemPath);
                    }

                    SafeRelease(psi);
                }
            }
        }

        SafeRelease(pfod);
    }

    return WU2U(result);
}

UnicodeString UIFileDialog::BrowseForFile(const UnicodeString& fileExtensions)
{
    PRECONDITION_RETURN(fileExtensions.empty() == false, "");

    WideUnicodeString result;

    UnicodeStringArray     filterSpecs = UnicodeStringTokenize(fileExtensions, UnicodeChar('|'));
    WideUnicodeStringArray wideFileExtensions;
    for(size_t i = 0; i < filterSpecs.size(); i++)
    {
        wideFileExtensions.push_back(UnicodeStringToWideUnicodeString(filterSpecs[i]));
    }

    const size_t       filterCount = filterSpecs.size() / 2;
    COMDLG_FILTERSPEC* filters     = SafeAllocArray<COMDLG_FILTERSPEC>(filterCount);
    if(filters != nullptr)
    {
        for(size_t j = 0; j < filterCount; ++j)
        {
            const size_t offset = j * 2;
            filters[j].pszName  = (LPCWSTR)wideFileExtensions[offset].c_str();
            filters[j].pszSpec  = (LPCWSTR)wideFileExtensions[offset + 1].c_str();
        }

        IFileOpenDialog* pfod = nullptr;
        HRESULT          hr   = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC, IID_PPV_ARGS(&pfod));
        if(SUCCEEDED(hr))
        {
            pfod->SetTitle(reinterpret_cast<LPCWSTR>(U2WU(caption_).c_str()));
            pfod->SetFileTypes(static_cast<UINT>(filterCount), filters);

            FILEOPENDIALOGOPTIONS fos = FOS_STRICTFILETYPES | FOS_FILEMUSTEXIST;
            pfod->SetOptions(fos);

            hr = pfod->Show(reinterpret_cast<HWND>(ReaperGateway::View()));
            if(SUCCEEDED(hr))
            {
                IShellItem* psi = nullptr;
                hr              = pfod->GetResult(&psi);
                if(SUCCEEDED(hr))
                {
                    LPWSTR fileSystemPath = nullptr;
                    hr                    = psi->GetDisplayName(SIGDN_FILESYSPATH, &fileSystemPath);
                    if(SUCCEEDED(hr) && (nullptr != fileSystemPath))
                    {
                        result = reinterpret_cast<WideUnicodeChar*>(fileSystemPath);
                        CoTaskMemFree(fileSystemPath);
                    }

                    SafeRelease(psi);
                }
            }

            SafeRelease(pfod);
        }

        SafeDeleteArray(filters);
    }

    return WU2U(result);
}

}} // namespace ultraschall::reaper

#endif // #ifdef _WIN32