#include <base/Logger.h>

struct fuck {
	fuck() {data = 0;};
	int &Get() {return data;};
	int data;
};

int main() {

	fuck f;
	f.Get() = 12;
	LOG_INFO << "reference " << f.data << " \n";

	LOG_TRACE << "what the fuck\n" ;
	LOG_DEBUG << "fuck!!!\n";
	LOG_INFO << "fuck!!!\n";
	LOG_ERROR << "fuck!!!\n";
	LOG_FATAL << "fuck!!!\n";


};