/*This file can be renamed later, but just plain "soapserver.cpp"
**is already created automatically by the soapcpp2 tool
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/


#include "CaptureSoapServer.h"

#include "soapH.h" 
#include "capture.nsmap" 
#include "Visitor.h"
#include "b64.h"

CaptureSoapServer::CaptureSoapServer(Visitor* v){
	CaptureSoapServerThread = new Thread(this);
	CaptureSoapServerThread->start("CaptureSoapServer");
}

CaptureSoapServer::~CaptureSoapServer(){}

void
CaptureSoapServer::run(){

	char debug = 0;
	//The below code is taken verbatim from the gsoap standalone server example page
   struct soap soap;
   SOCKET m, s; // master and slave sockets


   soap_init(&soap);
   //TODO: This needs to be configurable
   m = soap_bind(&soap, "192.168.0.131", 1234, 100);
   if (m < 0)
      soap_print_fault(&soap, stderr);
   else
   {
      if(debug) fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
      for (int i = 1; ; i++)
      {
         s = soap_accept(&soap);
         if (s < 0)
         {
            soap_print_fault(&soap, stderr);
            break;
         }
         if(debug) fprintf(stderr, "%d: accepted connection from IP=%d.%d.%d.%d socket=%d\n", i,
            (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF, s);
         if (soap_serve(&soap) != SOAP_OK) // process RPC request
            soap_print_fault(&soap, stderr); // print error
         if(debug) fprintf(stderr, "you got served!\n");
         soap_destroy(&soap); // clean up class instances
         soap_end(&soap); // clean up everything and close socket
      }
   }
   soap_done(&soap); // close master socket and detach environment

}


// Implementation of the "add" remote method: 
int ns__add(struct soap *soap, int a, int b, int &result) 
{ 
   //printf("add got %d and %d\n", a, b);
   result = a + b;

   return SOAP_OK;
} 

int ns__ping(struct soap *soap, char * a, char ** result) 
{ 
   printf("%s\n", a);
   *result = "pong";

   return SOAP_OK; 
}

int ns__visit(struct soap *soap, char * url, char ** result){
	wchar_t xURL[1024];
	wsprintf(xURL, L"%hs", url);
	//Build my own new-fangled Element to pass to Visitor:onServerEvent which I think will open 
    typedef boost::signal<void (Element*)> signal_serverEvent;
	Attribute att;
	att.name = L"url";
	att.value = xURL; //Now expecting you to pass the URL
	Element e;
	e.name = L"visit";
	e.attributes.push_back(att);
	e.data = NULL;
	e.dataLength = 0;
	printf("visiting %s\n", url);
	EventController::getInstance()->notifyListeners(&e);

	return SOAP_OK;
}

// Implementation of the "sub" remote method: 
int ns__sub(struct soap *soap, double a, double b, double &result) 
{ 
   result = a - b; 
   return SOAP_OK; 
}


int ns__junks(struct soap *soap, char * a, ns__myStruct &result)
{
	printf("in ns__junks\n");
	ns__myStruct x;
	x.first = "a";
	x.last = "b";
	result = x;

	return SOAP_OK;
}

int ns__sendFileBase64(struct soap *soap, char * fileName, char * data, unsigned int encodedLength, unsigned int decodedLength, ns__myStruct &result){
	printf("in ns__sendFileBase64\n");

	printf("encodedLength = %d, decodedLength = %d, data[0][1][2][3] = %c%c%c%c\n", encodedLength, decodedLength,
		data[0], data[1], data[2], data[3]);

	//Sanity check
	if(decodedLength != b64::b64_decode(data, encodedLength, NULL, NULL)){
		printf("The decode will not be correct. Exiting\n");
		return SOAP_ERR;
	}
	//Decode the data
	char * decodedData = new char[decodedLength];
	b64::b64_decode(data, encodedLength, decodedData, decodedLength);

	printf("decodedData[0][1] = %c%c\n", decodedData[0], decodedData[1]);
	//Open a file to write the decoded data to
	HANDLE myHandle = CreateFileA(fileName, (GENERIC_READ | GENERIC_WRITE), 
									NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(myHandle == INVALID_HANDLE_VALUE){
		printf("couldn't open the file. Exiting\n");
		return SOAP_ERR;
	}

	//Write the data
	DWORD numWrote;
	BOOL b = WriteFile(myHandle, decodedData, decodedLength, &numWrote, NULL);
	if(b){
		printf("Wrote %d bytes of data to %s\n", numWrote, fileName);
	}
	CloseHandle(myHandle);
	delete[] decodedData;

	ns__myStruct x;
	x.first = "a";
	x.last = "b";
	result = x;

	return SOAP_OK;

}

int ns__receiveFileBase64(struct soap *soap, char * fileName, ns__receiveFileStruct &result){
	printf("in ns__receiveFileBase64, about to open %s\n", fileName);

	//Open the file
	HANDLE myHandle = CreateFileA(fileName, GENERIC_READ, NULL, NULL, 
									OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(myHandle == INVALID_HANDLE_VALUE){
		printf("couldn't open the file %s. Exiting\n", fileName);
		return SOAP_ERR;
	}

	//Get the size and then read it into a buffer
	unsigned int fileSize = (unsigned int)GetFileSize(myHandle, NULL);
	if(fileSize <= 0){
		printf("Error, or zero-length file\n");
		return SOAP_ERR;
	}
	char * buffer = new char[fileSize];

	DWORD numRead = 0;
	BOOL b = ReadFile(myHandle, buffer, fileSize, &numRead,NULL);
	if(!b || numRead != fileSize){
		printf("ReadFile error\n");
		return SOAP_ERR;
	}
	else{
		printf("Read the file successfully\n");
	}

	//base64 the file
	unsigned int encodedLength = b64::b64_encode(buffer, fileSize, NULL, NULL);
	char * encodedData = new char[encodedLength];
	size_t ret = b64::b64_encode(buffer, fileSize, encodedData, encodedLength);
	if(ret == 0){
		printf("size of the buffer was insufficient, or the length of the * converted buffer was longer than destLen\n");
		return SOAP_ERR;
	}

	//return the file
	result.data = encodedData;
	result.encodedLength = encodedLength;
	result.decodedLength = fileSize;

	printf("cleaning up\n");
	CloseHandle(myHandle);
	delete[] buffer;
	delete[] encodedData;

	return SOAP_OK;
}

int ns__sendMIME(struct soap *soap, int magicNumber, int &result){
	printf("In ns__sendMIME\n");

	struct soap_multipart * attachment;
	for(attachment = soap->mime.list; attachment; attachment = attachment->next){
	   printf("MIME attachment:\n"); 
	   printf("Memory=%p\n", (*attachment).ptr); 
	   printf("Size=%ul\n", (*attachment).size); 
	   printf("Encoding=%d\n", (int)(*attachment).encoding); 
	   printf("Type=%s\n", (*attachment).type?(*attachment).type:"null"); 
	   printf("ID=%s\n", (*attachment).id?(*attachment).id:"null"); 
	   printf("Location=%s\n", (*attachment).location?(*attachment).location:"null"); 
	   printf("Description=%s\n", (*attachment).description?(*attachment).description:"null");
	}

	printf("magicNumber = %#x\n", magicNumber);
	if(magicNumber == 123){
		result = 42;
	}
	else{
		result = 0;
	}

	return SOAP_OK;
}