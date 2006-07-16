/*
// ============================================================================
//
// = LIBRARY
//     OSE
//
// = FILENAME
//     oseversion.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1992 1993 OTC LIMITED
//     Copyright 1994 TELSTRA CORPORATION LIMITED
//     Copyright 1994-2001 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OSE/OSE.h>

/* ------------------------------------------------------------------------- */

char const* const OSE_RELEASE_TAG =
 "@(#)OSE Release " OSE_RELEASE_NAME " (" OSE_RELEASE_NUMBER ")";

char const* OSE_RELEASE_INFORMATION() { return &OSE_RELEASE_TAG[0]; }

/* ------------------------------------------------------------------------- */
