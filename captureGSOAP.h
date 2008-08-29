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
struct ns__regEvent{
	char *	time;
	char *	eventType;
	int		procPID;
	char *	procName;
	char *	keyName;
	char *	valueName;
	char *	valueType;
	char *	valueData;
};

struct ns__fileEvent{
	char *	time;
	char *	eventType;
	int		procPID;
	char *	procName;
	char *	fileName;
};

struct ns__procEvent{
	char *	time;
	char *	eventType;
	int		parentPID;
	char *	parentName;
	int		procPID;
	char *	procName;
};


struct ns__dynRegArray{
	struct ns__regEvent * __ptr;
	int	__size;	//number of elements, not total size
};

struct ns__dynFileArray{
	struct ns__fileEvent * __ptr;
	int	__size;	//number of elements, not total size
};

struct ns__dynProcArray{
	struct ns__procEvent * __ptr;
	int	__size;	//number of elements, not total size
};

struct ns__allEvents{
	struct ns__dynRegArray * regEvents;
	struct ns__dynFileArray * fileEvents;
	struct ns__dynProcArray * procEvents;
};

//A poor substitute for MIME, but it works
typedef struct s1{
	char * data;
	unsigned int encodedLength;
	unsigned int decodedLength;
} ns__receiveFileStruct;

int ns__ping(char * a, char ** result);
int ns__visitURL(char * URL, struct ns__allEvents &result);
int ns__sendFileBase64(char * fileName, char * data, unsigned int encodedLength, unsigned int decodedLength, int &result);
int ns__receiveFileBase64(char * fileName, ns__receiveFileStruct &result);
int ns__openDocument(char * fileName, int waitTimeMillisec, int &result);
int ns__returnEvents(int maxEventsToReturn, struct ns__allEvents &result);

//Not working, see definition comments
int ns__sendMIME(int magicNumber, int &result);