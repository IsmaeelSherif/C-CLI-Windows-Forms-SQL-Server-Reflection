#pragma once
#include <string>
#include "UserData.h"
#include <vector>

using namespace std;

class Node {
public:
	long long value;

	Node(long long val) {
		value = val;
	}
};

class MyDataStruct
{
public:
	int numb;
	string name;
	bool sBool = true;
	float loat = 3.14;
	double doob = 3.14;
	Node* nod = new Node(21234563);
	vector<MyDataStruct> vec;

	MyDataStruct(UserData^ dbUser);

	MyDataStruct(int number, string nam);

	void print();
};

