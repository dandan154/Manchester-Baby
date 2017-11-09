/*
	ASSIGN:	3 - Manchester Baby
	GROUP:	11
			(Marc Giannandrea:	150014251)
			(Ross Campbell:		150005746)
			(Karl Theoren:		150008734)
			(Daniel McGuinness:	150007957)
	DATE:	21/11/16
*/


#ifndef	HEADER_BABY
#define HEADER_BABY

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>

#include "Constants.h"

using namespace std;

class Baby{

	private:
		int accum;					//stores number to be used in calculation
		int conInstruct;			//control instruction
		string preInstruct;			//present instruction
		string opCode;				//opcode
		int operand; 				//operand 
		bool isEnd;					//determine if program is at end
		vector <string> store;		//main memory


	public:

		//Constructor and Destructor
		Baby();
		~Baby();

		//write program to memory
		void initialise();

		//essential operation functions 
		void increment_CI();
		void fetch();
		void decode();
		void execute();

		//display stored info
		void displayStore();
		void displayAcc();
		void displayCon();
		
		//binary/decimal conversion
		int binToDec(string bin);
		string decToBin(int dec);

		//instruction set
		void jmp();
		void jrp();
		void ldn();
		void sto();
		void sub();
		void cmp();
		void stp();

		//line processing
		void getOpCode(string line);
		void getOperand(string line);

		//variable setting
		bool getIsEnd();
};

#endif
