//#include "calc.h"
#include "Thread.h"

class MySoapServer : public Runnable
{
public:
	MySoapServer();
	~MySoapServer();

	void run();

	Thread * MySoapServerThread;

};
