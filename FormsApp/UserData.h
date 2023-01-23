#pragma once

using namespace System;

ref class UserData {
public:

	int id; //if this will be primary key and auto generated, it should be the first member (like this)
	long long veryLong;
	String^ name;
	bool myBool;
	float afloats;
	double dooobl;

	//void doTh();
};

