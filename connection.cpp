//=======================================================================================
/**
 *  @file    connection.cpp
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#include "connection.h"
#include "statement.h"
#include "exception.h"

namespace mysqlppc {
	Connection::Connection(const string& host, unsigned port,
			const string& uname, const string& passwd, 
			const string& db)
	{
		m_conn = mysql_init(NULL);
		CONN_Throw(m_conn, 
				mysql_real_connect(m_conn, host.c_str(),
					uname.c_str(), passwd.c_str(),
					db.c_str(), port, NULL, 0)
			);
		this->charset("utf8");
	}

	Connection::Connection(const string& sock,
			const string& uname, const string& passwd, 
			const string& db)
	{
		m_conn = mysql_init(NULL);
		CONN_Throw(m_conn, 
				mysql_real_connect(m_conn, NULL,
					uname.c_str(), passwd.c_str(),
					db.c_str(), 0, sock.c_str(), 0)
			);
		this->charset("utf8");
	}

	Connection::~Connection()
	{
		mysql_close(m_conn);
	}	

	Connection::operator bool()
	{
		return (mysql_ping(m_conn) == 0);
	}
	
	Statement* Connection::prepare(const string& sql)
	{
		return new Statement(m_conn, sql);
	}
	
	void Connection::charset(const std::string& cs)
	{
		string sql;
		sql.assign("set NAMES '").append(cs).append("'");
		CONN_Throw(m_conn,
				mysql_real_query(m_conn, sql.c_str(), sql.size()));
	}
}
