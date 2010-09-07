#include "Application.h"
#include "MengerSponge.h"

class Sponge : public Application {

public:
	Sponge(){
		width = 1366;
		height = 768;
		programName = "Sponge";
  }

	void scene(){
		MengerSponge * sponge = new MengerSponge(4);
	}
};

int main(int argc, char *argv[])
{
	Sponge().run();

}
