/***
*tchar.h - definitions for generic international text functions
*
*       Copyright (c) Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Definitions for generic international functions, mostly defines
*       which map string/formatted-io/ctype functions to char, wchar_t, or
*       MBCS versions.  To be used for compatibility between single-byte,
*       multi-byte and Unicode text models.
*
*       [Public]
*
****/

/*
** $Id: 2009/02/07, Taras Tovchenko $
** Changed for Mac OS and iPhone OS support
** See Copyright Notice in dCocoaCore.h
*/

#if     _MSC_VER > 1000
#pragma once
#endif

#ifndef _INC_TCHAR
#define _INC_TCHAR

#ifdef _STRSAFE_H_INCLUDED_
#error Need to include strsafe.h after tchar.h
#endif

#ifdef  _MSC_VER
#pragma warning(disable:4514)       /* disable unwanted C++ /W4 warning */
/* #pragma warning(default:4514) */ /* use this to reenable, if necessary */
#endif  /* _MSC_VER */

/* Notes */

/* There is no:
 *      _tcscat_l
 *      _tcscpy_l
 * because mbscat and mbscpy just behave like strcat and strcpy,
 * so no special locale-specific behavior is needed.
 */

/* Functions like:
 *      _strncat_l
 *      _strncat_s_l
 * are only available if ANSI is defined (i.e. no _UNICODE nor _MBCS),
 * because these functions are only accessible through the _tcs macros.
 */

#ifdef  __cplusplus
extern "C" {
#endif

#define _ftcscat    _tcscat
#define _ftcschr    _tcschr
#define _ftcscpy    _tcscpy
#define _ftcscspn   _tcscspn
#define _ftcslen    _tcslen
#define _ftcsncat   _tcsncat
#define _ftcsncpy   _tcsncpy
#define _ftcspbrk   _tcspbrk
#define _ftcsrchr   _tcsrchr
#define _ftcsspn    _tcsspn
#define _ftcsstr    _tcsstr
#define _ftcstok    _tcstok

#define _ftcsdup    _tcsdup
#define _ftcsnset   _tcsnset
#define _ftcsrev    _tcsrev
#define _ftcsset    _tcsset

#define _ftcscmp      _tcscmp
#define _ftcsicmp     _tcsicmp
#define _ftcsnccmp    _tcsnccmp
#define _ftcsncmp     _tcsncmp
#define _ftcsncicmp   _tcsncicmp
#define _ftcsnicmp    _tcsnicmp

#define _ftcscoll     _tcscoll
#define _ftcsicoll    _tcsicoll
#define _ftcsnccoll   _tcsnccoll
#define _ftcsncoll    _tcsncoll
#define _ftcsncicoll  _tcsncicoll
#define _ftcsnicoll   _tcsnicoll

/* Redundant "logical-character" mappings */

#define _ftcsclen   _tcsclen
#define _ftcsnccat  _tcsnccat
#define _ftcsnccpy  _tcsnccpy
#define _ftcsncset  _tcsncset

#define _ftcsdec    _tcsdec
#define _ftcsinc    _tcsinc
#define _ftcsnbcnt  _tcsnbcnt
#define _ftcsnccnt  _tcsnccnt
#define _ftcsnextc  _tcsnextc
#define _ftcsninc   _tcsninc
#define _ftcsspnp   _tcsspnp

#define _ftcslwr    _tcslwr
#define _ftcsupr    _tcsupr

#define _ftclen     _tclen
#define _ftccpy     _tccpy
#define _ftccmp     _tccmp

#ifndef _CONST_RETURN
#ifdef  __cplusplus
#define _CONST_RETURN  const
#define _CRT_CONST_CORRECT_OVERLOADS
#else
#define _CONST_RETURN
#endif
#endif

/* For backwards compatibility */
#define _WConst_return _CONST_RETURN

#ifdef  _UNICODE

#ifdef  __cplusplus
}   /* ... extern "C" */
#endif

/* ++++++++++++++++++++ UNICODE ++++++++++++++++++++ */

#include <wchar.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef _WCTYPE_T_DEFINED
typedef unsigned short wint_t;
typedef unsigned short wctype_t;
#define _WCTYPE_T_DEFINED
#endif

#ifndef __TCHAR_DEFINED
typedef wchar_t     _TCHAR;
typedef wchar_t     _TSCHAR;
typedef wchar_t     _TUCHAR;
typedef wchar_t     _TXCHAR;
typedef wint_t      _TINT;
#define __TCHAR_DEFINED
#endif

#ifndef _TCHAR_DEFINED
typedef wchar_t     TCHAR;
#define _TCHAR_DEFINED
#endif

#define _TEOF       WEOF

#define __T(x)      L ## x


/* Program */

#define _tmain      wmain
#define _tWinMain   wWinMain
#define _tenviron   _wenviron
#define __targv     __wargv

/* Formatted i/o */

