/*
Copyright (c) 2011, Philipp Stephani <st_philipp@yahoo.de>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <CoreFoundation/CoreFoundation.h>
#include <ApplicationServices/ApplicationServices.h>

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>

#include "fonts_impl.h"

int lltxplatform_get_installed_fonts_impl(struct lltxplatform_fontinfo **const fonts, unsigned int *const count) {
  int status = -1;
  ATSFontIterator iter = NULL;
  if (ATSFontIteratorCreate(kATSFontContextGlobal, NULL, NULL, kATSOptionFlagsRestrictedScope, &iter) == noErr) {
    const CFMutableArrayRef names = CFArrayCreateMutable(kCFAllocatorDefault, 0l, &kCFTypeArrayCallBacks);
    if (names != NULL) {
      const CFMutableArrayRef paths = CFArrayCreateMutable(kCFAllocatorDefault, 0l, &kCFTypeArrayCallBacks);
      if (paths != NULL) {
        ATSFontRef font;
        OSStatus result;
        while ((result = ATSFontIteratorNext(iter, &font)) == noErr) {
          CFStringRef name;
          FSRef file;
          CFStringRef path = NULL;
          if (ATSFontGetName(font, kATSOptionFlagsDefault, &name) == noErr) {
            CFArrayAppendValue(names, name);
            CFRelease(name);
          } else {
            CFArrayAppendValue(names, kCFNull);
          }
          if (ATSFontGetFileReference(font, &file) == noErr) {
            const CFURLRef url = CFURLCreateFromFSRef(kCFAllocatorDefault, &file);
            if (url != NULL) {
              path = CFURLCopyFileSystemPath(url, (CFURLPathStyle) kCFURLPOSIXPathStyle);
              CFRelease(url);
            }
          }
          if (path != NULL) {
            CFArrayAppendValue(paths, path);
            CFRelease(path);
          } else {
            CFArrayAppendValue(paths, kCFNull);
          }
        }
        if (result == kATSIterationCompleted) {
          const CFIndex cnt = CFArrayGetCount(names);
          if (cnt > 0 && cnt < INT_MAX && cnt == CFArrayGetCount(paths)) {
            struct lltxplatform_fontinfo *const array = (struct lltxplatform_fontinfo *) calloc((size_t) cnt, sizeof(struct lltxplatform_fontinfo));
            if (array != NULL) {
              CFIndex i;
              status = 0;
              for (i = 0; i < cnt; ++i) {
                struct lltxplatform_fontinfo *const info = &array[i];
                const void *const raw_name = CFArrayGetValueAtIndex(names, i);
                const void *const raw_path = CFArrayGetValueAtIndex(paths, i);
                const CFStringRef name = raw_name != kCFNull ? (CFStringRef) raw_name : NULL;
                const CFStringRef path = raw_path != kCFNull ? (CFStringRef) raw_path : NULL;
                info->name = NULL;
                info->path = NULL;
                if (name != NULL) {
                  const CFIndex length = CFStringGetLength(name);
                  const CFIndex size = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
                  if (size > 1) {
                    char *const buffer = (char *) malloc((size_t) size);
                    if (buffer != NULL) {
                      if (CFStringGetCString(name, buffer, size, kCFStringEncodingUTF8)) {
                        info->name = buffer;
                      }
                    }
                  }
                }
                if (path != NULL) {
                  const CFIndex size = CFStringGetMaximumSizeOfFileSystemRepresentation(path);
                  if (size > 0) {
                    char *const buffer = (char *) malloc((size_t) size);
                    if (buffer != NULL) {
                      if (CFStringGetFileSystemRepresentation(path, buffer, size)) {
                        info->path = buffer;
                      }
                    }
                  }
                }
              }
              *fonts = array;
              *count = (unsigned int) cnt;
            }
          }
        }
        CFRelease(paths);
      }
      CFRelease(names);
    }
    ATSFontIteratorRelease(&iter);
  }
  return status;
}


int lltxplatform_get_inactive_fonts_impl(char ***const fonts, unsigned int *const count) {
  *fonts = NULL;
  *count = 0;
  return 0;
}
