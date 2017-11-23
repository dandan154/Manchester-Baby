#include "Baby.h"
#include <ctime>

using namespace std;

/*pauses the program for a given number of seconds*/
void pause(double pause){

	time_t start = time(NULL);
	time_t end = time(NULL);

	while(difftime(end, start) < pause)
	{
		end = time(NULL);
	}

}
int main(){
	
	int count = 1;
	Baby baby = Baby(); 
	baby.initialise();

	do{
		//system("clear");
		cout << "============================================" << endl;
		cout << "TURN " << count << endl;
		cout << "============================================" << endl;
		baby.increment_CI();
		baby.fetch();
		baby.decode();
		baby.execute();
		baby.displayStore(); 
		baby.displayAcc();
		baby.displayCon();
		cout << "============================================" << endl;
		count++;
		pause(1);
	}while(baby.getIsEnd() == false);

}
