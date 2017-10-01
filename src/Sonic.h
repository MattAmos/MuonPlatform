#ifndef SONIC_H
#define SONIC_H

class Sonic
{
	private:
	int triggerO;
	int echoI;

	public:

	Sonic(int t, int e);
	~Sonic();
	void setup();
	int getCM();
};

#endif
