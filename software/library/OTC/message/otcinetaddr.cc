/*
// ============================================================================
//
// = LIBRARY
//     OTC
//
// = FILENAME
//     message/otcinetaddr.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
//
// = COPYRIGHT
//     Copyright 1991 OTC LIMITED
//     Copyright 1995-2004 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/message/inetaddr.hh"
#endif
#endif

#include <OTC/message/inetaddr.hh>

#if defined(HAVE_MEMORY_H)
#include <memory.h>
#endif

#if defined(OSE_SYS_UNIX)
#include <netdb.h>
#include <arpa/inet.h>
#endif

#if defined(OSE_SYS_WIN32)
#include <winsock2.h>
#include <windows.h>
#endif

/* ------------------------------------------------------------------------- */
char const* const OTC_InetAddress::ANY = "0.0.0.0";
char const* const OTC_InetAddress::BROADCAST = "255.255.255.255";

/* ------------------------------------------------------------------------- */
OTC_InetAddress::OTC_InetAddress()
{
  resetAddress();
}

/* ------------------------------------------------------------------------- */
OTC_InetAddress::OTC_InetAddress(char const* theHostName, int thePort)
{
  initAddress(theHostName,thePort);
}

/* ------------------------------------------------------------------------- */
OTC_InetAddress::OTC_InetAddress(int thePort)
{
  initAddress(thePort);
}

/* ------------------------------------------------------------------------- */
OTC_InetAddress::OTC_InetAddress(
 char const* theHostName,
 char const* theService,
 char const* theProtocol
)
{
  initAddress(theHostName,theService,theProtocol);
}

/* ------------------------------------------------------------------------- */
OTC_InetAddress::OTC_InetAddress(
 char const* theService,
 char const* theProtocol
)
{
  initAddress(theService,theProtocol);
}

/* ------------------------------------------------------------------------- */
OTC_InetAddress::OTC_InetAddress(OTC_InetAddress const& theAddress)
{
  length_ = theAddress.length_;
  memcpy(&address_,&theAddress.address_,length_);
}