#define _tprintf        wprintf
#define _tprintf_l      _wprintf_l
#define _tprintf_s      wprintf_s
#define _tprintf_s_l    _wprintf_s_l
#define _tprintf_p      _wprintf_p
#define _tprintf_p_l    _wprintf_p_l
#define _tcprintf       _cwprintf
#define _tcprintf_l     _cwprintf_l
#define _tcprintf_s     _cwprintf_s
#define _tcprintf_s_l   _cwprintf_s_l
#define _tcprintf_p     _cwprintf_p
#define _tcprintf_p_l   _cwprintf_p_l
#define _vtcprintf      _vcwprintf
#define _vtcprintf_l    _vcwprintf_l
#define _vtcprintf_s    _vcwprintf_s
#define _vtcprintf_s_l  _vcwprintf_s_l
#define _vtcprintf_p    _vcwprintf_p
#define _vtcprintf_p_l  _vcwprintf_p_l
#define _ftprintf       fwprintf
#define _ftprintf_l     _fwprintf_l
#define _ftprintf_s     fwprintf_s
#define _ftprintf_s_l   _fwprintf_s_l
#define _ftprintf_p     _fwprintf_p
#define _ftprintf_p_l   _fwprintf_p_l
#define _stprintf       _swprintf
#define _stprintf_l     __swprintf_l
#define _stprintf_s     swprintf_s
#define _stprintf_s_l   _swprintf_s_l
#define _stprintf_p     _swprintf_p
#define _stprintf_p_l   _swprintf_p_l
#define _sctprintf      _scwprintf
#define _sctprintf_l    _scwprintf_l
#define _sctprintf_p    _scwprintf_p
#define _sctprintf_p_l  _scwprintf_p_l
#define _sntprintf      _snwprintf
#define _sntprintf_l    _snwprintf_l
#define _sntprintf_s    _snwprintf_s
#define _sntprintf_s_l  _snwprintf_s_l
#define _vtprintf       vwprintf
#define _vtprintf_l     _vwprintf_l
#define _vtprintf_s     vwprintf_s
#define _vtprintf_s_l   _vwprintf_s_l
#define _vtprintf_p     _vwprintf_p
#define _vtprintf_p_l   _vwprintf_p_l
#define _vftprintf      vfwprintf
#define _vftprintf_l    _vfwprintf_l
#define _vftprintf_s    vfwprintf_s
#define _vftprintf_s_l  _vfwprintf_s_l
#define _vftprintf_p    _vfwprintf_p
#define _vftprintf_p_l  _vfwprintf_p_l
#define _vstprintf      _vswprintf
#define _vstprintf_l    _vswprintf_l
#define _vstprintf_s    vswprintf_s
#define _vstprintf_s_l  _vswprintf_s_l
#define _vstprintf_p    _vswprintf_p
#define _vstprintf_p_l  _vswprintf_p_l
#define _vsctprintf     _vscwprintf
#define _vsctprintf_l   _vscwprintf_l
#define _vsctprintf_p   _vscwprintf_p
#define _vsctprintf_p_l _vscwprintf_p_l
#define _vsntprintf     _vsnwprintf
#define _vsntprintf_l   _vsnwprintf_l
#define _vsntprintf_s   _vsnwprintf_s
#define _vsntprintf_s_l _vsnwprintf_s_l

#define _tscanf         wscanf
#define _tscanf_l       _wscanf_l
#define _tscanf_s       wscanf_s
#define _tscanf_s_l     _wscanf_s_l
#define _tcscanf        _cwscanf
#define _tcscanf_l      _cwscanf_l
#define _tcscanf_s      _cwscanf_s
#define _tcscanf_s_l    _cwscanf_s_l
#define _ftscanf        fwscanf
#define _ftscanf_l      _fwscanf_l
#define _ftscanf_s      fwscanf_s
#define _ftscanf_s_l    _fwscanf_s_l
#define _stscanf        swscanf
#define _stscanf_l      _swscanf_l
#define _stscanf_s      swscanf_s
#define _stscanf_s_l    _swscanf_s_l
#define _sntscanf       _snwscanf
#define _sntscanf_l     _snwscanf_l
#define _sntscanf_s     _snwscanf_s
#define _sntscanf_s_l   _snwscanf_s_l


/* Unformatted i/o */

#define _fgettc         fgetwc
#define _fgettc_nolock  _fgetwc_nolock
#define _fgettchar      _fgetwchar
#define _fgetts         fgetws
#define _fputtc         fputwc
#define _fputtc_nolock  _fputwc_nolock
#define _fputtchar      _fputwchar
#define _fputts         fputws
#define _cputts         _cputws
#define _cgetts         _cgetws
#define _cgetts_s       _cgetws_s
#define _gettc          getwc
#define _gettc_nolock   _getwc_nolock
#define _gettch         _getwch
#define _gettch_nolock  _getwch_nolock
#define _gettche        _getwche
#define _gettche_nolock _getwche_nolock
#define _gettchar       getwchar
#define _gettchar_nolock _getwchar_nolock
#define _getts          _getws
#define _getts_s        _getws_s
#define _puttc          putwc
#define _puttc_nolock   _putwc_nolock
#define _puttchar       putwchar
#define _puttchar_nolock _putwchar_nolock
#define _puttch         _putwch
#define _puttch_nolock  _putwch_nolock
#define _putts          _putws
#define _ungettc        ungetwc
#define _ungettc_nolock _ungetwc_nolock
#define _ungettch       _ungetwch
#define _ungettch_nolock _ungetwch_nolock


/* String conversion functions */

#define _tcstod     wcstod
#define _tcstol     wcstol
#define _tcstoul    wcstoul
#define _tcstoi64   _wcstoi64
#define _tcstoui64  _wcstoui64
#define _tstof      _wtof
#define _tstol      _wtol
#define _tstoi      _wtoi
#define _tstoi64    _wtoi64
#define _tcstod_l     _wcstod_l
#define _tcstol_l     _wcstol_l
#define _tcstoul_l    _wcstoul_l
#define _tcstoi64_l   _wcstoi64_l
#define _tcstoui64_l  _wcstoui64_l
#define _tstof_l      _wtof_l
#define _tstol_l      _wtol_l
#define _tstoi_l      _wtoi_l
#define _tstoi64_l    _wtoi64_l

#define _itot_s     _itow_s
#define _ltot_s     _ltow_s
#define _ultot_s    _ultow_s
#define _itot       _itow
#define _ltot       _ltow
#define _ultot      _ultow
#define _ttoi       _wtoi
#define _ttol       _wtol

#define _ttoi64     _wtoi64
#define _i64tot_s   _i64tow_s
#define _ui64tot_s  _ui64tow_s
#define _i64tot     _i64tow
#define _ui64tot    _ui64tow

/* String functions */

