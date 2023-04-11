#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

const int alphabet_size = 26;

struct TrieNode // STRUCT DEFINITION FOR TRIE'S NODE
{
     struct TrieNode *children[alphabet_size]; // ALPHABET SIZE NODE
     int value; // VALUE OF NODES
};

struct TrieNode *getNode(void) // CREATE A NEW NODE
{
	struct TrieNode *pNode =  new TrieNode;

	pNode->value = -1; // -1 FOR NULL

	for (int i = 0; i < alphabet_size; i++) // INITIALIZE ALL CHILDREN TO NULL
	    pNode->children[i] = NULL;

	return pNode;
};

void insert(struct TrieNode *root, string key, int val)
{
    struct TrieNode *p = root;

    for (int i = 0; i < key.length(); i++) // ITERATE OVER KEY'S LETTERS
    {
        int index = key[i] - 'a'; // TAKE INDEX OF LETTER
        if (!p->children[index]) // IF THIS INDEX POINTS NULL:
        {
            p->children[index] = getNode(); // GET NODE
        }

        p = p->children[index]; // STORE LETTER IN THIS NODE

        if(i == key.length() - 1) // IF LAST LETTER:
        {
        	p->value = val; // EQUAL TO THIS VALUE
        }
    }
}

bool isAnother(TrieNode *root) // IS THERE ANOTHER WORD WITH THIS ROOT
{
	for(int k = 0; k < alphabet_size; k++) // ITERATE
	{
		if(root->children[k] != NULL) // IF YES, RETURN TRUE
		{
			return true;
		}
	}
	return false; // ELSE, RETURN FALSE
}

int search(ofstream& output, struct TrieNode *root, string key, bool mode = false)
{
	// MODE FALSE FOR NOT WRITING OPERATIONS. IF COMMAND IS SEARCH, MODE = TRUE FOR WRITING
	struct TrieNode *temp = root;
	int depth = 0;

	for(int i = 0; i < key.length(); i++)
	{
		int index = key[i] - 'a';

		if(!temp->children[index])
		{
			if(depth == 0) // IF FIRST CHARACTER FAILS
			{
				if(mode == true) // WRITING MODE ON OR OFF
				{
					output << '"' << "no record" << '"' << endl;
				}
				return -1; // RETURN FALSE
			}
			break; // BREAK
		}
		depth++;

		temp = temp->children[index];
	}

	if(depth < key.length() - 1) // IF SEARCH IS NOT COMPLETED
	{
		if(mode == true) // WRITING MODE ON OR OFF
		{
			if(isAnother(temp) == true)
			{
				output << '"' << "incorrect Dothraki word" << '"' << endl;
			}
		}

		return -1; // RETURN FALSE
	}

	else // IF SEARCH IS COMPLETED
	{
		if(temp->value != -1) // IF SOMETHING IS FOUND
		{
			return temp->value;
		}

		else // IF NOT
		{
			if(mode == true) // WRITING MODE ON OR OFF
			{
				output << '"' << "not enough Dothraki word" << '"' << endl;
			}

			return -1; // RETURN FALSE
		}
	}
}

void preorder(ofstream& output, struct TrieNode *root) // PREORDER ITERATE OVER TRIE
{
	struct TrieNode *temp = root;

	if(temp == NULL){return;}

	for(int index = 0; index < alphabet_size; index++)
	{
		if(temp->children[index]) // IF HAVE CHILDREN
		{
			char c = 'a' + index; // GET CHARACTER VALUE
			output << c;
			if(temp->children[index]->value != -1){output << endl;} // IF CHILDREN IS END OF WORD
			preorder(output, temp->children[index]); // RECURSIVE CALL OVER CHILDREN
		}
	}
}

TrieNode* deletE(TrieNode* root, string key, int depth = 0)
{
	if(root == NULL){return NULL;} // IS TREE EMPTY

	if(depth == key.size()) // IF LAST CHARACTER PROCESSING:
	{
		if(root->value != -1)
		{
			root->value = -1; // CHANGE VALUE TO NULL (-1 IN MY CODE)
		}

		if(isAnother(root) == false) // IF THERE IS NOT ANY WORD WITH THIS ROOT
		{
			delete(root);
			root = NULL;
		}

		return root;
	}

	int index = key[depth] -'a'; // OBTAIN ASCII VALUE

	root->children[index] = deletE(root->children[index], key, depth+1); // GO TO THE LAST CHARACTER

	if(isAnother(root) == false && root->value == -1) // ROOT DOESN'T HAVE CHILD AND NOT AN ENDING OF WORD
	{
		delete(root);
		root = NULL;
	}

	return root;
}

