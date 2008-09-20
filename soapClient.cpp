/* soapClient.cpp
   Generated by gSOAP 2.7.10 from captureGSOAP.h
   Copyright(C) 2000-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapClient.cpp ver 2.7.10 2008-09-17 18:29:41 GMT")


SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__ping(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *a, char **result)
{	struct ns__ping soap_tmp_ns__ping;
	struct ns__pingResponse *soap_tmp_ns__pingResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__ping.a = a;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__ping(soap, &soap_tmp_ns__ping);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__ping(soap, &soap_tmp_ns__ping, "ns:ping", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__ping(soap, &soap_tmp_ns__ping, "ns:ping", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	*result = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__pingResponse = soap_get_ns__pingResponse(soap, NULL, "ns:pingResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (result && soap_tmp_ns__pingResponse->result)
		*result = *soap_tmp_ns__pingResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__visitURL(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *URL, int &result)
{	struct ns__visitURL soap_tmp_ns__visitURL;
	struct ns__visitURLResponse *soap_tmp_ns__visitURLResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__visitURL.URL = URL;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__visitURL(soap, &soap_tmp_ns__visitURL);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__visitURL(soap, &soap_tmp_ns__visitURL, "ns:visitURL", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__visitURL(soap, &soap_tmp_ns__visitURL, "ns:visitURL", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__visitURLResponse = soap_get_ns__visitURLResponse(soap, NULL, "ns:visitURLResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns__visitURLResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__sendFileBase64(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *fileName, char *data, unsigned int encodedLength, unsigned int decodedLength, int &result)
{	struct ns__sendFileBase64 soap_tmp_ns__sendFileBase64;
	struct ns__sendFileBase64Response *soap_tmp_ns__sendFileBase64Response;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__sendFileBase64.fileName = fileName;
	soap_tmp_ns__sendFileBase64.data = data;
	soap_tmp_ns__sendFileBase64.encodedLength = encodedLength;
	soap_tmp_ns__sendFileBase64.decodedLength = decodedLength;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__sendFileBase64(soap, &soap_tmp_ns__sendFileBase64);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__sendFileBase64(soap, &soap_tmp_ns__sendFileBase64, "ns:sendFileBase64", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__sendFileBase64(soap, &soap_tmp_ns__sendFileBase64, "ns:sendFileBase64", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__sendFileBase64Response = soap_get_ns__sendFileBase64Response(soap, NULL, "ns:sendFileBase64Response", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns__sendFileBase64Response->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__receiveFileBase64(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *fileName, struct s1 &result)
{	struct ns__receiveFileBase64 soap_tmp_ns__receiveFileBase64;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__receiveFileBase64.fileName = fileName;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__receiveFileBase64(soap, &soap_tmp_ns__receiveFileBase64);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__receiveFileBase64(soap, &soap_tmp_ns__receiveFileBase64, "ns:receiveFileBase64", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__receiveFileBase64(soap, &soap_tmp_ns__receiveFileBase64, "ns:receiveFileBase64", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	soap_default_ns__receiveFileStruct(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get_ns__receiveFileStruct(soap, &result, "ns:receiveFileStruct", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__openDocument(struct soap *soap, const char *soap_endpoint, const char *soap_action, char *fileName, int waitTimeMillisec, int &result)
{	struct ns__openDocument soap_tmp_ns__openDocument;
	struct ns__openDocumentResponse *soap_tmp_ns__openDocumentResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__openDocument.fileName = fileName;
	soap_tmp_ns__openDocument.waitTimeMillisec = waitTimeMillisec;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__openDocument(soap, &soap_tmp_ns__openDocument);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__openDocument(soap, &soap_tmp_ns__openDocument, "ns:openDocument", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__openDocument(soap, &soap_tmp_ns__openDocument, "ns:openDocument", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__openDocumentResponse = soap_get_ns__openDocumentResponse(soap, NULL, "ns:openDocumentResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns__openDocumentResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__returnRegistryEvents(struct soap *soap, const char *soap_endpoint, const char *soap_action, int maxEventsToReturn, struct ns__dynRegArray **result)
{	struct ns__returnRegistryEvents soap_tmp_ns__returnRegistryEvents;
	struct ns__returnRegistryEventsResponse *soap_tmp_ns__returnRegistryEventsResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__returnRegistryEvents.maxEventsToReturn = maxEventsToReturn;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__returnRegistryEvents(soap, &soap_tmp_ns__returnRegistryEvents);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__returnRegistryEvents(soap, &soap_tmp_ns__returnRegistryEvents, "ns:returnRegistryEvents", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__returnRegistryEvents(soap, &soap_tmp_ns__returnRegistryEvents, "ns:returnRegistryEvents", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	*result = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__returnRegistryEventsResponse = soap_get_ns__returnRegistryEventsResponse(soap, NULL, "ns:returnRegistryEventsResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (result && soap_tmp_ns__returnRegistryEventsResponse->result)
		*result = *soap_tmp_ns__returnRegistryEventsResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__returnFileEvents(struct soap *soap, const char *soap_endpoint, const char *soap_action, int maxEventsToReturn, struct ns__dynFileArray **result)
{	struct ns__returnFileEvents soap_tmp_ns__returnFileEvents;
	struct ns__returnFileEventsResponse *soap_tmp_ns__returnFileEventsResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__returnFileEvents.maxEventsToReturn = maxEventsToReturn;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__returnFileEvents(soap, &soap_tmp_ns__returnFileEvents);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__returnFileEvents(soap, &soap_tmp_ns__returnFileEvents, "ns:returnFileEvents", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__returnFileEvents(soap, &soap_tmp_ns__returnFileEvents, "ns:returnFileEvents", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	*result = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__returnFileEventsResponse = soap_get_ns__returnFileEventsResponse(soap, NULL, "ns:returnFileEventsResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (result && soap_tmp_ns__returnFileEventsResponse->result)
		*result = *soap_tmp_ns__returnFileEventsResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__returnProcessEvents(struct soap *soap, const char *soap_endpoint, const char *soap_action, int maxEventsToReturn, struct ns__dynProcArray **result)
{	struct ns__returnProcessEvents soap_tmp_ns__returnProcessEvents;
	struct ns__returnProcessEventsResponse *soap_tmp_ns__returnProcessEventsResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__returnProcessEvents.maxEventsToReturn = maxEventsToReturn;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__returnProcessEvents(soap, &soap_tmp_ns__returnProcessEvents);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__returnProcessEvents(soap, &soap_tmp_ns__returnProcessEvents, "ns:returnProcessEvents", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__returnProcessEvents(soap, &soap_tmp_ns__returnProcessEvents, "ns:returnProcessEvents", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	*result = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__returnProcessEventsResponse = soap_get_ns__returnProcessEventsResponse(soap, NULL, "ns:returnProcessEventsResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (result && soap_tmp_ns__returnProcessEventsResponse->result)
		*result = *soap_tmp_ns__returnProcessEventsResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__returnEvents(struct soap *soap, const char *soap_endpoint, const char *soap_action, int maxEventsToReturn, struct a1 **result)
{	struct ns__returnEvents soap_tmp_ns__returnEvents;
	struct ns__returnEventsResponse *soap_tmp_ns__returnEventsResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__returnEvents.maxEventsToReturn = maxEventsToReturn;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__returnEvents(soap, &soap_tmp_ns__returnEvents);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__returnEvents(soap, &soap_tmp_ns__returnEvents, "ns:returnEvents", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__returnEvents(soap, &soap_tmp_ns__returnEvents, "ns:returnEvents", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	*result = NULL;
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__returnEventsResponse = soap_get_ns__returnEventsResponse(soap, NULL, "ns:returnEventsResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	if (result && soap_tmp_ns__returnEventsResponse->result)
		*result = *soap_tmp_ns__returnEventsResponse->result;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call_ns__sendMIME(struct soap *soap, const char *soap_endpoint, const char *soap_action, int magicNumber, int &result)
{	struct ns__sendMIME soap_tmp_ns__sendMIME;
	struct ns__sendMIMEResponse *soap_tmp_ns__sendMIMEResponse;
	if (!soap_endpoint)
		soap_endpoint = "http://0.0.0.0:1234";
	soap->encodingStyle = "";
	soap_tmp_ns__sendMIME.magicNumber = magicNumber;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize_ns__sendMIME(soap, &soap_tmp_ns__sendMIME);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_ns__sendMIME(soap, &soap_tmp_ns__sendMIME, "ns:sendMIME", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_ns__sendMIME(soap, &soap_tmp_ns__sendMIME, "ns:sendMIME", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	soap_default_int(soap, &result);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_tmp_ns__sendMIMEResponse = soap_get_ns__sendMIMEResponse(soap, NULL, "ns:sendMIMEResponse", "");
	if (soap->error)
	{	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
			return soap_recv_fault(soap);
		return soap_closesock(soap);
	}
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	result = soap_tmp_ns__sendMIMEResponse->result;
	return soap_closesock(soap);
}

/* End of soapClient.cpp */
