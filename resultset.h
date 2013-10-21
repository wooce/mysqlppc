//=======================================================================================
/**
 *  @file    resultset.h
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#ifndef _MYSQLPPC_RESULTSET_H_
#define _MYSQLPPC_RESULTSET_H_

#include <mysql/mysql.h>
#include <string>
#include "result_metadata.h"

namespace mysqlppc {
	using std::string;
	class ResultSet {
		public:
			ResultSet(MYSQL_STMT* stmt);
			~ResultSet();

			unsigned getColumnCount() { return m_metadata->columnsReturned(); }
			int getRowCount() { return m_rows; }
			bool next();

			int getInt(unsigned columnIndex);
			long long getLong(unsigned columnIndex);
			string getString(unsigned columnIndex);
			float getFloat(unsigned columnIndex);
            		int getInt(string name);
            		long long getLong(string name);
            		string getString(string name);
            		float getFloat(string name);
		private:
			MYSQL_STMT* m_stmt;
			class ResultMetadata* m_metadata;
		   	int m_rows;
	};
}

#endif
