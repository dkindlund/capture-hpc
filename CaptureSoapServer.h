/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#pragma once
#include "CaptureGlobal.h" //This needs to be first, due to a macro it defines
#include "Visitor.h"

using namespace std;
using namespace boost;

class CaptureSoapServer : public Runnable
{
public:
	typedef boost::signal<void (DWORD, DWORD, wstring, wstring)> signal_visitEvent;

	CaptureSoapServer(Visitor *);
	~CaptureSoapServer();

	void run();
	Thread * CaptureSoapServerThread;
	static Visitor * myVisitor;

};
