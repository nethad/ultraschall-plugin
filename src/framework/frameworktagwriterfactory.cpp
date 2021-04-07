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
#include "frameworktagwriterfactory.h"
#include "frameworkid3v2writer.h"
#include "frameworkfilemanager.h"

namespace ultraschall { namespace framework {

ITagWriter* TagWriterFactory::Create(const runtime::String& targetName)
{
    PRECONDITION_RETURN(targetName.empty() == false, 0);
    PRECONDITION_RETURN(targetName.length() > 4, 0);

    ITagWriter*                  tagWriter  = nullptr;
    const FileManager::FILE_TYPE targetType = FileManager::QueryFileType(targetName);
    if(targetType == FileManager::FILE_TYPE::MP3) {
        tagWriter = new ID3V2Writer();
    }
    else {
        tagWriter = nullptr;
    }

    return tagWriter;
}

}} // namespace ultraschall::framework