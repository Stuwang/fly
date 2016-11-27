#include <iostream>
#include <cstdio>

using namespace std;

int main(){
	FILE *p = fopen("aaaa","a");
	int len = fwrite("aaaaa",1,5,p);
	cout << len;
}
