//=======================================================================================
/**
 *  @file    binder.cpp
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#include <cstdio>
#include <string.h>
#include "binder.h"
#include "result_metadata.h"
#include "exception.h"

namespace mysqlppc {
	Binder::Binder(std::size_t size)
	{
		m_nSize = size;

		m_data = new MYSQL_BIND[m_nSize];
		m_vals = new ValType[m_nSize];
		
		for (unsigned i = 0; i < m_nSize; ++i)
		{
			m_data[i] = {0};
			m_data[i].buffer_type = MYSQL_TYPE_NULL;
			m_data[i].buffer_length = 0;
			m_data[i].buffer = NULL;
		}
	}

	Binder::~Binder()
	{
		for (unsigned i = 0; i < m_nSize; ++i)
		{
			safe_free_str(i);
		}
		delete[] m_vals;
		delete[] m_data;
	}

	void Binder::bind(std::size_t pos, const int& val)
	{
		if (pos >= m_nSize)
			throw(MysqlException(OUTOFRANGE,"bind pos [%d]:out of range",pos));
		m_data[pos].buffer_type = MYSQL_TYPE_LONG;
		m_data[pos].buffer = & m_vals[pos].ival;
		m_vals[pos].ival = val;
	}
	
	void Binder::bind(std::size_t pos, const long long& val)
	{
		if (pos >= m_nSize)
			throw(MysqlException(OUTOFRANGE,"bind pos [%d]:out of range",pos));
		m_data[pos].buffer_type = MYSQL_TYPE_LONGLONG;
		m_data[pos].buffer = & m_vals[pos].llval;
		m_vals[pos].llval = val;
	}
	
	void Binder::bind(std::size_t pos, const string& val)
	{
		if (pos >= m_nSize)
			throw(MysqlException(OUTOFRANGE,"bind pos [%d]:out of range",pos));
		m_data[pos].buffer_type = MYSQL_TYPE_STRING;
		m_data[pos].buffer = m_vals[pos].strval = new char[val.size()];
		m_data[pos].buffer_length = val.size();
		memcpy(m_vals[pos].strval, val.c_str(), val.size());
	}

	void Binder::bind(std::size_t pos, const float& val)
	{
		if (pos >= m_nSize)
			throw(MysqlException(OUTOFRANGE,"bind pos [%d]:out of range",pos));
		m_data[pos].buffer_type = MYSQL_TYPE_FLOAT;
		m_data[pos].buffer = & m_vals[pos].fval;
		m_vals[pos].fval = val;
	}
};
