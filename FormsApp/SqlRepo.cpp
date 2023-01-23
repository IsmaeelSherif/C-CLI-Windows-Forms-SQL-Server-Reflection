#include "pch.h"
#include "SqlRepo.h"
#include "UserData.h"
#include <assert.h>

using namespace System;
using namespace System::Data::SqlClient;

namespace DBRepository {

	void SqlDbManager::startConnection(String^ connectionString)
	{
		if (sqlConnection == nullptr) {
			sqlConnection = gcnew SqlConnection(connectionString);
			sqlConnection->Open();
		}
		
	}



	generic <class T>
	String^ SqlRepo<T>::getTableName()
	{
		if (tableName->Length > 0) {
			return tableName;
		}

		Type^ objectType = T::typeid;
		return objectType->Name + "s";
	}

	generic <class T>
	bool SqlRepo<T>::createTable()
	{
		return createTable("");
	}


	generic <class T>
	bool SqlRepo<T>::createTable(String^ keyColumnNames)
	{
		assert(!tableExists());

		Type^ objectType = T::typeid;
		String^ sqlQuery = "CREATE TABLE " + getTableName() + "(";

		keyColumnNames = keyColumnNames->ToLower();
		array<String^>^ keyColumns = keyColumnNames->Split(',');

		array<Reflection::FieldInfo^>^ properties = objectType->GetFields();

		for (size_t i = 0; i < properties->Length; i++)
		{
			String^ fieldName = properties[i]->Name->ToLower();
			String^ fieldDataType = properties[i]->FieldType->Name;
			String^ sqlDataType;

			if (fieldDataType == "Int32") {
				sqlDataType = "int";
			}
			else if (fieldDataType == "Int64") {
				sqlDataType = "bigint";
			}
			else if (fieldDataType == "String") {
				sqlDataType = "nvarchar(max)";
				if (keyColumnNames->Contains(fieldName)) {
					sqlDataType = "nvarchar(450)";
				}
			}
			else if (fieldDataType == "Boolean") {
				sqlDataType = "bit";
			}
			else if (fieldDataType == "Single") {
				sqlDataType = "real";
			}
			else if (fieldDataType == "Double") {
				sqlDataType = "float";
			}
			else {
				throw gcnew Exception();
			}

			String^ keySyntax = "";

			if (keyColumnNames->Contains(fieldName)) {
				keySyntax = " NOT NULL ";
				if (i == 0 && autoIncrementId) {
					keySyntax += "IDENTITY(1,1) ";
				}
			}


			sqlQuery += " " + fieldName + " " + sqlDataType + keySyntax + ",";
		}


		if (keyColumns->Length > 0) {
			sqlQuery += " CONSTRAINT PK_" + getTableName() + " PRIMARY KEY (";
			for each (String ^ colName in keyColumns)
			{
				sqlQuery += colName + ",";
			}
			sqlQuery = sqlQuery->Remove(sqlQuery->Length - 1);
			sqlQuery += "),";
		}

		sqlQuery = sqlQuery->Remove(sqlQuery->Length-1);
		sqlQuery += ");";

		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();
		if (reader->Read()) {
			return true;
		}
		reader->Close();
		return false;
	}


	generic <class T>
	bool SqlRepo<T>::tableExists()
	{
		Type^ objectType = T::typeid;
		String^ sqlQuery = "select count (*) as myColName from information_schema.tables where table_name = '"+ getTableName() + "'";
		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();
		bool exists = false;
		if (reader->Read()) {
			Object^ tableCount = reader["myColName"];
			exists = tableCount->Equals(1);
		}
		reader->Close();
		return exists;
	}

	generic <class T>
	T SqlRepo<T>::getRowFromReader(SqlDataReader^ reader)
	{
		Type^ objectType = T::typeid;
		T instance = (T)Activator::CreateInstance(objectType);

		for each (Reflection::FieldInfo ^ prop in objectType->GetFields())
		{
			try
			{
				String^ fieldName = prop->Name;
				Object^ val = reader[fieldName];
				prop->SetValue(instance, val);
			}
			catch (Exception^ e) {} //field is not in the database table

		}

		return instance;
	}

	generic <class T>
	String^ SqlRepo<T>::prepareInsertionData(T instance, Reflection::FieldInfo^ prop, bool isFirstColumn)
	{
		String^ fieldDataType = prop->FieldType->Name;
		Object^ propVal = prop->GetValue(instance);
		String^ sqlVal;

		if (isFirstColumn && fieldDataType->StartsWith("Int") && autoIncrementId) {
			return "";
		}

		if (fieldDataType == "String") {
			sqlVal = "'" + propVal + "'";
		}
		else if (fieldDataType == "Boolean") {
			sqlVal = propVal->ToString()->ToLower() == "true" ? "1" : "0";
		}
		else {
			sqlVal = propVal->ToString();
		}

		return sqlVal;

	}


	generic <class T>
	List<T>^ SqlRepo<T>::getAll()
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		String^ sqlQuery = "SELECT * FROM " + getTableName();
		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();

		List<T>^ list = gcnew List<T>();

		while (reader->Read()) { //1 iteration = 1 row
			T instance = getRowFromReader(reader);
			list->Add(instance);
		}

