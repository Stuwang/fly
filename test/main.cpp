// #include <base/Logger.h>
// #include <base/Endian.h>

// #include <arpa/inet.h>

// #include <iostream>

using namespace std;

struct fuck {
	fuck() {data = 0;};
	int &Get() {return data;};
	int data;
};

void test() {
	int i = 1;
	if (*(char*)&i > *( ((char*)&i) + 2) ) {
		std::cout << "xiaoduan";
	} else {
		std::cout << "daduan";
	}
};

int main() {
	int i = fly::NetToHost(fly::HostToNet(1));
	printf("%08X\n", 1);
	printf("%08X\n", i);
	int l = ntohl(1);
	assert(i == l);
	test();
};

#define  err(msg)\
perror("msg");

err(aaaa);