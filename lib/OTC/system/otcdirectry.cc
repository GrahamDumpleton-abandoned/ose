/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     system/otcdirectry.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/system/directry.hh"
#endif
#endif

#include <OTC/system/directry.hh>
#include <OTC/text/globex.hh>

#include <sys/types.h>

#ifdef SYS_UNIX
#ifdef HAVE_DIRENT_H
#include <dirent.h>
#else
#define dirent direct
#ifdef HAVE_SYS_NDIR_H
#include <sys/ndir.h>
#else
#ifdef HAVE_SYS_DIR_H
#include <sys/dir.h>
#else
#ifdef HAVE_NDIR_H
#include <ndir.h>
#else
#undef dirent
#endif
#endif
#endif
#endif
#else
#ifdef __WATCOMC__
#include <stdlib.h>
#include <direct.h>
#endif
#ifdef __BORLANDC__
#include <dirent.h>
#endif
#ifdef _MSC_VER
#include <windows.h>
#endif
#ifdef __IBMCPP__
#include <direct.h>
#endif
#endif

#if defined(ENV_OSTORE)
/* ------------------------------------------------------------------------- */
os_typespec* OTC_Directory::typespec()
{
  static os_typespec ts("OTC_Directory");
  return &ts;
}
#endif

/* ------------------------------------------------------------------------- */
OTC_Directory::OTC_Directory(char const* theName)
  : name_(theName)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Directory::OTC_Directory(OTC_Directory const& theDir)
  : name_(theDir.name_)
{
  // Nothing to do.
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Directory::isSearchable() const
{
  if (name_.isEmpty())
    return OTCLIB_FALSE;

#ifdef __IBMCPP__

  // XXX

  return OTCLIB_FALSE;

#else
#ifdef _MSC_VER

  HANDLE theHandle = INVALID_HANDLE_VALUE;

  theHandle = CreateFile(
   (char*)name_.string(),
   GENERIC_READ,
   FILE_SHARE_READ|FILE_SHARE_WRITE,
   0,
   OPEN_EXISTING,
   FILE_ATTRIBUTE_NORMAL|FILE_FLAG_BACKUP_SEMANTICS,
   0);

  if (theHandle != INVALID_HANDLE_VALUE)
  {
    CloseHandle(theHandle);
    return OTCLIB_TRUE;
  }

  return OTCLIB_FALSE;

#else

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle != 0)
  {
    closedir(theHandle);
    return OTCLIB_TRUE;
  }

  return OTCLIB_FALSE;

#endif
#endif
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Directory::files() const
{
  if (name_.isEmpty())
    return 0;

#ifdef __IBMCPP__

  // XXX

  return 0;

#else
#ifdef _MSC_VER

  HANDLE theHandle = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA theEntry;

  OTC_String thePath;

  thePath += name_;
  thePath += '/';
  thePath += '*';

  theHandle = FindFirstFile((char*)thePath.string(),&theEntry);

  if (theHandle == INVALID_HANDLE_VALUE)
    return 0;

  OTC_Deque<OTC_String> theNames;
  theNames.addLast((char const*)theEntry.cFileName);

  while (FindNextFile(theHandle,&theEntry))
    theNames.addLast((char const*)theEntry.cFileName);

  FindClose(theHandle);

  return theNames.items();

#else

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle == 0)
    return 0;

  OTC_Deque<OTC_String> theNames;
  dirent* theEntry;
  for (theEntry = readdir(theHandle);
   theEntry != 0; theEntry = readdir(theHandle))
  {
    theNames.addLast(theEntry->d_name);
  }
  closedir(theHandle);
  return theNames.items();

#endif
#endif
}

/* ------------------------------------------------------------------------- */
OTC_Iterator<OTC_String> OTC_Directory::files(char const* thePattern) const
{
  if (name_.isEmpty())
    return 0;

#ifdef __IBMCPP__

  // XXX

  return 0;

#else
#ifdef _MSC_VER

  HANDLE theHandle = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA theEntry;

  OTC_Globex theGlobex(thePattern);
  if (!theGlobex.isValid())
    return 0;

  OTC_String thePath;

  thePath += name_;
  thePath += '/';
  thePath += '*';

  theHandle = FindFirstFile((char*)thePath.string(),&theEntry);

  if (theHandle == INVALID_HANDLE_VALUE)
    return 0;

  OTC_Deque<OTC_String> theNames;

  if (theGlobex.match(theEntry.cFileName))
    theNames.addLast((char const*)theEntry.cFileName);

  while (FindNextFile(theHandle,&theEntry))
  {
    if (theGlobex.match(theEntry.cFileName))
      theNames.addLast((char const*)theEntry.cFileName);
  }

  FindClose(theHandle);

  return theNames.items();

#else

  OTC_Globex theGlobex(thePattern);
  if (!theGlobex.isValid())
    return 0;

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle != 0)
  {
    OTC_Deque<OTC_String> theNames;
    dirent* theEntry;
    for (theEntry = readdir(theHandle);
     theEntry != 0; theEntry = readdir(theHandle))
    {
      if (theGlobex.match(theEntry->d_name))
	theNames.addLast(theEntry->d_name);
    }
    closedir(theHandle);
    return theNames.items();
  }

  return 0;

#endif
#endif
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_Directory::contains(char const* theName) const
{
  if (name_.isEmpty())
    return OTCLIB_FALSE;

#ifdef __IBMCPP__

  // XXX

  return OTCLIB_FALSE;

#else
#ifdef _MSC_VER

  HANDLE theHandle = INVALID_HANDLE_VALUE;
  WIN32_FIND_DATA theEntry;

  OTC_String thePath;

  thePath += name_;
  thePath += '/';
  thePath += '*';

  theHandle = FindFirstFile((char*)thePath.string(),&theEntry);

  if (theHandle == INVALID_HANDLE_VALUE)
    return OTCLIB_FALSE;

  if (strcmp(theName,theEntry.cFileName) == 0)
  {
    FindClose(theHandle);
    return OTCLIB_TRUE;
  }

  while (FindNextFile(theHandle,&theEntry))
  {
    if (strcmp(theName,theEntry.cFileName) == 0)
    {
      FindClose(theHandle);
      return OTCLIB_TRUE;
    }
  }

  FindClose(theHandle);

  return OTCLIB_FALSE;

#else

  DIR* theHandle = 0;
  theHandle = opendir((char*)name_.string());
  if (theHandle == 0)
    return OTCLIB_FALSE;

  dirent* theEntry;
  for (theEntry = readdir(theHandle);
   theEntry != 0; theEntry = readdir(theHandle))
  {
    if (strcmp(theName,theEntry->d_name) == 0)
    {
      closedir(theHandle);
      return OTCLIB_TRUE;
    }
  }
  closedir(theHandle);

  return OTCLIB_FALSE;

#endif
#endif
}

/* ------------------------------------------------------------------------- */
