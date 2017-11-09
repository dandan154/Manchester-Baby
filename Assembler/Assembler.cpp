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
#include <cstring>
#include <vector>
#include <stdio.h>
#include <ctype.h>
#include <fstream>
#include <algorithm>	
#include <stdlib.h>
#include "Assembler.h"
#include "Label.h"

using namespace std;

int main(){
	cout << "Manchester SSEM Assembly Language Assembler" << endl;
	cout << "Please enter the file name of the source code:" << endl;
	string answer;
	getline(cin,answer);
	Assembler* newAs = new Assembler();
	newAs->setfileN(answer);
	cout << "Output will be saved as MachineCode.txt" << endl;
	cout << "---------------------------------------" << endl;
	newAs->loadFile();
	delete newAs;
}
//Init all assembler variables to default
Assembler::Assembler(){
	fileN = "original.txt";
	curLineNum = 0;
	totalLines = 0;
}
//Destructor for Assembler
Assembler::~Assembler(){
	fileN.clear();
	curLineNum = 0;
	totalLines = 0;
	//delete SymbolTable;
	//delete tempStorage;
	for(int i = 0; i < SymbolTable.size(); i++){
		if(SymbolTable.at(i) != NULL){
			delete SymbolTable.at(i);
			SymbolTable.at(i) = NULL;
		}
	}
	SymbolTable.clear();
	for(int i = 0; i < tempStorage.size(); i++){
		if(tempStorage.at(i) != NULL){
			delete tempStorage.at(i);
			tempStorage.at(i) = NULL;
		}
	}
	tempStorage.clear();
	SymbolTable.clear();
};

void Assembler::setfileN(string fileName){
	fileN = fileName;
}

void Assembler::setTotalLines(int lineNum){
	totalLines = lineNum;
}

//Loads the source file line by line and decodes it. It will then make a second pass to convert variables to addresses
void Assembler::loadFile(){
	string line;

	for(int i = 0; i < 2; i++){
		ifstream source(fileN.c_str());
		if (source.is_open()) {
			while (getline(source, line))
			{
				//curLineNum++;
				decodeLine(i+1,line);//i = number of passes e.g 1st or second
			}
			source.close();
			curLineNum = 0;	
		}
	}
	saveFile();
}

