#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <stdbool.h>
#include <vector>
#include <algorithm>
#include <fstream>
using namespace std;

#define ALPHABET_SIZE 26

vector<string> keys;
vector<string> values;
vector<string> sorted;
vector<string> sortedValues(100);

/*Trie struct*/
struct TrieNode
{
	bool isEnd;
	string value_of_last_char;
	struct TrieNode* child[ALPHABET_SIZE];

};


/*Inıtializing Tries with this function.*/
struct TrieNode *init()
{
	struct TrieNode* node = new TrieNode;
	
	node->isEnd = false;
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		node->child[i] = NULL;
	}

	return node;
	
}



/*Insert function.*/
void insert(TrieNode *root, string key,string value)
{
	struct TrieNode* tempNode = root;
	int depth = 0;
	while (depth < key.length())
	{
		int index = key[depth] - 'a';
		if (tempNode->child[index] == NULL)//If child is NULL then initialize child.
			tempNode->child[index] = init();

		tempNode = tempNode->child[index];
		depth++;
	}
	if (tempNode->isEnd)
	{
		cout << key << " already exist.\n";
	}
	else
	{
		tempNode->isEnd = true;
		tempNode->value_of_last_char = value;
		keys.push_back(key); //Adding keys and values to vector for listing keys.
		values.push_back(value);
		cout << key << " was added.\n";
	}
}

/*Empty or not.*/
bool isEmpty(TrieNode* root)
{
	for (int i = 0; i < ALPHABET_SIZE; i++)
		if (root->child[i] != NULL)
			return false;
	return true;
}

/*Search function.*/
string search(struct TrieNode* root, string key)
{
	struct TrieNode* tempNode = root;
	int len = 0;
	
	if (root == NULL)
		return "no record.\n";

	while (len < key.size())
	{
		int index = key[len] - 'a';
		
		tempNode = tempNode->child[index];

		if (tempNode == NULL && len == 0)
			return "no record.\n";
		else if (tempNode == NULL)
			return "incorrect Dothraki word.\n";
		len++;
	}

	if (tempNode->isEnd)
		return "english equivalent is " + tempNode->value_of_last_char + "\n";
	if (!isEmpty(tempNode) && !tempNode->isEnd)
		return "not enough dothraki word.\n";
}

/*Delete function*/
TrieNode* deletion(TrieNode* &root, string key, int level = 0)
{
	if (root == NULL && level == 1)
	{
		cout << "no record.\n"; //If NULL and first char then no record.
		return NULL;
	}
	else if (root == NULL)
	{
		cout << "incorrect Dothraki word.\n";
		return NULL;
	}

	if (level == key.size())
	{
		/*If is end then delete keys and values from vectors.*/
		if (root->isEnd)
		{
			root->isEnd = false;
			root->value_of_last_char = "";
			cout << key << " deletion is successful.\n";
			keys.erase(remove(keys.begin(), keys.end(), key), keys.end());
			values.erase(remove(values.begin(), values.end(), root->value_of_last_char), values.end());
		}
		/*If is empty then delete.*/
		if (isEmpty(root))
		{
			delete(root);
			root = NULL;
		}
		else
			cout << "not enough Dothraki word.\n";

		return root;
	}

	int index = key[level] - 'a';
	root->child[index] = deletion(root->child[index], key, level + 1);//Recursive function for deletion.

	if (isEmpty(root) && !root->isEnd)
	{
		delete(root);
		root = NULL;
	}

	return root;
}


void print_search(struct TrieNode* root, string key)
{
	cout << search(root, key);
}




bool compareKey(string a, string b) { return a < b; }

/*Listing function. We need 4 vector because when we sort alphabetically indexes were change.*/
void list()
{
	sorted = keys;
	sort(keys.begin(), keys.end(), compareKey);

	for (int i = 0; i < size(sorted); i++)
	{
		for (int j = 0; j < size(sorted); j++)
		{
			if (sorted[i] == keys[j])
			{
				sortedValues[j] = values[i];
			}
		}
	}
	
	for (int i = 0; i < sorted.size(); i++)
	{
		cout << "Key = " << keys[i] << " Value = " <<  sortedValues[i] << "\n";
	}
}

int main()
{
	struct TrieNode *root = init();
	string line;
	ifstream input("C:\\Users\\an-sa\\Desktop\\visual s tudio projelerim\\Trie\\Trie\\io.txt");

	
	/*Reading input file.*/
	if (input.is_open()) {
		while (getline(input, line))
		{
			int bracket = 0;
			int comma = 0;
			int bracket2 = 0;
			int comma2 = 0;
			string x; // x and y strings for parameters of functions.
			string y;//we find brackets and commas then catch keys and values.
			/*If contain insert*/
			if (line.find("insert") != string::npos)
			{
				x = line.substr(line.find('(') + 1, line.find(')'));
				y = line.substr(line.find('(') + 1, line.find(')'));
				x = x.substr(0, x.find(','));
				y.pop_back();
				y = y.substr(y.find(',') + 1, y.find(')'));
				insert(root, x, y);

			}
			/*If contain search*/
			else if (line.find("search") != string::npos)
			{
				x = line.substr(line.find('(') + 1, line.find(')'));
				x = x.substr(0, x.find(','));
				x.pop_back();
				print_search(root, x);
			}
			
			else if (line.find("list") != string::npos)
			{
				list();//If contain list.
			}

			/*If contain delete.*/
			else if (line.find("delete") != string::npos)
			{
				x = line.substr(line.find('(') + 1, line.find(')'));
				x = x.substr(0, x.find(','));
				x.pop_back();
				deletion(root, x);
			}

			
		}
		input.close();
	}
	
}

