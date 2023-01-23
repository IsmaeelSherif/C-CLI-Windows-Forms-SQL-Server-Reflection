#include "pch.h"

using namespace System;

#include "Form1.h"

using namespace System::Windows::Forms;

#include "Globals.h"

#include "SqlRepo.h"

using namespace Globals;

[STAThread]
int main() {
	SqlDbManager::startConnection("Data Source=DESKTOP-H1U4ABL;Initial Catalog=DemoAppDb;Integrated Security=True");
	if (!usersRepo->tableExists()) {
		usersRepo->createTable("id");
	}

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinformsProjekt::Form1());
	return 0;
}