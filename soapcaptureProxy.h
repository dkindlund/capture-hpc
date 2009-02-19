/* soapcaptureProxy.h
   Generated by gSOAP 2.7.10 from captureGSOAP.h
   Copyright(C) 2000-2008, Robert van Engelen, Genivia Inc. All Rights Reserved.
   This part of the software is released under one of the following licenses:
   GPL, the gSOAP public license, or Genivia's license for commercial use.
*/

#ifndef soapcaptureProxy_H
#define soapcaptureProxy_H
#include "soapH.h"
class capture
{   public:
	/// Runtime engine context allocated in constructor
	struct soap *soap;
	/// Endpoint URL of service 'capture' (change as needed)
	const char *endpoint;
	/// Constructor allocates soap engine context, sets default endpoint URL, and sets namespace mapping table
	capture()
	{ soap = soap_new(); endpoint = "http://0.0.0.0:1234"; if (soap && !soap->namespaces) { static const struct Namespace namespaces[] = 
{
	{"SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/", "http://www.w3.org/*/soap-envelope", NULL},
	{"SOAP-ENC", "http://schemas.xmlsoap.org/soap/encoding/", "http://www.w3.org/*/soap-encoding", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"ns", "capture", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};
	soap->namespaces = namespaces; } };
	/// Destructor frees deserialized data and soap engine context
	virtual ~capture() { if (soap) { soap_destroy(soap); soap_end(soap); soap_free(soap); } };
	/// Invoke 'ping' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__ping(char *a, char **result) { return soap ? soap_call_ns__ping(soap, endpoint, NULL, a, result) : SOAP_EOM; };
	/// Invoke 'visitURL' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__visitURL(char *URL, int &result) { return soap ? soap_call_ns__visitURL(soap, endpoint, NULL, URL, result) : SOAP_EOM; };
	/// Invoke 'sendFileBase64' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__sendFileBase64(char *fileName, char *data, unsigned int encodedLength, unsigned int decodedLength, int &result) { return soap ? soap_call_ns__sendFileBase64(soap, endpoint, NULL, fileName, data, encodedLength, decodedLength, result) : SOAP_EOM; };
	/// Invoke 'receiveFileBase64' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__receiveFileBase64(char *fileName, struct s1 **result) { return soap ? soap_call_ns__receiveFileBase64(soap, endpoint, NULL, fileName, result) : SOAP_EOM; };
	/// Invoke 'openDocument' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__openDocument(char *fileName, int waitTimeMillisec, int &result) { return soap ? soap_call_ns__openDocument(soap, endpoint, NULL, fileName, waitTimeMillisec, result) : SOAP_EOM; };
	/// Invoke 'returnRegistryEvents' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__returnRegistryEvents(int maxEventsToReturn, struct ns__dynRegArray **result) { return soap ? soap_call_ns__returnRegistryEvents(soap, endpoint, NULL, maxEventsToReturn, result) : SOAP_EOM; };
	/// Invoke 'returnFileEvents' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__returnFileEvents(int maxEventsToReturn, struct ns__dynFileArray **result) { return soap ? soap_call_ns__returnFileEvents(soap, endpoint, NULL, maxEventsToReturn, result) : SOAP_EOM; };
	/// Invoke 'returnProcessEvents' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__returnProcessEvents(int maxEventsToReturn, struct ns__dynProcArray **result) { return soap ? soap_call_ns__returnProcessEvents(soap, endpoint, NULL, maxEventsToReturn, result) : SOAP_EOM; };
	/// Invoke 'returnEvents' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__returnEvents(int maxEventsToReturn, struct a1 **result) { return soap ? soap_call_ns__returnEvents(soap, endpoint, NULL, maxEventsToReturn, result) : SOAP_EOM; };
	/// Invoke 'sendMIME' of service 'capture' and return error code (or SOAP_OK)
	virtual int ns__sendMIME(int magicNumber, int &result) { return soap ? soap_call_ns__sendMIME(soap, endpoint, NULL, magicNumber, result) : SOAP_EOM; };
};
#endif