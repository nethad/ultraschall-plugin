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

#include "MediaProperties.h"
#include "ReaperProject.h"
#include "StringUtilities.h"

namespace ultraschall { namespace reaper {

MediaProperties::MediaProperties() {}

MediaProperties::~MediaProperties()
{
    Clear();
}

MediaProperties MediaProperties::ParseProjectNotes()
{
    PRECONDITION_RETURN(ReaperProject::Current().IsValid() == true, MediaProperties());

    MediaProperties result;

    const UnicodeString notes = ReaperProject::Current().Notes();
    if(notes.empty() == false)
    {
        UnicodeStringArray tokens = UnicodeStringTokenize(notes, '\n');
        if(tokens.empty() == false)
        {
            result.podcast_ = tokens[0];

            if(tokens.size() > 1)
            {
                result.author_ = tokens[1];
            }

            if(tokens.size() > 2)
            {
                result.episode_ = tokens[2];
            }

            if(tokens.size() > 3)
            {
                result.date_ = tokens[3];
            }

            if(tokens.size() > 4)
            {
                result.genre_ = tokens[4];
            }

            if(tokens.size() > 5)
            {
                result.comments_ = tokens[5];
            }
        }
    }

    return result;
}

bool MediaProperties::Validate() const
{
    return (podcast_.empty() == false) || (author_.empty() == false) || (episode_.empty() == false)
           || (date_.empty() == false) || (genre_.empty() == false);
}

void MediaProperties::Clear()
{
    podcast_.clear();
    author_.clear();
    episode_.clear();
    date_.clear();
    genre_.clear();
    comments_.clear();
}

}} // namespace ultraschall::reaper