		reader->Close();
		return list;
	}

	generic <class T>
	T SqlRepo<T>::getItem(Object^ pkValue)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		String^ colName = objectType->GetFields()[0]->Name->ToLower();

		String^ sqlQuery = "SELECT * FROM " + getTableName() + " WHERE " + colName + "=" + pkValue;
		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();

		T instance;
		if (reader->Read()) {
			instance = getRowFromReader(reader);
		}

		reader->Close();
		return instance;
	}

	

	generic <class T>
	List<T>^ SqlRepo<T>::getFromCustomQuery(String^ sqlQuery)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();

		List<T>^ list = gcnew List<T>();

		while (reader->Read()) { //1 iteration = 1 row
			T instance = getRowFromReader(reader);
			list->Add(instance);
		}

		reader->Close();
		return list;
	}

	generic <class T>
	List<T>^ SqlRepo<T>::getFiltered(QueryFilter^ filter)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		String^ sqlQuery = "SELECT * FROM " + getTableName() + filter->wcond();

		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();

		List<T>^ list = gcnew List<T>();
		while (reader->Read()) { //1 iteration = 1 row
			T instance = getRowFromReader(reader);
			list->Add(instance);
		}

		reader->Close();
		return list;
	}

	generic <class T>
	bool SqlRepo<T>::insert(T val)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;
		String^ sqlQuery = "INSERT INTO " + getTableName() + " VALUES(";

		array<Reflection::FieldInfo^>^ properties = objectType->GetFields();

		for (size_t i = 0; i < properties->Length; i++) {
			bool isFirstColumn = i == 0;
			String^ sqlVal = prepareInsertionData(val, properties[i], isFirstColumn);
			if (sqlVal->Length > 0) {
				sqlQuery += " " + sqlVal + ",";
			}
		}


		sqlQuery = sqlQuery->Remove(sqlQuery->Length-1);
		sqlQuery += ");";

		bool success = true;
		try
		{
			SqlCommand command(sqlQuery, sqlConnection);
			SqlDataReader^ reader = command.ExecuteReader();
			reader->Close();
		}
		catch (Exception^ e)
		{
			success = false;
		}
		
		return success;
	}

	generic <class T>
	bool SqlRepo<T>::update(Object^ pkValue, T val)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		String^ pkColName = objectType->GetFields()[0]->Name->ToLower();
		String^ sqlQuery = "UPDATE " + getTableName() + " SET";

		array<Reflection::FieldInfo^>^ properties = objectType->GetFields();

		for (size_t i = 1; i < properties->Length; i++) {
			String^ sqlVal = prepareInsertionData(val, properties[i], false);
			if (sqlVal->Length > 0) {
				String^ fieldName = properties[i]->Name;
				sqlQuery += " " + fieldName + " = " + sqlVal + ",";
			}
			
		}
		sqlQuery = sqlQuery->Remove(sqlQuery->Length - 1);

		sqlQuery += " WHERE " + pkColName + " = " + pkValue;

		bool success = true;
		try
		{
			SqlCommand command(sqlQuery, sqlConnection);
			SqlDataReader^ reader = command.ExecuteReader();
			reader->Close();
		}
		catch (Exception^ e)
		{
			success = false;
		}
		
		
		return success;
	}

	generic <class T>
	bool SqlRepo<T>::updateFiltered(QueryFilter^ filter, T val)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		String^ sqlQuery = "UPDATE " + getTableName() + " SET";

		array<Reflection::FieldInfo^>^ properties = objectType->GetFields();

		for (size_t i = 1; i < properties->Length; i++) {
			String^ sqlVal = prepareInsertionData(val, properties[i], false);
			if (sqlVal->Length > 0) {
				String^ fieldName = properties[i]->Name;
				sqlQuery += " " + fieldName + " = " + sqlVal + ",";
			}
			
		}
		sqlQuery = sqlQuery->Remove(sqlQuery->Length - 1);

		sqlQuery += filter->wcond();

		bool success = true;
		try
		{
			SqlCommand command(sqlQuery, sqlConnection);
			SqlDataReader^ reader = command.ExecuteReader();
			reader->Close();
		}
		catch (Exception^ e)
		{
			success = false;
		}
		
		
		return success;
	}

	generic <class T>
	bool SqlRepo<T>::remove(Object^ pkValue)
	{
		assert(tableExists());

		Type^ objectType = T::typeid;

		String^ colName = objectType->GetFields()[0]->Name->ToLower();
		String^ sqlQuery = "DELETE FROM " + getTableName() + " WHERE " + colName + "=" + pkValue;

		bool success = true;
		try
		{
			SqlCommand command(sqlQuery, sqlConnection);
			SqlDataReader^ reader = command.ExecuteReader();
			reader->Close();
		}
		catch (Exception^ e)
		{
			success = false;
		}
		
		
		return success;
	}

	generic <class T>
	void SqlRepo<T>::removeFiltered(QueryFilter^ filter)
	{
		assert(tableExists());

		String^ sqlQuery = "DELETE FROM " + getTableName() + filter->wcond();

		SqlCommand command(sqlQuery, sqlConnection);
		SqlDataReader^ reader = command.ExecuteReader();

		reader->Close();
	}

}


