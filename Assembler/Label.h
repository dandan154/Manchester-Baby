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
#ifndef LABEL_H
#define LABEL_H
using namespace std;

class Label{
	private:
		string symbol;//Contains symbol of label
		string addr;//Contains address
		int curline;//Contains line the label is declared
		int size;//Declares size of label

	public:
		Label(string symb, int line, int s);
		~Label();
		//Converts int to a string of binary
		void convertBinary();
		void setSymbol(string symb);
		void setCurLine(int line);
		void setSize(int s);
		void setAddress(string address);
		string getSymbol();
		int getCurLine();
		int getSize();
		string getAddress();
};
#endif