#include "RegistryMonitor.h"

RegistryMonitor::RegistryMonitor(void)
{
	wchar_t exListDriverPath[1024];
	wchar_t kernelDriverPath[1024];

	driverInstalled = false;
	monitorRunning = false;

	hMonitorStoppedEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	GetFullPathName(L"RegistryMonitor.exl", 1024, exListDriverPath, NULL);
	Monitor::loadExclusionList(exListDriverPath);

	onRegistryExclusionReceivedConnection = EventController::getInstance()->connect_onServerEvent(L"registry-exclusion", boost::bind(&RegistryMonitor::onRegistryExclusionReceived, this, _1));

	// Load registry monitor kernel driver
	GetFullPathName(L"CaptureRegistryMonitor.sys", 1024, kernelDriverPath, NULL);
	if(Monitor::installKernelDriver(kernelDriverPath, L"CaptureRegistryMonitor", L"Capture Registry Monitor"))
	{	
		hDriver = CreateFile(
					L"\\\\.\\CaptureRegistryMonitor",
					GENERIC_READ | GENERIC_WRITE, 
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					0,                     // Default security
					OPEN_EXISTING,
					FILE_FLAG_OVERLAPPED,  // Perform asynchronous I/O
					0);                    // No template
		if(INVALID_HANDLE_VALUE == hDriver) {
			printf("RegistryMonitor: ERROR - CreateFile Failed: %i\n", GetLastError());
		} else {
			driverInstalled = true;
		}
	}
	initialiseObjectNameMap();
}

RegistryMonitor::~RegistryMonitor(void)
{
	stop();
	if(isDriverInstalled())
	{
		driverInstalled = false;
		CloseHandle(hDriver);
	}
	CloseHandle(hMonitorStoppedEvent);
}

void
RegistryMonitor::initialiseObjectNameMap()
{
	HKEY hTestKey;
	wchar_t szTemp[256];

	DWORD dwError = RegOpenCurrentUser(KEY_READ , &hTestKey);
	if (dwError == ERROR_SUCCESS )
	{
		NTSTATUS status;
		DWORD RequiredLength;
		PPUBLIC_OBJECT_TYPE_INFORMATION t;

		typedef DWORD (WINAPI *pNtQueryObject)(HANDLE,DWORD,VOID*,DWORD,VOID*);
		pNtQueryObject NtQueryObject = (pNtQueryObject)GetProcAddress(GetModuleHandle(L"ntdll.dll"), (LPCSTR)"NtQueryObject");
		
		status = NtQueryObject(hTestKey, 1, NULL,0,&RequiredLength);

		if(status == STATUS_INFO_LENGTH_MISMATCH)
		{
			t = (PPUBLIC_OBJECT_TYPE_INFORMATION)VirtualAlloc(NULL, RequiredLength, MEM_COMMIT, PAGE_READWRITE);
			if(status != NtQueryObject(hTestKey, 1,t,RequiredLength,&RequiredLength))
			{
				ZeroMemory(szTemp, 256);
				CopyMemory(&szTemp, t->TypeName.Buffer, RequiredLength);

				// Dont change the order of these ... _Classes should be inserted first
				// Small bug but who cares
				wstring temp2 = szTemp;
				temp2 += L"_CLASSES";
				wstring name2 = L"HKCR";
				objectNameMap.push_back(ObjectPair(temp2, name2));
				wstring temp1 = szTemp;
				wstring name1 = L"HKCU";
				objectNameMap.push_back(ObjectPair(temp1, name1));
				wstring temp3 = L"\\REGISTRY\\MACHINE";
				wstring name3 = L"HKLM";
				objectNameMap.push_back(ObjectPair(temp3, name3));
				wstring temp4 = L"\\REGISTRY\\USER";
				wstring name4 = L"HKU";
				objectNameMap.push_back(ObjectPair(temp4, name4));
				wstring temp5 = L"\\Registry\\Machine";
				wstring name5 = L"HKLM";
				objectNameMap.push_back(ObjectPair(temp5, name5));

			}
			VirtualFree(t, 0, MEM_RELEASE);
		}
	}
}

boost::signals::connection 
RegistryMonitor::connect_onRegistryEvent(const signal_registryEvent::slot_type& s)
{ 
	return signal_onRegistryEvent.connect(s); 
}

void
RegistryMonitor::onRegistryExclusionReceived(Element* pElement)
{
	wstring excluded = L"";
	wstring registryEventType = L"";
	wstring processPath = L"";
	wstring registryPath = L"";

	vector<Attribute>::iterator it;
	for(it = pElement->attributes.begin(); it != pElement->attributes.end(); it++)
	{
		if(it->name == L"action") {
			registryEventType = it->value;
		} else if(it->name == L"object") {
			registryPath = it->value;
		} else if(it->name == L"subject") {
			processPath = it->value;
		} else if(it->name == L"excluded") {
			excluded = it->value;
		}
	}
	Monitor::addExclusion(excluded, registryEventType, processPath, registryPath);
}

