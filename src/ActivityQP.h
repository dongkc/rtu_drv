#include <Poco/Activity.h>

#include "bsp.h"


class ActivityQP
{
public:
	ActivityQP():
		_activity(this, &ActivityQP::runActivity)
	{
	}
	
	void start()
	{
		_activity.start();
	}
	
	void stop()
	{
        Zebra::BSP_terminate(-1);

		_activity.stop();
		_activity.wait();
	}

protected:
	void runActivity()
	{
		while (!_activity.isStopped())
		{
            Zebra::qpRun();
		}
	}
	
private:
    Poco::Activity<ActivityQP> _activity;
};


#if 0
int main(int argc, char** argv)
{
	ActivityQP example;
	example.start();

    while(1){
        std::cout << "OK" << std::endl;
        Thread::sleep(1000);
    };

	return 0;
}
#endif