/* ------------------------------------------------------------------------- */
OTC_InetAddress& OTC_InetAddress::operator=(OTC_InetAddress const& theAddress)
{
  length_ = theAddress.length_;
  memcpy(&address_,&theAddress.address_,length_);

  return *this;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetAddress::hostname() const
{
  hostent* h = 0;

#if defined(HAVE_FUNC_GETHOSTBYADDR_R_5)
    struct hostent hostEntBuf;
    struct hostent_data hostEntData;
    memset(&hostEntData,0,sizeof(hostEntData));

    if (!gethostbyaddr_r((char*)&address_.sin_addr,4,AF_INET,
     &hostEntBuf,&hostEntData)) h = &hostEntBuf;
#else
#if defined(HAVE_FUNC_GETHOSTBYADDR_R_7)
  struct hostent hostEntBuf;
  char stringBuf[2048];
  int result;

  h = gethostbyaddr_r((char*)&address_.sin_addr,
   sizeof(address_.sin_addr),AF_INET,&hostEntBuf,
   stringBuf,sizeof(stringBuf),&result);
#else
#if defined(HAVE_FUNC_GETHOSTBYADDR_R_8)
  struct hostent hostEntBuf;
  struct hostent* hostEntPtr = 0;
  char stringBuf[2048];
  memset(&stringBuf,0,sizeof(stringBuf));
  int herrno;

  if (!gethostbyaddr_r((char*)&address_.sin_addr,4,AF_INET,&hostEntBuf,
   stringBuf,sizeof(stringBuf),&hostEntPtr,&herrno)) h = &hostEntBuf;
#else
  h = gethostbyaddr((char*)&address_.sin_addr,
   sizeof(address_.sin_addr),AF_INET);
#endif
#endif
#endif

  if (h)
  {
    OTC_String theResult;
    theResult = numericHostname();

    if (theResult != h->h_name)
    {
      return h->h_name;
    }
    else
    {
      while (*h->h_aliases != 0)
      {
	if (theResult != *h->h_aliases)
	  return *h->h_aliases;

	h->h_aliases++;
      }

      return theResult;
    }
  }
  else
    return inet_ntoa(address_.sin_addr);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetAddress::numericHostname() const
{
  return inet_ntoa(address_.sin_addr);
}

/* ------------------------------------------------------------------------- */
int OTC_InetAddress::port() const
{
  return ntohs(address_.sin_port);
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetAddress::address() const
{
  OTC_String theResult;

  theResult += hostname();
  theResult += ':';

  char buf[32];
  sprintf(buf,"%d",port());

  theResult += buf;

  return theResult;
}

/* ------------------------------------------------------------------------- */
OTC_String OTC_InetAddress::numericAddress() const
{
  OTC_String theResult;

  theResult += numericHostname();
  theResult += ':';

  char buf[32];
  sprintf(buf,"%d",port());

  theResult += buf;

  return theResult;
}

/* ------------------------------------------------------------------------- */
sockaddr* OTC_InetAddress::addr()
{
  return (sockaddr*)(void*)&address_;
}

/* ------------------------------------------------------------------------- */
sockaddr const* OTC_InetAddress::addr() const
{
  return (sockaddr const*)(void const*)&address_;
}

/* ------------------------------------------------------------------------- */
void OTC_InetAddress::resetAddress()
{
  length_ = sizeof(address_);
  memset(&address_,0,length_);
}

/* ------------------------------------------------------------------------- */
void OTC_InetAddress::initAddress(char const* theHostName, int thePort)
{
  resetAddress();
  address_.sin_family = AF_INET;
  address_.sin_port = htons(thePort);
  address_.sin_addr.s_addr = inet_addr(theHostName);
  if (address_.sin_addr.s_addr == 0xFFFFFFFF)
  {
    hostent* h = 0;

#if defined(HAVE_FUNC_GETHOSTBYNAME_R_3)
    struct hostent hostEntBuf;
    struct hostent_data hostEntData;
    memset(&hostEntData,0,sizeof(hostEntData));

    if (!gethostbyname_r(theHostName,&hostEntBuf,&hostEntData))
      h = &hostEntBuf;
#else
#if defined(HAVE_FUNC_GETHOSTBYNAME_R_5)
    struct hostent hostEntBuf;
    char stringBuf[2048];
    int result;

    h = gethostbyname_r(theHostName,&hostEntBuf,stringBuf,
     sizeof(stringBuf),&result);
#else
#if defined(HAVE_FUNC_GETHOSTBYNAME_R_6)
  struct hostent hostEntBuf;
  struct hostent* hostEntPtr = 0;
  char stringBuf[2048];
  memset(&stringBuf,0,sizeof(stringBuf));
  int herrno;

  if (!gethostbyname_r(theHostName,&hostEntBuf,stringBuf,
   sizeof(stringBuf),&hostEntPtr,&herrno)) h = &hostEntBuf;
#else
    h = gethostbyname(theHostName);
#endif
#endif
#endif

    if (h)
      memcpy(&address_.sin_addr,h->h_addr,h->h_length);
  }
}

/* ------------------------------------------------------------------------- */
void OTC_InetAddress::initAddress(
 char const* theHostName,
 char const* theService,
 char const* theProtocol
)
{
  resetAddress();
  servent* s = 0;

#if defined(HAVE_FUNC_GETSERVBYNAME_R_4)
  struct servent servEntBuf;
  struct servent_data servEntData;
  memset(&servEntData,0,sizeof(servEntData));

  if (!getservbyname_r((char*)theService,(char*)theProtocol,
   &servEntBuf,&servEntData)) s = &servEntBuf;
#else
#if defined(HAVE_FUNC_GETSERVBYNAME_R_5)
  struct servent servEntBuf;
  char stringBuf[2048];

  s = getservbyname_r((char*)theService,(char*)theProtocol,&servEntBuf,
   stringBuf,sizeof(stringBuf));
#else
#if defined(HAVE_FUNC_GETSERVBYNAME_R_6)
  struct servent servEntBuf;
  char dataBuf[2048];
  memset(&dataBuf,0,sizeof(dataBuf));

  getservbyname_r((char*)theService,(char*)theProtocol,
   &servEntBuf,(char*)&dataBuf,sizeof(dataBuf),&s);
#else
  s = getservbyname((char*)theService,(char*)theProtocol);
#endif
#endif
#endif

  if (s)
    initAddress(theHostName,ntohs(s->s_port));
}

/* ------------------------------------------------------------------------- */