//Decodes the current line e.g if first pass then turn all operands and opcodes into machine code
//Second pass turn variables into addresses and modify current lines
void Assembler::decodeLine(int pass,string line){
	char first = line[0];
	bool label = false;
	int labellength = 0;
		if(first != ';'){ //If the line is a comment 
			if(pass == 1){
				line.erase(remove(line.begin(),line.end(),' '),line.end());//Remove all spaces from the line
				for(int i = 0; i < line.length(); i++){
					char letter = line[i];//Set the letter to the current char of the string at the index
					if(letter == ':'){
						string tempLabel;
						labellength = i+1;
						tempLabel = line.substr(0,i);//Get the label name
						Label* newLabel = new Label(tempLabel,curLineNum,i);//Create new Label
						if(newLabel == NULL){
							cout << "Memory Allocation ERROR." << endl;
							exit(1);
						}
						if(SymbolTable.size() <= curLineNum){
							SymbolTable.resize(curLineNum+1);
						}
						SymbolTable[curLineNum] = newLabel;//Add label to SymbolTable
						label = true;
						break;
					}
				}
				string opcode = "";
				string operand = "";
				//If there was no label then we can assume 0 is the opcode
				if(!label){
					opcode = line.substr(0,3);
					for(int i = 3; i < line.length(); i++){
						if(line[i] != ';'){
							//cout << "test 1" << endl;
							operand += line[i];
						}else{
							break;
						}
					}
				//Otherwise opcode is after label declaration
				}else if (label){
					opcode = line.substr(labellength,3);
					for(int i = labellength+3; i < line.length(); i++){
						//cout << labellength << "//" << line[i] << "||" << i << endl;
						if(line[i] != ';'){
							//cout << "test 2" << endl;
							operand += line[i];
						}else{
							break;
						}
					}
				}
				string tempstoreline;
				//If the opcode is stop then assign it an operand of 0 to deal with later on
				if(opcode == "STP"){
					opcode = getOptCode(opcode);
					operand = "0";
					operand = convertBinary(atoi(operand.c_str()),13);
					tempstoreline = operand + opcode;
					tempstoreline.append(16,'0');

				//If a variable is being declared then the store line will be a 32 bit variable
				}else if(opcode == "VAR"){
					operand = convertBinary(atoi(operand.c_str()),32);
					tempstoreline = operand;
				}else{
					opcode = getOptCode(opcode);

					//If the operand is a variable
					if(isalpha(operand[0])){
						tempstoreline = operand+"|";//Used to easily distinguish end of variable later on
						tempstoreline = tempstoreline + opcode;
						tempstoreline.append(16,'0');
					}else{
					 	operand = convertBinary(atoi(operand.c_str()),13);
						tempstoreline = operand + opcode;
						tempstoreline.append(16,'0');
					}
				}
				//Appends are being use as the last 16 bits of an instruction is unused by the SSEM
				string * permstoreline = new string(tempstoreline); //Create a new string with the store line inside
				if(tempStorage.size() <= curLineNum){
					tempStorage.resize(curLineNum+1);
				}
				tempStorage[curLineNum] = permstoreline;//Add it to our temp storage

			//If second pass
			}else{
				string address = "";
				string* tempLine;
				string var;
				for(int i = 0; i < tempStorage.size(); i++){
					tempLine = tempStorage[i];
					if(tempLine != NULL){
						string defstring = tempStorage[i]->c_str(); //Get string from pointer
						char letter = defstring[0];
						if(isalpha(letter)){ //If first letter is a variable then we will change it to an address
							var = defstring.substr(0, defstring.find("|"));//Used to find where variable ends
							for(int j = 0; j < SymbolTable.size(); j++){
									if(SymbolTable.at(j) != NULL){
										if(SymbolTable.at(j)->getSymbol() == var){
											//Get the opcode from the current version of defstring
											string tempopcode = defstring.substr(defstring.find("|")+1,3);
											//Rewrite defstring with address
											cout << "Var: " << var << " Converted to : " << SymbolTable.at(j)->getAddress() << endl;
											defstring = (SymbolTable.at(j)->getAddress()) + tempopcode;
											defstring.append(16,'0');
											//Used to bread out of loop and improve readability
											goto finish;
										}
									}
								}
							finish:
							delete(tempStorage[i]);
							string* defstringPt = new string(defstring);
							if(defstringPt == NULL){
							cout << "Memory Allocation ERROR." << endl;
							exit(1);
						}
							tempStorage[i] = defstringPt;
						}
					}
				}
			}
				cout << "Current Line: " << curLineNum << " Current Pass: " << pass << endl; //Print current location
			 	curLineNum++;

		}
		//break;
	
			
}

void Assembler::saveFile(){
	if(tempStorage.size() == 0){
		cout << "File to load did not exist!" << endl;
		exit(1);
	}
	string outputF = "MachineCode.txt";
	ofstream source(outputF.c_str());
	if (source.is_open()) {
		for(int i = 0; i < tempStorage.size(); i++){
			if(tempStorage[i] != NULL){
				source << *tempStorage[i] << "\n";
			}
		}
		source.close();
		cout << "Assembled Sucsessfull" << endl;
	}
}

string Assembler::getOptCode(string instruction){
	if(instruction == "JMP"){
		return "000";
	}else if(instruction == "JRP"){
		return "100";
	}else if(instruction == "LDN"){
		return "010";
	}else if(instruction == "STO"){
		return "110";
	}else if(instruction == "SUB"){
		return "001";
	}else if(instruction == "CMP"){
		return "011";
	}else if(instruction == "STP"){
		return "111";
	}else{
		return "";
	}
}


string Assembler::convertBinary(int dec,int bit){
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
	return bin;

}
