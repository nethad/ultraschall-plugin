////////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) The Ultraschall Project (http://ultraschall.fm)
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

#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#include "PlatformCommon.h"
#include "PlatformFileDialog.h"
#include "ReaperGateway.h"

namespace ultraschall { namespace platform {

FileDialog::FileDialog(
    const runtime::String& caption, const runtime::String& initialDirectory, const runtime::String& initialFile) :
    caption_(caption),
    initialDirectory_(initialDirectory), initialFile_(initialFile)
{}

FileDialog::~FileDialog() {}

runtime::String FileDialog::SelectChaptersFile()
{
    runtime::String result;

    NSOpenPanel* fileDialog = [NSOpenPanel openPanel];
    if(nil != fileDialog)
    {
        fileDialog.canChooseFiles          = YES;
        fileDialog.canChooseDirectories    = NO;
        fileDialog.canCreateDirectories    = NO;
        fileDialog.allowsMultipleSelection = NO;
        fileDialog.title                   = [NSString stringWithUTF8String:caption_.c_str()];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        if([fileDialog runModalForTypes:[[NSArray alloc] initWithObjects:@"chapters.txt", @"mp4chaps", @"txt", nil]] ==
           NSFileHandlingPanelOKButton)
#pragma clang diagnostic pop
        {
            result = [[fileDialog URL] fileSystemRepresentation];
        }

        fileDialog = nil;
    }

    return result;
}

runtime::String FileDialog::SelectAudioFile()
{
    runtime::String result;

    NSOpenPanel* fileDialog = [NSOpenPanel openPanel];
    if(nil != fileDialog)
    {
        fileDialog.canChooseFiles          = YES;
        fileDialog.canChooseDirectories    = NO;
        fileDialog.canCreateDirectories    = NO;
        fileDialog.allowsMultipleSelection = NO;
        fileDialog.title                   = [NSString stringWithUTF8String:caption_.c_str()];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        if([fileDialog runModalForTypes:[[NSArray alloc] initWithObjects:@"mp3", nil]] == NSFileHandlingPanelOKButton)
#pragma clang diagnostic pop
        {
            result = [[fileDialog URL] fileSystemRepresentation];
        }

        fileDialog = nil;
    }

    return result;
}

runtime::String FileDialog::SelectPictureFile()
{
    runtime::String result;

    NSOpenPanel* fileDialog = [NSOpenPanel openPanel];
    if(nil != fileDialog)
    {
        fileDialog.canChooseFiles          = YES;
        fileDialog.canChooseDirectories    = NO;
        fileDialog.canCreateDirectories    = NO;
        fileDialog.allowsMultipleSelection = NO;
        fileDialog.title                   = [NSString stringWithUTF8String:caption_.c_str()];
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        if([fileDialog runModalForTypes:[[NSArray alloc] initWithObjects:@"png", @"jpg", @"jpeg", nil]] ==
           NSFileHandlingPanelOKButton)
#pragma clang diagnostic pop
        {
            result = [[fileDialog URL] fileSystemRepresentation];
        }

        fileDialog = nil;
    }

    return result;
}

runtime::String FileDialog::ChooseChaptersFileName()
{
    runtime::String result;

    NSSavePanel* fileDialog = [NSSavePanel savePanel];
    if(nil != fileDialog)
    {
        fileDialog.allowedFileTypes     = [[NSArray alloc] initWithObjects:@"chapters.txt", @"mp4chaps", @"txt", nil];
        fileDialog.allowsOtherFileTypes = NO;
        fileDialog.canCreateDirectories = YES;
        fileDialog.title                = [NSString stringWithUTF8String:caption_.c_str()];

        NSString* initialDirectory = nil;
        if(initialDirectory_.empty() == false)
        {
            initialDirectory = [NSString stringWithUTF8String:initialDirectory_.c_str()];
        }

        NSString* initialFile = nil;
        if(initialFile_.empty() == false)
        {
            initialFile = [NSString stringWithUTF8String:initialFile_.c_str()];
        }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
        //        if([fileDialog runModal] == NSFileHandlingPanelOKButton)
        if([fileDialog runModalForDirectory:initialDirectory file:initialFile] == NSFileHandlingPanelOKButton)
#pragma clang diagnostic pop
        {
            result = [[fileDialog URL] fileSystemRepresentation];
        }

        fileDialog = nil;
    }

    return result;
}

}} // namespace ultraschall::platform
