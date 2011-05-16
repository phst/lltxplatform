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

#include <stdlib.h>
#include <string.h>

#include <fontconfig/fontconfig.h>

#include "fonts_impl.h"


int lltxplatform_get_installed_fonts_impl(struct lltxplatform_fontinfo **const fonts, unsigned int *const count) {
  int status = -1;
  FcPattern *const pattern = FcPatternCreate();
  if (pattern != NULL) {
    FcObjectSet *const objects = FcObjectSetBuild(FC_FULLNAME, FC_FILE, NULL);
    if (objects != NULL) {
      FcFontSet *const list = FcFontList(NULL, pattern, objects);
      if (list != NULL) {
        if (list->nfont > 0) {
          const unsigned int cnt = (unsigned int) list->nfont;
          struct lltxplatform_fontinfo *const array = (struct lltxplatform_fontinfo *) calloc((size_t) cnt, sizeof(struct lltxplatform_fontinfo));
          if (array != NULL) {
            unsigned int i;
            status = 0;
            for (i = 0; i < cnt; ++i) {
              struct lltxplatform_fontinfo *const info = &array[i];
              FcPattern *const font = list->fonts[i];
              FcChar8 *value;
              if (FcPatternGetString(font, FC_FULLNAME, 0, &value) == FcResultMatch) {
                info->name = strdup((char *) value);
              } else {
                info->name = NULL;
              }
              if (FcPatternGetString(font, FC_FILE, 0, &value) == FcResultMatch) {
                info->path = strdup((char *) value);
              } else {
                info->path = NULL;
              }
            }
            *count = cnt;
            *fonts = array;
          }
        } else {
          status = 0;
          *count = 0;
          *fonts = NULL;
        }
        FcFontSetDestroy(list);
      }
      FcObjectSetDestroy(objects);
    }
    FcPatternDestroy(pattern);
  }
  return status;
}


int lltxplatform_get_inactive_fonts_impl(char ***const fonts, unsigned int *const count) {
  *fonts = NULL;
  *count = 0;
  return 0;
}