void
RegistryMonitor::start()
{
	if(!isMonitorRunning() && isDriverInstalled())
	{
		registryEventsBuffer = (BYTE*)malloc(REGISTRY_EVENTS_BUFFER_SIZE);
		registryMonitorThread = new Thread(this);
		registryMonitorThread->start("RegistryMonitor");
	}
}

void
RegistryMonitor::stop()
{
	if(isMonitorRunning() && isDriverInstalled())
	{	
		monitorRunning = false;
		WaitForSingleObject(hMonitorStoppedEvent, 1000);
		registryMonitorThread->stop();
		delete registryMonitorThread;
		free(registryEventsBuffer);
	}
}

wstring
RegistryMonitor::getRegistryEventName(int registryEventType)
{
	wstring eventName = L"<UNKNOWN>";
	switch(registryEventType)
	{
		case RegNtPostCreateKey:
			eventName = L"CreateKey";
			break;
		case RegNtPostOpenKey:
			eventName = L"OpenKey";
			break;
		case RegNtPreDeleteKey:
			eventName = L"DeleteKey";
			break;
		case RegNtDeleteValueKey:
			eventName = L"DeleteValueKey";
			break;
		case RegNtPreSetValueKey:
			eventName = L"SetValueKey";
			break;
		case RegNtEnumerateKey:
			eventName = L"EnumerateKey";
			break;
		case RegNtEnumerateValueKey:
			eventName = L"EnumerateValueKey";
			break;
		case RegNtQueryKey:
			eventName = L"QueryKey";
			break;
		case RegNtQueryValueKey:
			eventName = L"QueryValueKey";
			break;
		case RegNtKeyHandleClose:
			eventName = L"CloseKey";
			break;
		default:
			break;
	}
	return eventName;
}

wstring
RegistryMonitor::convertRegistryObjectNameToHiveName(wstring registryObjectName)
{	
	/* Convert /Registry/Machine etc to the actual hive name like HKLM */
	std::list<ObjectPair>::iterator it;
	for(it = objectNameMap.begin(); it != objectNameMap.end(); it++)
	{
		size_t position = registryObjectName.rfind(it->first,0);
		if(position != wstring::npos)
		{
			return registryObjectName.replace(position, it->first.length(), it->second, 0, it->second.length());
		}
	}
	return registryObjectName;
}

