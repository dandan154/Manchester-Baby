/*
	ASSIGN:	3 - Manchester Baby
	GROUP:	11
			(Marc Giannandrea:	150014251)
			(Ross Campbell:		150005746)
			(Karl Theoren:		150008734)
			(Daniel McGuinness:	150007957)
	DATE:	21/11/16
*/
#include <iostream>
#include <string>
#include <vector>
#include "Label.h"
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

using namespace std;
class Assembler{
	private:
		string fileN;
		int curLineNum;
		int totalLines;
		vector<Label*> SymbolTable;
		vector<string*> tempStorage;


	public:
		Assembler();
		~Assembler();
		void setfileN(string fileName);
		void setTotalLines(int lineNum);

		string getSymbol();
		int getCurLine();

		void loadFile();
		//Decodes line given and applying changes dependant on current pass
		void decodeLine(int pass, string line);
		void saveFile();

		string getOptCode(string instruction);
		//Convers Dec to a string of binary
		string convertBinary(int dec,int bit);
};

#endif