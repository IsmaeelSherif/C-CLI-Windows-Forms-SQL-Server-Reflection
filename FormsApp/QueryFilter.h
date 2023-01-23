#pragma once

using namespace System;

namespace DBRepository {
	

	ref class QueryFilter {

	protected:

		ref class Column;
		ref class FilterResult;
		ref class OrderResult;
		ref class AndOrResult;

		String^ query = "";

	public:
		QueryFilter() {}

		String^ wcond() { return query; };

		


		OrderResult^ orderBy(String^ columnName, bool isDescending);

		Column^ whereColumn(String^ columnName);

		

		ref class Column {

		protected:
			QueryFilter^ filter;

		public:

			Column(QueryFilter^ filtr) {
				filter = filtr;
			}


			FilterResult^ isNull();

			FilterResult^ isNotNull();

			FilterResult^ isEqualTo(Object^ value);

			FilterResult^ isGreaterThan(Object^ value);

			FilterResult^ isGreaterThanOrEqual(Object^ value);

			FilterResult^ isSmallerThan(Object^ value);

			FilterResult^ isSmallerThanOrEqual(Object^ value);
			
		};


		ref class OrderResult {

		protected:
			QueryFilter^ filter;
		public:

			OrderResult(QueryFilter^ filtr) {
				filter = filtr;
			}

			OrderResult^ orderBy(String^ columnName, bool isDescending);

			QueryFilter^ build();
		};



		ref class FilterResult {

		protected:
			QueryFilter^ filter;

		public:

			FilterResult(QueryFilter^ filtr) {
				filter = filtr;
			}

			AndOrResult^and ();

			AndOrResult^or ();

			OrderResult^ orderBy(String^ columnName, bool isDescending);

			QueryFilter^ build();
		};



		ref class AndOrResult {

		protected:
			QueryFilter^ filter;

		public:

			AndOrResult(QueryFilter^ filtr) {
				filter = filtr;
			}

			Column^ whereColumn(String^ columnName);
		};


	};
}
