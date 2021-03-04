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

#ifndef __ULTRASCHALL_PLUGIN_SAVE_CHAPTERS2_H_INCL__
#define __ULTRASCHALL_PLUGIN_SAVE_CHAPTERS2_H_INCL__

#include "PluginClass.h"

namespace ultraschall { namespace plugin {

class SaveChapterMarkersToProjectAction : public reaper::CustomAction
{
public:
    static const runtime::Char* UniqueId()
    {
        return "ULTRASCHALL_SAVE_CHAPTERS_TO_PROJECT";
    }

    static const runtime::Char* UniqueName()
    {
        return "ULTRASCHALL: Save chapter markers to project folder";
    }

    static ICustomAction* CreateCustomAction()
    {
        return new SaveChapterMarkersToProjectAction();
    }

    virtual ServiceStatus Execute() override;

private:
    runtime::String        target_;
    model::ChapterTagArray chapterMarkers_;

    bool ConfigureSources();
    bool ConfigureTargets();
};

}} // namespace ultraschall::plugin

#endif // #ifndef __ULTRASCHALL_PLUGIN_SAVE_CHAPTERS2_H_INCL__
