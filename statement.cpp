//=======================================================================================
/**
 *  @file    statement.cpp
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#include "statement.h"
#include "binder.h"
#include "resultset.h"
#include "exception.h"

namespace mysqlppc {
	Statement::Statement(MYSQL* conn, const string& sql)
	{
		m_conn = conn;
		m_stmt = mysql_stmt_init(m_conn);
		if(!m_stmt)
			throw(MysqlException(ERRORS,"mysql_stmt_init error"));
		m_state = STMT_INITED;
		m_binder = NULL;

		STMT_Throw(m_stmt,
				mysql_stmt_prepare(m_stmt, sql.data(), sql.size())
			);
		int pc = mysql_stmt_param_count(m_stmt);
		if (pc > 0)
			m_binder = new Binder(pc);
		m_state = STMT_COMPILED;
	}

	Statement::~Statement()
	{
		mysql_stmt_close(m_stmt);
		if(m_binder!=NULL)
			delete m_binder;
	}

	void Statement::bindIntParam(unsigned i, int val)
	{
		if(m_binder==NULL)
			throw(MysqlException(ERRORS,"no param to bind"));
		m_binder->bind(i, val);
	}

	void Statement::bindFloatParam(unsigned i, float val)
	{
		if(m_binder==NULL)
			throw(MysqlException(ERRORS,"no param to bind"));
		m_binder->bind(i, val);
	}

	void Statement::bindLongParam(unsigned i, long long val)
	{
		if(m_binder==NULL)
			throw(MysqlException(ERRORS,"no param to bind"));
		m_binder->bind(i, val);
	}

	void Statement::bindStrParam(unsigned i, const string& val)
	{
		if(m_binder==NULL)
			throw(MysqlException(ERRORS,"no param to bind"));
		m_binder->bind(i, val);
	}

	void Statement::execute()
	{
		if (m_binder != NULL)
		{
			STMT_Throw(m_stmt, 
					mysql_stmt_bind_param(m_stmt, m_binder->data())
				);
		}
		STMT_Throw(m_stmt,
				mysql_stmt_execute(m_stmt)
			);
		m_state = STMT_EXECUTED;
	}
	
	long long Statement::lastId()
	{
		return mysql_stmt_insert_id(m_stmt);
	}

	int Statement::affectedRows()
	{
		return mysql_stmt_affected_rows(m_stmt);
	}
	
	ResultSet* Statement::getResultSet()
	{
		if(m_state!=STMT_EXECUTED)
			throw(MysqlException(ERRORS,"statement has not been executed!"));
		return new ResultSet(m_stmt);
	}
};
