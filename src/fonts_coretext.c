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
  const CFArrayRef urls = CTFontManagerCopyAvailableFontURLs();
  if (urls != NULL) {
    const CFIndex cnt = CFArrayGetCount(urls);
    if (cnt > 0 && cnt < INT_MAX) {
      struct lltxplatform_fontinfo *const array = (struct lltxplatform_fontinfo *) calloc((size_t) cnt, sizeof(struct lltxplatform_fontinfo));
      if (array != NULL) {
        const CFStringRef empty = CFSTR("");
        if (empty != NULL) {
          const CFStringRef prefix = CFSTR("postscript-name=");
          if (prefix != NULL) {
            const CFIndex prefix_len = CFStringGetLength(prefix);
            CFIndex i;
            status = 0;
            for (i = 0; i < cnt; ++i) {
              struct lltxplatform_fontinfo *const info = &array[i];
              const CFURLRef url = (CFURLRef) CFArrayGetValueAtIndex(urls, i);
              info->name = NULL;
              info->path = NULL;
              if (url != NULL) {
                const CFStringRef path = CFURLCopyFileSystemPath(url, (CFURLPathStyle) kCFURLPOSIXPathStyle);
                const CFStringRef fragment = CFURLCopyFragment(url, empty);
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
                  CFRelease(path);
                }
                if (fragment != NULL) {
                  if (CFStringHasPrefix(fragment, prefix)) {
                    const CFIndex fragment_len = CFStringGetLength(fragment);
                    const CFRange range = CFRangeMake(prefix_len, fragment_len - prefix_len);
                    const CFStringRef name = CFStringCreateWithSubstring(NULL, fragment, range);
                    if (name != NULL) {
                      const CFIndex name_len = CFStringGetLength(name);
                      const CFIndex size = CFStringGetMaximumSizeForEncoding(name_len, kCFStringEncodingUTF8) + 1;
                      if (size > 1) {
                        char *const buffer = (char *) malloc((size_t) size);
                        if (buffer != NULL) {
                          if (CFStringGetCString(name, buffer, size, kCFStringEncodingUTF8)) {
                            info->name = buffer;
                          }
                        }
                      }
                      CFRelease(name);
                    }
                  }
                  CFRelease(fragment);
                }
              }
            }
            *fonts = array;
            *count = (unsigned int) cnt;
            CFRelease(prefix);
          }
          CFRelease(empty);
        }
      }
    }
    CFRelease(urls);
  }
  return status;
}


int lltxplatform_get_inactive_fonts_impl(char ***const fonts, unsigned int *const count) {
  *fonts = NULL;
  *count = 0;
  return 0;
}