#define _tcscat         wcscat
#define _tcscat_s       wcscat_s
#define _tcschr         wcschr
#define _tcscpy         wcscpy
#define _tcscpy_s       wcscpy_s
#define _tcscspn        wcscspn
#define _tcslen         wcslen
#define _tcsnlen        wcsnlen
#define _tcsncat        wcsncat
#define _tcsncat_s      wcsncat_s
#define _tcsncat_l      _wcsncat_l
#define _tcsncat_s_l    _wcsncat_s_l
#define _tcsncpy        wcsncpy
#define _tcsncpy_s      wcsncpy_s
#define _tcsncpy_l      _wcsncpy_l
#define _tcsncpy_s_l    _wcsncpy_s_l
#define _tcspbrk        wcspbrk
#define _tcsrchr        wcsrchr
#define _tcsspn         wcsspn
#define _tcsstr         wcsstr
#define _tcstok         wcstok
#define _tcstok_s       wcstok_s
#define _tcstok_l       _wcstok_l
#define _tcstok_s_l     _wcstok_s_l
#define _tcserror       _wcserror
#define _tcserror_s     _wcserror_s
#define __tcserror      __wcserror
#define __tcserror_s    __wcserror_s

#define _tcsdup         _wcsdup
#define _tcsnset        _wcsnset
#define _tcsnset_s      _wcsnset_s
#define _tcsnset_l      _wcsnset_l
#define _tcsnset_s_l    _wcsnset_s_l
#define _tcsrev         _wcsrev
#define _tcsset         _wcsset
#define _tcsset_s       _wcsset_s
#define _tcsset_l       _wcsset_l
#define _tcsset_s_l     _wcsset_s_l

#define _tcscmp         wcscmp
#define _tcsicmp        _wcsicmp
#define _tcsicmp_l      _wcsicmp_l
#define _tcsnccmp       wcsncmp
#define _tcsncmp        wcsncmp
#define _tcsncicmp      _wcsnicmp
#define _tcsncicmp_l    _wcsnicmp_l
#define _tcsnicmp       _wcsnicmp
#define _tcsnicmp_l     _wcsnicmp_l

#define _tcscoll        wcscoll
#define _tcscoll_l      _wcscoll_l
#define _tcsicoll       _wcsicoll
#define _tcsicoll_l     _wcsicoll_l
#define _tcsnccoll      _wcsncoll
#define _tcsnccoll_l    _wcsncoll_l
#define _tcsncoll       _wcsncoll
#define _tcsncoll_l     _wcsncoll_l
#define _tcsncicoll     _wcsnicoll
#define _tcsncicoll_l   _wcsnicoll_l
#define _tcsnicoll      _wcsnicoll
#define _tcsnicoll_l    _wcsnicoll_l

#ifdef _DEBUG
#define _tcsdup_dbg _wcsdup_dbg
#endif

/* Execute functions */

#define _texecl     _wexecl
#define _texecle    _wexecle
#define _texeclp    _wexeclp
#define _texeclpe   _wexeclpe
#define _texecv     _wexecv
#define _texecve    _wexecve
#define _texecvp    _wexecvp
#define _texecvpe   _wexecvpe

#define _tspawnl    _wspawnl
#define _tspawnle   _wspawnle
#define _tspawnlp   _wspawnlp
#define _tspawnlpe  _wspawnlpe
#define _tspawnv    _wspawnv
#define _tspawnve   _wspawnve
#define _tspawnvp   _wspawnvp
#define _tspawnvp   _wspawnvp
#define _tspawnvpe  _wspawnvpe

#define _tsystem    _wsystem


/* Time functions */

#define _tasctime   _wasctime
#define _tctime     _wctime
#define _tctime32   _wctime32
#define _tctime64   _wctime64
#define _tstrdate   _wstrdate
#define _tstrtime   _wstrtime
#define _tutime     _wutime
#define _tutime32   _wutime32
#define _tutime64   _wutime64
#define _tcsftime   wcsftime
#define _tcsftime_l _wcsftime_l

#define _tasctime_s   _wasctime_s
#define _tctime_s     _wctime_s
#define _tctime32_s   _wctime32_s
#define _tctime64_s   _wctime64_s
#define _tstrdate_s   _wstrdate_s
#define _tstrtime_s   _wstrtime_s

/* Directory functions */

#define _tchdir             _wchdir
#define _tgetcwd            _wgetcwd
#define _tgetdcwd           _wgetdcwd
#define _tgetdcwd_nolock    _wgetdcwd_nolock
#define _tmkdir             _wmkdir
#define _trmdir             _wrmdir

#ifdef _DEBUG
#define _tgetcwd_dbg        _wgetcwd_dbg
#define _tgetdcwd_dbg       _wgetdcwd_dbg
#define _tgetdcwd_lk_dbg    _wgetdcwd_lk_dbg
#endif

/* Environment/Path functions */

#define _tfullpath      _wfullpath
#define _tgetenv        _wgetenv
#define _tgetenv_s      _wgetenv_s
#define _tdupenv_s      _wdupenv_s
#define _tmakepath      _wmakepath
#define _tmakepath_s    _wmakepath_s
#define _tpgmptr        _wpgmptr
#define _get_tpgmptr    _get_wpgmptr
#define _tputenv        _wputenv
#define _tputenv_s      _wputenv_s
#define _tsearchenv     _wsearchenv
#define _tsearchenv_s   _wsearchenv_s
#define _tsplitpath     _wsplitpath
#define _tsplitpath_s   _wsplitpath_s

#ifdef _DEBUG
#define _tfullpath_dbg  _wfullpath_dbg
#define _tdupenv_s_dbg  _wdupenv_s_dbg
#endif

/* Stdio functions */

#define _tfdopen    _wfdopen
#define _tfsopen    _wfsopen
#define _tfopen     _wfopen
#define _tfopen_s   _wfopen_s
#define _tfreopen   _wfreopen
#define _tfreopen_s _wfreopen_s
#define _tperror    _wperror
#define _tpopen     _wpopen
#define _ttempnam   _wtempnam
#define _ttmpnam    _wtmpnam
#define _ttmpnam_s  _wtmpnam_s

#ifdef _DEBUG
#define _ttempnam_dbg   _wtempnam_dbg
#endif


