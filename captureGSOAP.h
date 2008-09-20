/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

//NOTE: these comments are used by soapcpp2.exe. Changing them changes the auto-generated code
//gsoap ns service name: capture
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service port: http://0.0.0.0:1234
//gsoap ns service namespace: capture

//structures for events
//Names of fields used in structs are what will show up in XML

//Just using similar to previous perl names
struct ns__regkey{
	char *	created_at;
	char *	event_type;
	int		pid;
	char *	proc_name;
	char *	name;
	char *	value_name;
	char *	value_type;
	char *	value;
};

struct ns__processFile{
	char *	created_at;
	char *	event_type;
	int		pid;
	char *	proc_name;
	char *	name;
};

struct ns__osProcess{
	char *	created_at;
	char *	event_type;
	int		parent_pid;
	char *	parent_name;
	int		pid;
	char *	name;
};


struct ns__dynRegArray{
	struct ns__regkey * __ptr;
	int	__size;	//number of elements, not total size
};

struct ns__dynFileArray{
	struct ns__processFile * __ptr;
	int	__size;	//number of elements, not total size
};

struct ns__dynProcArray{
	struct ns__osProcess * __ptr;
	int	__size;	//number of elements, not total size
};

typedef struct a1{
	struct ns__dynRegArray * regkeys;
	struct ns__dynFileArray * process_files;
	struct ns__dynProcArray * os_processes;
} ns__allEvents;

//A poor substitute for MIME, but it works
typedef struct s1{
	char * data;
	unsigned int encodedLength;
	unsigned int decodedLength;
} ns__receiveFileStruct;

int ns__ping(char * a, char ** result);
int ns__visitURL(char * URL, int &result);
int ns__sendFileBase64(char * fileName, char * data, unsigned int encodedLength, unsigned int decodedLength, int &result);
int ns__receiveFileBase64(char * fileName, ns__receiveFileStruct &result);
int ns__openDocument(char * fileName, int waitTimeMillisec, int &result);
int ns__returnRegistryEvents(int maxEventsToReturn, struct ns__dynRegArray **result);
int ns__returnFileEvents(int maxEventsToReturn, struct ns__dynFileArray **result);
int ns__returnProcessEvents(int maxEventsToReturn, struct ns__dynProcArray **result);
int ns__returnEvents(int maxEventsToReturn, ns__allEvents **result);

//Not working, see definition comments
int ns__sendMIME(int magicNumber, int &result);