void
RegistryMonitor::run()
{
	DWORD dwReturn; 
	monitorRunning = true;
	int waitTime = REGISTRY_DEFAULT_WAIT_TIME;
	while(isMonitorRunning())
	{
		ZeroMemory(registryEventsBuffer, REGISTRY_EVENTS_BUFFER_SIZE);
		DeviceIoControl(hDriver,
			IOCTL_GET_REGISTRY_EVENTS, 
			0, 
			0, 
			registryEventsBuffer, 
			REGISTRY_EVENTS_BUFFER_SIZE, 
			&dwReturn, 
			NULL);
		/* Go through all the registry events received. Events are variable sized
		   so the starts of them are calculated by adding the lengths of the various
		   data stored in it */
		if(dwReturn >= sizeof(REGISTRY_EVENT))
		{
			UINT offset = 0;
			do {
				/* e->registryData contains the registry path first and then optionally
				   some data */
				PREGISTRY_EVENT e = (PREGISTRY_EVENT)(registryEventsBuffer + offset);
				BYTE* registryData = NULL;
				wchar_t* szRegistryPath = NULL;
				
				wstring registryEventName = getRegistryEventName(e->eventType);
				/* Get the registry string */
				szRegistryPath = (wchar_t*)malloc(e->registryPathLengthB);
				CopyMemory(szRegistryPath, e->registryData, e->registryPathLengthB);
				wstring registryPath = convertRegistryObjectNameToHiveName(szRegistryPath);
				wstring processPath = ProcessManager::getInstance()->getProcessPath((DWORD)e->processId);
				
				/* If there is data stored retrieve it */
				if(e->dataLengthB > 0)
				{
					registryData = (BYTE*)malloc(e->dataLengthB);
					CopyMemory(registryData, e->registryData+e->registryPathLengthB, e->dataLengthB);				
				}
				
				/* Is the event excluded */
				if(!Monitor::isEventAllowed(registryEventName,processPath,registryPath))
				{
					wchar_t szTempTime[256];
					convertTimefieldsToString(e->time, szTempTime, 256);
					wstring time = szTempTime;
	
					//Handle all the post-processing to format the data
					wchar_t szTemp[256];
					wstring other;
					size_t tmp_len;
					vector<wstring> extraData;
					extraData.push_back(e->valueName);


					//MS description of data types:
					//http://support.microsoft.com/kb/256986
					switch(e->dataType){
						case REG_NONE:
							extraData.push_back(L"REG_NONE");
							extraData.push_back(L""); //This is so that the logger adds an extra blank value
							break;
						case REG_SZ:
							extraData.push_back(L"REG_SZ");
							extraData.push_back((wchar_t *)registryData);
							break;
						case REG_EXPAND_SZ:
							extraData.push_back(L"REG_EXPAND_SZ");
							extraData.push_back((wchar_t *)registryData);
							break;
						case REG_BINARY:
							extraData.push_back(L"REG_BINARY");
							//FIXME: Better way?
							if(e->dataLengthB < 256){
								for(unsigned char n = 0; n < e->dataLengthB; n++){
									swprintf_s(szTemp, 256, L"%x", registryData[n]);
									other.append(szTemp);
								}
								extraData.push_back(other);
							}
							else{
								for(unsigned short n = 0; n <= 255; n++){
									swprintf_s(szTemp, 256, L"%x", registryData[n]);
									other.append(szTemp);
								}
								extraData.push_back(other);
							}
							break;
						case REG_DWORD:
							extraData.push_back(L"REG_DWORD");
							swprintf_s(szTemp, 256, L"%lx", ((DWORD *)registryData)[0]);
							extraData.push_back(szTemp);
							break;
						//TODO: Untested
						case REG_DWORD_BIG_ENDIAN:
							extraData.push_back(L"REG_DWORD_BIG_ENDIAN");
							swprintf_s(szTemp, 256, L"%x%x%x%x", registryData[0],registryData[1],registryData[2],registryData[3]);
							extraData.push_back(szTemp);
							break;
						//From MS: "A Unicode string naming a symbolic link."
						//TODO: Untested
						case REG_LINK:
							extraData.push_back(L"REG_LINK");
							extraData.push_back((wchar_t *)registryData);
							break;
						//TODO: regedit won't let me make a string,empty string, string, but that 
						// doesn't mean something else might not be able to do it. Look into it as it would
						// break the while condition into ending early.
						case REG_MULTI_SZ:
							extraData.push_back(L"REG_MULTI_SZ");
							while(((wchar_t *)registryData)[0] != '\0' && ((wchar_t *)registryData)[1] != '\0'){
								other.append((wchar_t *)registryData);
								other.append(L"-|-");
								tmp_len = wcsnlen((wchar_t *)registryData, 512);
								registryData = (BYTE *)((wchar_t *)registryData + (tmp_len + 1));
							}
							extraData.push_back(other);
							break;
						//TODO: Untested, "A series of nested arrays..."
						case REG_RESOURCE_LIST:
							extraData.push_back(L"REG_RESOURCE_LIST");
							extraData.push_back(L"FILL IN");
							break;
						//TODO: Untested, "A series of nested arrays..."
						case REG_FULL_RESOURCE_DESCRIPTOR:
							extraData.push_back(L"REG_FULL_RESOURCE_DESCRIPTOR");
							extraData.push_back(L"FILL IN");
							break;
						//TODO: Untested, "A series of nested arrays..."
						case REG_RESOURCE_REQUIREMENTS_LIST:
							extraData.push_back(L"REG_RESOURCE_REQUIREMENTS_LIST");
							extraData.push_back(L"FILL IN");
							break;
						case REG_QWORD_LITTLE_ENDIAN:
							extraData.push_back(L"REG_QWORD");
							swprintf_s(szTemp, 256, L"%lx%lx", ((DWORD *)registryData)[0],((DWORD *)registryData)[1]);
							extraData.push_back(szTemp);
							break;
						default:
							extraData.push_back(L"UNKNOWN TYPE!");
							swprintf_s(szTemp, 256, L"%ld", e->dataType);
							extraData.push_back(szTemp);
							break;
					}
					signal_onRegistryEvent(registryEventName, time, processPath, registryPath, extraData);
				}
				if(registryData != NULL)
					free(registryData);
				if(szRegistryPath != NULL)
					free(szRegistryPath);
				offset += sizeof(REGISTRY_EVENT) + e->registryPathLengthB + e->dataLengthB;
			}while(offset < dwReturn);
			
			
		}

		if(dwReturn == (REGISTRY_EVENTS_BUFFER_SIZE))
		{
			waitTime = REGISTRY_BUFFER_FULL_WAIT_TIME;
		} else {
			waitTime = REGISTRY_DEFAULT_WAIT_TIME;
		}

		Sleep(waitTime);
	}
	SetEvent(hMonitorStoppedEvent);
}
