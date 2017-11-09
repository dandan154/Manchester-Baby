/*
	ASSIGN:	3 - Manchester Baby
	GROUP:	11
			(Marc Giannandrea:	150014251)
			(Ross Campbell:		150005746)
			(Karl Theoren:		150008734)
			(Daniel McGuinness:	150007957)
	DATE:	21/11/16
*/

#include "Baby.h"

using namespace std;

Baby::Baby(){


}

Baby::~Baby(){

}

/*Estabilishes initial global variable values and writes to the store by reading from file*/
void Baby::initialise(){

	//declare initial variables
	isEnd = false; 
	string line;
	int lineCount = 0;
	ifstream input;
	
	//resize store to given memory
	store.resize(MEMORY_SIZE);

	//read instructions from file
	input.open("MachineCode.txt");

	//read lines until end of file
	while(input.eof() == false)
	{
		
		getline(input, line);
		store.at(lineCount) = line;
		lineCount++;
	}
	//account for extra line count
	lineCount= lineCount-1;

	//check for empty vector slots
	if (lineCount < (MEMORY_SIZE-1))
	{

		line = "";
		line.append(MEMORY_SIZE,'0');

		//fill remaining memory with empty strings
		for(int i = lineCount; i < (MEMORY_SIZE); i++)
		{
			store.at(lineCount) = line; 
			lineCount++;
		}
	}

	conInstruct = 0;

}

//increment the control instruction so that it points to next address
void Baby::increment_CI(){
	conInstruct++;
}

//set present instruction to store value
void Baby::fetch(){
	preInstruct = store[conInstruct];
	cout << "Fetch: " << preInstruct << endl;
}

//determine the opcode and operand from the present instruction
void Baby::decode(){
	getOpCode(preInstruct);
	getOperand(preInstruct);
}

//execute appropriate instruction based on opcode
void Baby::execute(){
	if(opCode == "000"){
		jmp();
	}
	else if(opCode == "100"){
		jrp();
	}
	else if(opCode == "010"){
		ldn();
	}
	else if(opCode == "110"){
		sto();
	}
	else if(opCode == "001" || opCode == "101"){
		sub();
	}
	else if(opCode == "011"){
		cmp();
	}
	else{
		stp();
	}
}

/*print the entire store to the screen*/
void Baby::displayStore(){
	cout << "Store:" << endl;
	int storeSize = store.size();
	for(int i = 0; i < storeSize; i++){
		cout << store[i] << endl;
	}
	cout << "\n" << endl;
}

/*print the accumulator to the screen*/
void Baby::displayAcc(){
	cout << "Accumulator:" << endl;
	cout << "decimal: " << accum << endl;
	string acc = decToBin(accum);
	cout << "binary: " << acc << endl;
	cout << "\n" << endl;
}


void Baby::displayCon(){
	cout << "Control Instruction:" << endl;
	cout << "decimal: " << conInstruct << endl;
	string con = decToBin(conInstruct);
	cout << "binary: " << con << endl;
}

//convert binary string to decimal integer
int Baby::binToDec(string bin){

	//declare variables
	int dec = 0;
	int temp = 0;
	int base = 1;
	int binSize = bin.length();
	
	//convert binary to decimal
	for(int i = 0; i < binSize-1; i++){
		temp = bin[i] - '0';
		dec = dec + temp*base;
		base = base*2;
	}

	//determine if negative
	if(bin[binSize-1] == '1'){dec = -dec;}
	return dec;
}

//convert decimal integer to binary string
string Baby::decToBin(int dec){

	string bin = "";	//binary value
	int rem;			//remainder
	int binSize; 		//size of bin value 
	int decTemp = dec;	//temp decimal value


	//make decimal positive for calculation
	if(dec < 0){
		decTemp = -decTemp;
	}

	//loop until full binary number has been created
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
	//fill in blank operand space
	if(binSize < MEMORY_SIZE-1){
		bin.append((MEMORY_SIZE-1) - (binSize),'0');
	}
	//take into account twos compliment
	if(dec < 0){
		bin = bin + '1';
	}
	else{
		bin = bin + '0';
	}
	return bin;
}

//CI = S
//unconditional jump to specified address
void Baby::jmp(){
	conInstruct = operand; 
}
//CI = CI + S
//jump to address by adding operand to current instruction
void Baby::jrp(){
	conInstruct = conInstruct + operand; 
}

//A = -S
//load data into accumulator
void Baby::ldn(){
	accum = binToDec(store[operand]); 
	accum = -accum; 
}

//S = A
//write accumulator to store
void Baby::sto(){
	store.at(operand) = decToBin(accum);
}

//A = A - S
//subtract store data value from accumulator
void Baby::sub(){
	int temp = binToDec(store[operand]);
	accum = accum - temp; 
}

//if A < 0 then CL = CL + 1
//skip instruction if accumulator is negative
void Baby::cmp(){
	if(accum < 0)
	{
		increment_CI(); 
	}
}

//terminates program loop
void Baby::stp(){
	isEnd = true;  
}

//takes in binary string and sets opcode 
void Baby::getOpCode(string line){
	opCode = line.substr(OPCODE_START, 3);
}

//takes in binary string and sets operand
void Baby::getOperand(string line){
	string op = line.substr(0, OPCODE_START);
	operand = binToDec(op);
}

//returns isEnd value
bool Baby::getIsEnd(){
	return isEnd; 
}


