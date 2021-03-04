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

#ifndef __ULTRASCHALL_FRAMEWORK_HTTP_CLIENT_H_INCL__
#define __ULTRASCHALL_FRAMEWORK_HTTP_CLIENT_H_INCL__

#include "FrameworkClass.h"

namespace ultraschall { namespace framework {

class HttpClient : public runtime::SharedObject
{
public:
    HttpClient();
    virtual ~HttpClient();

    runtime::String DownloadUrl(const runtime::String& url);

    static runtime::String EncodeUrl(const runtime::String& url);
    static runtime::String DecodeUrl(const runtime::String& url);

private:
    void* handle_ = nullptr;

    HttpClient(const HttpClient&) = delete;
    HttpClient& operator=(const HttpClient&) = delete;

    static size_t ReceiveDataHandler(void* pData, size_t dataSize, size_t itemSize, void* pParam);

    static runtime::String StreamToString(const runtime::SequentialStream* pStream);
};

}} // namespace ultraschall::framework

#endif // #ifndef __ULTRASCHALL_FRAMEWORK_HTTP_CLIENT_H_INCL__
