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

#ifndef __ULTRASCHALL_PLUGIN_INSERT_CHAPTERS_H_INCL__
#define __ULTRASCHALL_PLUGIN_INSERT_CHAPTERS_H_INCL__

#include "PluginClass.h"

namespace ultraschall { namespace plugin {

class InsertChapterMarkersAction : public reaper::CustomAction
{
public:
    static const runtime::Char* UniqueId()
    {
        return "ULTRASCHALL_INSERT_CHAPTERS";
    }

    static const runtime::Char* UniqueName()
    {
        return "ULTRASCHALL: Import chapter markers...";
    }

    static ICustomAction* CreateCustomAction()
    {
        return new InsertChapterMarkersAction();
    }

    virtual ServiceStatus Execute() override;

private:
    runtime::String        source_;
    model::ChapterTagArray chapterMarkers_;

    bool ConfigureTargets();
    bool ConfigureSources();

    static model::ChapterTagArray ReadTextFile(const runtime::String& filename);
    static model::ChapterTagArray ReadMP3File(const runtime::String& filename);
};

}} // namespace ultraschall::plugin

#endif // #ifndef __ULTRASCHALL_PLUGIN_INSERT_CHAPTERS_H_INCL__
