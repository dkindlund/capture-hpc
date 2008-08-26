/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#include "CaptureSoapServer.h"
#include "soapH.h"
#include "capture.nsmap" 
#include "Visitor.h"
#include "b64.h" //nice small 3rd party lib for base64 encode/decode

//Only global so that the destructors can be used in ~CaptureSoapServer
//Didn't go in the class definition because adding soapH.h to CaptureSoapServer.h caused compile issues
//that I didn't want to deal with
struct soap soap;

	std::queue<struct ns__regEvent> regQ;
	std::queue<struct ns__fileEvent> fileQ;
	std::queue<struct ns__procEvent> procQ;


CaptureSoapServer::CaptureSoapServer(Visitor* v, RegistryMonitor * r, FileMonitor * f, ProcessMonitor * p){
	registryMonitor = r;
	fileMonitor = f;
	processMonitor = p;
	CaptureSoapServerThread = new Thread(this);
	CaptureSoapServerThread->start("CaptureSoapServer");
}

CaptureSoapServer::~CaptureSoapServer(){
	//FIXME: I have no idea if these are appropriate here. Also need to find the correct way to cleanly shut down
	//so that it doesn't output a connection error at the end.
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
}

void
CaptureSoapServer::run(){

	char debug = 0;
   SOCKET m, s; // master and slave sockets

	onProcessEventConnection = processMonitor->connect_onProcessEvent(boost::bind(&CaptureSoapServer::onProcessEvent, this, _1, _2, _3, _4, _5, _6));
	onRegistryEventConnection = registryMonitor->connect_onRegistryEvent(boost::bind(&CaptureSoapServer::onRegistryEvent, this, _1, _2, _3, _4, _5));
	onFileEventConnection = fileMonitor->connect_onFileEvent(boost::bind(&CaptureSoapServer::onFileEvent, this, _1, _2, _3, _4, _5));

   //The below code is taken mostly from the gsoap standalone server example page
   soap_init(&soap);
   //FIXME TODO: This needs to be configurable
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

//From RegistryMonitor.cpp
//registry event extra.at(0) == PID
//registry event extra.at(1) == name of registry value
//registry event extra.at(2) == registry value type
//registry event extra.at(3) == registry value data (if any)
void CaptureSoapServer::onRegistryEvent (wstring registryEventType, wstring time, 
										wstring processPath, wstring registryEventPath, 
										vector<wstring> extra)
{
	printf("CaptureSoapServer::onRegistryEvent got an event for time = %ls, length = %d\n", time.c_str(), time.length());

	//now begins the arduous process of converting the values into char *s
	ns__regEvent_t r;
	//TODO: trace through and verify that all mallocs are cleaned up by the soap code
	r.time = (char *)malloc(time.length()+1);
	sprintf(r.time, "%ls", time.c_str());
	printf("r.time = %s\n", r.time);

	r.eventType = (char *)malloc(registryEventType.length()+1);
	sprintf(r.eventType, "%ls", registryEventType.c_str());

	char * tmp = (char *)malloc(extra.at(0).length()+1);
	sprintf(tmp, "%ls", extra.at(0).c_str());
	r.procPID = atoi(tmp);
	printf("r.procPID = %d\n", r.procPID);
	free(tmp);

	r.procName = (char *)malloc(processPath.length()+1);
	sprintf(r.procName, "%ls", processPath.c_str());

	r.keyName = (char *)malloc(registryEventPath.length()+1);
	sprintf(r.keyName, "%ls", registryEventPath.c_str());

	r.valueName = (char *)malloc(extra.at(1).length()+1);
	sprintf(r.valueName, "%ls", extra.at(1).c_str());

	r.valueType = (char *)malloc(extra.at(2).length()+1);
	sprintf(r.valueType, "%ls", extra.at(2).c_str());

	r.valueData = (char *)malloc(extra.at(3).length()+1);
	sprintf(r.valueData, "%ls", extra.at(3).c_str());

	regQ.push(r);
	printf("added one event to regQ. Now there are %d elements in the queue\n", regQ.size());
}

//From FileMonitor.cpp
//file event extra.at(0) == PID
void CaptureSoapServer::onFileEvent(wstring fileEventType, wstring time, 
									wstring processPath, wstring fileEventPath, 
									vector<wstring> extra)
{
	printf("CaptureSoapServer::onFileEvent got an event for time = %ls\n", time.c_str());
}

void CaptureSoapServer::onProcessEvent(BOOLEAN created, wstring time, 
										DWORD parentProcessId, wstring parentProcess, 
										DWORD processId, wstring process)
{
	printf("CaptureSoapServer::onProcessEvent got an event for time = %ls\n", time.c_str());
}


int ns__ping(struct soap *soap, char * a, char ** result) 
{ 
   printf("%s\n", a);
   *result = "pong";

   return SOAP_OK; 
}

//Give it a url to browse to
int ns__visitURL(struct soap *soap, char * url, char ** result){
	wchar_t xURL[1024];
	wsprintf(xURL, L"%hs", url);
	//Build my own new-fangled Element to pass to Visitor:onServerEvent
    typedef boost::signal<void (Element*)> signal_serverEvent;
	Attribute att;
	att.name = L"url";
	att.value = xURL;
	Element e;
	e.name = L"visit";
	e.attributes.push_back(att);
	e.data = NULL;
	e.dataLength = 0;
	printf("visiting %s\n", url);
	EventController::getInstance()->notifyListeners(&e);

	return SOAP_OK;
}

int ns__sendFileBase64(struct soap *soap, char * fileName, char * data, unsigned int encodedLength, unsigned int decodedLength, int &result){
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
		printf("CreateFile failed with %d\n", GetLastError());
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

	result = 1;

	return SOAP_OK;

}

//Allows you to ask for a file and get it back in base64.
//NOTE: While I know it will be mitigated by the network architecture, features like this
//are why we should look into SOAP over SSL
int ns__receiveFileBase64(struct soap *soap, char * fileName, ns__receiveFileStruct &result){
	int debug = 0;
	
	if(debug) printf("in ns__receiveFileBase64, about to open %s\n", fileName);

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
	memset(buffer, 0, fileSize);

	DWORD numRead = 0;
	BOOL b = ReadFile(myHandle, buffer, fileSize, &numRead,NULL);
	if(!b || numRead != fileSize){
		printf("ReadFile error\n");
		return SOAP_ERR;
	}
	else{
		if(debug) printf("Read the file successfully\n");
	}

	//base64 the file
	unsigned int encodedLength = (unsigned int)b64::b64_encode(buffer, fileSize, NULL, NULL);
	if(debug) printf("encodedLength = %d\n", encodedLength);
	char * encodedData = new char[encodedLength];
	memset(encodedData, 0, encodedLength);
	size_t ret = b64::b64_encode(buffer, fileSize, encodedData, encodedLength);
	if(ret == 0){
		printf("size of the buffer was insufficient, or the length of the * converted buffer was longer than destLen\n");
		return SOAP_ERR;
	}

	//return the file
	result.data = encodedData;
	result.encodedLength = encodedLength;
	result.decodedLength = fileSize;

	if(debug) printf("cleaning up\n");
	CloseHandle(myHandle);
	delete[] buffer;
	//Don't delete[] encodedData because the SOAP stuff will need to grab the data from there for sending
	//I'm just hoping that it deletes the memory and doesn't leak it
	if(debug) printf("cleaned up successfully\n");

	return SOAP_OK;
}

//After we have sent a document into the VM, we need a way to open it.
//We want to do this in a generic way, so rather than calling specific applications with the
//document as the parameter, we instead exploit the fact that as long as default handlers are 
//set for a given file type, it can be run from the command line by simply typing its name.
//Thus we run cmd.exe with the /K option and the document name as a parameter.
//From cmd.exe help: "/K      Carries out the command specified by string but remains"

//fileName = absolute or relative file name you want to open
//waitTimeMillisec = milliseconds to Sleep() for. Should be unsigned int but SOAP::Lite doesn't
// have an 'unsigned int' type by default, and I didn't want to make one. Make sure you cast it to DWORD.
int ns__openDocument(struct soap *soap, char * fileName, int waitTimeMillisec, int &result){
	int debug = 1;
	if(debug) printf("in ns__openDocument, waitTimeMillisec = %d\n", waitTimeMillisec);

	//Create the string for the parameters
	wchar_t * docName = new wchar_t[1024];
	wsprintf(docName, L"/K %hs", fileName);

	//Create a job object to bind the processes I launch to
	//This lets us get around the problem of Windows not having good ways of determining
	//parent/child relationships, so that we don't have to care, and can for all intents
	//and purposes, terminate the entire process tree starting with the cmd.exe we're launching
	HANDLE myJobObj = CreateJobObject(NULL, NULL);
	if(myJobObj == NULL){
		printf("CreateJobObject failed with error %d\n", GetLastError());
	}

	//open with cmd.exe
	STARTUPINFO myStart;
	memset(&myStart, 0, sizeof(STARTUPINFO));
	PROCESS_INFORMATION procInfo;
	BOOL b = CreateProcess(L"C:\\WINDOWS\\system32\\cmd.exe", docName, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, 
							NULL, NULL, &myStart, &procInfo);
	if(!b){
		printf("CreateProcess failed with error %d\n", GetLastError());
		return SOAP_ERR;
	}
	
	//Add the process to the job object
	b = AssignProcessToJobObject(myJobObj, procInfo.hProcess);
	if(!b){
		printf("AssignProcessToJobObject failed with error %d\n", GetLastError());
		return SOAP_ERR;
	}

	if(debug) printf("dwProcessId = %d, dwThreadId = %d\n", procInfo.dwProcessId, procInfo.dwThreadId);
	if(debug) printf("Sleeping for %d seconds\n", (DWORD)waitTimeMillisec/1000);
	Sleep((DWORD)waitTimeMillisec);
	if(debug) printf("\n\nDone sleeping\n\n");
	
	//TODO: Before we terminate the jobs, see if it created any events. If so, let it run, and tell the
	//HC Manager about it. The Manager should then request the information about events separately.

	b = TerminateJobObject(myJobObj, 0);
	if(!b){
		printf("TerminateProcess failed with error %d\n", GetLastError());
		return SOAP_ERR;
	}
	//Just incase. I don't know if this is still needed
	CloseHandle(procInfo.hProcess);
	CloseHandle(procInfo.hThread);

	result = 1;
	return SOAP_OK;
}

//If maxEventsReturned == -1, then then send as many as possible.
//If there are no events to send back, it will send back <eventType>No Events</eventType>
int ns__receiveEventsBase64(struct soap *soap, int maxEventsReturned, struct ns__regEvent &result){
	
	if(regQ.empty()){
		printf("No events to send back\n");
		struct ns__regEvent t;
		memset(&t, 0, sizeof(struct ns__regEvent));
		//The soap will not try to serialize all the char * = 0 from the memset, but it will send back 
		//<ns:regEvent><procPID>0</procPID></ns:regEvent>
		//so parse that to mean there are no results
		t.eventType = "No Events";
		result = t;
	}
	else{
		printf("Sending back the first event\n");
		result = regQ.front();
		regQ.pop();
	}

	return SOAP_OK;
}

//Thus far, SOAP::Lite hasn't been sending the data correctly, so we never get into this function.
//Removing the "This is a multipart message in MIME format..." from MIME::Entity's Entity.pm at least gets 
//rid of the gSOAP "No XML element" error, but then it doesn't seem to ever exit the SOAP::Lite send.
int ns__sendMIME(struct soap *soap, int magicNumber, int &result){
	printf("In ns__sendMIME\n");

	//From the gSOAP documentation example
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