/* Io functions */

#define _taccess    _waccess
#define _taccess_s  _waccess_s
#define _tchmod     _wchmod
#define _tcreat     _wcreat
#define _tfindfirst       _wfindfirst
#define _tfindfirst32     _wfindfirst32
#define _tfindfirst64     _wfindfirst64
#define _tfindfirsti64    _wfindfirsti64
#define _tfindfirst32i64  _wfindfirst32i64
#define _tfindfirst64i32  _wfindfirst64i32
#define _tfindnext        _wfindnext
#define _tfindnext32      _wfindnext32
#define _tfindnext64      _wfindnext64
#define _tfindnexti64     _wfindnexti64
#define _tfindnext32i64   _wfindnext32i64
#define _tfindnext64i32   _wfindnext64i32
#define _tmktemp    _wmktemp
#define _tmktemp_s  _wmktemp_s
#define _topen      _wopen
#define _tremove    _wremove
#define _trename    _wrename
#define _tsopen     _wsopen
#define _tsopen_s   _wsopen_s
#define _tunlink    _wunlink

#define _tfinddata_t      _wfinddata_t
#define _tfinddata32_t    _wfinddata32_t
#define _tfinddata64_t    _wfinddata64_t
#define _tfinddatai64_t   _wfinddatai64_t
#define _tfinddata32i64_t _wfinddata32i64_t
#define _tfinddata64i32_t _wfinddata64i32_t


/* Stat functions */

#define _tstat      _wstat
#define _tstat32    _wstat32
#define _tstat32i64 _wstat32i64
#define _tstat64    _wstat64
#define _tstat64i32 _wstat64i32
#define _tstati64   _wstati64


/* Setlocale functions */

#define _tsetlocale _wsetlocale


/* Redundant "logical-character" mappings */

#define _tcsclen        wcslen
#define _tcscnlen       wcsnlen
#define _tcsclen_l(_String, _Locale) wcslen(_String)
#define _tcscnlen_l(_String, _Max_count, _Locale) wcsnlen((_String), (_Max_count))
#define _tcsnccat       wcsncat
#define _tcsnccat_s     wcsncat_s
#define _tcsnccat_l     _wcsncat_l
#define _tcsnccat_s_l   _wcsncat_s_l
#define _tcsnccpy       wcsncpy
#define _tcsnccpy_s     wcsncpy_s
#define _tcsnccpy_l     _wcsncpy_l
#define _tcsnccpy_s_l   _wcsncpy_s_l
#define _tcsncset       _wcsnset
#define _tcsncset_s     _wcsnset_s
#define _tcsncset_l     _wcsnset_l
#define _tcsncset_s_l   _wcsnset_s_l

#define _tcsdec     _wcsdec
#define _tcsinc     _wcsinc
#define _tcsnbcnt   _wcsncnt
#define _tcsnccnt   _wcsncnt
#define _tcsnextc   _wcsnextc
#define _tcsninc    _wcsninc
#define _tcsspnp    _wcsspnp

#define _tcslwr     _wcslwr
#define _tcslwr_l   _wcslwr_l
#define _tcslwr_s   _wcslwr_s
#define _tcslwr_s_l _wcslwr_s_l
#define _tcsupr     _wcsupr
#define _tcsupr_l   _wcsupr_l
#define _tcsupr_s   _wcsupr_s
#define _tcsupr_s_l _wcsupr_s_l
#define _tcsxfrm    wcsxfrm
#define _tcsxfrm_l  _wcsxfrm_l



/* ctype functions */

#define _istalnum   iswalnum
#define _istalnum_l   _iswalnum_l
#define _istalpha   iswalpha
#define _istalpha_l   _iswalpha_l
#define _istascii   iswascii
#define _istcntrl   iswcntrl
#define _istcntrl_l   _iswcntrl_l
#define _istdigit   iswdigit
#define _istdigit_l   _iswdigit_l
#define _istgraph   iswgraph
#define _istgraph_l   _iswgraph_l
#define _istlower   iswlower
#define _istlower_l   _iswlower_l
#define _istprint   iswprint
#define _istprint_l   _iswprint_l
#define _istpunct   iswpunct
#define _istpunct_l   _iswpunct_l
#define _istspace   iswspace
#define _istspace_l   _iswspace_l
#define _istupper   iswupper
#define _istupper_l   _iswupper_l
#define _istxdigit  iswxdigit
#define _istxdigit_l  _iswxdigit_l

#define _totupper   towupper
#define _totupper_l   _towupper_l
#define _totlower   towlower
#define _totlower_l   _towlower_l

#define _istlegal(_Char)                (1)
#define _istlead(_Char)                 (0)
#define _istleadbyte(_Char)             (0)
#define _istleadbyte_l(_Char, _Locale)  (0)




#else   /* ndef _UNICODE */

/* ++++++++++++++++++++ SBCS and MBCS ++++++++++++++++++++ */

#ifdef  __cplusplus
}   /* ... extern "C" */
#endif

#include <string.h>

