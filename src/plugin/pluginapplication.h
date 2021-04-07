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

#ifndef __ULTRASCHALL_PLUGIN_APPLICATION_H_INCL__
#define __ULTRASCHALL_PLUGIN_APPLICATION_H_INCL__

#include "PluginClass.h"
#include "PluginCommon.h"

namespace ultraschall { namespace plugin {

class Application
{
public:
    virtual ~Application();

    static Application& Instance();

    ServiceStatus Start(intptr_t handle);
    void          Stop();

    template<class T> ServiceStatus RegisterCustomAction() const;
    template<class T> void          InvokeCustomAction() const;
    static bool                     OnCustomAction(const int32_t id);

    inline intptr_t Handle() const;

private:
    intptr_t                     handle_ = 0;
    mutable std::recursive_mutex lock_;

    Application();
};

typedef struct
{
    int uniqueSectionId; // 0/100=main/main alt, 32063=media explorer, 32060=midi editor, 32061=midi event list editor,
                         // 32062=midi inline editor, etc
    const char* idStr;   // must be unique across all sections
    const char* name;
    void*       extra; // reserved for future use
} custom_action_register_t;

template<class T> ServiceStatus Application::RegisterCustomAction() const
{
    PRECONDITION_RETURN(T::UniqueId() != nullptr, SERVICE_INVALID_ARGUMENT);
    PRECONDITION_RETURN(T::UniqueName() != nullptr, SERVICE_INVALID_ARGUMENT);

    typedef T custom_action_type;

    ServiceStatus status = SERVICE_FAILURE;

    const runtime::Char*         uniqueId      = custom_action_type::UniqueId();
    reaper::CustomActionFactory& factory       = reaper::CustomActionFactory::Instance();
    reaper::ICustomAction*       pCustomAction = 0;
    status                                     = factory.CreateCustomAction(uniqueId, pCustomAction);
    if(ServiceSucceeded(status) && (pCustomAction != 0))
    {
        custom_action_register_t action = {0};
        action.idStr                    = uniqueId;
        action.name                     = custom_action_type::UniqueName();
        const int32_t id                = reaper::ReaperGateway::RegisterCustomAction("custom_action", (void*)&action);
        if(id != 0)
        {
            reaper::CustomActionManager& manager = reaper::CustomActionManager::Instance();
            status                               = manager.RegisterCustomAction(uniqueId, id, pCustomAction);
        }

        runtime::SafeRelease(pCustomAction);
    }

    return status;
}

template<class T> void Application::InvokeCustomAction() const
{
    typedef T custom_action_type;

    reaper::CustomActionManager& manager       = reaper::CustomActionManager::Instance();
    reaper::ICustomAction*       pCustomAction = 0;
    ServiceStatus                status = manager.LookupCustomAction(custom_action_type::UniqueId(), pCustomAction);
    if(ServiceSucceeded(status) && (pCustomAction != 0))
    {
        pCustomAction->Execute();
        runtime::SafeRelease(pCustomAction);
    }
}

inline intptr_t Application::Handle() const
{
    std::lock_guard<std::recursive_mutex> cs(lock_);
    return handle_;
}

}} // namespace ultraschall::plugin

#endif // #ifndef __ULTRASCHALL_PLUGIN_APPLICATION_H_INCL__