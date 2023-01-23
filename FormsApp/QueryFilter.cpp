#include "pch.h"
#include "QueryFilter.h"

namespace DBRepository {
    
    
    QueryFilter::OrderResult^ QueryFilter::orderBy(String^ columnName, bool isDescending)
    {
        String^ ascDesc = isDescending ? " DESC" : " ASC";
        query = " ORDER BY " + columnName + ascDesc;
        return gcnew OrderResult(this);
    }

    QueryFilter::Column^ QueryFilter::whereColumn(String^ columnName)
    {
        query = " WHERE ";
        query += columnName->ToLower();
        return gcnew Column(this);
    }



    
    QueryFilter::FilterResult^ QueryFilter::Column::isNull()
    {
        filter->query += " IS NULL";
        return gcnew FilterResult(filter);
    }

    QueryFilter::FilterResult^ QueryFilter::Column::isNotNull()
    {
        filter->query += " IS NOT NULL";
        return gcnew FilterResult(filter);
    }

    QueryFilter::FilterResult^ QueryFilter::Column::isEqualTo(Object^ value)
    {
        Type^ dataType = value->GetType();
        if (dataType->Name == "String") {
            value = "'" + value + "'";
        }
        else if (dataType->Name == "Boolean") {
            value = value->Equals(true) ? "1" : "0";
        }

        filter->query += " = " + value;
        return gcnew FilterResult(filter);
    }

    QueryFilter::FilterResult^ QueryFilter::Column::isGreaterThan(Object^ value)
    {
        filter->query += " > " + value;
        return gcnew FilterResult(filter);
    }

    QueryFilter::FilterResult^ QueryFilter::Column::isGreaterThanOrEqual(Object^ value)
    {
        filter->query += " >= " + value;
        return gcnew FilterResult(filter);
    }

    QueryFilter::FilterResult^ QueryFilter::Column::isSmallerThan(Object^ value)
    {
        filter->query += " < " + value;
        return gcnew FilterResult(filter);
    }

    QueryFilter::FilterResult^ QueryFilter::Column::isSmallerThanOrEqual(Object^ value)
    {
        filter->query += " <= " + value;
        return gcnew FilterResult(filter);
    }




    QueryFilter::AndOrResult^ QueryFilter::FilterResult::and ()
    {
        filter->query += " AND ";
        return gcnew AndOrResult(filter);
    }

    QueryFilter::AndOrResult^ QueryFilter::FilterResult:: or ()
    {
        filter->query += " OR ";
        return gcnew AndOrResult(filter);
    }

    QueryFilter::OrderResult^ QueryFilter::FilterResult::orderBy(String^ columnName, bool isDescending)
    {
        String^ ascDesc = isDescending ? " DESC" : " ASC";
        filter->query += " ORDER BY " + columnName + ascDesc;
        return gcnew OrderResult(filter);
    }



    QueryFilter::OrderResult^ QueryFilter::OrderResult::orderBy(String^ columnName, bool isDescending)
    {
        String^ ascDesc = isDescending ? " DESC" : " ASC";
        filter->query += ", " + columnName + ascDesc;
        return this;
    }

    QueryFilter^ QueryFilter::OrderResult::build()
    {
        return filter;
    }


    QueryFilter::Column^ QueryFilter::AndOrResult::whereColumn(String^ columnName)
    {
        filter->query += columnName->ToLower();
        return gcnew Column(filter);
    }

    QueryFilter^ QueryFilter::FilterResult::build()
    {
        return filter;
    }

}