#ifdef  __cplusplus
extern "C" {
#endif


#define _TEOF       EOF

#define __T(x)      x


/* Program */

#define _tmain      main
#define _tWinMain   WinMain
#ifdef  _POSIX_
#define _tenviron   environ
#else
#define _tenviron  _environ
#endif
#define __targv     __argv


/* Formatted i/o */

#define _tprintf        printf
#define _tprintf_l      _printf_l
#define _tprintf_s      printf_s
#define _tprintf_s_l    _printf_s_l
#define _tprintf_p      _printf_p
#define _tprintf_p_l    _printf_p_l
#define _tcprintf       _cprintf
#define _tcprintf_l     _cprintf_l
#define _tcprintf_s     _cprintf_s
#define _tcprintf_s_l   _cprintf_s_l
#define _tcprintf_p     _cprintf_p
#define _tcprintf_p_l   _cprintf_p_l
#define _vtcprintf      _vcprintf
#define _vtcprintf_l    _vcprintf_l
#define _vtcprintf_s    _vcprintf_s
#define _vtcprintf_s_l  _vcprintf_s_l
#define _vtcprintf_p    _vcprintf_p
#define _vtcprintf_p_l  _vcprintf_p_l
#define _ftprintf       fprintf
#define _ftprintf_l     _fprintf_l
#define _ftprintf_s     fprintf_s
#define _ftprintf_s_l   _fprintf_s_l
#define _ftprintf_p     _fprintf_p
#define _ftprintf_p_l   _fprintf_p_l
#define _stprintf       sprintf
#define _stprintf_l     _sprintf_l
#define _stprintf_s     sprintf_s
#define _stprintf_s_l   _sprintf_s_l
#define _stprintf_p     _sprintf_p
#define _stprintf_p_l   _sprintf_p_l
#define _sctprintf      _scprintf
#define _sctprintf_l    _scprintf_l
#define _sctprintf_p    _scprintf_p
#define _sctprintf_p_l  _scprintf_p_l
#define _sntprintf      _snprintf
#define _sntprintf_l    _snprintf_l
#define _sntprintf_s    _snprintf_s
#define _sntprintf_s_l  _snprintf_s_l
#define _vtprintf       vprintf
#define _vtprintf_l     _vprintf_l
#define _vtprintf_s     vprintf_s
#define _vtprintf_s_l   _vprintf_s_l
#define _vtprintf_p     _vprintf_p
#define _vtprintf_p_l   _vprintf_p_l
#define _vftprintf      vfprintf
#define _vftprintf_l    _vfprintf_l
#define _vftprintf_s    vfprintf_s
#define _vftprintf_s_l  _vfprintf_s_l
#define _vftprintf_p    _vfprintf_p
#define _vftprintf_p_l  _vfprintf_p_l
#define _vstprintf      vsprintf
#define _vstprintf_l    _vsprintf_l
#define _vstprintf_s    vsprintf_s
#define _vstprintf_s_l  _vsprintf_s_l
#define _vstprintf_p    _vsprintf_p
#define _vstprintf_p_l  _vsprintf_p_l
#define _vsctprintf     _vscprintf
#define _vsctprintf_l   _vscprintf_l
#define _vsctprintf_p   _vscprintf_p
#define _vsctprintf_p_l _vscprintf_p_l
#define _vsntprintf     _vsnprintf
#define _vsntprintf_l   _vsnprintf_l
#define _vsntprintf_s   _vsnprintf_s
#define _vsntprintf_s_l _vsnprintf_s_l

#define _tscanf         scanf
#define _tscanf_l       _scanf_l
#define _tscanf_s       scanf_s
#define _tscanf_s_l     _scanf_s_l
#define _tcscanf        _cscanf
#define _tcscanf_l      _cscanf_l
#define _tcscanf_s      _cscanf_s
#define _tcscanf_s_l    _cscanf_s_l
#define _ftscanf        fscanf
#define _ftscanf_l      _fscanf_l
#define _ftscanf_s      fscanf_s
#define _ftscanf_s_l    _fscanf_s_l
#define _stscanf        sscanf
#define _stscanf_l      _sscanf_l
#define _stscanf_s      sscanf_s
#define _stscanf_s_l    _sscanf_s_l
#define _sntscanf       _snscanf
#define _sntscanf_l     _snscanf_l
#define _sntscanf_s     _snscanf_s
#define _sntscanf_s_l   _snscanf_s_l


/* Unformatted i/o */

#define _fgettc         fgetc
#define _fgettc_nolock  _fgetc_nolock
#define _fgettchar      _fgetchar
#define _fgetts         fgets
#define _fputtc         fputc
#define _fputtc_nolock  _fputc_nolock
#define _fputtchar      _fputchar
#define _fputts         fputs
#define _cputts         _cputs
#define _gettc          getc
#define _gettc_nolock   _getc_nolock
#define _gettch         _getch
#define _gettch_nolock  _getch_nolock
#define _gettche        _getche
#define _gettche_nolock _getche_nolock
#define _gettchar       getchar
#define _gettchar_nolock _getchar_nolock
#define _getts          gets
#define _getts_s        gets_s
#define _cgetts         _cgets
#define _cgetts_s       _cgets_s
#define _puttc          putc
#define _puttc_nolock   _putc_nolock
#define _puttchar       putchar
#define _puttchar_nolock _putchar_nolock
#define _puttch         _putch
#define _puttch_nolock  _putch_nolock
#define _putts          puts
#define _ungettc        ungetc
#define _ungettc_nolock _ungetc_nolock
#define _ungettch       _ungetch
#define _ungettch_nolock _ungetch_nolock

/* String conversion functions */

#define _tcstod     strtod
#define _tcstol     strtol
#define _tcstoul    strtoul
#define _tstof      atof
#define _tstol      atol
#define _tstoi      atoi
#define _tstoi64    _atoi64
#define _tcstod_l     _strtod_l
#define _tcstol_l     _strtol_l
#define _tcstoul_l    _strtoul_l
#define _tstof_l      _atof_l
#define _tstol_l      _atol_l
#define _tstoi_l      _atoi_l
#define _tstoi64_l    _atoi64_l

#define _itot_s     _itoa_s
#define _ltot_s     _ltoa_s
#define _ultot_s    _ultoa_s
#define _itot       _itoa
#define _ltot       _ltoa
#define _ultot      _ultoa
#define _ttoi       atoi
#define _ttol       atol

#define _ttoi64     _atoi64
#define _tcstoi64   _strtoi64
#define _tcstoi64_l   _strtoi64_l
#define _tcstoui64  _strtoui64
#define _tcstoui64_l  _strtoui64_l
#define _i64tot_s   _i64toa_s
#define _ui64tot_s  _ui64toa_s
#define _i64tot     _i64toa
#define _ui64tot    _ui64toa

/* String functions */

/* Note that _mbscat, _mbscpy and _mbsdup are functionally equivalent to 
   strcat, strcpy and strdup, respectively. */

#define _tcscat     strcat
#define _tcscat_s   strcat_s
#define _tcscpy     strcpy
#define _tcscpy_s   strcpy_s
#define _tcsdup     _strdup
#define _tcslen     strlen
#define _tcsnlen    strnlen
#define _tcsxfrm    strxfrm
#define _tcsxfrm_l    _strxfrm_l
#define _tcserror   strerror
#define _tcserror_s   strerror_s
#define __tcserror  _strerror
#define __tcserror_s  _strerror_s

#ifdef _DEBUG
#define _tcsdup_dbg _strdup_dbg
#endif

/* Execute functions */

#define _texecl     _execl
#define _texecle    _execle
#define _texeclp    _execlp
#define _texeclpe   _execlpe
#define _texecv     _execv
#define _texecve    _execve
#define _texecvp    _execvp
#define _texecvpe   _execvpe

#define _tspawnl    _spawnl
#define _tspawnle   _spawnle
#define _tspawnlp   _spawnlp
#define _tspawnlpe  _spawnlpe
#define _tspawnv    _spawnv
#define _tspawnve   _spawnve
#define _tspawnvp   _spawnvp
#define _tspawnvpe  _spawnvpe

#define _tsystem    system


/* Time functions */

#define _tasctime   asctime
#define _tctime     ctime
#define _tctime32   _ctime32
#define _tctime64   _ctime64
#define _tstrdate   _strdate
#define _tstrtime   _strtime
#define _tutime     _utime
#define _tutime32   _utime32
#define _tutime64   _utime64
#define _tcsftime   strftime
#define _tcsftime_l _strftime_l

#define _tasctime_s   asctime_s
#define _tctime_s     ctime_s
#define _tctime32_s   _ctime32_s
#define _tctime64_s   _ctime64_s
#define _tstrdate_s   _strdate_s
#define _tstrtime_s   _strtime_s

/* Directory functions */

#define _tchdir             _chdir
#define _tgetcwd            _getcwd
#define _tgetdcwd           _getdcwd
#define _tgetdcwd_nolock    _getdcwd_nolock
#define _tmkdir             _mkdir
#define _trmdir             _rmdir

#ifdef _DEBUG
#define _tgetcwd_dbg        _getcwd_dbg
#define _tgetdcwd_dbg       _getdcwd_dbg
#define _tgetdcwd_lk_dbg    _getdcwd_lk_dbg
#endif

/* Environment/Path functions */

#define _tfullpath      _fullpath
#define _tgetenv        getenv
#define _tgetenv_s      getenv_s
#define _tdupenv_s      _dupenv_s
#define _tmakepath      _makepath
#define _tmakepath_s    _makepath_s
#define _tpgmptr        _pgmptr
#define _get_tpgmptr    _get_pgmptr
#define _tputenv        _putenv
#define _tputenv_s      _putenv_s
#define _tsearchenv     _searchenv
#define _tsearchenv_s   _searchenv_s
#define _tsplitpath     _splitpath
#define _tsplitpath_s   _splitpath_s

#ifdef _DEBUG
#define _tfullpath_dbg  _fullpath_dbg
#define _tdupenv_s_dbg  _dupenv_s_dbg
#endif

/* Stdio functions */

#ifdef  _POSIX_
#define _tfdopen    fdopen
#else
#define _tfdopen    _fdopen
#endif
#define _tfsopen    _fsopen
#define _tfopen     fopen
#define _tfopen_s   fopen_s
#define _tfreopen   freopen
#define _tfreopen_s freopen_s
#define _tperror    perror
#define _tpopen     _popen
#define _ttempnam   _tempnam
#define _ttmpnam    tmpnam
#define _ttmpnam_s  tmpnam_s

#ifdef _DEBUG
#define _ttempnam_dbg   _tempnam_dbg
#endif


/* Io functions */

#define _tchmod     _chmod
#define _tcreat     _creat
#define _tfindfirst      _findfirst
#define _tfindfirst32    _findfirst32
#define _tfindfirst64    _findfirst64
#define _tfindfirsti64   _findfirsti64
#define _tfindfirst32i64 _findfirst32i64
#define _tfindfirst64i32 _findfirst64i32
#define _tfindnext       _findnext
#define _tfindnext32     _findnext32
#define _tfindnext64     _findnext64
#define _tfindnexti64    _findnexti64
#define _tfindnext32i64  _findnext32i64
#define _tfindnext64i32  _findnext64i32
#define _tmktemp            _mktemp
#define _tmktemp_s          _mktemp_s

#ifdef  _POSIX_
#define _topen      open
#define _taccess    access
#else
#define _topen      _open
#define _taccess    _access
#define _taccess_s  _access_s
#endif

#define _tremove    remove
#define _trename    rename
#define _tsopen     _sopen
#define _tsopen_s   _sopen_s
#define _tunlink    _unlink

#define _tfinddata_t      _finddata_t
#define _tfinddata32_t    _finddata32_t
#define _tfinddata64_t    __finddata64_t
#define _tfinddatai64_t   _finddatai64_t
#define _tfinddata32i64_t _finddata32i64_t
#define _tfinddata64i32_t _finddata64i32_t

/* ctype functions */
#define _istascii       __isascii
#define _istcntrl       iscntrl
#define _istcntrl_l     _iscntrl_l
#define _istxdigit      isxdigit
#define _istxdigit_l    _isxdigit_l

/* Stat functions */
#define _tstat      _stat
#define _tstat32    _stat32
#define _tstat32i64 _stat32i64
#define _tstat64    _stat64
#define _tstat64i32 _stat64i32
#define _tstati64   _stati64


/* Setlocale functions */

#define _tsetlocale setlocale


#ifdef  _MBCS

/* ++++++++++++++++++++ MBCS ++++++++++++++++++++ */

#ifdef  __cplusplus
}   /* ... extern "C" */
#endif

