//=======================================================================================
/**
 *  @file    exception.h
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#ifndef _MYSQLPPC_EXCEPTION_H_
#define _MYSQLPPC_EXCEPTION_H_

#include <mysql/mysql.h>
#include <string>
#include <exception>

namespace mysqlppc {
	enum RETCODE
	{
		NORMAL=0,
		ERRORS,
		MEMNOTENOUGH,
		OUTOFRANGE,
	};
	
	class MysqlException: public std::exception {
		public:
			virtual ~MysqlException() throw();
			virtual const char* what() const throw();
			virtual unsigned error() const throw();
			MysqlException(void){}
			MysqlException(int nErrorCode,const char * fmt, ... );

		protected:
			std::string m_msg;
			unsigned m_error;
	};
	
	class ConnectionException: public MysqlException {
		public:
			ConnectionException(MYSQL* conn) throw ();
	};

	class StatementException: public MysqlException {
		public:
			StatementException(MYSQL_STMT* stmt) throw();
	};

	inline void STMT_Throw(MYSQL_STMT* stmt, int ret)
	{
		if (ret != 0) throw StatementException(stmt);
	}
	
	inline void CONN_Throw(MYSQL* conn, MYSQL* ret)
	{
		if (ret == NULL) throw ConnectionException(conn);
	}

	inline void CONN_Throw(MYSQL* conn, int ret)
	{
		if (ret != 0) throw ConnectionException(conn);
	}
};

#endif
