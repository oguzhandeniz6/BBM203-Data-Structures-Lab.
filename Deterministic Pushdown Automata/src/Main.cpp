#include <iostream>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <fstream>
#include <stack>

using namespace std;

vector<string> stringSplitter(string str, char split)
{
	vector<string> tokens;
	string word;
	stringstream X(str);
	while(getline(X,word, split)){ // split with which character?
		tokens.push_back(word);
	}

	return tokens;
}

vector<string> stringSplitter(string str, string split)
{
	vector<string> tokens;
	string word;
	size_t pos = 0;

	while ((pos = str.find(split)) != string::npos) {
	    word = str.substr(0, pos);
	    tokens.push_back(word);
	    str.erase(0, pos + split.length());
	}

	tokens.push_back(str);

	return tokens;
}

vector<string> fileReader(string fileName)
{
	vector<string> arr;
	string line;
	ifstream myfile (fileName);

	if (myfile.is_open())
	{
		while(getline(myfile, line))
		{
			arr.push_back(line);
		}
		myfile.close();
	}
	else
	{
		cout << "Can't open file: " << fileName << endl;
	}

	return arr;
}

bool checkAlphabet(vector<string> alphabet, string compare)
{
	bool returnValue = false;
	for(int a = 0; a < alphabet.size(); a++)
	{
		if(alphabet.at(a) == compare)
		{
			returnValue = true;
			break;
		}
	}

	return returnValue;
}

void printStack(stack<string> stacki, ofstream& output)
{
	stack<string> reverse;
	while(stacki.empty() != true)
	{
		string p;
		p = stacki.top();
		stacki.pop();
		reverse.push(p);
	}
	string out;
	while(reverse.empty() != true)
	{
		out += reverse.top();
		out += ",";
		reverse.pop();
	}
	out.pop_back();
	output << out << "\n";
}

bool gameover(stack<string> stacki, vector<string> startfinal, bool isAccepted, string currentState, string lastElm)
{
	// CHECK IF ENDS
	if( (currentState == startfinal.at(1)) || (currentState == startfinal.at(2))) // CHECK IF IT IS REACHED TO A FINAL STATE
	{
		if(stacki.empty() == false) // IF STACK IS NOT EMPTY (PRACTICALLY)
		{
			if(stacki.top() == lastElm) // CHECK IF LAST ELEMENT IN THE STACK IS THE ACTUAL LASTELM
			{
				isAccepted = true; // IF NOT, REJECT THE INPUT
			}
		}

		else // IF STACK IS EMPTY (PRACTICALLY)
		{
			if(lastElm == "") // IF LASTELM IS NOT BLANK
			{
				isAccepted = true; // REJECT THE INPUT
			}
		}
	}

	return isAccepted;
}

bool dpdaStart(ofstream& output, vector<vector<string>> transitions, vector<string> temp, vector<string> inputLine, vector<string> startfinal, stack<string> stackl, string currentState, string e, string lastElm, bool isAccepted, int x, int maxIteration)
{
	maxIteration++;
	for(int t = 0; t < transitions.size(); t++)
	{
		temp = transitions.at(t); // 0 = currentState | 1 = readInput | 2 = popStack | 3 = nextState | 4 = pushStack

		if(temp.at(0) == currentState)// CHECK THE STATE IS CURRENT STATE OR NOT
		{
			if(temp.at(1) != e) // CHECK THE READ VALUE IS "e" OR NOT
			{
				if(inputLine.at(x) == temp.at(1)) // IF NOT, CHECK INPUT
				{
					if(temp.at(2) != e) // CHECK THE POP VALUE IS "e" OR NOT
					{
						if(stackl.top() == temp.at(2))
						{
							stackl.pop();
						}
						else
						{
							continue;
						}
					}
					currentState = temp.at(3); // UPDATE CURRENT STATE
					if(temp.at(4) != e) // CHECK THE PUSH VALUE IS "e" OR NOT
					{
						stackl.push(temp.at(4));// STACK.PUSH(TEMP.AT(3))
					}
					x++; // ITERATE INPUT INDEX, BECAUSE WE USED IT (not "e" situation)
					output << temp.at(0) << "," << temp.at(1) << "," << temp.at(2) << " => " << temp.at(3) << "," << temp.at(4) << " [STACK]:";
					printStack(stackl, output);
				}
			}

			else
			{
				if(temp.at(2) != e) // CHECK THE POP VALUE IS "e" OR NOT
				{
					if(stackl.top() == temp.at(2))
					{
						stackl.pop();
					}
					else
					{
						continue;
					}
				}
				currentState = temp.at(3); // UPDATE CURRENT STATE
				if(temp.at(4) != e) // CHECK THE PUSH VALUE IS "e" OR NOT
				{
					stackl.push(temp.at(4));// STACK.PUSH(TEMP.AT(3))
				}
				output << temp.at(0) << "," << temp.at(1) << "," << temp.at(2) << " => " << temp.at(3) << "," << temp.at(4) << " [STACK]:";
				printStack(stackl, output);
			}
		}
	}

	isAccepted = gameover(stackl, startfinal, isAccepted, currentState, lastElm);

	stack<string> newStack = stackl;
	string newCurrState = currentState;
	int newX = x;
	int newMaxIteration = maxIteration;

	if(newMaxIteration == 3) {return isAccepted;}

	if(isAccepted == false && newX < inputLine.size() - 1 && newMaxIteration < 3)
	{
		isAccepted = dpdaStart(output, transitions, temp, inputLine, startfinal, newStack, newCurrState, e, lastElm, isAccepted, newX, newMaxIteration);
	}

	return isAccepted;
}

