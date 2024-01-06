// CppPractice.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "vld.h" //visual leak detector 2019
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <queue>
#include <vector>
#include "text.h"
#include "metric.h"
#include "anode.h"

using namespace std;

uint32_t noiselimit;

int main()
{
    string db, ns, intxt;
    int matches;
    char c;
    cout << "input text:" << endl;
    cin >>intxt;
    cout << "database (0 for default of mylib.txt):"<<endl;
    cin >> db;
    if (db == "0") {db = "mylib.txt";}
    cout << "noise (0 for default of noise.txt):"<<endl;
    cin >> ns;
    if (ns == "0") {ns = "noise.txt";}
    cout << "matches (0 for default of 5):" << endl;
    cin >> matches;
    if (matches == 0) { matches=5; }
    cout << "noise words (0 for default of 100):" << endl;
    cin >> noiselimit;
    if (noiselimit == 0) { noiselimit = 100; }
    cout << "metric - e, m or c (default e)" << endl;
    cin >> c;
    Metric metric = EUCLIDEAN;
    switch (c) {
    case 'm':
        metric = MANHATTAN;
        break;
    case 'c':
        metric = COSINE;
        break;
    default:
        metric = EUCLIDEAN;
    }

    ifstream infile;
    infile.open(ns);
    Text* noise = new Text(&infile, nullptr);
    infile.close();
    infile.open(intxt);
    Text* intext = new Text(&infile, noise); 
    infile.close();

    ifstream libfile;
    libfile.open(db);
    string nstr;
    getline(libfile, nstr);
    int n = stoi(nstr);
    //n = 20;
    priority_queue<AuthorNode, vector<AuthorNode>, AuthorNodeCmp> pq;
    for (auto i = 0; i < n; i++) {
        string name, location;
        getline(libfile, name);
        getline(libfile, location);
        cout << name << '\t' << location << endl;
        infile.open(location);
        Text* newText = new Text(&infile, noise);
        AuthorNode* node = new AuthorNode(name, intext->dist(newText, metric));
        pq.push(*node);
        delete node;
        infile.close();
    }

    cout << "Most to least similar to: " << intxt<<endl;
    for (auto i = 0; !pq.empty() && i < matches; i++) {
        AuthorNode node = pq.top();
        node.print();
        pq.pop();
    }
    delete noise;
    delete intext;
    return 0;

}

