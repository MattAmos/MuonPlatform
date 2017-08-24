#include <iostream>
#include "../src/Runner.h"
#include <string>

using namespace std;

int main (int argc, char** argv)
{
	GPIO test = GPIO("6");
	test.setDir("in");

	string stringo = "a";

	while(true)
	{
		if(test.getVal(stringo) == 0)
		{
			cout << "Done it" << endl;
		}
	}
	return 0;
}