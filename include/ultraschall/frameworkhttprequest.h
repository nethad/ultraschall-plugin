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

#ifndef __ULTRASCHALL_FRAMEWORK_HTTP_REQUEST_H_INCL__
#define __ULTRASCHALL_FRAMEWORK_HTTP_REQUEST_H_INCL__

#include "frameworkclass.h"

namespace ultraschall { namespace framework {

enum class HttpRequestType
{
    GET
};

class HttpRequest : public runtime::SharedObject
{
public:
    static HttpRequest* Create(const HttpRequestType type, const runtime::String& url);
    static HttpRequest* Create(
        const HttpRequestType type, const runtime::String& url, const runtime::StringDictionary& header);

    inline const runtime::String&           Url() const;
    inline const runtime::StringDictionary& Header() const;

protected:
    virtual ~HttpRequest();

    HttpRequest(const HttpRequestType type, const runtime::String& url);
    HttpRequest(const HttpRequestType type, const runtime::String& url, const runtime::StringDictionary& header);

private:
    HttpRequestType           type_;
    runtime::String           url_;
    runtime::StringDictionary header_;
};

inline const runtime::String& HttpRequest::Url() const
{
    return url_;
}

inline const runtime::StringDictionary& HttpRequest::Header() const
{
    return header_;
}

}} // namespace ultraschall::framework

#endif // #ifdef __ULTRASCHALL_FRAMEWORK_HTTP_REQUEST_H_INCL__
