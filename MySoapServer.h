/*
**Created by Xeno Kovah of the MITRE HoneyClient Project 5/20/2008
*/

#include "captureGSOAP.h"
#include "Thread.h"

class MySoapServer : public Runnable
{
public:
	MySoapServer();
	~MySoapServer();

	void run();

	Thread * MySoapServerThread;

};
