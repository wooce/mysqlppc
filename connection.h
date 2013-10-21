//=======================================================================================
/**
 *  @file    connection.h
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#ifndef _MYSQLPPC_CONNECTION_H_
#define _MYSQLPPC_CONNECTION_H_

#include <mysql/mysql.h>
#include <string>

namespace mysqlppc {
	using std::string;
	class Connection {
		public:
			Connection(const string& host, unsigned port,
					const string& uname, const string& passwd, 
					const string& db);
			Connection(const string& sock,
					const string& uname, const string& passwd, 
					const string& db);

			~Connection();
			operator bool ();

			class Statement* prepare(const string& sql);
			void charset(const std::string& cs);
		private:
			MYSQL* m_conn;
		private:
			Connection(const Connection& other);
			const Connection& operator=(const Connection& other);
	};
}

#endif
