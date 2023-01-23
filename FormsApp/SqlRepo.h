#pragma once
#include "UserData.h"
#include "QueryFilter.h"

using namespace System;
using namespace System::Data::SqlClient;
using namespace System::Collections::Generic;

namespace DBRepository {


	ref class SqlDbManager{

	protected:
		static SqlConnection^ sqlConnection;

	public:
		// must be called in main function to connect to the Database
		static void startConnection(String^ connectionString);
	};

	generic <class T>
	ref class SqlRepo : SqlDbManager {

	private:

		String^ tableName = "";

		bool autoIncrementId = false;

		String^ getTableName();

		T getRowFromReader(SqlDataReader^ reader);

		String^ prepareInsertionData(T instance, Reflection::FieldInfo^ prop, bool isFirstColumn);

	public:
		SqlRepo(){}

		SqlRepo(String^ tableName) {
			this->tableName = tableName;

		}
		SqlRepo(String^ tableName, bool autoIncrementId) {
			this->tableName = tableName;
			this->autoIncrementId = autoIncrementId;
		}

		// creates the table with the same name as Template Class name
		// returns true if table created, false if failed
		bool createTable();

		// creates the table with the same name as Template Class name
		// returns true if table created, false if failed
		// for composite key : separate the column names by comma
		bool createTable(String^ keyColumnNames);


		bool tableExists();

		List<T>^ getAll();

		// To get a row using the value of PK
		// For this to work :
		// the primary key variable must be declared the first one in the class
		T getItem(Object^ pkValue);

		// For adding custom where conditions
		// may throw exceptions if used wrong
		List<T>^ getFromCustomQuery(String^ sqlQuery);

		//To use some comparison on columns using where
		List<T>^ getFiltered(QueryFilter^ filter);

		// returns true if record inserted, false if failed
		bool insert(T val);

		// pkValue should be the value of the first class member (should be the primary key)
		// returns true if record successfully updated, false if failed
		bool update(Object^ pkValue, T val);

		
		bool updateFiltered(QueryFilter^ filter, T val);

		// pkValue should be the value of the first class member (should be the primary key)
		// returns true if record successfully deleted, false if failed
		bool remove(Object^ pkValue);

		//To use some comparison on columns using where
		void removeFiltered(QueryFilter^ filter);

	};

}