#include <mbstring.h>

#ifdef  __cplusplus
extern "C" {
#endif


#ifndef __TCHAR_DEFINED
typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef unsigned char   _TXCHAR;
typedef unsigned int    _TINT;
#define __TCHAR_DEFINED
#endif

#ifndef _TCHAR_DEFINED
#if     !__STDC__
typedef char            TCHAR;
#endif
#define _TCHAR_DEFINED
#endif


#ifdef  _MB_MAP_DIRECT

/* use mb functions directly - types must match */

/* String functions */

#define _tcschr         _mbschr
#define _tcscspn        _mbscspn
#define _tcsncat        _mbsnbcat
#define _tcsncat_s      _mbsnbcat_s
#define _tcsncat_l      _mbsnbcat_l
#define _tcsncat_s_l    _mbsnbcat_s_l
#define _tcsncpy        _mbsnbcpy
#define _tcsncpy_s      _mbsnbcpy_s
#define _tcsncpy_l      _mbsnbcpy_l
#define _tcsncpy_s_l    _mbsnbcpy_s_l
#define _tcspbrk        _mbspbrk
#define _tcsrchr        _mbsrchr
#define _tcsspn         _mbsspn
#define _tcsstr         _mbsstr
#define _tcstok         _mbstok
#define _tcstok_s       _mbstok_s
#define _tcstok_l       _mbstok_l
#define _tcstok_s_l     _mbstok_s_l

#define _tcsnset        _mbsnbset
#define _tcsnset_l      _mbsnbset_l
#define _tcsnset_s      _mbsnbset_s
#define _tcsnset_s_l    _mbsnbset_s_l
#define _tcsrev         _mbsrev
#define _tcsset         _mbsset
#define _tcsset_s       _mbsset_s
#define _tcsset_l       _mbsset_l
#define _tcsset_s_l     _mbsset_s_l

#define _tcscmp         _mbscmp
#define _tcsicmp        _mbsicmp
#define _tcsicmp_l      _mbsicmp_l
#define _tcsnccmp       _mbsncmp
#define _tcsncmp        _mbsnbcmp
#define _tcsncicmp      _mbsnicmp
#define _tcsncicmp_l    _mbsnicmp_l
#define _tcsnicmp       _mbsnbicmp
#define _tcsnicmp_l     _mbsnbicmp_l

#define _tcscoll        _mbscoll
#define _tcscoll_l      _mbscoll_l
#define _tcsicoll       _mbsicoll
#define _tcsicoll_l     _mbsicoll_l
#define _tcsnccoll      _mbsncoll
#define _tcsnccoll_l    _mbsncoll_l
#define _tcsncoll       _mbsnbcoll
#define _tcsncoll_l     _mbsnbcoll_l
#define _tcsncicoll     _mbsnicoll
#define _tcsncicoll_l   _mbsnicoll_l
#define _tcsnicoll      _mbsnbicoll
#define _tcsnicoll_l    _mbsnbicoll_l

/* "logical-character" mappings */

#define _tcsclen        _mbslen
#define _tcscnlen       _mbsnlen
#define _tcsclen_l      _mbslen_l
#define _tcscnlen_l     _mbsnlen_l
#define _tcsnccat       _mbsncat
#define _tcsnccat_s     _mbsncat_s
#define _tcsnccat_l     _mbsncat_l
#define _tcsnccat_s_l   _mbsncat_s_l
#define _tcsnccpy       _mbsncpy
#define _tcsnccpy_s     _mbsncpy_s
#define _tcsnccpy_l     _mbsncpy_l
#define _tcsnccpy_s_l   _mbsncpy_s_l
#define _tcsncset       _mbsnset
#define _tcsncset_s     _mbsnset_s
#define _tcsncset_l     _mbsnset_l
#define _tcsncset_s_l   _mbsnset_s_l

/* MBCS-specific mappings */

#define _tcsdec     _mbsdec
#define _tcsinc     _mbsinc
#define _tcsnbcnt   _mbsnbcnt
#define _tcsnccnt   _mbsnccnt
#define _tcsnextc   _mbsnextc
#define _tcsninc    _mbsninc
#define _tcsspnp    _mbsspnp

#define _tcslwr     _mbslwr
#define _tcslwr_l   _mbslwr_l
#define _tcslwr_s   _mbslwr_s
#define _tcslwr_s_l _mbslwr_s_l
#define _tcsupr     _mbsupr
#define _tcsupr_l   _mbsupr_l
#define _tcsupr_s   _mbsupr_s
#define _tcsupr_s_l _mbsupr_s_l

#define _tclen      _mbclen
#define _tccpy      _mbccpy
#define _tccpy_l    _mbccpy_l
#define _tccpy_s    _mbccpy_s
#define _tccpy_s_l  _mbccpy_s_l

#else /* _MB_MAP_DIRECT */

#endif  /* _MB_MAP_DIRECT */


/* MBCS-specific mappings */

#define _tccmp(_cp1,_cp2)   _tcsnccmp(_cp1,_cp2,1)


/* ctype functions */

#define _istalnum       _ismbcalnum
#define _istalnum_l     _ismbcalnum_l
#define _istalpha       _ismbcalpha
#define _istalpha_l     _ismbcalpha_l
#define _istdigit       _ismbcdigit
#define _istdigit_l     _ismbcdigit_l
#define _istgraph       _ismbcgraph
#define _istgraph_l     _ismbcgraph_l
#define _istlegal       _ismbclegal
#define _istlegal_l     _ismbclegal_l
#define _istlower       _ismbclower
#define _istlower_l     _ismbclower_l
#define _istprint       _ismbcprint
#define _istprint_l     _ismbcprint_l
#define _istpunct       _ismbcpunct
#define _istpunct_l     _ismbcpunct_l
#define _istspace       _ismbcspace
#define _istspace_l     _ismbcspace_l
#define _istupper       _ismbcupper
#define _istupper_l     _ismbcupper_l

#define _totupper       _mbctoupper
#define _totupper_l     _mbctoupper_l
#define _totlower       _mbctolower
#define _totlower_l     _mbctolower_l

#define _istlead        _ismbblead
#define _istleadbyte    isleadbyte
#define _istleadbyte_l  _isleadbyte_l

#else   /* !_MBCS */

/* ++++++++++++++++++++ SBCS ++++++++++++++++++++ */


#ifndef __TCHAR_DEFINED
typedef char            _TCHAR;
typedef signed char     _TSCHAR;
typedef unsigned char   _TUCHAR;
typedef char            _TXCHAR;
typedef int             _TINT;
#define __TCHAR_DEFINED
#endif

#ifndef _TCHAR_DEFINED
#if     !__STDC__
typedef char            TCHAR;
#endif
#define _TCHAR_DEFINED
#endif


/* String functions */

#define _tcschr         strchr
#define _tcscspn        strcspn
#define _tcsncat        strncat
#define _tcsncat_s      strncat_s
#define _tcsncat_l      _strncat_l
#define _tcsncat_s_l    _strncat_s_l
#define _tcsncpy        strncpy
#define _tcsncpy_s      strncpy_s
#define _tcsncpy_l      _strncpy_l
#define _tcsncpy_s_l    _strncpy_s_l
#define _tcspbrk        strpbrk
#define _tcsrchr        strrchr
#define _tcsspn         strspn
#define _tcsstr         strstr
#define _tcstok         strtok
#define _tcstok_s       strtok_s
#define _tcstok_l       _strtok_l
#define _tcstok_s_l     _strtok_s_l

#define _tcsnset        _strnset
#define _tcsnset_s      _strnset_s
#define _tcsnset_l      _strnset_l
#define _tcsnset_s_l    _strnset_s_l
#define _tcsrev         _strrev
#define _tcsset         _strset
#define _tcsset_s       _strset_s
#define _tcsset_l       _strset_l
#define _tcsset_s_l     _strset_s_l

#define _tcscmp         strcmp
#define _tcsicmp        _stricmp
#define _tcsicmp_l      _stricmp_l
#define _tcsnccmp       strncmp
#define _tcsncmp        strncmp
#define _tcsncicmp      _strnicmp
#define _tcsncicmp_l    _strnicmp_l
#define _tcsnicmp       _strnicmp
#define _tcsnicmp_l     _strnicmp_l

#define _tcscoll        strcoll
#define _tcscoll_l      _strcoll_l
#define _tcsicoll       _stricoll
#define _tcsicoll_l     _stricoll_l
#define _tcsnccoll      _strncoll
#define _tcsnccoll_l    _strncoll_l
#define _tcsncoll       _strncoll
#define _tcsncoll_l     _strncoll_l
#define _tcsncicoll     _strnicoll
#define _tcsncicoll_l   _strnicoll_l
#define _tcsnicoll      _strnicoll
#define _tcsnicoll_l    _strnicoll_l

/* "logical-character" mappings */

#define _tcsclen        strlen
#define _tcscnlen       strnlen
#define _tcsclen_l(_String, _Locale) strlen(_String)
#define _tcscnlen_l(_String, _Max_count, _Locale) strnlen((_String), (_Max_count))
#define _tcsnccat       strncat
#define _tcsnccat_s     strncat_s  
#define _tcsnccat_l     _strncat_l  
#define _tcsnccat_s_l   _strncat_s_l
#define _tcsnccpy       strncpy
#define _tcsnccpy_s     strncpy_s  
#define _tcsnccpy_l     _strncpy_l  
#define _tcsnccpy_s_l   _strncpy_s_l
#define _tcsncset       _strnset
#define _tcsncset_s     _strnset_s
#define _tcsncset_l     _strnset_l
#define _tcsncset_s_l   _strnset_s_l

/* MBCS-specific functions */

#define _tcsdec     _strdec
#define _tcsinc     _strinc
#define _tcsnbcnt   _strncnt
#define _tcsnccnt   _strncnt
#define _tcsnextc   _strnextc
#define _tcsninc    _strninc
#define _tcsspnp    _strspnp

#define _tcslwr     _strlwr
#define _tcslwr_l   _strlwr_l
#define _tcslwr_s   _strlwr_s
#define _tcslwr_s_l _strlwr_s_l
#define _tcsupr     _strupr
#define _tcsupr_l   _strupr_l
#define _tcsupr_s   _strupr_s
#define _tcsupr_s_l _strupr_s_l
#define _tcsxfrm    strxfrm
#define _tcsxfrm_l  _strxfrm_l

#define _istlead(_Char)                 (0)
#define _istleadbyte(_Char)             (0)
#define _istleadbyte_l(_Char, _Locale)  (0)


/* ctype-functions */

#define _istalnum   isalnum
#define _istalnum_l   _isalnum_l
#define _istalpha   isalpha
#define _istalpha_l   _isalpha_l
#define _istdigit   isdigit
#define _istdigit_l   _isdigit_l
#define _istgraph   isgraph
#define _istgraph_l   _isgraph_l
#define _istlower   islower
#define _istlower_l   _islower_l
#define _istprint   isprint
#define _istprint_l   _isprint_l
#define _istpunct   ispunct
#define _istpunct_l   _ispunct_l
#define _istspace   isspace
#define _istspace_l   _isspace_l
#define _istupper   isupper
#define _istupper_l   _isupper_l

#define _totupper   toupper
#define _totupper_l   _toupper_l
#define _totlower   tolower
#define _totlower_l   _tolower_l

#define _istlegal(_c)   (1)


/* the following is optional if functional versions are available */

/* define NULL pointer value */
#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif



#endif  /* _MBCS */


#endif  /* _UNICODE */


/* Generic text macros to be used with string literals and character constants.
   Will also allow symbolic constants that resolve to same. */

#define _T(x)       __T(x)
#define _TEXT(x)    __T(x)


#ifdef  __cplusplus
}   /* ... extern "C" */
#endif

#endif  /* _INC_TCHAR */

