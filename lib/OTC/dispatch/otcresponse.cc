/*
// ============================================================================
//
// = LIBRARY
//     OTC
// 
// = FILENAME
//     dispatch/otcresponse.cc
//
// = AUTHOR(S)
//     Graham Dumpleton
// 
// = COPYRIGHT
//     Copyright 1999-2002 DUMPLETON SOFTWARE CONSULTING PTY LIMITED
//
// ============================================================================
*/

#ifdef __GNUG__
#if (__GNUC__ < 3) && (__GNUC__MINOR__ < 95)
#pragma implementation "OTC/dispatch/response.hh"
#endif
#endif

#include <OTC/dispatch/response.hh>

#include <OTC/dispatch/message.hh>

/* ------------------------------------------------------------------------- */
int OTCEV_Response::gTypeId_ = 0;

/* ------------------------------------------------------------------------- */
OTCEV_Response::~OTCEV_Response()
{
  envelope_->destroy();
}

/* ------------------------------------------------------------------------- */
OTCEV_Response::OTCEV_Response(OTCEV_Envelope* theEnvelope)
  : envelope_(0)
{
  OTCLIB_ENSURE_FN((theEnvelope != 0),
   "OTCEV_Response::OTCEV_Response(OTCEV_Envelope*)",
   "invalid envelope");

  envelope_ = (OTCEV_Envelope*)theEnvelope->clone();

  content_.decode(envelope_->message()->content());
}

/* ------------------------------------------------------------------------- */
void* OTCEV_Response::type() const
{
  return typeId();
}

/* ------------------------------------------------------------------------- */
void OTCEV_Response::dump(ostream& outs) const
{
  outs << "<OTC> RESPONSE - recipient = " << envelope_->to();
  outs << ", sender = " << envelope_->from();
  outs << ", id = " << envelope_->conversationId();
  outs << ", seq = " << envelope_->sequenceNumber();
  outs << ", contentType = " << envelope_->message()->contentType();
  outs << ", content = " << envelope_->message()->content();
}

/* ------------------------------------------------------------------------- */
