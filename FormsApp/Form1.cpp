#include "pch.h"
#include "Form1.h"
#include "Globals.h"
#include "UserData.h"
#include "MyDataStruct.h"

using namespace Globals;

System::Void CppCLRWinformsProjekt::Form1::button1_Click(System::Object^ sender, System::EventArgs^ e)
{
	MyDataStruct* mydata = new MyDataStruct(12 , "lll");
	//UserData^ dbUser = usersRepo->getItem(1);

	//myGlobalDS = new MyDataStruct(dbUser);

	//this->textBox1->Text = stdToCLI(myGlobalDS->name);
	//MessageBox::Show("output: " + stdToCLI(myGlobalDS->name) + " num = " + myGlobalDS->numb + " doob = " + myGlobalDS->doob + " sBool = " + myGlobalDS->sBool + " node = " + myGlobalDS->nod->value);

	

	//Add a user to the Database
	UserData^ newUser = gcnew UserData();
	newUser->id = 4;
	newUser->veryLong = 999678;
	newUser->name = "Ahsw";
	newUser->myBool = true;
	newUser->dooobl = 2082.3532;
	newUser->afloats = 0.443231;
	//usersRepo->insert(newUser);

	//get All Users from Database
	List<UserData^>^ allUsers = usersRepo->getAll();


	//update user with id = 1
	usersRepo->update(1, newUser);

	//delete a user whose id = 4
	//usersRepo->remove(4);
	QueryFilter^ filter = gcnew QueryFilter();
	filter = filter->whereColumn("id")->isGreaterThanOrEqual(4)->build();
	bool isUpdated = usersRepo->updateFiltered(filter, newUser);

	filter = gcnew QueryFilter();

	filter = filter->whereColumn("mybool")->isEqualTo(false)-> and ()->whereColumn("")->isNull()->orderBy("", true)->orderBy("", 2)->build();

	List<UserData^>^ test = usersRepo->getFiltered(filter);


	List<UserData^>^ filteredUsers = usersRepo->getFiltered(filter);

	// get users from your custom query
	// may throw exceptions if used wrong
	List<UserData^>^ myUsers = usersRepo->getFromCustomQuery("SELECT * FROM UserData WHERE mybool = 1");


}

