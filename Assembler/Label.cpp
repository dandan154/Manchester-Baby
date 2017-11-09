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
#include <algorithm>
#include "Label.h"

using namespace std;

Label::Label(string symb,int line, int s){
	symbol = symb;
	curline = line;
	size = s;
	convertBinary();
}

Label::~Label(){
	symbol = "";
	curline = 0;
	size = 0;
}

void Label::convertBinary(){
	int dec = curline;
	int bit = 13;
	string bin = "";	//binary value
	int rem;			//remainder
	int binSize; 		//size of bin value 
	int decTemp = dec;		//temp decimal value


	//make decimal positive for calculation
	if(dec < 0){
		decTemp = -decTemp;
	}

	while(decTemp != 0)
	{
		rem = decTemp%2;
		decTemp = decTemp/2;
		if(rem == 0){
			bin = bin + '0';
		}
		else{
			bin = bin + '1';
		}
	}

	binSize = bin.length();
	if(binSize < bit-1){
		bin.append((bit-1) - (binSize),'0');
	}

	if(dec < 0){
		bin = bin + '1';
	}
	else{
		bin = bin + '0';
	}
	addr = bin;
}

void Label::setSymbol(string symb){
	symbol = symb;
}

void Label::setCurLine(int line){
	curline = line;
}

void Label::setSize(int s){
	size = s;
}
void Label::setAddress(string address){
	addr = address;
}

string Label::getSymbol(){
	return symbol;
}

int Label::getCurLine(){
	return curline;
}

int Label::getSize(){
	return size;
}

string Label::getAddress(){
	return addr;
}