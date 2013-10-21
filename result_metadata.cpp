//=======================================================================================
/**
 *  @file    result_metadata.cpp
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#include <stdio.h>
#include "exception.h"
#include "result_metadata.h"

namespace mysqlppc
{
	class ResultMetadataHandle
		/// Simple exception-safe wrapper
	{
	public:

		explicit ResultMetadataHandle(MYSQL_STMT* stmt)
		{
			h = mysql_stmt_result_metadata(stmt);
		}

		~ResultMetadataHandle()
		{
			if (h)
			{
				mysql_free_result(h);
			}
		}

		operator MYSQL_RES* ()
		{
			return h;
		}

	private:

		MYSQL_RES* h;
	};
	
	ResultMetadata::~ResultMetadata()
	{
		for (unsigned i = 0; i < m_cols; ++i)
		{
			safe_free_str(i);
		}
		if(m_length!=NULL)
			delete[] m_length;
		if(m_vals!=NULL)
			delete[] m_vals;
		if(m_isNull!=NULL)
			delete[] m_isNull;
		if(m_data!=NULL)
			delete[] m_data;
	}

	ResultMetadata::ResultMetadata(MYSQL_STMT* stmt):m_data(NULL),m_vals(NULL),m_length(NULL),m_isNull(NULL),m_cols(0)
	{
		ResultMetadataHandle h(stmt);

		if (!h)
		{
			// all right, it is normal
			// querys such an "INSERT INTO" just does not have result at all
			return;
		}

		size_t count = mysql_num_fields(h);
		MYSQL_FIELD* fields = mysql_fetch_fields(h);

		m_cols = count;
		m_length = new unsigned long[m_cols];
		m_vals = new ValType[m_cols];
		m_isNull = new my_bool[m_cols];

		m_data = new MYSQL_BIND[m_cols];
		

		::std::fill(m_length, m_length+m_cols, 0);
		::std::fill(m_isNull, m_isNull+m_cols, 0);

		for (unsigned i = 0; i < m_cols; ++i)
		{
			m_data[i].buffer_type = MYSQL_TYPE_NULL;
			m_data[i].buffer_length = 0;
			m_data[i].buffer = NULL;

			m_data[i].length = &m_length[i];
			m_data[i].is_null = &m_isNull[i];
			m_data[i].is_unsigned = 0;
			m_data[i].error = NULL;
		}
		
		for (size_t i = 0; i < count; i++)
		{
			switch(fields[i].type)
			{
				case MYSQL_TYPE_TINY:
				case MYSQL_TYPE_SHORT:
				case MYSQL_TYPE_INT24:
				case MYSQL_TYPE_YEAR:
				case MYSQL_TYPE_LONG:
					makeInt(i);
					break;
				case MYSQL_TYPE_LONGLONG:
					makeLong(i);
					break;
				case MYSQL_TYPE_NEWDECIMAL:
				case MYSQL_TYPE_VAR_STRING:
				case MYSQL_TYPE_STRING:
				case MYSQL_TYPE_BLOB:
					makeString(i, fields[i].length, fields[i].type);
					break;
				case MYSQL_TYPE_DATE:
				case MYSQL_TYPE_DATETIME:
				case MYSQL_TYPE_TIME:
				case MYSQL_TYPE_TIMESTAMP:
					makeTime(i, fields[i].type);
					break;
				case MYSQL_TYPE_FLOAT:
					makeFloat(i);					
				default:
					break;
			}
			setFieldName(i,fields[i].name);
		}
		
	}

	void ResultMetadata::setFieldName(unsigned i,std::string name)
	{
		m_fieldNameMap.insert(std::pair<std::string,int>(name,(int)i));
	}

	void ResultMetadata::makeInt(unsigned i)
	{
		safe_free_str(i);
		m_data[i].buffer_type = MYSQL_TYPE_LONG;
		m_data[i].buffer = & m_vals[i].ival;
		m_length[i] = m_data[i].buffer_length = sizeof(int);
	}
	
	void ResultMetadata::makeFloat(unsigned i)
	{
		safe_free_str(i);
		m_data[i].buffer_type = MYSQL_TYPE_FLOAT;
		m_data[i].buffer = & m_vals[i].fval;
		m_length[i] = m_data[i].buffer_length = sizeof(float);
	}
	
	void ResultMetadata::makeLong(unsigned i)
	{
		safe_free_str(i);
		m_data[i].buffer_type = MYSQL_TYPE_LONGLONG;
		m_data[i].buffer = & m_vals[i].llval;
		m_length[i] = m_data[i].buffer_length = sizeof(long long);
	}
	
	void ResultMetadata::makeTime(unsigned i, int type)
	{
		safe_free_str(i);
		m_data[i].buffer_type = (enum_field_types)type;
		m_data[i].buffer = m_vals[i].strval = new char [sizeof(MYSQL_TIME)];
		m_length[i] = m_data[i].buffer_length = sizeof(MYSQL_TIME);
	}
	
	void ResultMetadata::makeString(unsigned i, unsigned size)
	{
		makeString(i, size, MYSQL_TYPE_STRING);
	}

	void ResultMetadata::makeString(unsigned i, unsigned size, int type)
	{
		safe_free_str(i);

		m_data[i].buffer_type = (enum_field_types)type;
		m_data[i].buffer = m_vals[i].strval = new char[size];
		m_length[i] = m_data[i].buffer_length = size;
	}

	int ResultMetadata::getInt(unsigned i)
	{
		if(i>=m_cols)
			throw(MysqlException(OUTOFRANGE,"Resultset access[%d]:out of range",i));
		return m_isNull[i] ? 0: m_vals[i].ival;
	}

	float ResultMetadata::getFloat(unsigned i)
	{
		if(i>=m_cols)
			throw(MysqlException(OUTOFRANGE,"Resultset access[%d]:out of range",i));
		return m_isNull[i] ? 0: m_vals[i].fval;
	}

	long long ResultMetadata::getLong(unsigned i)
	{
		if(i>=m_cols)
			throw(MysqlException(OUTOFRANGE,"Resultset access[%d]:out of range",i));
		return m_isNull[i] ? 0: m_vals[i].llval;
	}
	
	string ResultMetadata::getString(unsigned i)
	{
		if(i>=m_cols)
			throw(MysqlException(OUTOFRANGE,"Resultset access[%d]:out of range",i));
		char buf[32];

		if (m_isNull[i]) return string();
		if (is_str(m_data[i].buffer_type))
			return string(m_vals[i].strval, m_length[i]);

		buf[0] = 0;
		if (is_time(m_data[i].buffer_type))
		{
			MYSQL_TIME* t = (MYSQL_TIME*)m_vals[i].strval;
			if (m_data[i].buffer_type == MYSQL_TYPE_TIME)
				snprintf(buf, 32, "%02d:%02d:%02d", t->hour, t->minute, t->second);
			else if (m_data[i].buffer_type == MYSQL_TYPE_DATE)
				snprintf(buf, 32, "%04d-%02d-%02d", t->year, t->month, t->day);
			else
				snprintf(buf, 32, "%04d-%02d-%02d %02d:%02d:%02d", t->year, t->month, t->day,
						t->hour, t->minute, t->second);
		}
		if (m_data[i].buffer_type == MYSQL_TYPE_LONG)
			snprintf(buf, 32, "%d", getInt(i));
		if (m_data[i].buffer_type == MYSQL_TYPE_LONGLONG)
			snprintf(buf, 32, "%lld", getLong(i));
		return string(buf);
	}

	unsigned ResultMetadata::getFieldIndex(std::string name)
	{
		std::map<std::string,unsigned>::iterator itor = m_fieldNameMap.find(name);
		if( itor==m_fieldNameMap.end() )
			throw(MysqlException(OUTOFRANGE,"Resultset access[%s]:no this column name",name.c_str())); 
		return itor->second;	
	}

	int ResultMetadata::getInt(string name)
	{
		return getInt(getFieldIndex(name));
	}

	long long ResultMetadata::getLong(string name)
	{
		return getLong(getFieldIndex(name));
	}

	string ResultMetadata::getString(string name)
	{
		return getString(getFieldIndex(name));
	}

	float ResultMetadata::getFloat(string name)
	{
		return getFloat(getFieldIndex(name));
	}

}


