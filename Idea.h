#pragma once
#include "Utility.h"

class Idea {
	int id;
	string proposer;
	vector<string> keywords;
	string content;
	vector<int> relatedIdeas;
	bool isKeyword(string word);
	bool isInContent(string word);

public:

	Idea() {}
	Idea(const int i, const string p, const vector<string>& k, const string c);

	bool operator==(const Idea& i);

	void addRelatedIdea(int ID);
	vector<int> getRelatedIdeas() { return relatedIdeas; }
	void resetRelatedIdeas() { relatedIdeas.clear(); }

	void display();

	bool hasWord(string word);

	int getID() { return id; }
	string getContent() { return content; }
	vector<string> getKeywords() { return keywords; }

};

Idea::Idea(const int i, const string p, const vector<string>& k, const string c)
{
	id = i;
	proposer = p;
	keywords = k;
	content = c;
}

bool Idea::operator==(const Idea& i)
{
	if (this->id == i.id)
		return true;
	else
		return false;
}

bool Idea::hasWord(string word) {
	return isKeyword(word) || isInContent(word);
}

bool Idea::isKeyword(string word)
{
	for (string w : keywords)
		if (w == word)
			return true;

	return false;
}

bool Idea::isInContent(string word)
{
	return (content.find(word) != string::npos);
}

void Idea::addRelatedIdea(int ID)
{
	if (ID == id || contains(ID, relatedIdeas))
		return;

	relatedIdeas.push_back(ID);
}


void Idea::display()
{
	cout << " idea id:\t " << id << endl;

	cout << " related:\t ";
	for (int i : relatedIdeas)
		cout << i << " ";
	cout << endl;

	cout << "proposer:\t " << proposer << endl;

	cout << "keywords:\t ";
	for (int i = 0; i < keywords.size(); i++)
		if (i < keywords.size() - 1)
			cout << keywords[i] << ", ";
		else
			cout << keywords[i];
	cout << endl;

	cout << " content:";
	const unsigned max_line_length(200);
	istringstream text_iss(content);

	std::string word;
	unsigned characters_written = 0;

	cout << "\t";
	while (text_iss >> word) {

		if (word.size() + characters_written > max_line_length) {
			cout << "\n" << "\t\t";
			characters_written = 0;
		}

		cout << word << " ";
		characters_written += word.size() + 1;
	}
	cout << endl << endl;
}

