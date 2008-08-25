/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

//NOTE: these comments are used by soapcpp2.exe. Changing them changes the auto-generated code
//gsoap ns service name: capture
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service port: http://192.168.0.131:1234
//gsoap ns service namespace: capture

//structures for events
//Names of fields used in structs are what will show up in XML

//Just using similar to previous perl names
typedef struct fe{
	char *	time;
	char *	eventType;
	int		procPID;
	char *	procName;
	char *	fileName;
}ns__fileEvent_t;

typedef struct re{
	char *	time;
	char *	eventType;
	char *	procPID;
	int		procName;
	char *	keyName;
	char *	valueName;
	char *	valueType;
}ns__regEvent_t;

typedef struct pe{
	char *	time;
	char *	eventType;
	int		parentPID;
	char *	parentName;
	int		procPID;
	char *	procName;
}ns__procEvent_t;


typedef struct s1{
	char * data;
	unsigned int encodedLength;
	unsigned int decodedLength;
} ns__receiveFileStruct;

typedef struct s2{
	char * data;
	char * interEventDelimiter;	//potentially multi-byte delimeter used to concatenate events
	char * intraEventDelimiter;	//potentially multi-byte delimeter used to concatenate fields of events
	unsigned int numEvents;	//For sanity checking
	char moreEvents;	//1 if there is more data which can be sent to the Manager after this, 0 otherwise
						//It is the Manager's responsibility to request the additional data.
} ns__receiveEventsStruct;

int ns__ping(char * a, char ** result);
int ns__visitURL(char * a, char ** result);
int ns__sendFileBase64(char * fileName, char * data, unsigned int encodedLength, unsigned int decodedLength, int &result);
int ns__receiveFileBase64(char * fileName, ns__receiveFileStruct &result);
int ns__sendMIME(int magicNumber, int &result);
int ns__openDocument(char * fileName, int waitTimeMillisec, int &result);
int ns__receiveEventsBase64(struct soap *soap, int maxEventsReturned, ns__receiveEventsStruct &result);

