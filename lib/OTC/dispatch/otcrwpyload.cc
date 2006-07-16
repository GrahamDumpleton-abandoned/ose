/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcrwpyload.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 2000-2003 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/rwpyload.hh"
#endif
#endif

#include <OTC/dispatch/rwpyload.hh>

#include <OTC/memory/cluster.hh>
#include <OTC/collctn/cbitset.hh>

#include <OTC/stdlib/strstream.h>

#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------------------- */
OTC_NRMutex OTC_RWPayload::mutex_;
OTC_Boolean OTC_RWPayload::initialised_ = OTCLIB_FALSE;
u_int OTC_RWPayload::capacity_ = 0;

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::setProperty(char const* theName, char const* theValue)
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_RWPayload::setProperty(char const*,char const*)",
   "invalid name string");

  if (strcmp(theName,"name") == 0)
    return;

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode->setProperty(theName,theValue);
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::setProperty(
 char const* theName,
 char const* theValue,
 u_int theLength
)
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_RWPayload::setProperty(char const*,char const*,u_int)",
   "invalid name string");
  OTCLIB_ENSURE_FN((theValue != 0 || theLength == 0),
   "OTC_RWPayload::setProperty(char const*,char const*,u_int)",
   "invalid value string");

  if (strcmp(theName,"name") == 0)
    return;

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode->setProperty(theName,theValue,theLength);
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::removeProperty(char const* theName)
{
  OTCLIB_ENSURE_FN((theName != 0),
   "OTC_RWPayload::removeProperty(char const*)",
   "invalid name string");

  if (strcmp(theName,"name") == 0)
    return;

  OTC_DOMNode* theNode;
  theNode = resolve();

  theNode->removeProperty(theName);
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::removeAllProperties()
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  resetProperties(theNode);
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::findProperty(char const* theName)
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->findProperty(theName);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::findProperty(char const* theName) const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->findProperty(theName);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::findProperty(
 char const* theName,
 char const* theValue
)
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->findProperty(theName,theValue);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::findProperty(
 char const* theName,
 char const* theValue
) const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->findProperty(theName,theValue);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::assign(char const* theString, char const* theType)
{
  if (root() == 0)
    return 0;

  OTC_DOMNode* theNode;
  theNode = install();

  resetProperties(theNode);

  if (theNode->name() != "scalar")
  {
    theNode->removeAllChildren();
    theNode->setName("scalar");
  }

  theNode->setContent(theString);

  if (theType != 0 && *theType != EOS)
    theNode->setProperty("type",theType);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::assign(
 char const* theString,
 u_int theLength,
 char const* theType
)
{
  if (root() == 0)
    return 0;

  OTC_DOMNode* theNode;
  theNode = install();

  resetProperties(theNode);

  if (theNode->name() != "scalar")
  {
    theNode->removeAllChildren();
    theNode->setName("scalar");
  }

  theNode->setContent(theString,theLength);

  if (theType != 0 && *theType != EOS)
    theNode->setProperty("type",theType);

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::assign(OTC_ROPayload const& theObject)
{
  if (root() == 0)
    return 0;

  OTC_DOMNode* theNode;
  OTC_DOMNode const* rhsNode;

  theNode = peek();
  rhsNode = theObject.peek();

  if (rhsNode != 0 && theNode == rhsNode)
    return resolve();

  // We have to clone the right hand side
  // at the beginning because just the
  // process of accessing the left hand side
  // may result in new nodes being created.

  OTC_Cluster* theCluster;
  theCluster = cluster();

  if (theCluster == 0)
    return 0;

  rhsNode = theObject.resolve();

  OTC_DOMNode* theClone = 0;

  if (rhsNode != 0)
  {
    theClone = rhsNode->cloneChildren(theCluster);
    theClone->removeProperty("name");
  }

  theNode = install();

  if (theNode != 0)
  {
    if (theClone == 0)
    {
      // The right hand side is an invalid node
      // reference. Turn this node into an "invalid"
      // node.

      resetProperties(theNode);

      theNode->removeAllChildren();

      theNode->setName("invalid");
      theNode->setContent(0);

      return theNode;
    }

    // Remove children and properties of the
    // left hand side. Unlink children of clone
    // and add them to the left hand side.
    // Update properties of left hand side.
    // We do waste space in that we don't reuse
    // the top node of the clone, but can't
    // replace this node as would invalidate
    // references and in particular destroy any
    // sibling relationships.

    theNode->removeAllChildren();

    resetProperties(theNode);

    theNode->setName(rhsNode->name().string());

    theNode->setContent(rhsNode->content());

    OTC_DOMNode* theChild;
    theChild = theClone->firstChild();

    while (theChild != 0)
    {
      OTC_DOMNode* tmpChild;
      tmpChild = theChild->nextSibling();

      theChild->unlink();
      theNode->insertChild(theChild);

      theChild = tmpChild;
    }

    OTC_DOMProperty const* theProperty;
    theProperty = rhsNode->properties();

    while (theProperty != 0)
    {
      if (theProperty->name() != "name")
      {
	theNode->setProperty(theProperty->name().string(),
	 theProperty->value());
      }

      theProperty = theProperty->next();
    }

    return theNode;
  }

  return 0;
}

#if defined(HAVE_BOOL)
/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(OTC_Boolean theValue)
{
  if (theValue == OTCLIB_FALSE)
    assign("false","xsd:boolean");
  else
    assign("true","xsd:boolean");
}
#endif

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(long theValue)
{
  char buf[128];
  sprintf(buf,"%ld",theValue);

  if (sizeof(theValue) == 8)
    assign(buf,"xsd:long");
  else
    assign(buf,"xsd:int");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(int theValue)
{
  char buf[128];
  sprintf(buf,"%d",theValue);

  assign(buf,"xsd:int");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(short theValue)
{
  char buf[128];
  sprintf(buf,"%d",int(theValue));

  assign(buf,"xsd:short");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(char theValue)
{
  char buf[128];
  sprintf(buf,"%d",int(theValue));

  assign(buf,"xsd:byte");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(u_long theValue)
{
  char buf[128];
  sprintf(buf,"%lu",theValue);

  if (sizeof(theValue) == 8)
    assign(buf,"xsd:unsignedLong");
  else
    assign(buf,"xsd:unsignedInt");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(u_int theValue)
{
  char buf[128];
  sprintf(buf,"%u",theValue);

  assign(buf,"xsd:unsignedInt");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(u_short theValue)
{
  char buf[128];
  sprintf(buf,"%u",u_int(theValue));

  assign(buf,"xsd:unsignedShort");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(u_char theValue)
{
  char buf[128];
  sprintf(buf,"%u",u_int(theValue));

  assign(buf,"xsd:unsignedByte");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(double theValue)
{
  char buf[128];
  sprintf(buf,"%.12g",theValue);

  assign(buf,"xsd:double");
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::operator<<=(float theValue)
{
  char buf[128];
  sprintf(buf,"%.12g",theValue);

  assign(buf,"xsd:float");
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::sibling(char const* thePath, u_int theLength)
{
  OTCLIB_ENSURE_FN((thePath != 0 || theLength == 0),
   "OTC_RWPayload::sibling(char const*,u_int)",
   "invalid path");

  OTC_DOMNode* theNode;
  theNode = install();

  if (theNode == 0)
    return 0;

  return OTC_RWPayload(theNode,thePath,theLength,-3);
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::operator[](char const* thePath)
{
  OTCLIB_ENSURE_FN((thePath != 0),
   "OTC_RWPayload::operator[](char const*)",
   "invalid path");

  OTC_DOMNode* theNode;
  theNode = install();

  if (theNode == 0)
    return 0;

  return OTC_RWPayload(theNode,thePath,strlen(thePath),-2);
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::operator[](int theIndex)
{
  OTCLIB_ENSURE_FN((theIndex >= -1),
   "OTC_RWPayload::operator[](int)",
   "invalid index");

  OTC_DOMNode* theNode;
  theNode = install();

  if (theNode == 0)
    return 0;

  OTCLIB_ENSURE_FN((theIndex <= int(theNode->numChildren())),
   "OTC_RWPayload::operator[](int)",
   "invalid index");

  if (theIndex == int(theNode->numChildren()))
    return OTC_RWPayload(theNode,0,0,-1);

  return OTC_RWPayload(theNode,0,0,theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::sibling(
 char const* thePath,
 u_int theLength
) const
{
  OTCLIB_ENSURE_FN((thePath != 0 || theLength == 0),
   "OTC_RWPayload::sibling(char const*,u_int)",
   "invalid path");

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  return OTC_ROPayload(theNode,thePath,theLength,-3);
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::operator[](char const* thePath) const
{
  OTCLIB_ENSURE_FN((thePath != 0),
   "OTC_RWPayload::operator[](char const*)",
   "invalid path");

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  return OTC_ROPayload(theNode,thePath,strlen(thePath),-2);
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::operator[](int theIndex) const
{
  OTCLIB_ENSURE_FN((theIndex >= -1),
   "OTC_RWPayload::operator[](int)",
   "invalid index");

  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode == 0)
    return 0;

  if (theNode->name() != "array")
    return 0;

  OTCLIB_ENSURE_FN((theIndex < int(theNode->numChildren())),
   "OTC_RWPayload::operator[](int) const",
   "invalid index");

  if (theIndex >= int(theNode->numChildren()))
    return 0;

  return OTC_ROPayload(theNode,0,0,theIndex);
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::parent()
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->parent();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::parent() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->parent();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::firstChild()
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->firstChild();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::firstChild() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->firstChild();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::lastChild()
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->lastChild();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::lastChild() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->lastChild();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::prevSibling()
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->prevSibling();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::prevSibling() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->prevSibling();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_RWPayload OTC_RWPayload::nextSibling()
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->nextSibling();

  return theNode;
}

/* ------------------------------------------------------------------------- */
OTC_ROPayload OTC_RWPayload::nextSibling() const
{
  OTC_DOMNode* theNode;
  theNode = resolve();

  if (theNode != 0)
    theNode = theNode->nextSibling();

  return theNode;
}

/* ------------------------------------------------------------------------- */
void OTC_RWPayload::pruneChild(char const* thePath)
{
  OTCLIB_ENSURE_FN((thePath != 0),
   "OTC_RWPayload::pruneChild(char const*)",
   "invalid path string");

  OTC_DOMNode* theNode;
  theNode = resolve();

  OTC_RWPayload theObject(theNode,thePath,strlen(thePath),-2);
  theNode = theObject.resolve();

  OTC_DOMNode* theParent;
  theParent = theNode->parent();

  if (theNode != 0)
    theNode->destroy();

  while (theParent != 0)
  {
    theNode = theParent;

    theParent = theNode->parent();

    if (theNode->hasChildren())
      break;

    if (theParent == 0)
    {
      resetProperties(theNode);
      theNode->setName("scalar");

      break;
    }

    theNode->destroy();
  }
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decode(istream& theStream, u_int theCapacity)
{
  if (theCapacity == 0)
  {
    if (initialised_ == OTCLIB_FALSE)
    {
      mutex_.lock();

      int tmpSize = -1;
      char* tmpString;

      tmpString = getenv("OTCLIB_PAYLOADINPUT");
      if (tmpString != 0)
	tmpSize = atoi(tmpString);

      if (tmpSize <= 0)
	tmpSize = 8185;

      capacity_ = u_int(tmpSize);

      initialised_ = OTCLIB_TRUE;

      mutex_.unlock();
    }

    theCapacity = capacity_;
  }

  OTC_Capacity tmpCapacity(theCapacity);
  OTC_String theWorkarea(tmpCapacity);

  return decode_(theStream,theWorkarea);
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decode(
 OTC_String const& theString,
 u_int theCapacity
)
{
  if (theCapacity == 0)
    theCapacity = theString.length();

  istrstream theStream((char*)theString.string(),theString.length());

  OTC_Boolean theSuccess;
  theSuccess = decode(theStream,theCapacity);

  if (theSuccess != OTCLIB_FALSE)
  {
    theStream >> ws;
    if (!theStream.eof())
      theSuccess = OTCLIB_FALSE;
  }

  return theSuccess;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decode_(istream& theStream, OTC_String& theWorkarea)
{
  OTC_DOMNode* theNode;
  theNode = install();

  if (theNode == 0)
    return OTCLIB_FALSE;

  OTC_Capacity theCapacity(63);
  OTC_String theString(theCapacity);

  OTC_Boolean theSuccess;

  theSuccess = OTCLIB_TRUE;

  // Remove anything the node may
  // already hold.

  theNode->setName(0);
  theNode->setContent(0);
  theNode->removeAllProperties();
  theNode->removeAllChildren();

  // Skip leading white space.

  theStream >> ws;

  // Next thing should be the actual
  // root element tag. Determine what
  // it is then recursively look for
  // the elements it contains.

  if (theSuccess != OTCLIB_FALSE)
  {
    theStream >> ws;

    if (theStream.peek() != '<')
      theSuccess = OTCLIB_FALSE;
  }

  if (theSuccess != OTCLIB_FALSE)
  {
    theStream.ignore(1);

    OTC_CBitSet theDelims("/>\t\n\r ");

    theString.truncate();

    int c;
    
    c = theStream.peek();

    while (theDelims.test(c) == 0)
    {
      theString += theStream.get();

      c = theStream.peek();
    }

    if (!theString.isEmpty())
    {
      theNode->setName(theString);

      theSuccess = decodeElement_(theStream,theString,theNode,theWorkarea);
    }
    else
      theSuccess = OTCLIB_FALSE;
  }

  // If things have failed, cleanup.

  if (theSuccess == OTCLIB_FALSE)
  {
    // theStream.setstate(ios::failbit);
    theStream.clear(ios::failbit | theStream.rdstate());

    theNode->removeAllProperties();
    theNode->removeAllChildren();
    theNode->setContent(0);
    theNode->setName("invalid");
  }

  return theSuccess;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decodeElement_(
 istream& theStream,
 OTC_String const& theTag,
 OTC_DOMNode* theNode,
 OTC_String& theWorkarea
)
{
  // Read in any properties.

  theStream >> ws;

  while (theStream.peek() != '>' && theStream.peek() != '/')
  {
    // Decode each property.

    if (decodeProperty_(theStream,theNode) == OTCLIB_FALSE)
      return OTCLIB_FALSE;

    theStream >> ws;
  }

  if (theStream.peek() == '/')
  {
    // Can only be a empty element.

    theStream.ignore(1);

    if (theStream.peek() != '>')
      return OTCLIB_FALSE;

    theStream.ignore(1);

    if (theTag == "index" || theTag == "array" ||
     theTag == "scalar" || theTag == "invalid")
    {
      return OTCLIB_TRUE;
    }
  }
  else
  {
    theStream.ignore(1);

    // Work out what to do with this
    // type of element.

    if (theTag == "index" || theTag == "array")
      return decodeChildren_(theStream,theTag,theNode,theWorkarea);
    else if (theTag == "scalar" || theTag == "invalid")
      return decodeContent_(theStream,theTag,theNode,theWorkarea);

    // Unrecognised type of element.
  }

  return OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decodeProperty_(
 istream& theStream,
 OTC_DOMNode* theNode
)
{
  OTC_Capacity theCapacity(63);
  OTC_String theName(theCapacity);

  theName.readToDelim(theStream,'=');

  if (theStream.fail())
    return OTCLIB_FALSE;

  theName.rtrim();

  if (!validProperty(theName))
    return OTCLIB_FALSE;

  theStream >> ws;

  OTC_String theValue(theCapacity);

  if (theStream.peek() == '"')
  {
    theStream.ignore(1);

    if (decodeString_(theStream,'"',theValue) == OTCLIB_FALSE)
      return OTCLIB_FALSE;

    theNode->setProperty(theName,theValue,theValue.length());
  }
  else if (theStream.peek() == '\'')
  {
    theStream.ignore(1);

    if (decodeString_(theStream,'\'',theValue) == OTCLIB_FALSE)
      return OTCLIB_FALSE;

    theNode->setProperty(theName,theValue,theValue.length());
  }
  else
    return OTCLIB_FALSE;

  return OTCLIB_TRUE;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decodeChildren_(
 istream& theStream,
 OTC_String const& theTag,
 OTC_DOMNode* theNode,
 OTC_String& theWorkarea
)
{
  OTC_Capacity theCapacity(63);
  OTC_String theString(theCapacity);

  // Go through each top level element
  // and decode it, stop when we find
  // our end tag.

  while (1)
  {
    theStream >> ws;

    if (theStream.peek() != '<')
      return OTCLIB_FALSE;

    theStream.ignore(1);

    OTC_CBitSet theDelims(">\t\n\r ");
    if (theStream.peek() != '/')
      theDelims.set('/');

    theString.truncate();

    int c;
    
    c = theStream.peek();

    while (theDelims.test(c) == 0)
    {
      theString += theStream.get();

      c = theStream.peek();
    }

    if (theString.isEmpty())
      return OTCLIB_FALSE;

    if (theString[u_int(0)] == '/')
    {
      // Found an end tag.

      theStream >> ws;

      if (theStream.peek() != '>')
	return OTCLIB_FALSE;

      theStream.ignore(1);

      if (theString.compare(1,theTag))
	return OTCLIB_TRUE;
      else
	return OTCLIB_FALSE;
    }

    OTC_DOMNode* theChild;
    theChild = theNode->createChild();

    theChild->setName(theString);

    if (decodeElement_(theStream,theString,
     theChild,theWorkarea) == OTCLIB_FALSE)
    {
      return OTCLIB_FALSE;
    }
  }

  return OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decodeContent_(
 istream& theStream,
 OTC_String const& theTag,
 OTC_DOMNode* theNode,
 OTC_String& theWorkarea
)
{
  theWorkarea.truncate();

  OTC_DOMProperty* theProperty;
  theProperty = theNode->property("type");

  if (theProperty != 0 && theProperty->value() == "xsd:base64Binary")
  {
    if (decodeBinary_(theStream,'<',theWorkarea) == OTCLIB_FALSE)
      return OTCLIB_FALSE;
  }
  else
  {
    if (decodeString_(theStream,'<',theWorkarea) == OTCLIB_FALSE)
      return OTCLIB_FALSE;
  }

  theNode->setContent(theWorkarea,theWorkarea.length());

  if (theStream.peek() != '/')
    return OTCLIB_FALSE;

  theStream.ignore();

  // Okay to reuse workarea here.

  theWorkarea.readToDelim(theStream,'>');

  theWorkarea.rtrim();

  if (theWorkarea != theTag)
    return OTCLIB_FALSE;

  return OTCLIB_TRUE;
}

/* ------------------------------------------------------------------------- */
OTC_Boolean OTC_RWPayload::decodeString_(
 istream& theStream,
 char theDelim,
 OTC_String& theString
)
{
  int theInput;

  OTC_Capacity theCapacity(15);
  OTC_String theEntity(theCapacity);

  char theBuffer[1024];
  u_int theActive = 0;

  theInput = theStream.get();

  while (theStream.good())
  {
    if (theInput == theDelim)
    {
      if (theActive != 0)
	theString.append(theBuffer,theActive);

      return OTCLIB_TRUE;
    }

    if (theInput == '&')
    {
      // This should be an XML escape.

      theEntity = OTC_String::get(theStream,';');

      if (!theStream.good())
	return OTCLIB_FALSE;

      theStream.ignore(1);

      if (theEntity.isEmpty())
	return OTCLIB_FALSE;

      if (theEntity[u_int(0)] == '#')
      {
	// Character reference.

	if (theEntity.length() > 2)
	{
	  if (theEntity[u_int(1)] == 'x')
	  {
	    // Hexadecimal.

	    int v1 = 0;
	    char v2 = 0;

	    if (sscanf(&theEntity[u_int(2)],"%x%c",&v1,&v2) != 1)
	      return OTCLIB_FALSE;

	    if (v1 > 0xFF)
	      return OTCLIB_FALSE;

	    // theString += char(v1);

	    theBuffer[theActive++] = char(v1);

	    if (theActive == sizeof(theBuffer))
	    {
	      theString.append(theBuffer,theActive);
	      theActive = 0;
	    }
	  }
	  else
	  {
	    // Numeric.

	    int v1 = 0;
	    char v2 = 0;

	    if (sscanf(&theEntity[u_int(1)],"%u%c",&v1,&v2) != 1)
	      return OTCLIB_FALSE;

	    if (v1 > 0xFF)
	      return OTCLIB_FALSE;

	    // theString += char(v1);

	    theBuffer[theActive++] = char(v1);

	    if (theActive == sizeof(theBuffer))
	    {
	      theString.append(theBuffer,theActive);
	      theActive = 0;
	    }
	  }
        }
	else
	  return OTCLIB_FALSE;
      }
      else
      {
	// Entity reference.

	if (theEntity == "amp")
	{
	  // theString += '&';
	  theBuffer[theActive++] = '&';
	}
	else if (theEntity == "lt")
	{
	  // theString += '<';
	  theBuffer[theActive++] = '<';
	}
	else if (theEntity == "gt")
	{
	  // theString += '>';
	  theBuffer[theActive++] = '>';
	}
	else if (theEntity == "apos")
	{
	  // theString += '\'';
	  theBuffer[theActive++] = '\'';
	}
	else if (theEntity == "quot")
	{
	  // theString += '"';
	  theBuffer[theActive++] = '"';
	}
	else
	  return OTCLIB_FALSE;

	if (theActive == sizeof(theBuffer))
	{
	  theString.append(theBuffer,theActive);
	  theActive = 0;
	}
      }
    }
    else
    {
      // theString += theInput;

      theBuffer[theActive++] = theInput;

      if (theActive == sizeof(theBuffer))
      {
	theString.append(theBuffer,theActive);
	theActive = 0;
      }
    }

    theInput = theStream.get();
  }

  if (theStream.fail())
    return OTCLIB_FALSE;

  if (theActive != 0)
    theString.append(theBuffer,theActive);

  return OTCLIB_TRUE;
}

/* ------------------------------------------------------------------------- */
/* Base-64 decoding.  This represents binary data as printable ASCII
** characters.  Three 8-bit binary bytes are turned into four 6-bit
** values, like so:
**
**   [11111111]  [22222222]  [33333333]
**
**   [111111] [112222] [222233] [333333]
**
** Then the 6-bit values are represented using the characters "A-Za-z0-9+/".
*/

static int b64Decode_table[256] = {
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 00-0F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 10-1F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,  /* 20-2F */
  52,53,54,55,56,57,58,59,60,61,-1,-1,-1,-1,-1,-1,  /* 30-3F */
  -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,  /* 40-4F */
  15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,  /* 50-5F */
  -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,  /* 60-6F */
  41,42,43,44,45,46,47,48,49,50,51,-1,-1,-1,-1,-1,  /* 70-7F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 80-8F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* 90-9F */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* A0-AF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* B0-BF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* C0-CF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* D0-DF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,  /* E0-EF */
  -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1   /* F0-FF */
};

/* Do base-64 decoding on a string.  Ignore any non-base64 bytes.
** Return the actual number of bytes generated.  The decoded size will
** be at most 3/4 the size of the encoded, and may be smaller if there
** are padding characters (blanks, newlines).
*/

OTC_Boolean OTC_RWPayload::decodeBinary_(
 istream& theStream,
 char theDelim,
 OTC_String& theString
)
{
  int d = 0;
  int prev_d = 0;
  unsigned char c = 0;

  int thePhase = 0;

  int theInput;

  theStream >> ws;

  theInput = theStream.get();

  while (theStream.good())
  {
    if (theInput == theDelim)
    {
      if (thePhase == 0)
	return OTCLIB_TRUE;

      return OTCLIB_FALSE;
    }
    else if (isspace(theInput))
    {
      if (thePhase == 0)
      {
	theStream >> ws;

	theInput = theStream.get();

	if (theInput == theDelim)
	  return OTCLIB_TRUE;

        // return OTCLIB_FALSE;
      }
    }

    if (theInput != '=')
    {
      d = b64Decode_table[theInput];

      if (d != -1)
      {
	switch (thePhase)
	{
	  case 0:
	  {
	    thePhase++;

	    break;
	  }

	  case 1:
	  {
	    c = ((prev_d << 2) | ((d & 0x30) >> 4));

	    theString += c;

	    thePhase++;

	    break;
	  }

	  case 2:
	  {
	    c = (((prev_d & 0x0f) << 4) | ((d & 0x3c) >> 2));

	    theString += c;

	    thePhase++;

	    break;
	  }

	  case 3:
	  {
	    c = (((prev_d & 0x03) << 6) | d);

	    theString += c;

	    thePhase = 0;

	    break;
	  }
	}
   
	prev_d = d;
      }
      else
      {
	theStream.putback(theInput);

	return OTCLIB_FALSE;
      }
    }
    else
    {
      switch (thePhase)
      {
	case 2:
	{
	  if ((prev_d & 0x0f) != 0)
            return OTCLIB_FALSE;

	  theInput = theStream.get();

	  if (theInput != '=')
	    return OTCLIB_FALSE;

	  theInput = theStream.get();

	  if (isspace(theInput))
	  {
	    theStream >> ws;
	    theInput = theStream.get();
	  }

	  if (theInput != theDelim)
	    return OTCLIB_FALSE;

	  return OTCLIB_TRUE;
	}

	case 3:
	{
	  if ((prev_d & 0x03) != 0)
            return OTCLIB_FALSE;

	  theInput = theStream.get();

	  if (isspace(theInput))
	  {
	    theStream >> ws;
	    theInput = theStream.get();
	  }

	  if (theInput != theDelim)
	    return OTCLIB_FALSE;

	  return OTCLIB_TRUE;
	}

	default:
	{
	  return OTCLIB_FALSE;
	}
      }
    }

    theInput = theStream.get();
  }

  return OTCLIB_FALSE;
}

/* ------------------------------------------------------------------------- */
