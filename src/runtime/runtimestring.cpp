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
   #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif // #ifdef _WIN32
#include <codecvt>
#ifdef _WIN32
   #undef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif // #ifdef _WIN32

#include <string>

#include <ultraschall/runtimestring.h>

#include "runtimecommon.h"

namespace ultraschall { namespace runtime {

class String::Impl
{
public:
   inline bool empty() const
   {
      return str_.empty();
   }

private:
   std::string str_;
};

String::String() : pimpl_(new String::Impl()) {}

String::~String()
{
   // SafeDelete(pimpl_);
   if(pimpl_ != nullptr) {
      delete pimpl_;
      pimpl_ = nullptr;
   }
}

bool String::empty() const
{
   return pimpl_->empty();
}

}} // namespace ultraschall::runtime
