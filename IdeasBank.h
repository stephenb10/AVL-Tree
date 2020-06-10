#pragma once
#include <fstream>
#include <map>
#include "AVL_ADT.h"

struct Index {
	string key;
	vector<int> idList;
};

void printIndecies(Index data);

class IdeasBank {
	map<int, Idea> ideas;
	AvlTree<Index, string> indexTree;

	void indexWord(string word);
	void indexIdea();

	int getRandomUniqueID();
	void generateRelatedIdeas();
	vector<int> searchTree(string word);
	vector<int> searchMap(string word);
	void add(Idea idea);
	void remove(int id);
	Idea ideaFromInput();

public:
	void ideasFromFile();

	void addIdea();
	void deleteIdea();
	void display();
	void printData();

	Idea* getIdea(int id);

	void searchIdeas(bool treeSearch);
	void displayRelatedIdeas();
};

Idea* IdeasBank::getIdea(int id)
{
	return &ideas.find(id)->second;
}

Idea IdeasBank::ideaFromInput()
{
	int id = getRandomUniqueID();
	string proposer;
	vector<string> keywords;
	string content;

	cout << "\nEnter the name of the proposer: ";
	cin >> proposer;

	cin.ignore();
	cout << "\nEnter the idea: " << endl;
	getline(cin, content);

	cout << "\nEnter the keywords, enter \"end\" to finish: " << endl;
	string input;

	do
	{
		cin >> input;
		if (input != "end")
			keywords.push_back(input);
		else
			if (keywords.empty())
				cout << "You must enter at least one keyword!" << endl;
		
	} while (input != "end" || keywords.empty());
	cout << endl;

	return Idea(id, proposer, keywords, content);
}

void IdeasBank::ideasFromFile()
{
	ifstream infile;
	do {
		cout << "Enter the name of the file: ";
		string fileName;
		cin >> fileName;
		infile.open(fileName);
	}
	while (!infile);

	
	string keys, s, content, proposer;
	vector<string> keywords;
	int id = 1;
	while (!infile.eof())
	{
		getline(infile, proposer);
		getline(infile, keys);
		getline(infile, content);

		stringstream ss(keys);
		keywords.clear();
		while (getline(ss, s, ' '))
			keywords.push_back(s);

		getline(infile, s);
		Idea idea(id, proposer, keywords, content);
		id++;
		ideas.insert(pair<int, Idea>(idea.getID(), idea));
	}
	indexIdea();
	cout << "Ideas Imported\n\n";
}

void IdeasBank::addIdea()
{
	add(ideaFromInput());
}

void IdeasBank::deleteIdea()
{
	cout << "Enter the id of the Idea to delete: ";
	int id;
	cin >> id;
	remove(id);
	cout << endl;
}

void IdeasBank::add(Idea idea)
{
	ideas.insert(pair<int, Idea>(idea.getID(), idea));
	indexIdea();
}

void IdeasBank::remove(int id)
{
	map<int, Idea>::iterator itr;
	itr = ideas.find(id);
	if (itr != ideas.end())
	{
		Index data;
		for (string keyword : itr->second.getKeywords())
			if (indexTree.AVL_Retrieve(keyword, data))
				if (data.idList.size() <= 1)
					indexTree.AVL_Delete(keyword);
				
		istringstream ss(itr->second.getContent());
		for (string s; ss; ss >> s)
			if (indexTree.AVL_Retrieve(s, data))
				if (data.idList.size() <= 1)
					indexTree.AVL_Delete(s);

		ideas.erase(id);
	}

	indexIdea();
}

void IdeasBank::display()
{
	map<int, Idea>::iterator itr;
	for (itr = ideas.begin(); itr != ideas.end(); ++itr)
	{
		itr->second.display();
		cout << endl;
	}

	cout << endl << endl;
}

void IdeasBank::printData()
{
	indexTree.AVL_Traverse(&printIndecies);
	cout << endl;
}

int IdeasBank::getRandomUniqueID()
{
	int id;
	bool unique = true;
	do
	{
		id = rand() % 1000;
		map<int, Idea>::iterator itr;
		itr = ideas.find(id);
		unique = (itr == ideas.end());
	} while (!unique);

	return id;
}

void IdeasBank::indexIdea()
{
	map<int, Idea>::iterator itr;

	for (itr = ideas.begin(); itr != ideas.end(); ++itr)
	{
		itr->second.resetRelatedIdeas();

		for (string keyword : itr->second.getKeywords())
			indexWord(keyword);

		istringstream ss(itr->second.getContent());
		for (string s; ss; ss >> s)
			indexWord(s);
	}

	generateRelatedIdeas();
}

void IdeasBank::indexWord(string word)
{
	char chars[] = ".,!?()\"";
	for(char c : chars)
		word.erase(std::remove(word.begin(), word.end(), c), word.end());

	if (word.empty())
		return;

	map<int, Idea>::iterator itr;
	vector<int> relatedIdeas;
	for (itr = ideas.begin(); itr != ideas.end(); ++itr)
		if (itr->second.hasWord(word))
			relatedIdeas.push_back(itr->second.getID());

	Index index;
	index.key = word;
	index.idList = relatedIdeas;
	if (!indexTree.AVL_Update(word, index))
		indexTree.AVL_Insert(index);
}

void IdeasBank::searchIdeas(bool treeSearch)
{
	vector<int> relatedIdeas;
	string word;
	cout << "Enter a word to search: ";
	cin >> word;

	if (treeSearch)
		relatedIdeas = searchTree(word);
	else
		relatedIdeas = searchMap(word);

	if (relatedIdeas.empty())
		cout << "No results for \"" << word << "\"\n";
	else
	{
		cout << "\nResults for \"" << word <<"\"\n\n";
		for (int i : relatedIdeas)
			getIdea(i)->display();
	}
	cout << endl;
}

vector<int> IdeasBank::searchTree(string word)
{
	vector<int> relatedIdeas;
	Index data;
	if (indexTree.AVL_Retrieve(word, data))
		relatedIdeas = data.idList;

	return relatedIdeas;
}

vector<int> IdeasBank::searchMap(string word) 
{
	vector<int> relatedIdeas;
	map<int, Idea>::iterator itr;
	for (itr = ideas.begin(); itr != ideas.end(); ++itr)
		if (itr->second.hasWord(word))
			relatedIdeas.push_back(itr->second.getID());

	return relatedIdeas;
}

void IdeasBank::generateRelatedIdeas()
{
	Index data;
	map<int, Idea>::iterator itr, inner;
	for (itr = ideas.begin(); itr != ideas.end(); ++itr)
		for (string keyword : itr->second.getKeywords())
			if (indexTree.AVL_Retrieve(keyword, data))
				for (int id : data.idList)
					getIdea(id)->addRelatedIdea(itr->second.getID());
}

void IdeasBank::displayRelatedIdeas()
{
	int id;

	cout << "Enter the id of the Idea to find related ideas: ";
	cin >> id;

	Idea* idea = getIdea(id);

	for (int i : idea->getRelatedIdeas())
		cout << i << " ";

	cout << endl << endl;
}

void printIndecies(Index data) 
{
	cout << "KEY: " << data.key;
	cout << " - Related Ideas: ";
	for (int id : data.idList)
		cout << id << " ";
	cout << endl;
}