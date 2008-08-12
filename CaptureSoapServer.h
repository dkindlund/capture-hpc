/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#pragma once
#include "captureGSOAP.h"
#include "Thread.h"

#include "CaptureGlobal.h"
#include <string>
#include <queue>
#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include <hash_map>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include <boost/algorithm/string/finder.hpp> 
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>
#include "Url.h"
#include "ApplicationPlugin.h"
#include "Visitor.h"

using namespace std;
using namespace boost;

typedef split_iterator<string::iterator> sf_it;


class CaptureSoapServer : public Runnable
{
public:
	typedef boost::signal<void (DWORD, DWORD, wstring, wstring)> signal_visitEvent;
	typedef pair <HMODULE, std::list<ApplicationPlugin*>*> PluginPair;
	typedef pair <wstring, ApplicationPlugin*> ApplicationPair;
	typedef pair <ApplicationPlugin*, Url*> VisitPair;
public:
	CaptureSoapServer(Visitor *);
	~CaptureSoapServer();

	void run();

	//Stolen from Visitor
	void loadClientPlugins();
	ApplicationPlugin* createApplicationPluginObject(HMODULE hPlugin);
	void onServerEvent(Element* pElement);

	Thread * CaptureSoapServerThread;
	signal_visitEvent signalVisitEvent;
	stdext::hash_map<HMODULE, std::list<ApplicationPlugin*>*> applicationPlugins;
	boost::signals::connection onServerVisitEventConnection;
	stdext::hash_map<wstring, ApplicationPlugin*> applicationMap;

	static void test();
	static Visitor * myVisitor;

};