int main(int argc, char** argv)
{
	vector<string> dpda; vector<string> input; vector<string> temp; vector<string> states; vector<string> startfinal; vector<string> ialphabet; vector<string> salphabet; vector<vector<string>> transitions; vector<string> inputLine;
	char ddot = ':'; char comma = ','; string arrow = " => "; string e = "e"; string lastElm = "$"; bool dpdaValid = true;

	ofstream output;
	output.open(argv[3]);
	dpda = fileReader(argv[1]); // READ DPDA RULES FROM .txt FILE
	input = fileReader(argv[2]); // READ DPDA INPUT FROM .txt FILE
	// READ FIRST LINE (STATES)
	temp = stringSplitter(dpda.at(0), ddot); // DELETE DOUBLE DOT
	temp = stringSplitter(temp.at(1), arrow); // DELETE ARROW

	states = stringSplitter(temp.at(0), comma); // SEPERATE ALL STATES
	startfinal = stringSplitter(temp.at(1), comma); // SEPERATE START AND FINAL STATES

	for(int i = 0; i < startfinal.size(); i++) // REMOVE BRACKETS FROM START AND FINAL STATES
	{
		int j = 0;
		while(j < startfinal.at(i).size())
		{
			if( (startfinal.at(i)[j] == '(') || (startfinal.at(i)[j] == ')') || (startfinal.at(i)[j] == '[') || (startfinal.at(i)[j] == ']') || (startfinal.at(i)[j] == ' '))
			{
				startfinal.at(i).erase(j,1);
			}
			else
			{
				j++;
			}
		}
	}

	// READ SECOND LINE (INPUT ALPHABET)
	temp = stringSplitter(dpda.at(1), ddot); // DELETE DOUBLE DOT
	ialphabet = stringSplitter(temp.at(1), comma); // SEPERATE ALL INPUT ALPHABET CHARACTERS

	// READ THIRD LINE (STACK ALPHABET)
	temp = stringSplitter(dpda.at(2), ddot); // DELETE DOUBLE DOT
	salphabet = stringSplitter(temp.at(1), comma); // SEPERATE ALL STACK ALPHABET CHARACTERS

	if(checkAlphabet(salphabet, lastElm) == false) // CHECK INPUT ALPHABET CONTAINS "$"
	{
		lastElm = ""; // IF NOT, LAST ELEMENT IN STACK IS BLANK
	}

	// READ OTHER LINES
	for(int i = 3; i < dpda.size(); i++) // ITERATE ALL TRANSITIOINS
	{
		temp = stringSplitter(dpda.at(i), ddot); // DELETE DOUBLE DOT
		temp = stringSplitter(temp.at(1), comma); // SEPERATE ALL TRANSITIOIN KEYS
		if(checkAlphabet(ialphabet, temp.at(1)) == false) // CHECK VALID DPDA FOR INPUT ALPHABET
		{
			if(temp.at(1) != e) // CORRECTION FOR "e" VALUES
			{
				dpdaValid = false;
			}
		}
		if(checkAlphabet(salphabet, temp.at(2)) == false) // CHECK VALID DPDA FOR STACK ALPHABET (POP VALUE)
		{
			if(temp.at(2) != e) // CORRECTION FOR "e" VALUES
			{
				dpdaValid = false;
			}
		}
		if(checkAlphabet(states, temp.at(3)) == false) // CHECK VALID DPDA FOR NEXT STATE
		{
			dpdaValid = false;
		}
		if(checkAlphabet(salphabet, temp.at(4)) == false) // CHECK VALID DPDA FOR STACK ALPHABET (PUSH VALUE)
		{
			if(temp.at(4) != e) // CORRECTION FOR "e" VALUES
			{
				dpdaValid = false;
			}
		}
		transitions.push_back(temp); // PUT TRANSITION PIECES INTO GENERAL TRANSITIOINS VECTOR
	}

	if(dpdaValid == true){
		// DETERMINISTIC PUSHDOWN AUTOMATA
		for(int i = 0; i < input.size(); i++) // ITERATE ALL INPUT LINES
		{
			stack<string> stack;
			inputLine = stringSplitter(input.at(i), comma); // SEPERATE ALL INPUT PIECES
			inputLine.push_back(" "); // for index error
			int x = 0; // INPUT INDEX
			int maxIteration = 0;
			string currentState = startfinal.at(0); // CURRENT STATE (START)
			bool isAccepted = false;

			isAccepted = dpdaStart(output, transitions, temp, inputLine, startfinal, stack, currentState, e, lastElm, isAccepted, x, maxIteration);

			if(isAccepted == true) {output << "ACCEPT\n\n";}
			else {output << "REJECT\n\n";}
		}

	}

	else {output << "Error [1]:DPDA description is invalid!\n";}

	output.close();
	return 0;
}
