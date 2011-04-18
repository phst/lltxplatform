The ``lualatex-platform`` package
=================================

LuaTeX_ does not contain platform-specific code, and while often
platform-specific problems can be handled purely in Lua_, it sometimes happens
that access to the native operating system API is required.  For such tasks,
Lua allows loading *extension modules* written in C (or other languages
provided that they can produce a dynamic library that exports a certain C-style
function).  The ``lualatex-platform`` package aims to provide such an extension
module that is usable from both Lua and LuaTeX and adds a small set of features
not available otherwise.  At the moment, it merely supports listing the fonts
known to the operating system.

.. _Lua: http://lua.org/
.. _LuaTeX: http://luatex.org/


Installation
------------

The whole bundle consists of a LaTeX_ package ``lualatex-platform.sty``, a
LuaLaTeX_ module ``lualatex-platform.lua``, and the actual C extension module
``lltxplatform.(dll|so)``.  The extension module should compile for recent
versions of Windows (with MinGW_), OS X, and Linux.  The compilation process is
based on the `GNU build system`_, so you should be able to compile the module with
the commands::

  ./configure && make

The standard C headers and the Lua headers (``lua.h`` etc.) must be present in
the include path.  On Windows, the compiler needs to access the Lua import
library (``lua51.lib`` or ``liblua51.a``), which must be located in the library
search path.  Run ``./configure --help`` for more options.  On Linux, the
compiler needs to access the FontConfig_ header files
(``fontconfig/fontconfig.h``) and library (``libfontconfig.so``), which must be
present in the respective search paths.

After compiling, make sure that the files ``lualatex-platform.sty``,
``lualatex-platform.lua``, and ``lltxplatform.(dll|so)`` can be found by
LuaLaTeX.

.. _LaTeX: http://www.latex-project.org/
.. _LuaLaTeX: http://mirror.ctan.org/info/luatex/lualatex-doc/lualatex-doc.pdf
.. _GNU build system: http://en.wikipedia.org/wiki/GNU_build_system
.. _MinGW: http://mingw.org/
.. _FontConfig: http://fontconfig.org/


Usage
-----

In a LaTeX document or package, load the ``lualatex-platform`` package with::

  \usepackage{lualatex-platform}

(or ``\RequirePackage``, of course).  The document should be compiled with the
LuaTeX engine, and the ``luatexbase`` module has to be installed.

You can also load the Lua module directly::

  \RequireLuaModule{lualatex-platform}

or, from Lua::

  luatexbase.require_module "lualatex-platform"

After the module has been loaded, the table ``lualatex.platform`` contains the
functions described below.

Note that on OS X, loading extension modules currently requires_ a
custom-build LuaTeX binary.

.. _requires: http://tracker.luatex.org/view.php?id=555


Querying system fonts
---------------------

The module table ``lualatex.platform`` contains two parameterless functions
related to operating system font listing:

* ``get_installed_fonts()`` returns either ``nil`` (in the case of an error),
  or an array of tables each of which represents one font known to the
  operating system and contains an arbitrary combination of the following
  items:

  - An item with key ``name``: The value of this item is a string containing
    the name of the font as reported by the operating system.  This name is
    platform-dependent and largely arbitrary; there is no guarantee that the
    name is present or meaningful, and the names are not guaranteed to be
    unique within the fonts array.  On Windows and OS X, the string is encoded
    in UTF-8; on Linux, the encoding depends on the configuration of the
    operating system.  If the font name cannot be obtained, this field is
    ``nil``.

  - An item with key ``path``: The value of this item is a string containing
    the file path where the font is located.  There is no guarantee that a file
    with this name exists and contains a valid font, and the paths are not
    guaranteed to be unique within the fonts array.  On Windows, relative path
    names are assumed to be relative to the operating system font directory.
    There is no guarantee about whether the path is relative or which path
    syntax is used.  On Windows, the string is encoded in the legacy 8-bit file
    system encoding (“`ANSI code page`_”) because the C standard library
    functions in the Microsoft runtime don’t accept Unicode strings; if the
    file name cannot be represented in the legacy encoding, this field is
    ``nil``.  On OS X and Linux, the string is encoded in the current file
    system encoding.  If the path cannot be obtained, this field is ``nil``.

  The array is in arbitrary order, and duplicates are possible.

* ``get_inactive_fonts()`` returns either ``nil`` (in the case of an error), or
  an array of strings each of which represents one font treated as inactive by
  the operating system.  The concept of *inactive fonts* exists only on
  Windows 7 and upwards; the inactive fonts are still present and usable, but
  are hidden in standard UI elements.  The array of inactive fonts is in
  arbitrary order and the font names are not necessarily unique.  There is no
  guarantee that the names returned by ``get_inactive_fonts()`` correspond to
  actual fonts installed in the system, or to the font names returned by
  ``get_installed_fonts()``.  The font names are encoded in UTF-8.

.. _ANSI code page: http://en.wikipedia.org/wiki/Windows_code_page
