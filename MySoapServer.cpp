/*This file can be renamed later, but just plain "soapserver.cpp"
**is already created automatically by the soapcpp2 tool
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#include "MySoapServer.h"

#include "soapH.h" 
#include "calculator.nsmap" 

#include "Visitor.h"

Visitor * globVisitor;

MySoapServer::MySoapServer(Visitor* v){

	globVisitor = v;
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

   ///loadClientPlugins();

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

void MySoapServer::test(){

//	printf("myVisitor = %#x\n", MySoapServer::myVisitor);

}

int ns__ping(struct soap *soap, char * a, char ** result) 
{ 
   printf("%s\n", a);
   *result = "pong";

	//Build my own new-fangled Element to pass to Visitor:onServerEvent which I think will open 
    typedef boost::signal<void (Element*)> signal_serverEvent;
	Attribute att;
	att.name = L"url";
	att.value = L"http://slashdot.org";
	Element e;
	e.name = L"iexplore";
	e.attributes.push_back(att);
	e.data = NULL;
	e.dataLength = 0;
//	printf("trying with notifyListeners\n");
//	EventController::getInstance()->notifyListeners(&e);
	printf("trying with globVisitor\n");
	globVisitor->onServerEvent(&e);

   return SOAP_OK; 
}

// Implementation of the "sub" remote method: 
int ns__sub(struct soap *soap, double a, double b, double &result) 
{ 
   result = a - b; 
   return SOAP_OK; 
}

void
MySoapServer::loadClientPlugins()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	wchar_t pluginDirectoryPath[1024];

	GetFullPathName(L"plugins\\Application_*.dll", 1024, pluginDirectoryPath, NULL);
	DebugPrint(L"Capture-SOAP Server: Plugin directory - %ls\n", pluginDirectoryPath);
	hFind = FindFirstFile(pluginDirectoryPath, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		typedef void (*AppPlugin)(void*);
		do
		{
			wstring pluginDir = L"plugins\\";
			pluginDir += FindFileData.cFileName;			
			HMODULE hPlugin = LoadLibrary(pluginDir.c_str());

			if(hPlugin != NULL)
			{
				list<ApplicationPlugin*>* apps = new std::list<ApplicationPlugin*>();
				applicationPlugins.insert(PluginPair(hPlugin, apps));
				ApplicationPlugin* applicationPlugin = createApplicationPluginObject(hPlugin);
				if(applicationPlugin == NULL) {
					FreeLibrary(hPlugin);
				} else {
					printf("Loaded plugin: %ls\n", FindFileData.cFileName);
					unsigned int g = applicationPlugin->getPriority();
					wchar_t** supportedApplications = applicationPlugin->getSupportedApplicationNames();
					for(int i = 0; supportedApplications[i] != NULL; i++)
					{
						stdext::hash_map<wstring, ApplicationPlugin*>::iterator it;
						it = applicationMap.find(supportedApplications[i]);
						/* Check he application isn't already being handled by a plugin */
						if(it != applicationMap.end())
						{
							/* Check the priority of the existing application plugin */
							unsigned int p = it->second->getPriority();
							if(applicationPlugin->getPriority() > p)
							{
								/* Over ride the exisiting plugin if the priority of the loaded one
								   is greater */
								applicationMap.erase(supportedApplications[i]);
								printf("\toverride: added application: %ls\n", supportedApplications[i]);
								applicationMap.insert(ApplicationPair(supportedApplications[i], applicationPlugin));
							} else {
								printf("\tplugin overridden: not adding application: %ls\n", supportedApplications[i]);
							}
						} else {
							printf("\tinserted: added application: %ls\n", supportedApplications[i]);
							applicationMap.insert(ApplicationPair(supportedApplications[i], applicationPlugin)); 
						}
					}
				}
			}
		} while(FindNextFile(hFind, &FindFileData) != 0);
		FindClose(hFind);
	}
	
}

ApplicationPlugin*
MySoapServer::createApplicationPluginObject(HMODULE hPlugin)
{
	typedef void (*PluginExportInterface)(void*);
	PluginExportInterface pluginCreateInstance = NULL;
	ApplicationPlugin* applicationPlugin = NULL;
	/* Get the function address to create a plugin object */
	pluginCreateInstance = (PluginExportInterface)GetProcAddress(hPlugin,"New");
	/* Create a new plugin object in the context of the plugin */
	pluginCreateInstance(&applicationPlugin);
	/* If the object was created then add it to a list so we can track it */
	if(applicationPlugin != NULL)
	{
		stdext::hash_map<HMODULE, std::list<ApplicationPlugin*>*>::iterator it;
		it = applicationPlugins.find(hPlugin);
		if(it != applicationPlugins.end())
		{
			list<ApplicationPlugin*>* apps = it->second;
			apps->push_back(applicationPlugin);
		}
	}
	return applicationPlugin;
}

void
MySoapServer::onServerEvent(Element* pElement)
{
	wstring applicationName = L"iexplore";
	wstring url = L"";
	int time = 30;
	vector<Attribute>::iterator it;
	for(it = pElement->attributes.begin(); it != pElement->attributes.end(); it++)
	{
		if(it->name == L"url") {
			url = it->value;
		} else if(it->name == L"program") {
			applicationName = it->value;
		} else if(it->name == L"time") {
			time = boost::lexical_cast<int>(it->value);
		}
	}
	if(url != L"")
	{
		url = CaptureGlobal::urlDecode(url);
		stdext::hash_map<wstring, ApplicationPlugin*>::iterator vit;
		vit = applicationMap.find(applicationName);
		if(vit != applicationMap.end())
		{
			ApplicationPlugin* applicationPlugin = vit->second;
			Url* visiturl = new Url(url, applicationName, time);
			DWORD minorErrorCode = 0;
			DWORD majorErrorCode = 0;
			printf("Visiting: %ls -> %ls\n", visiturl->getApplicationName().c_str(), visiturl->getUrl().c_str());
		
			/* Pass the actual visitation process of to the application plugin */
			majorErrorCode = applicationPlugin->visitUrl(visiturl, &minorErrorCode);
			///toVisit.push(VisitPair(applicationPlugin, visiturl));
			///SetEvent(hQueueNotEmpty);
		} else {
			printf("MySoapServer-onServerEvent: ERROR could not find client %ls path, url not queued for visitation\n", applicationName.c_str());
		}
	} else {
		printf("MySoapServer-onServerEvent: ERROR no url specified for visit event\n");
	}
}