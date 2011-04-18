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

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>

#include <stddef.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "fonts_impl.h"


static char *encode(LPCWSTR string, unsigned int length, UINT encoding, DWORD flags, LPBOOL default_char_used);
static char *encode_utf8(LPCWSTR string, unsigned int length);
static char *encode_legacy(LPCWSTR string, unsigned int length);


int lltxplatform_get_installed_fonts_impl(struct lltxplatform_fontinfo **const fonts, unsigned int *const count) {
  int status = -1;
  WCHAR directory[MAX_PATH];
  if (SUCCEEDED(SHGetFolderPathW(NULL, CSIDL_FONTS, NULL, SHGFP_TYPE_CURRENT, directory))) {
    const LPCWSTR path = L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts";
    HKEY handle;
    if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, path, 0, KEY_QUERY_VALUE, &handle) == ERROR_SUCCESS) {
      DWORD cnt, name_max, value_max;
      if (RegQueryInfoKeyW(handle, NULL, NULL, NULL, NULL, NULL, NULL, &cnt, &name_max, &value_max, NULL, NULL) == ERROR_SUCCESS && cnt > 0) {
        struct lltxplatform_fontinfo *const array = (struct lltxplatform_fontinfo *) calloc(cnt, sizeof(struct lltxplatform_fontinfo));
        const LPWSTR name = (LPWSTR) malloc((name_max + 1) * sizeof(WCHAR));
        const LPWSTR value = (LPWSTR) malloc(value_max);
        if (array != NULL) {
          unsigned int i;
          status = 0;
          for (i = 0; i < cnt; ++i) {
            struct lltxplatform_fontinfo *info = &array[i];
            DWORD name_size = name_max + 1;
            DWORD value_size = value_max;
            DWORD type;
            info->name = NULL;
            info->path = NULL;
            if (RegEnumValueW(handle, i, name, &name_size, NULL, &type, (LPBYTE) value, &value_size) == ERROR_SUCCESS && type == REG_SZ) {
              const unsigned int length = value_size / 2 - 1;
              info->name = encode_utf8(name, name_size);
              value[length] = L'\0';
              if (PathIsRelativeW(value)) {
                WCHAR buffer[MAX_PATH];
                wcscpy(buffer, directory);
                if (PathAppendW(buffer, value)) {
                  info->path = encode_legacy(buffer, (unsigned int) wcslen(buffer));
                }
              } else {
                info->path = encode_legacy(value, length);
              }
            }
          }
          *fonts = array;
          *count = cnt;
        }
        free(name);
        free(value);
      }
      RegCloseKey(handle);
    }
  }
  return status;
}


int lltxplatform_get_inactive_fonts_impl(char ***const fonts, unsigned int *const count) {
  int status = -1;
  const LPCWSTR path = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Font Management";
  HKEY handle;
  LONG result = RegOpenKeyExW(HKEY_CURRENT_USER, path, 0, KEY_QUERY_VALUE, &handle);
  if (result == ERROR_SUCCESS) {
    const LPCWSTR name = L"Inactive Fonts";
    DWORD type, size;
    result = RegQueryValueExW(handle, name, NULL, &type, NULL, &size);
    if (result == ERROR_SUCCESS && type == REG_MULTI_SZ) {
      const LPWSTR buffer = (LPWSTR) malloc(size);
      if (buffer != NULL) {
        result = RegQueryValueExW(handle, name, NULL, &type, (LPBYTE) buffer, &size);
        if (result == ERROR_SUCCESS && type == REG_MULTI_SZ) {
          const unsigned int length = size / sizeof(WCHAR);
          unsigned int cnt = 0;
          unsigned int i;
          for (i = 0; i < length - 1; ++i) {
            if (buffer[i] == L'\0') ++cnt;
          }
          *count = cnt;
          if (cnt > 0) {
            char **const array = (char **) calloc(cnt, sizeof(char *));
            if (array != NULL) {
              unsigned int j = 0;
              status = 0;
              for (i = 0; i < cnt && j < length - 1; ++i) {
                const unsigned int k = j;
                while (j < length - 1 && buffer[j] != L'\0') ++j;
                ++j;
                array[i] = encode_utf8(&buffer[k], j - k);
              }
              *fonts = array;
            }
          } else {
            status = 0;
            *fonts = NULL;
          }
        } else if (result == ERROR_FILE_NOT_FOUND) {
          status = 0;
          *count = 0;
          *fonts = NULL;
        }
        free(buffer);
      }
    }
    RegCloseKey(handle);
  } else if (result == ERROR_FILE_NOT_FOUND) {
    status = 0;
    *count = 0;
    *fonts = NULL;
  }
  return status;
}


static char *encode(const LPCWSTR string, const unsigned int length, const UINT encoding, const DWORD flags, const LPBOOL default_char_used) {
  const unsigned int size = 3 * length + 1;
  char *buffer = NULL;
  if (size < INT_MAX) {
    buffer = (char *) malloc(size);
    if (buffer != NULL) {
      const int result = WideCharToMultiByte(encoding, flags, string, (int) length, buffer, (int) size, NULL, default_char_used);
      if (result > 0 && (unsigned int) result < size - 1) {
        buffer[result] = '\0';
      } else {
        free(buffer);
        buffer = NULL;
      }
    }
  }
  return buffer;
}


static char *encode_utf8(const LPCWSTR string, const unsigned int length) {
  return encode(string, length, CP_UTF8, 0, NULL);
}


static char *encode_legacy(const LPCWSTR string, const unsigned int length) {
  const UINT encoding = AreFileApisANSI() ? CP_ACP : CP_OEMCP;
  BOOL default_char_used = FALSE;
  char *result = encode(string, length, encoding, WC_NO_BEST_FIT_CHARS, &default_char_used);
  if (result != NULL && default_char_used) {
    free(result);
    result = NULL;
  }
  return result;
}
