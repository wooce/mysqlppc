//=======================================================================================
/**
 *  @file    result_metadata.h
 *
 *  ver 1.0.0 2013/10/01 mysqlppc,a lightweight C++ wrapper for accessing MySQL database
 *
 *  @author Wooce (Wooce Yang,mailto:wooce@21cn.com) 2013/10/01 created.
 */
//=======================================================================================
#ifndef _MYSQLPPC_RESULTMETADATA_H
#define _MYSQLPPC_RESULTMETADATA_H

#include <mysql/mysql.h>
#include <string>
#include <map>

inline bool is_str(int t)
{
	return (t == MYSQL_TYPE_VAR_STRING || t == MYSQL_TYPE_STRING || t == MYSQL_TYPE_BLOB || t== MYSQL_TYPE_NEWDECIMAL);
}
inline bool is_time(int t)
{
	return (t == MYSQL_TYPE_DATE || t == MYSQL_TYPE_DATETIME || t == MYSQL_TYPE_TIME || t == MYSQL_TYPE_TIMESTAMP);
}

#define safe_free_str(i) \
{ if (is_str(m_data[i].buffer_type) || \
		is_time(m_data[i].buffer_type) ) \
			delete[] m_vals[i].strval; }

namespace mysqlppc
{
	using std::string;
	
class ResultMetadata
	/// MySQL result metadata
{
public:
	ResultMetadata(MYSQL_STMT* stmt);
		/// Initializes the metadata.
	~ResultMetadata();

	unsigned columnsReturned() const{ return m_cols; }
		/// Returns the number of columns in resultset.	

	int getInt(unsigned i);
	long long getLong(unsigned i);
	std::string getString(unsigned i);
	float getFloat(unsigned i);
	
	int getInt(string name);
	long long getLong(string name);
	string getString(string name);
	float getFloat(string name);

	MYSQL_BIND* data() { return m_data; }
	
private:
	void setFieldName(unsigned i,std::string name);
	unsigned getFieldIndex(std::string name);
	
	void makeInt(unsigned i);
	void makeLong(unsigned i);
	void makeString(unsigned i, unsigned size);
	void makeString(unsigned i, unsigned size, int type);
	void makeTime(unsigned i, int type);
	void makeFloat(unsigned i);
	
	MYSQL_BIND* m_data;
	union ValType {
		int ival;
		long long llval;
		char* strval;
		float fval;
	};
	ValType* m_vals;
	unsigned long* m_length;
	my_bool* m_isNull;
	unsigned m_cols;
	std::map<std::string,unsigned> m_fieldNameMap;
};

}

#endif // RESULTMETADATA_H
