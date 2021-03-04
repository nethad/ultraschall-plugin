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
    #define CURL_STATICLIB
#endif // #ifdef _WIN32
#include <curl/curl.h>
#include <curl/easy.h>

#include "RuntimeMalloc.h"
#include "RuntimeSequentialStream.h"

#include "FrameworkCommon.h"
#include "FrameworkHttpClient.h"
#include "FrameworkHttpRequest.h"
#include "FrameworkHttpResponse.h"

namespace ultraschall { namespace framework {

HttpClient::HttpClient() : handle_(curl_easy_init()) {}

HttpClient::~HttpClient()
{
    if(handle_ != nullptr)
    {
        curl_easy_cleanup(handle_);
        handle_ = nullptr;
    }
}

runtime::String HttpClient::DownloadUrl(const runtime::String& url)
{
    PRECONDITION_RETURN(handle_ != nullptr, runtime::String());
    PRECONDITION_RETURN(url.empty(), runtime::String());

    runtime::String result;

    const runtime::String encodedUrl = EncodeUrl(url);
    if(encodedUrl.empty() == false)
    {
        HttpRequest* pRequest = HttpRequest::Create(HttpRequestType::GET, encodedUrl);
        if(pRequest != nullptr)
        {
            curl_easy_setopt(handle_, CURLOPT_URL, pRequest->Url().c_str());
            curl_easy_setopt(handle_, CURLOPT_FOLLOWLOCATION, 1L);
            curl_easy_setopt(handle_, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(handle_, CURLOPT_ACCEPT_ENCODING, "deflate");
            curl_easy_setopt(handle_, CURLOPT_WRITEFUNCTION, ReceiveDataHandler);

            runtime::SequentialStream* pStream = new runtime::SequentialStream();
            if(pStream != nullptr)
            {
                curl_easy_setopt(handle_, CURLOPT_WRITEDATA, pStream);
                const CURLcode curlResult = curl_easy_perform(handle_);
                if(CURLE_OK == curlResult)
                {
                    HttpResponse* pResponse = HttpResponse::Create(HttpResultCode::SUCCESS, pStream);
                    if(pResponse != nullptr)
                    {
                        result = StreamToString(pResponse->Result());
                        runtime::SafeRelease(pResponse);
                    }
                }

                runtime::SafeRelease(pStream);
            }

            runtime::SafeRelease(pRequest);
        }
    }

    return result;
}

size_t HttpClient::ReceiveDataHandler(void* pData, size_t dataSize, size_t itemSize, void* pParam)
{
    PRECONDITION_RETURN(pData != 0, 0);
    PRECONDITION_RETURN(itemSize > 0, 0);
    PRECONDITION_RETURN(pParam != 0, 0);

    size_t result = -1;

    runtime::SequentialStream* pStream = reinterpret_cast<runtime::SequentialStream*>(pParam);
    if(pStream != nullptr)
    {
        if(pStream->Write(reinterpret_cast<uint8_t*>(pData), dataSize * itemSize) == true)
        {
            result = dataSize * itemSize;
        }
    }

    return result;
}

runtime::String HttpClient::StreamToString(const runtime::SequentialStream* pStream)
{
    PRECONDITION_RETURN(pStream != nullptr, runtime::String());
    PRECONDITION_RETURN(pStream->Data() != nullptr, runtime::String());
    PRECONDITION_RETURN(pStream->DataSize() > 0, runtime::String());

    runtime::String result;

    const size_t   stringSize = pStream->DataSize() + sizeof(runtime::Char);
    runtime::Char* pString    = new runtime::Char[stringSize];
    if(pString != nullptr)
    {
        memset(pString, 0, stringSize);
        memcpy(pString, pStream->Data(), pStream->DataSize());
        result = pString;

        runtime::SafeDeleteArray(pString);
    }

    return result;
}

runtime::String HttpClient::EncodeUrl(const runtime::String& url)
{
    PRECONDITION_RETURN(url.empty() == false, runtime::String());

    runtime::String encodedUrl;

    void* curlHandle = curl_easy_init();
    if(curlHandle != nullptr)
    {
        char* urlBuffer = curl_easy_escape(curlHandle, url.c_str(), static_cast<int>(url.length()));
        if(urlBuffer != nullptr)
        {
            encodedUrl = urlBuffer;
            curl_free(urlBuffer);
            urlBuffer = nullptr;
        }

        curl_easy_cleanup(curlHandle);
        curlHandle = nullptr;
    }

    return encodedUrl;
}

runtime::String HttpClient::DecodeUrl(const runtime::String& url)
{
    PRECONDITION_RETURN(url.empty() == false, runtime::String());

    runtime::String decodedUrl;

    void* curlHandle = curl_easy_init();
    if(curlHandle != nullptr)
    {
        char* urlBuffer = curl_easy_unescape(curlHandle, url.c_str(), static_cast<int>(url.length()), nullptr);
        if(urlBuffer != nullptr)
        {
            decodedUrl = urlBuffer;
            curl_free(urlBuffer);
            urlBuffer = nullptr;
        }

        curl_easy_cleanup(curlHandle);
        curlHandle = nullptr;
    }

    return decodedUrl;
}

}} // namespace ultraschall::framework
