/*
// ============================================================================
//
// = FILENAME
//     mksrc.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1992 OTC LIMITED
//     Copyright 1995-1999 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#include <OSE/OSE.h>

#undef OSE_TARGET_HOME
#undef OSE_RELEASE_DIRECTORY

#include "config.hh"
#include "mksrc.hh"

#include <OTC/program/program.hh>
#include <OTC/files/pathname.hh>
#include <OTC/text/record.hh>
#include <OTC/text/globex.hh>
#include <OTC/collctn/deque.hh>
#include <OTC/debug/tracer.hh>
#include <OTC/debug/tracetag.hh>

#include <OTC/stdlib/iostream.h>
#include <OTC/stdlib/strstream.h>
#include <OTC/stdlib/fstream.h>
#include <stdlib.h>

#if defined(NEED_PUTENV) || (defined(M_XENIX) && !defined(_COMPAT322))
extern "C" int putenv(char const*);
#endif

#if defined(CXX_OS1_2) || defined(CXX_OS2_0)
extern "C" int system(const char*);
extern "C" int putenv(char const*);
#endif

/* ------------------------------------------------------------------------- */

OTC_String APP_ConfigItem::file_;
OTC_TraceTag PROGRAM("MKSRC");

APP_ConfigItem::APP_ConfigItem(APP_ConfigItem const& theItem)
  : project_(theItem.project()),
    pattern_(theItem.pattern()),
    program_(theItem.program()),
    helpFile_(theItem.helpFile())
{
  // Nothing to do.
}

istream& operator>>(istream& ins, APP_ConfigItem& theItem)
{
  while (ins.good())
  {
    OTC_String line = OTC_String::getline(ins,'\n');
    if (!ins.good() && line.isEmpty())
      break;
    int theHashIndex = line.index('#');
    if (theHashIndex != -1)
      line.truncate(theHashIndex);
    line.trim();
    if (!line.isEmpty())
    {
      OTC_Record record(line);
      if (record.numFields() >= 2 && record.numFields() <= 3)
      {
	theItem.project_ = "";
	theItem.helpFile_ = "";
	OTC_Pathname path(record.field(2));
	if (!path.isAbsolute())
	{
	  theItem.program_ = APP_ConfigItem::file();
	  theItem.program_.append("/");
	  theItem.program_.append(record.field(2));
	}
	else
	{
	  theItem.program_ = record.field(2);
	}
	int theSlashIndex = record.field(1).rindex('/');
	if (theSlashIndex == -1)
	{
	  theItem.pattern_ = record.field(1);
	}
	else
	{
	  theItem.project_ = record.field(1).before(theSlashIndex);
	  theItem.pattern_ = record.field(1).after(theSlashIndex);
	}
	if (record.numFields() == 3)
	{
	  OTC_Pathname helpfile(record.field(3));
	  if (!helpfile.isAbsolute())
	  {
	    theItem.helpFile_ = APP_ConfigItem::file();
	    theItem.helpFile_.append("/");
	    theItem.helpFile_.append(record.field(3));
	  }
	  else
	  {
	    theItem.helpFile_ = record.field(3);
	  }
	}
	return ins;
      }
      else
      {
	line.prepend("bad format: ");
	OTCLIB_TRACER(PROGRAM) << line << endl;
      }
    }
  }

  return ins;
}

#ifdef __OSE_TEMPLATES__
OSE_TEMPLATE OTC_BaseActions<APP_ConfigItem>
OSE_TEMPLATE OTC_Deque<APP_ConfigItem>
OSE_TEMPLATE OTC_Deque<OTC_String>;
#endif

void usage()
{
  cerr << "Usage: " << OTC_Program::basename() << " -dump |";
  cerr << " [ -help ] [ -group name ] [ options ] file ..." << endl;
  exit(1);
}

