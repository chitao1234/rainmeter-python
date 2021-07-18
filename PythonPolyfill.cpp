#include <Python.h>
#include "PythonPolyfill.h"

// unicodeobject.c from Python 3.2.6
/*

Unicode implementation based on original code by Fredrik Lundh,
modified by Marc-Andre Lemburg <mal@lemburg.com> according to the
Unicode Integration Proposal (see file Misc/unicode.txt).

Major speed upgrades to the method implementations at the Reykjavik
NeedForSpeed sprint, by Fredrik Lundh and Andrew Dalke.

Copyright (c) Corporation for National Research Initiatives.

--------------------------------------------------------------------
The original string type implementation is:

  Copyright (c) 1999 by Secret Labs AB
  Copyright (c) 1999 by Fredrik Lundh

By obtaining, using, and/or copying this software and/or its
associated documentation, you agree that you have read, understood,
and will comply with the following terms and conditions:

Permission to use, copy, modify, and distribute this software and its
associated documentation for any purpose and without fee is hereby
granted, provided that the above copyright notice appears in all
copies, and that both that copyright notice and this permission notice
appear in supporting documentation, and that the name of Secret Labs
AB or the author not be used in advertising or publicity pertaining to
distribution of the software without specific, written prior
permission.

SECRET LABS AB AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS.  IN NO EVENT SHALL SECRET LABS AB OR THE AUTHOR BE LIABLE FOR
ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
--------------------------------------------------------------------

*/
#if PY_VERSION_HEX < 0x03020000  // lower than Python 3.2
wchar_t*
PyUnicode_AsWideCharString(PyObject* unicode, Py_ssize_t* size)
{
    wchar_t* buffer;
    Py_ssize_t buflen;

    if (unicode == NULL) {
        PyErr_BadInternalCall();
        return NULL;
    }

    buflen = PyUnicode_AsWideChar((PyUnicodeObject*)unicode, NULL, 0);
    if (PY_SSIZE_T_MAX / sizeof(wchar_t) < buflen) {
        PyErr_NoMemory();
        return NULL;
    }

    buffer = (wchar_t*)PyMem_MALLOC(buflen * sizeof(wchar_t));
    if (buffer == NULL) {
        PyErr_NoMemory();
        return NULL;
    }
    buflen = PyUnicode_AsWideChar((PyUnicodeObject*)unicode, buffer, buflen);
    if (size != NULL)
        *size = buflen;
    return buffer;
}

// fileutils.c from Python 3.2.6
FILE*
_Py_wfopen(const wchar_t* path, const wchar_t* mode)
{
#ifndef MS_WINDOWS
    FILE* f;
    char* cpath;
    char cmode[10];
    size_t r;
    r = wcstombs(cmode, mode, 10);
    if (r == (size_t)-1 || r >= 10) {
        errno = EINVAL;
        return NULL;
    }
    cpath = _Py_wchar2char(path, NULL);
    if (cpath == NULL)
        return NULL;
    f = fopen(cpath, cmode);
    PyMem_Free(cpath);
    return f;
#else
    return _wfopen(path, mode);
#endif
}
#endif