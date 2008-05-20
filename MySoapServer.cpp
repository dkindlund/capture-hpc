/*This file can be renamed later, but just plain "soapserver.cpp"
**is already created automatically by the soapcpp2 tool
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#include "MySoapServer.h"

#include "soapH.h" 
#include "calculator.nsmap" 

MySoapServer::MySoapServer(){

	MySoapServerThread = new Thread(this);
	MySoapServerThread->start("MySoapServer");

}

MySoapServer::~MySoapServer(){}

void
MySoapServer::run(){

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
//int ns__add(struct soap *soap, int a, int b, int &result) 
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

// Implementation of the "sub" remote method: 
int ns__sub(struct soap *soap, double a, double b, double &result) 
{ 
   result = a - b; 
   return SOAP_OK; 
}

