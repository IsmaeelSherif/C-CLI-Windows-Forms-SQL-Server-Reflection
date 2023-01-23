#include "pch.h"
#include "MyDataStruct.h"
#include <iostream>
#include "Globals.h"



MyDataStruct::MyDataStruct(UserData^ dbUser)
{
	name = Globals::cliToSTD(dbUser->name);
	numb = dbUser->id;
	sBool = dbUser->myBool;
	loat = dbUser->afloats;
	doob = dbUser->dooobl;
	nod->value = dbUser->veryLong;
}

MyDataStruct::MyDataStruct(int number, string nam) {
	numb = number;
	name = nam;
}

void MyDataStruct::print()
{
	std::cout << "Hello";
}


