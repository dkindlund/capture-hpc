/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/
//gsoap ns service name: capture
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded
//gsoap ns service port: http://192.168.0.131:1234
//gsoap ns service namespace: capture


typedef struct s{
	char * first; 
	char * last;
} ns__myStruct;

typedef struct rcvS{
	char * data; 
	unsigned int encodedLength;
	unsigned int decodedLength;
} ns__receiveFileStruct;

int ns__junks(char * a, ns__myStruct &result);
int ns__sendFileBase64(char * fileName, char * data, unsigned int encodedLength, unsigned int decodedLength, ns__myStruct &result);
int ns__receiveFileBase64(char * fileName, ns__receiveFileStruct &result);
int ns__sendMIME(int magicNumber, int &result);

int ns__add(int a, int b, int &result); 
int ns__ping(char * a, char ** result);
int ns__visit(char * a, char ** result);
int ns__sub(double a, double b, double &result);
