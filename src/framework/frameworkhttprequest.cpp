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

#include "FrameworkCommon.h"
#include "FrameworkHttpRequest.h"

namespace ultraschall { namespace framework {

HttpRequest::HttpRequest(const HttpRequestType type, const runtime::String& url) : type_(type), url_(url) {}

HttpRequest::HttpRequest(
    const HttpRequestType type, const runtime::String& url, const runtime::StringDictionary& header) :
    type_(type),
    url_(url), header_(header)
{}

HttpRequest::~HttpRequest() {}

HttpRequest* HttpRequest::Create(const HttpRequestType type, const runtime::String& url)
{
    return new HttpRequest(HttpRequestType::GET, url);
}

HttpRequest* HttpRequest::Create(
    const HttpRequestType type, const runtime::String& url, const runtime::StringDictionary& header)
{
    return new HttpRequest(HttpRequestType::GET, url, header);
}

}} // namespace ultraschall::framework