vector<string> fileReader(string fileName) // FILE READING FUNCTION
{
	vector<string> arr; // FOR KEEPING LINES
	string line; // FOR TAKING LINE
	ifstream myfile (fileName); // OPEN FILE WITH GIVEN ARG NAME

	if (myfile.is_open()) // IF IT IS OPEN
	{
		while(getline(myfile, line)) // GET LINE FROM FILE
		{
			arr.push_back(line); // PUSH LINES TO ARRAY
		}
		myfile.close(); // CLOSE WHEN FINISHED
	}
	else // IF CAN'T OPEN
	{
		cout << "Can't open file: " << fileName << endl; // ERROR MESSAGE
	}

	return arr; // RETURN ARRAY OF LINES
}

string removeBracket(string str) // REMOVING AND REPLACING BRACKETS FOR EASY USAGE
{
	int s = 0; // ITERATION VARIABLE

	while (s < str.size())
	{
		if (str[s] == '(') // IF '(':
		{
			str.replace(s, 1, " "); // CHANGE WITH ' '
		}

		else if (str[s] == ')') // IF ')':
		{
			str.erase(s, 1); // DELETE IT
		}

		else
		{
			s++; // ELSE, KEEP ITERATING
		}
	}

	return str; // RETURN NEW STRING
}

vector<string> stringSplitter(string str, char split)
{
	vector<string> arr; // RETURN ARRAY
	string word;
	stringstream ss(str);
	while(getline(ss, word, split))
	{
		arr.push_back(word); // PUSH SPLITTED WORDS TO ARRAY
	}

	return arr;
}

int main(int argc, char** argv)
{
	ofstream output;
	output.open(argv[2]);

	vector<string> inputFile = fileReader(argv[1]); // READ INPUT.TXT
	vector< vector<string> > inputs; // STORE INPUT COMMANDS

	map<string,string> dictionary; // STORE TRANSLATION PAIRS

	struct TrieNode *root = getNode(); // STORE DOTHRAKI WORDS

	int values = 0;

	for(int i = 0; i < inputFile.size(); i++) // ITERATE OVER INPUT LINES
	{
		inputFile.at(i) = removeBracket(inputFile.at(i)); // REMOVE AND REPLACE BRACKETS
		vector<string> temp = stringSplitter(inputFile.at(i), ' '); // SPLIT COMMAND FROM INPUT VALUES
		inputs.push_back(temp); // PUSH TO INPUT COMMANDS ARRAY
	}

	for(int c = 0; c < inputs.size(); c++) // ITERATE ALL INPUT LINES
	{
		string command;
		command = inputs.at(c).at(0); // EXTRACT COMMAND PART

		if (command == "insert") // IF COMMAND IS "INSERT":
		{
			vector<string> keyVal;
			keyVal = stringSplitter(inputs.at(c).at(1), ','); // SPLIT KEY AND VALUE PAIRS

			if(search(output, root, keyVal.at(0)) != -1) // IF DOTHRAKI WORD EXIST
			{
				if(dictionary[keyVal.at(0)] == keyVal.at(1)) // IF GIVEN ENGLISH MEANING IS SAME IN SYSTEM
				{
					output << '"' << keyVal.at(0) << '"' << " already exist" << endl;
				}

				else // IF IT IS NOT SAME
				{
					dictionary[keyVal.at(0)] = keyVal.at(1); // UPDATE
					output << '"' << keyVal.at(0) << '"' << " was updated" << endl;
				}
			}

			else // IF IT IS A BRAND NEW DOTHRAKI WORD
			{
				insert(root, keyVal.at(0), values); // INSERT INTO TRIE
				output << '"' << keyVal.at(0) << '"' << " was added" << endl;
				values++;
				dictionary[keyVal.at(0)] = keyVal.at(1); // ADD TO DICTIONARY
			}
		}

		else if (command == "search") // IF COMMAND IS "SEARCH":
		{
			 int result = search(output, root, inputs.at(c).at(1), true);

			 if(result != -1)
			 {
				 output << '"' << "The English equivalent is " << dictionary[inputs.at(c).at(1)] << '"' << endl;
			 }
		}

		else if (command == "list") // IF COMMAND IS "LIST":
		{
			preorder(output, root);
		}

		else if (command == "delete") // IF COMMAND IS "DELETE":
		{
			if(search(output, root, inputs.at(c).at(1), true) != -1) // IF THERE IS A WORD:
			{
				root = deletE(root, inputs.at(c).at(1)); // DELETE FROM TRIE
				dictionary.erase(inputs.at(c).at(1)); // DELETE FROM DICTIONARY
				output << '"' << inputs.at(c).at(1) << '"' << " deletion is successful" << endl;
			}
		}

		else // IF COMMAND IS WRONG
		{
			output << "ERROR: Wrong Command. '" << command << "' is not a valid command." << endl;
		}
	}

	output.close();

	return 0;
}
