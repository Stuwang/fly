#include <base/Logger.h>

struct fuck {
	fuck() {data = 0;};
	int &Get() {return data;};
	int data;
};

int main() {

	fuck f;
	f.Get() = 12;
	const char *a = "aaaaa";
	const char *b = "bbbbb";
	int ret = memcmp(a, b, 5);
	LOG_INFO << -1 << '\n' ;
};