#ifndef OTC_DEBUG_ERRORS_H
#define OTC_DEBUG_ERRORS_H
/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     debug/errors.h
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1996-1997 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifndef OSE_OSE_H
#include <OSE/OSE.h>
#endif

#ifndef OTC_MISC_MACROS_H
#include <OTC/misc/macros.h>
#endif
#ifndef OTC_DEBUG_STOPHERE_H
#include <OTC/debug/stophere.h>
#endif

/* ------------------------------------------------------------------------- */

/*
// [OTCLIB_TERMINATE]
//
// = TITLE
//     Function to force termination of program.
//
// = SYNOPSIS
//     typedef void(*PFV)();
//
//     void terminate();
//     PFV set_terminate(PFV);
//
//     void OTCLIB_TERMINATE();
//
// = DESCRIPTION
//     The <OTCLIB_TERMINATE(>) function is intended to be used in place of
//     <exit()>, when it is necessary to terminate the program in abnormal
//     situations. The function has C linkage and may be invoked in either C
//     or C++. The result of invoking <OTCLIB_TERMINATE()> is that the
//     <terminate()> function will be invoked. By default, the <terminate()>
//     function will invoke <abort()>. If a user terminate function is
//     provided, by way of using the function <set_terminate()>, then that
//     will be called. It is expected that the user terminate function does
//     not return. If it does, then <abort()> will still be called.
//
// = NOTES
//     The <terminate()> and <set_terminate()> are modelled after equivalent
//     functions, proposed as part of the exception mechanism for C++.
//     Where these functions exist as part of the standard C++ library, they
//     will be used over the versions in this library.
//
// = SEE ALSO
//     <abort(3)>
*/

#if defined(__cplusplus)
extern "C" OSE_EXPORT void OTCLIB_TERMINATE();
#else
extern OSE_EXPORT void OTCLIB_TERMINATE();
#endif

/*
// [OTCLIB_EXCEPTION]
//
// = TITLE
//     Macro to generate an exception.
//
// = SYNOPSIS
//     #define OTCLIB_EXCEPTION(description) ...
//
// = DESCRIPTION
//     The macro <OTCLIB_EXCEPTION()> can be used to generate a generic
//     exception of type <OTC_Exception> where the exception is described by
//     the <description> argument. This description will be sent to the
//     logger, as will information about the file in which the exception
//     occurred and the line within that file.
//
// = EXAMPLE
// = BEGIN<CODE>
//     if (aStr == 0)
//       OTCLIB_EXCEPTION("Nil pointer");
// = END<CODE>
//
// = FORMAT
//     If the exception is caught and the information it contains is dumped
//     to the logger, the format of the output will be:
//
// = BEGIN<CODE>
//     ERROR: Nil pointer
//     ERROR: Location: "errors_.cc", line 33
// = END<CODE>
//
// = NOTES
//     The macro may be used in either C or C++ code.
//
// = SEE ALSO
//    <OTC_Exception>
*/

#if defined(__cplusplus)
extern "C" OSE_EXPORT void otclib_exception(
 char const* theFile, int theLine, char const* theMessage
);
#else
extern OSE_EXPORT void otclib_exception();
#endif

#define OTCLIB_EXCEPTION(msg) \
 otclib_error_stop_here(),otclib_exception(__FILE__,__LINE__,msg)

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_ERRORS_H */
