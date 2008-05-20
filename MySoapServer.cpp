#include "MySoapServer.h"

#include "soapH.h" 
#include "calculator.nsmap" 

MySoapServer::MySoapServer(){

	printf("\n\n\nStarting MySoapServer thread\n");
	MySoapServerThread = new Thread(this);
	MySoapServerThread->start("MySoapServer");

}

MySoapServer::~MySoapServer(){}

void
MySoapServer::run(){

   struct soap soap;
   SOCKET m, s; // master and slave sockets

   printf("\n\n\nin MySoapServer run()\n");

   soap_init(&soap);
   m = soap_bind(&soap, "192.168.0.131", 1234, 100);
   if (m < 0)
      soap_print_fault(&soap, stderr);
   else
   {
      fprintf(stderr, "Socket connection successful: master socket = %d\n", m);
      for (int i = 1; ; i++)
      {
         s = soap_accept(&soap);
         if (s < 0)
         {
            soap_print_fault(&soap, stderr);
            break;
         }
         fprintf(stderr, "%d: accepted connection from IP=%d.%d.%d.%d socket=%d\n", i,
            (soap.ip >> 24)&0xFF, (soap.ip >> 16)&0xFF, (soap.ip >> 8)&0xFF, soap.ip&0xFF, s);
         if (soap_serve(&soap) != SOAP_OK) // process RPC request
            soap_print_fault(&soap, stderr); // print error
         fprintf(stderr, "you got served!\n");
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
   printf("add got %d and %d\n", a, b);
   result = a + b; 
   return SOAP_OK; 
} 

int ns__test(struct soap *soap, char * a, char ** result) 
{ 
   printf("test got %s\n", a);
   *result = "and junk";
   return SOAP_OK; 
} 
// Implementation of the "sub" remote method: 
int ns__sub(struct soap *soap, double a, double b, double &result) 
{ 
   result = a - b; 
   return SOAP_OK; 
}