int main(int argc, char* argv[])
{
  set_terminate(otclib_terminate_function);

  OTC_Program::initialise(argc,argv);

  // Also search directories defined in MKSRC_PATH
  // environment variable.

  OTC_Deque<OTC_String> path;
  char const* pathenv = getenv("MKSRC_PATH");
  if (pathenv != 0)
  {
    OTC_Record record(pathenv,':');
    OTC_Iterator<OTC_String> iter = 0;
    iter = record.fields();
    for (iter.reset(); iter.isValid(); iter.next())
      path.addLast(iter.item());
  }

  // Always search current directory before others.

  path.addFirst(".");

  // Search default directories last. These are what is compiled
  // in as the default, and that based on the setting of OSE_ROOT
  // environment variable.

  OTC_String tmpString;
  
  char const* rootenv = getenv("OSE_ROOT");
  char const* hostenv = getenv("OSE_HOST");
  if (rootenv != 0 && hostenv != 0)
  {
    tmpString = rootenv;
    tmpString.append("/");
    tmpString.append(OSE_RELEASE_DIRECTORY);
    tmpString.append("/");
    tmpString.append(hostenv);
    tmpString.append("/etc");

    path.addLast(tmpString);
  }

  tmpString = OSE_TARGET_HOME;
  tmpString.append("/etc");

  path.addLast(tmpString);

  // Define MKSRC_AUTHOR if not defined.

  char const* authorenv = getenv("MKSRC_AUTHOR");
  if (authorenv == 0)
  {
    OTC_String fullname = "???";

    fullname.prepend("MKSRC_AUTHOR=");
    authorenv = fullname.duplicate();
    putenv((char*)authorenv);
  }

  // Grab flags from environment.

  OTC_String flags;
  char const* flagsenv = getenv("MKSRC_FLAGS");
  if (flagsenv != 0)
    flags.append(flagsenv);

  // Parse command line options.

  OTC_Deque<OTC_String> files;
  OTC_String project;
  OTC_Boolean dump = OTCLIB_FALSE;
  OTC_Boolean help = OTCLIB_FALSE;

  OTC_Iterator<OTC_String> theOptions = 0;
  theOptions = OTC_Program::options();
  for (theOptions.reset(); theOptions.isValid(); theOptions.next())
  {
    OTC_String const& argument = theOptions.item();
    if (argument == "-group")
    {
      theOptions.next();
      if (theOptions.isValid())
	project = theOptions.item();
      else
	usage();
    }
    else if (argument == "-dump")
    {
      dump = OTCLIB_TRUE;
    }
    else if (argument == "-help")
    {
      help = OTCLIB_TRUE;
    }
    else if (argument.index('-') != 0)
    {
      files.addLast(argument);
    }
    else
    {
      flags.append(" ");
      flags.append(argument);
    }
  }

  // Check we have at least one file.

  if (files.isEmpty() && dump == OTCLIB_FALSE)
    usage();

  // Dump stuff for testing.

  OTC_String debug;

  debug = "dump = ";
  debug.append('0' + int(dump));
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  debug = "help = ";
  debug.append('0' + int(help));
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  debug = "author = ";
  debug.append(getenv("MKSRC_AUTHOR"));
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  debug = "group = ";
  debug.append(project);
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  debug = "flags = ";
  debug.append(flags);
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  OTC_Iterator<OTC_String> theFiles = 0;

  debug = "files =";
  theFiles = files.items();
  for (theFiles.reset(); theFiles.isValid(); theFiles.next())
  {
    debug.append(" ");
    debug.append(theFiles.item());
  }
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  debug = "path =";
  OTC_Iterator<OTC_String> thePaths = path.items();
  for (thePaths.reset(); thePaths.isValid(); thePaths.next())
  {
    debug.append(" ");
    debug.append(thePaths.item());
  }
  OTCLIB_TRACER(PROGRAM) << debug << endl;

  // Read in all the information from the configuration files.

  OTC_Deque<APP_ConfigItem> config;
  for (thePaths.reset(); thePaths.isValid(); thePaths.next())
  {
    OTC_String tmpFile(thePaths.item());
    APP_ConfigItem::setFile(tmpFile);
    tmpFile.append("/mksrc.cf");
    OTC_Pathname file(tmpFile);
    if (file.isReadable())
    {
      debug = "open file: ";
      debug.append(tmpFile);
      OTCLIB_TRACER(PROGRAM) << debug << endl;
      ifstream ins(tmpFile);
      while (ins.good())
      {
	APP_ConfigItem item;
	ins >> item;
	if (!ins.fail() && !item.pattern().isEmpty())
	  config.addLast(item);
      }
      ins.close();
    }
  }

  // If -dump option then dump out combination of config files.

  if (dump != OTCLIB_FALSE)
  {
    OTC_Iterator<APP_ConfigItem> theItems = 0;
    theItems = config.items();
    for (theItems.reset(); theItems.isValid(); theItems.next())
    {
      APP_ConfigItem const& item = theItems.item();
      if (!item.project().isEmpty())
	cout << item.project() << "/";
      cout << item.pattern();
      cout << " " << item.program();
      if (!item.helpFile().isEmpty())
	cout << " " << item.helpFile();
      cout << endl;
    }
    return 0;
  }

  // Find generators for files and run them.

  theFiles = files.items();
  for (theFiles.reset(); theFiles.isValid(); theFiles.next())
  {
    OTC_Pathname thePathname(theFiles.item());
    if (thePathname.isAccessable())
    {
      cerr << OTC_Program::basename() << ": ";
      cerr << theFiles.item() << " exists" << endl;
    }
    else
    {
      OTC_Boolean found = OTCLIB_FALSE;
      OTC_Iterator<APP_ConfigItem> items = config.items();
      for (items.reset(); items.isValid() && !found; items.next())
      {
	APP_ConfigItem const& item = items.item();
	if (item.project() == project)
	{
	  OTC_Globex pattern(item.pattern().string());
	  if (pattern.match(theFiles.item()))
	  {
	    if (help != OTCLIB_FALSE)
	    {
	      if (!item.helpFile().isEmpty())
	      {
		OTC_Pathname helpFile(item.helpFile());
		if (helpFile.isReadable())
		{
		  OTC_String command("more ");
		  command.append(item.helpFile());
		  system(command);
		}
		else
		{
		  cerr << OTC_Program::basename() << ": ";
		  cerr << "help file " << item.helpFile();
		  cerr << " doesn't exist" << endl;
		}
	      }
	      else
	      {
		cerr << OTC_Program::basename() << ": ";
		cerr << "no help file for " << item.pattern() << endl;
	      }
	    }
	    else
	    {
	      OTC_Pathname program(item.program());
	      if (program.isExecutable())
	      {
		OTC_String command(item.program());
		command.append(" ");
		if (!flags.isEmpty())
		{
		  command.append(flags);
		  command.append(" ");
		}
		command.append(theFiles.item());
		debug = "executing: ";
#if !defined(SYS_UNIX)
		OTC_String shellenv;
		shellenv = getenv("SHELL");
		if (!shellenv.isEmpty())
		{
#if defined(WIN32)
		  int i;
		  i = shellenv.index('/');
		  while (i != -1)
		  {
		    shellenv[(u_int)i] = '\\';
		    i = shellenv.index('/');
		  }
#endif
		  command.prepend(" ");
		  command.prepend(shellenv);
		}
#endif
		debug.append(command);
		OTCLIB_TRACER(PROGRAM) << debug << endl;
		system(command);
	      }
	      else
	      {
		cerr << OTC_Program::basename() << ": ";
		cerr << item.program() << " is not executable" << endl;
	      }
	    }
	    found = OTCLIB_TRUE;
	  }
	}
      }
      if (!found)
      {
	cerr << OTC_Program::basename() << ": ";
	cerr << "don't know how to generate " << theFiles.item() << endl;
      }
    }
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
