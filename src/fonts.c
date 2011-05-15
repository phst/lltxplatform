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
#include <limits.h>

#include <lua.h>
#include <lauxlib.h>

#include "fonts.h"
#include "fonts_impl.h"


int lltxplatform_get_installed_fonts(lua_State *const L) {
  struct lltxplatform_fontinfo *fonts = NULL;
  unsigned int count = 0;
  int results = 0;
  if (lltxplatform_get_installed_fonts_impl(&fonts, &count) == 0 && count < INT_MAX) {
    unsigned int i;
    lua_createtable(L, (signed int) count, 0);
    for (i = 0; i < count; ++i) {
      const struct lltxplatform_fontinfo *const info = &fonts[i];
      lua_createtable(L, 0, (info->name != NULL) + (info->path != NULL));
      if (info->name != NULL) {
        lua_pushstring(L, "name");
        lua_pushstring(L, info->name);
        free(info->name);
        lua_rawset(L, -3);
      }
      if (info->path != NULL) {
        lua_pushstring(L, "path");
        lua_pushstring(L, info->path);
        free(info->path);
        lua_rawset(L, -3);
      }
      lua_rawseti(L, -2, (signed int) i + 1);
    }
    results = 1;
  }
  if (fonts != NULL) {
    free(fonts);
  }
  return results;
}


int lltxplatform_get_inactive_fonts(lua_State *const L) {
  char **fonts = NULL;
  unsigned int count = 0;
  int results = 0;
  if (lltxplatform_get_inactive_fonts_impl(&fonts, &count) == 0 && count < INT_MAX) {
    unsigned int i;
    lua_createtable(L, (signed int) count, 0);
    for (i = 0; i < count; ++i) {
      char *const name = fonts[i];
      if (name != NULL) {
        lua_pushstring(L, name);
        free(name);
      } else {
        lua_pushboolean(L, 0);
      }
      lua_rawseti(L, -2, (signed int) i + 1);
    }
    results = 1;
  }
  if (fonts != NULL) {
    free(fonts);
  }
  return results;
}
