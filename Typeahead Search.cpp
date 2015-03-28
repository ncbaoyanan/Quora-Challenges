// test2.cpp : Defines the entry point for the console application.
//
using namespace std;
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <functional> 
#include <algorithm>
#include <fstream>
#include <list>
#include <map>

//split a string into words
std::vector<std::string> split(const std::string s, char delim) {
    std::stringstream ss(s);
    std::string item;
	std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

//sort strings from longest to shortest
bool lencomp(string s1, string s2){
	return (s1.length() > s2.length());
}

//a node of prefix tree
class Node{
public:
	map <char, Node *> dict;
	unordered_set<string> content;
	Node(){
	}
};


////prefix tree
class Data{
public:
	Node* root;
	unordered_set<std::string> blist; //black list, contains the id-s that have been deleted
	unordered_set<std::string> wlist; //with list, contains the id-s that have been added
	unordered_map<string, string> types; //four types
	unordered_map<string, double> scores; //original scores
	unordered_map<string, long> times; //the adding time 
	vector<std::string> elems; //words of a string
	unordered_map<string, double> candscores; //boosted scores
	unordered_map<unordered_set<string>*, long> querysizes; //returned sizes of querying differnt words
	long time;
	Data(){
		root = new Node;
		time = 0;
	}
	//add each word of a string to the prefix tree
	void AddEach(string word, string id){
		int len = word.length();
		Node * loc = root;
		for(int i = 0; i < len; i ++){
			char ind = word[i];
			if(!(loc->dict[ind])){
				loc->dict[ind] = new Node;
			}
			loc = loc->dict[ind];
			loc->content.insert(id);
		}
			
	}
	//add a string of words to the prefix tree
	void Add(string str){
		elems = split(str, ' ');
		string type = elems[1];
		string id = elems[2];
		string score = elems[3];
		
		time++;

		types[id] = type;
		scores[id] = stod(score);
		times[id] = time;

		
		blist.erase(id); 

		if(wlist.find(id) != wlist.end()){			
			return;
		}

		for(int i = 4; i < elems.size(); i++){
			string word = elems[i];
			for(int j = 0; j < word.length(); j++)
				word[j] = tolower(word[j]);
			AddEach(word, id);
		}
		wlist.insert(id);
	}

	//delete a string, put the string into black list
	void Del(string str){
		elems = split(str, ' ');
		blist.insert(elems[1]);

	}

	//query each word of the given string
	unordered_set<string>* QueryEach(string str){
		for(int i = 0; i < str.length(); i++)
				str[i] = tolower(str[i]);

		int len = str.length();
		Node * loc = root;
		for(int i = 0; i < len; i++){
			if(loc){
				loc = loc->dict[str[i]];
			}
			else
				return NULL;
		}
		if(loc){
			return &(loc->content);	
		}	
		return NULL;
	}

	//compare string without boost, based on their original scores
	bool compwo (string s1, string s2) { 
		if(scores[s1] > scores[s2] )
			return true;
		else if (scores[s1] < scores[s2])
			return false;
		else{
			return times[s1] > times[s2];
		}
	}

	//compare string with bost
	bool compw (string s1, string s2) { 
		if(candscores[s1] > candscores[s2])
			return true;
		else if (candscores[s1] < candscores[s2])
			return false;
		else{
			return times[s1] > times[s2];
		}
	}

	//sort the candidates returned from eachquery from fewest to greatest
	bool small2large (unordered_set<string>* s1, unordered_set<string>* s2) { 
		return (querysizes[s1] < querysizes[s2]);
	}

	//query a given string
	void Query(string str){
		elems = split(str, ' ');
		long N = stol(elems[1]);
		if(N <= 0){
			cout<<endl;
			return;
		}

		// query the lognest word first
		sort(elems.begin() + 2, elems.end(),lencomp);
		vector<unordered_set<string>*> nquery(elems.size()-2, NULL);
		querysizes.clear();
		list<string> soll;
		
		for(int i = 2; i < elems.size(); i++){
			string word = elems[i];
			unordered_set<string>* candone = QueryEach(word);
			if(!candone){
				cout<<endl;
				return;
			}
			nquery[i-2] = candone;
			querysizes[candone] = candone->size();
		}

		//check if a candidate has all the words needed, check from the set includes fewest candidates
		sort(nquery.begin(), nquery.end(), bind(&Data::small2large, this, placeholders::_1, placeholders::_2));
		bool first = true;
		for(int i = 0; i < nquery.size(); i++){
			if(first){
				for(auto it = nquery[i]->begin(); it != nquery[i]->end(); it++){
					soll.push_back(*it);
				}
				first = false;
			}
			else{
				for(auto it = soll.begin(); it != soll.end();){
					if(nquery[i]->find(*it) == nquery[i]->end()){
						it = soll.erase(it);
					}
					else{
						it++;
					}
				}
			}
			if(!soll.size()){
				cout<<endl;
				return;
			}
		}

		//output the results a score-based order
		if(soll.size() > 0){
			soll.sort(bind(&Data::compwo, this, placeholders::_1, placeholders::_2));
			list<string>::iterator it = soll.begin();
			bool first = true;
			int iout = 0;
			while(it != soll.end() && iout < N){
				if(blist.find(*it) == blist.end()){
					if(first){
						cout<<*it;
						first = false;
					}
					else
						cout<<" "<<*it;
					iout++;
				}
				it++;				
			}				
		}
		cout<<endl;		
	}

	//query with boost
	void WQuery(string str){
		elems = split(str, ' ');
		long N = stol(elems[1]);
		if(N <= 0){
			cout<<endl;
			return;
		}
		int Nboost = stoi(elems[2]);
		if(Nboost == 0){
			str = elems[0];
			str += " " + elems[1];
			for(vector<string>::iterator it = elems.begin() + 3; it < elems.end(); it++)
				str += " " + *it;
			Query(str);
			return;
		}
		unordered_map<string, double> boostw;

		for(int i = 3; i < Nboost + 3; i++){
			string oneb = elems[i];
			vector<string> pair = split(oneb,':');
			if(boostw.find(pair[0]) != boostw.end()){
				boostw[pair[0]] *= stof(pair[1]);
			}
			else
				boostw[pair[0]] = stof(pair[1]); 
		}
				
		// query the lognest word first
		sort(elems.begin() + Nboost + 3, elems.end(),lencomp);
		vector<unordered_set<string>*> nquery(elems.size()- (Nboost + 3), NULL);
		querysizes.clear();
		list<string> soll;

		int count = 0;
		for(int i = Nboost + 3; i < elems.size(); i++){
			string word = elems[i];
			unordered_set<string>* candone = QueryEach(word);
			if(!candone){
				cout<<endl;
				return;
			}
			nquery[count] = candone;
			querysizes[candone] = candone->size();
			count++;
		}
		//check if a candidate has all the words needed, check from the set includes fewest candidates
		sort(nquery.begin(), nquery.end(), bind(&Data::small2large, this, placeholders::_1, placeholders::_2));
		bool first = true;
		for(int i = 0; i < nquery.size(); i++){
			if(first){
				for(auto it = nquery[i]->begin(); it != nquery[i]->end(); it++){
					soll.push_back(*it);
				}
				first = false;
			}
			else{
				for(auto it = soll.begin(); it != soll.end();){
					if(nquery[i]->find(*it) == nquery[i]->end()){
						it = soll.erase(it);
					}
					else{
						it++;
					}
				}
			}
			if(!soll.size()){
				cout<<endl;
				return;
			}
		}

		if(soll.size() > 0){
			//compute the boosted scores
			candscores.clear();
			for(auto it = soll.begin(); it != soll.end(); it++){
				string oneid = *it;
				double s = scores[oneid];
				if(boostw.find(oneid) != boostw.end()){
					s *= boostw[oneid];
				}
				string onetype = types[oneid];
				if(boostw.find(onetype) != boostw.end()){
					s *= boostw[onetype];
				}
				candscores[oneid] = s;
			}
			soll.sort(bind(&Data::compw, this, placeholders::_1, placeholders::_2));
			list<string>::iterator it = soll.begin();
			bool first = true;
			int iout = 0;
			while(it != soll.end() && iout < N){
				if(blist.find(*it) == blist.end()){
					if(first){
						cout<<*it;
						first = false;
					}
					else
						cout<<" "<<*it;
					iout++;
				}
				it++;				
			}			
		}
		cout<<endl;		
	}
};

int main(int argc, char* argv[])
{
	long N; 
	cin >> N;
	string str;
	Data xdata;
	int timess = 0;
	int wait = 0;
	int line = 0;
	while(getline(cin,str)){
		// replace '\t' with ' '
		for(int i = 0; i < str.length(); i++){
			if(str[i] == '\t')
				str[i] = ' ';
		}
		if(str[0] == 'A'){
			xdata.Add(str);
		}
		else if(str[0] == 'D'){
			xdata.Del(str);
		}
		else if(str[0] == 'Q'){
			xdata.Query(str);
		}
		else if(str[0] == 'W'){
			xdata.WQuery(str);
		}
	}
	return 0;
}

