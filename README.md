# C-CLI-Windows-Forms-SQL-Server-Reflection
Example on Linking Data-Structure classes with SQL Server Database using Reflection

```
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
```
