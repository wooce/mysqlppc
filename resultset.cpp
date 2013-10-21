//=======================================================================================
/**
 *  @file    resultset.cpp
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#include "resultset.h"
#include "result_metadata.h"
#include "exception.h"

namespace mysqlppc {

	ResultSet::ResultSet(MYSQL_STMT* stmt)
	{
		m_stmt = stmt;
		
		m_metadata = new ResultMetadata(stmt);
		STMT_Throw(m_stmt, 
				mysql_stmt_bind_result(m_stmt, m_metadata->data())
			);
		STMT_Throw(m_stmt,
				mysql_stmt_store_result(m_stmt)
			);
		m_rows = mysql_stmt_affected_rows(m_stmt);
	}

	ResultSet::~ResultSet()
	{
		if (m_metadata != NULL)
			delete m_metadata;
	}

	bool ResultSet::next()
	{
		if(m_rows==0)
			return false;
		switch(mysql_stmt_fetch(m_stmt))
		{
			case 0:
				return true;
			case MYSQL_NO_DATA:
				return false;
			default:
				STMT_Throw(m_stmt, 1);
				break;
		}
		return false;
	}

	int ResultSet::getInt(unsigned i)
	{
		return m_metadata->getInt(i);
	}

	long long ResultSet::getLong(unsigned i)
	{
		return m_metadata->getLong(i);
	}

	string ResultSet::getString(unsigned i)
	{
		return m_metadata->getString(i);
	}
	
	float ResultSet::getFloat(unsigned i)
	{
		return m_metadata->getFloat(i);
	}	

	int ResultSet::getInt(string name)
	{
		return m_metadata->getInt(name);
	}

	long long ResultSet::getLong(string name)
	{
		return m_metadata->getLong(name);
	}

	string ResultSet::getString(string name)
	{
		return m_metadata->getString(name);
	}

	float ResultSet::getFloat(string name)
	{
		return m_metadata->getFloat(name);
	}	

};
