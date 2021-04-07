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
#include "ReaperSystemProperties.h"
#include "ReaperNotificationStore.h"

namespace ultraschall { namespace reaper {

template<>
void SystemProperty<runtime::String>::Save(
    const runtime::String& section, const runtime::String& key, const runtime::String& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    ReaperGateway::SaveSystemValue(section, key, value);
}

template<>
void SystemProperty<bool>::Save(const runtime::String& section, const runtime::String& key, const bool& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    const runtime::String boolValue = (value == true) ? "true" : "false";
    ReaperGateway::SaveSystemValue(section, key, boolValue);
}

template<> void SystemProperty<int>::Save(const runtime::String& section, const runtime::String& key, const int& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    runtime::StringStream is;
    is << value;
    ReaperGateway::SaveSystemValue(section, key, is.str());
}

template<>
void SystemProperty<runtime::String>::Set(
    const runtime::String& section, const runtime::String& key, const runtime::String& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    ReaperGateway::SetSystemValue(section, key, value);
}

template<> void SystemProperty<bool>::Set(const runtime::String& section, const runtime::String& key, const bool& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    const runtime::String boolValue = (value == true) ? "true" : "false";
    ReaperGateway::SetSystemValue(section, key, boolValue);
}

template<> void SystemProperty<int>::Set(const runtime::String& section, const runtime::String& key, const int& value)
{
    PRECONDITION(section.empty() == false);
    PRECONDITION(key.empty() == false);

    runtime::StringStream is;
    is << value;
    ReaperGateway::SetSystemValue(section, key, is.str());
}

template<>
runtime::String SystemProperty<runtime::String>::Query(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(section.empty() == false, runtime::String());
    PRECONDITION_RETURN(key.empty() == false, runtime::String());

    return RawValue(section, key);
}

template<> bool SystemProperty<bool>::Query(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(section.empty() == false, false);
    PRECONDITION_RETURN(key.empty() == false, false);

    bool value = false;

    const runtime::String& rawValue = RawValue(section, key);
    if(rawValue.empty() == false)
    {
        if((runtime::StringLowercase(rawValue) == "true") || (runtime::StringToInt(rawValue) != 0))
        {
            value = true;
        }
    }

    return value;
}

template<> int SystemProperty<int>::Query(const runtime::String& section, const runtime::String& key)
{
    PRECONDITION_RETURN(section.empty() == false, -1);
    PRECONDITION_RETURN(key.empty() == false, -1);

    int value = -1;

    const runtime::String rawValue = RawValue(section, key);
    if(rawValue.empty() == false)
    {
        value = runtime::StringToInt(rawValue);
    }

    return value;
}

}} // namespace ultraschall::reaper