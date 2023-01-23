#pragma once
#include "SqlRepo.h"
#include "UserData.h"
#include "MyDataStruct.h"

using namespace DBRepository;

#include <msclr/gcroot.h>
using namespace msclr;

#include<string>
using namespace std;

namespace Globals {
	extern MyDataStruct* myGlobalDS;

	extern gcroot<SqlRepo<UserData^>^> usersRepo;

	//convert .NET String to c++ string
	extern string cliToSTD(String^ cliString);

	//convert c++ string to .NET String
	extern String^ stdToCLI(string stdString);
}