
#include <stdio.h>

namespace MYENGINESTUDY
{
	__declspec(dllimport) void Print();
}

void main()
{
	MYENGINESTUDY::Print();

	// for pause
	char c[1] = {};
	scanf("enter char %s", &c);
}