/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#pragma once
#include "CaptureGlobal.h" //This needs to be first, due to a macro it defines
#include "Visitor.h"
#include "RegistryMonitor.h"
#include "FileMonitor.h"
#include "ProcessMonitor.h"

using namespace std;
using namespace boost;

class CaptureSoapServer : public Runnable
{
public:
	typedef boost::signal<void (DWORD, DWORD, wstring, wstring)> signal_visitEvent;
	boost::signals::connection onRegistryEventConnection;
	boost::signals::connection onFileEventConnection;
	boost::signals::connection onProcessEventConnection;

	CaptureSoapServer(Visitor *, RegistryMonitor * r, FileMonitor * f, ProcessMonitor * p);
	~CaptureSoapServer();
	void run();
	void onRegistryEvent(wstring registryEventType, wstring time, wstring processPath, 
						wstring registryEventPath, vector<wstring> extra);
	void onFileEvent(wstring fileEventType, wstring time, wstring processPath, wstring fileEventPath, 
						vector<wstring> extra);
	void onProcessEvent(BOOLEAN created, wstring time, DWORD parentProcessId, wstring parentProcess, 
						DWORD processId, wstring process);

	Thread * CaptureSoapServerThread;
	static Visitor * myVisitor;
	RegistryMonitor* registryMonitor;
	FileMonitor * fileMonitor;
	ProcessMonitor * processMonitor;
};
