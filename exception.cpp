//=======================================================================================
/**
 *  @file    exception.cpp
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#include <stdarg.h>
#include <stdio.h>
#include "exception.h"

namespace mysqlppc {

	MysqlException::~MysqlException() throw()
	{}

	MysqlException::MysqlException(int nErrorCode,const char * fmt, ... )
	{
		const int MAX_ERR_LEN=255;
		char	errorInfo[MAX_ERR_LEN+1];
		va_list va;
		va_start( va, fmt );
		m_error=nErrorCode;
		vsnprintf(errorInfo,MAX_ERR_LEN,fmt,va);
		errorInfo[MAX_ERR_LEN]='\0';
		va_end( va );
		m_msg = errorInfo;
	}

	const char* MysqlException::what() const throw() 
	{
		return m_msg.c_str();
	}
	
	unsigned MysqlException::error() const throw()
	{
		return m_error;
	}

	ConnectionException::ConnectionException(MYSQL* conn) throw()
	{
		m_error = mysql_errno(conn);
		m_msg = mysql_error(conn);
	}

	StatementException::StatementException(MYSQL_STMT* stmt) throw()
	{
		m_error = mysql_stmt_errno(stmt);
		m_msg = mysql_stmt_error(stmt);
	}
};
