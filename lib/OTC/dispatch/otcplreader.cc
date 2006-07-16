/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcplreader.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2001-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/plreader.hh"
#endif
#endif

#include <OTC/dispatch/plreader.hh>

#include <OTC/dispatch/ropyload.hh>

#include <errno.h>
#include <math.h>

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(OTC_String& theValue)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    theValue.assign(node_.value().string(),node_.value().length());

    return;
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(OTC_Symbol& theValue)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    theValue = OTC_String(node_.value().string(),node_.value().length());

    return;
  }

  flagError();
}

#if defined(HAVE_BOOL)
/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(OTC_Boolean& theValue)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:boolean")
    {
      if (node_.value() == "false" || node_.value() == "0")
      {
	theValue = OTCLIB_FALSE;

	return;
      }
      else if (node_.value() == "true" || node_.value() == "1")
      {
	theValue = OTCLIB_TRUE;

	return;
      }
    }
    else if (node_.valueType() == "xsd:integer" ||
     node_.valueType() == "xsd:long" ||
     node_.valueType() == "xsd:int" ||
     node_.valueType() == "xsd:short" ||
     node_.valueType() == "xsd:byte")
    {
      long tmpValue;

      (*this) >>= tmpValue;

      if (!errorOccurred())
	theValue = !!tmpValue;

      return;
    }
    else if (node_.valueType() == "xsd:unsignedLong" ||
     node_.valueType() == "xsd:unsignedInt" ||
     node_.valueType() == "xsd:unsignedShort" ||
     node_.valueType() == "xsd:unsignedByte")
    {
      u_long tmpValue;

      (*this) >>= tmpValue;

      if (!errorOccurred())
	theValue = !!tmpValue;

      return;
    }
    else if (node_.valueType() == "xsd:double" ||
     node_.valueType() == "xsd:float")
    {
      double tmpValue;

      (*this) >>= tmpValue;

      if (!errorOccurred())
	theValue = !!tmpValue;

      return;
    }
  }

  flagError();
}
#endif

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(long& theValue)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:integer" ||
     node_.valueType() == "xsd:long" ||
     node_.valueType() == "xsd:int" ||
     node_.valueType() == "xsd:short" ||
     node_.valueType() == "xsd:byte" ||
     node_.valueType() == "xsd:unsignedLong" ||
     node_.valueType() == "xsd:unsignedInt" ||
     node_.valueType() == "xsd:unsignedShort" ||
     node_.valueType() == "xsd:unsignedByte")
    {
      if (node_.value().length() != 0 &&
       node_.value().length() == strlen(node_.value().string()))
      {
	char* theNext = 0;

	theValue = strtol(node_.value().string(),&theNext,10);

	if (*theNext == EOS && errno != ERANGE)
	  return;
      }
    }
    else if (node_.valueType() == "xsd:boolean")
    {
      OTC_Boolean tmpValue = OTCLIB_FALSE;

      (*this) >>= tmpValue;

      theValue = long(tmpValue);

      return;
    }
    else if (node_.valueType() == "xsd:double" ||
     node_.valueType() == "xsd:float")
    {
      double tmpValue = 0;

      (*this) >>= tmpValue;

      theValue = long(tmpValue);
      if (theValue != floor(tmpValue))
	flagError();

      return;
    }
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(u_long& theValue)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:integer" ||
     node_.valueType() == "xsd:long" ||
     node_.valueType() == "xsd:int" ||
     node_.valueType() == "xsd:short" ||
     node_.valueType() == "xsd:byte" ||
     node_.valueType() == "xsd:unsignedLong" ||
     node_.valueType() == "xsd:unsignedInt" ||
     node_.valueType() == "xsd:unsignedShort" ||
     node_.valueType() == "xsd:unsignedByte")
    {
      if (node_.value().length() != 0 &&
       node_.value().length() == strlen(node_.value().string()))
      {
	char* theNext = 0;

	theValue = strtoul(node_.value().string(),&theNext,10);

	if (*theNext == EOS && errno != ERANGE)
	  return;
      }
    }
    else if (node_.valueType() == "xsd:boolean")
    {
      OTC_Boolean tmpValue = OTCLIB_FALSE;

      (*this) >>= tmpValue;

      theValue = u_long(tmpValue);

      return;
    }
    else if (node_.valueType() == "xsd:double" ||
     node_.valueType() == "xsd:float")
    {
      double tmpValue = 0;

      (*this) >>= tmpValue;

      theValue = u_long(tmpValue);
      if (theValue != floor(tmpValue))
	flagError();

      return;
    }
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(double& theValue)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:double" ||
     node_.valueType() == "xsd:float")
    {
      if (node_.value().length() != 0 &&
       node_.value().length() == strlen(node_.value().string()))
      {
	char* theNext = 0;

	theValue = strtod(node_.value().string(),&theNext);

	if (*theNext == EOS && errno != ERANGE)
	  return;
      }
    }
    else if (node_.valueType() == "xsd:boolean")
    {
      OTC_Boolean tmpValue = OTCLIB_FALSE;

      (*this) >>= tmpValue;

      theValue = double(tmpValue);

      return;
    }
    else if (node_.valueType() == "xsd:integer" ||
     node_.valueType() == "xsd:long" ||
     node_.valueType() == "xsd:int" ||
     node_.valueType() == "xsd:short" ||
     node_.valueType() == "xsd:byte")
    {
      long tmpValue = 0;

      (*this) >>= tmpValue;

      theValue = double(tmpValue);

      return;
    }
    else if (node_.valueType() == "xsd:unsignedLong" ||
     node_.valueType() == "xsd:unsignedInt" ||
     node_.valueType() == "xsd:unsignedShort" ||
     node_.valueType() == "xsd:unsignedByte")
    {
      u_long tmpValue = 0;

      (*this) >>= tmpValue;

      theValue = double(tmpValue);

      return;
    }
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(OTC_Date& theDate)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:date")
    {
      if (node_.value().length() != 0 &&
       node_.value().length() == strlen(node_.value().string()))
      {
	if (OTC_Date::isValidDate(node_.value().string()))
	{
	  theDate = OTC_Date(node_.value().string());

	  return;
	}
      }
    }
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(OTC_Time& theTime)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:dateTime")
    {
      if (node_.value().length() != 0 &&
       node_.value().length() == strlen(node_.value().string()))
      {
	if (OTC_Time::isValidTime(node_.value().string()))
	{
	  theTime = OTC_Time(node_.value().string());

	  return;
	}
      }
    }
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
void OTC_PLReader::operator>>=(OTC_Duration& theDuration)
{
  if (!node_.isValid())
  {
    flagError();

    return;
  }

  if (node_.nodeType() == "scalar")
  {
    if (node_.valueType() == "xsd:duration")
    {
      if (node_.value().length() != 0 &&
       node_.value().length() == strlen(node_.value().string()))
      {
	if (OTC_Duration::isValidDuration(node_.value().string()))
	{
	  theDuration = OTC_Duration(node_.value().string());

	  return;
	}
      }
    }
  }

  flagError();
}

/* ------------------------------------------------------------------------- */
