/* soapServer.cpp
   Generated by gSOAP 2.7.10 from captureGSOAP.h
   Copyright(C) 2000-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapServer.cpp ver 2.7.10 2008-08-29 08:41:22 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap *soap)
{
#ifndef WITH_FASTCGI
	unsigned int k = soap->max_keep_alive;
#endif

	do
	{
#ifdef WITH_FASTCGI
		if (FCGI_Accept() < 0)
		{
			soap->error = SOAP_EOF;
			return soap_send_fault(soap);
		}
#endif

		soap_begin(soap);

#ifndef WITH_FASTCGI
		if (soap->max_keep_alive > 0 && !--k)
			soap->keep_alive = 0;
#endif

		if (soap_begin_recv(soap))
		{	if (soap->error < SOAP_STOP)
			{
#ifdef WITH_FASTCGI
				soap_send_fault(soap);
#else 
				return soap_send_fault(soap);
#endif
			}
			soap_closesock(soap);

			continue;
		}

		if (soap_envelope_begin_in(soap)
		 || soap_recv_header(soap)
		 || soap_body_begin_in(soap)
		 || soap_serve_request(soap)
		 || (soap->fserveloop && soap->fserveloop(soap)))
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			return soap_send_fault(soap);
#endif
		}

#ifdef WITH_FASTCGI
		soap_destroy(soap);
		soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	return SOAP_OK;
}

#ifndef WITH_NOSERVEREQUEST
SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
	soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:ping"))
		return soap_serve_ns__ping(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:visitURL"))
		return soap_serve_ns__visitURL(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:sendFileBase64"))
		return soap_serve_ns__sendFileBase64(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:receiveFileBase64"))
		return soap_serve_ns__receiveFileBase64(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:openDocument"))
		return soap_serve_ns__openDocument(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:returnEvents"))
		return soap_serve_ns__returnEvents(soap);
	if (!soap_match_tag(soap, soap->tag, "ns:sendMIME"))
		return soap_serve_ns__sendMIME(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__ping(struct soap *soap)
{	struct ns__ping soap_tmp_ns__ping;
	struct ns__pingResponse soap_tmp_ns__pingResponse;
	char * soap_tmp_string;
	soap_default_ns__pingResponse(soap, &soap_tmp_ns__pingResponse);
	soap_tmp_string = NULL;
	soap_tmp_ns__pingResponse.result = &soap_tmp_string;
	soap_default_ns__ping(soap, &soap_tmp_ns__ping);
	soap->encodingStyle = "";
	if (!soap_get_ns__ping(soap, &soap_tmp_ns__ping, "ns:ping", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__ping(soap, soap_tmp_ns__ping.a, &soap_tmp_string);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__pingResponse(soap, &soap_tmp_ns__pingResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__pingResponse(soap, &soap_tmp_ns__pingResponse, "ns:pingResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__pingResponse(soap, &soap_tmp_ns__pingResponse, "ns:pingResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__visitURL(struct soap *soap)
{	struct ns__visitURL soap_tmp_ns__visitURL;
	struct ns__visitURLResponse soap_tmp_ns__visitURLResponse;
	soap_default_ns__visitURLResponse(soap, &soap_tmp_ns__visitURLResponse);
	soap_default_ns__visitURL(soap, &soap_tmp_ns__visitURL);
	soap->encodingStyle = "";
	if (!soap_get_ns__visitURL(soap, &soap_tmp_ns__visitURL, "ns:visitURL", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__visitURL(soap, soap_tmp_ns__visitURL.URL, soap_tmp_ns__visitURLResponse.result);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__visitURLResponse(soap, &soap_tmp_ns__visitURLResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__visitURLResponse(soap, &soap_tmp_ns__visitURLResponse, "ns:visitURLResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__visitURLResponse(soap, &soap_tmp_ns__visitURLResponse, "ns:visitURLResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__sendFileBase64(struct soap *soap)
{	struct ns__sendFileBase64 soap_tmp_ns__sendFileBase64;
	struct ns__sendFileBase64Response soap_tmp_ns__sendFileBase64Response;
	soap_default_ns__sendFileBase64Response(soap, &soap_tmp_ns__sendFileBase64Response);
	soap_default_ns__sendFileBase64(soap, &soap_tmp_ns__sendFileBase64);
	soap->encodingStyle = "";
	if (!soap_get_ns__sendFileBase64(soap, &soap_tmp_ns__sendFileBase64, "ns:sendFileBase64", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__sendFileBase64(soap, soap_tmp_ns__sendFileBase64.fileName, soap_tmp_ns__sendFileBase64.data, soap_tmp_ns__sendFileBase64.encodedLength, soap_tmp_ns__sendFileBase64.decodedLength, soap_tmp_ns__sendFileBase64Response.result);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__sendFileBase64Response(soap, &soap_tmp_ns__sendFileBase64Response);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__sendFileBase64Response(soap, &soap_tmp_ns__sendFileBase64Response, "ns:sendFileBase64Response", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__sendFileBase64Response(soap, &soap_tmp_ns__sendFileBase64Response, "ns:sendFileBase64Response", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__receiveFileBase64(struct soap *soap)
{	struct ns__receiveFileBase64 soap_tmp_ns__receiveFileBase64;
	struct s1 result;
	soap_default_ns__receiveFileStruct(soap, &result);
	soap_default_ns__receiveFileBase64(soap, &soap_tmp_ns__receiveFileBase64);
	soap->encodingStyle = "";
	if (!soap_get_ns__receiveFileBase64(soap, &soap_tmp_ns__receiveFileBase64, "ns:receiveFileBase64", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__receiveFileBase64(soap, soap_tmp_ns__receiveFileBase64.fileName, result);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__receiveFileStruct(soap, &result);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__receiveFileStruct(soap, &result, "ns:receiveFileStruct", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__receiveFileStruct(soap, &result, "ns:receiveFileStruct", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__openDocument(struct soap *soap)
{	struct ns__openDocument soap_tmp_ns__openDocument;
	struct ns__openDocumentResponse soap_tmp_ns__openDocumentResponse;
	soap_default_ns__openDocumentResponse(soap, &soap_tmp_ns__openDocumentResponse);
	soap_default_ns__openDocument(soap, &soap_tmp_ns__openDocument);
	soap->encodingStyle = "";
	if (!soap_get_ns__openDocument(soap, &soap_tmp_ns__openDocument, "ns:openDocument", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__openDocument(soap, soap_tmp_ns__openDocument.fileName, soap_tmp_ns__openDocument.waitTimeMillisec, soap_tmp_ns__openDocumentResponse.result);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__openDocumentResponse(soap, &soap_tmp_ns__openDocumentResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__openDocumentResponse(soap, &soap_tmp_ns__openDocumentResponse, "ns:openDocumentResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__openDocumentResponse(soap, &soap_tmp_ns__openDocumentResponse, "ns:openDocumentResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__returnEvents(struct soap *soap)
{	struct ns__returnEvents soap_tmp_ns__returnEvents;
	struct ns__allEvents result;
	soap_default_ns__allEvents(soap, &result);
	soap_default_ns__returnEvents(soap, &soap_tmp_ns__returnEvents);
	soap->encodingStyle = "";
	if (!soap_get_ns__returnEvents(soap, &soap_tmp_ns__returnEvents, "ns:returnEvents", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__returnEvents(soap, soap_tmp_ns__returnEvents.maxEventsToReturn, result);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__allEvents(soap, &result);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__allEvents(soap, &result, "ns:allEvents", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__allEvents(soap, &result, "ns:allEvents", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_ns__sendMIME(struct soap *soap)
{	struct ns__sendMIME soap_tmp_ns__sendMIME;
	struct ns__sendMIMEResponse soap_tmp_ns__sendMIMEResponse;
	soap_default_ns__sendMIMEResponse(soap, &soap_tmp_ns__sendMIMEResponse);
	soap_default_ns__sendMIME(soap, &soap_tmp_ns__sendMIME);
	soap->encodingStyle = "";
	if (!soap_get_ns__sendMIME(soap, &soap_tmp_ns__sendMIME, "ns:sendMIME", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = ns__sendMIME(soap, soap_tmp_ns__sendMIME.magicNumber, soap_tmp_ns__sendMIMEResponse.result);
	if (soap->error)
		return soap->error;
	soap_serializeheader(soap);
	soap_serialize_ns__sendMIMEResponse(soap, &soap_tmp_ns__sendMIMEResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__sendMIMEResponse(soap, &soap_tmp_ns__sendMIMEResponse, "ns:sendMIMEResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__sendMIMEResponse(soap, &soap_tmp_ns__sendMIMEResponse, "ns:sendMIMEResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

/* End of soapServer.cpp */
