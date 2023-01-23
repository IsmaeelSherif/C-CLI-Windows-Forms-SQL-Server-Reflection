# C-CLI-Windows-Forms-SQL-Server-Reflection
Example on Linking Data-Structure classes with SQL Server Database using Reflection

# Setup
You have to establish the connection with the Database using your Own "Connection String" to be able to make Queries.
In this case, we use SQL Server.
```
int main() {
	SqlDbManager::startConnection(ConnectionString);
	if (!usersRepo->tableExists()) {
		usersRepo->createTable("id");
	}

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew CppCLRWinformsProjekt::Form1());
	return 0;
}
```
# Usage
Here we can make GET / UPDATE queries using Builder Design Pattern:
```
UserData^ newUser = gcnew UserData();

QueryFilter^ filter = gcnew QueryFilter();
filter = filter->whereColumn("id")->isGreaterThanOrEqual(4)->build();
bool isUpdated = usersRepo->updateFiltered(filter, newUser);

filter = gcnew QueryFilter();
filter = filter->
		whereColumn("mybool")->isEqualTo(false)->
		and()->whereColumn("State")->isNull()->
		orderBy("Age", true)->
		orderBy("Salary", false)->
		build();
```
Also you can make custom queries yourself, but you have the responsibility of making sure the query is correct.
Otherwise, it will throw an exception.
```
List<UserData^>^ myUsers = usersRepo->getFromCustomQuery("SELECT * FROM UserData WHERE mybool = 1");
```
