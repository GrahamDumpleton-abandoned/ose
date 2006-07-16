#ifndef OTC_DEBUG_TERMFUNC_HH
#define OTC_DEBUG_TERMFUNC_HH
/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     debug/termfunc.hh
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//
// ============================================================================
*/

/* ------------------------------------------------------------------------- */

/*
// Terminate function. This invoked <OTC_TObject::terminateAll()>
// and displays a message via the log facility that the program is
// terminating.
*/

extern OSE_EXPORT void otclib_terminate_function();

/*
// Old name for this function. Provided for backward compatability only.
*/

#define otclib_terminate_handler otclib_terminate_function

/* ------------------------------------------------------------------------- */

#endif /* OTC_DEBUG_TERMFUNC_HH */
