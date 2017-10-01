#include "../src/Ranger.cpp"

int main()
{
	Ranger test = Ranger();
	for(int i=0; i< 300; i++)
	{
		printf("%d\n", test.getDistanceMM());
	}
}
