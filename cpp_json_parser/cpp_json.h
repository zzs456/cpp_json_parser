#pragma once
#ifndef CPP_JSON_H
#define CPP_JSON_H

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

class CPP_JSON {
	enum class JSON_TYPE
	{
		JSON_NOTTYPE,
		JSON_NULL,
		JSON_BOOL,
		JSON_DOUBLE,
		JSON_STRING,
		JSON_ARRAY,
		JSON_OBJECT
	};
private:
	static int indent_num;
public:
	// ������������
	static void set_Indent(int indent) {
		indent_num = indent;
	}
	// ��ȡ����
	static int get_Indent() {
		return indent_num;
	}
private:

	std::string json_data;
	int data_length;

	JSON_TYPE json_type;										// json ������

	bool json_bool;												// ��� json �� bool ���ͣ���¼ json �� bool ֵ
	double json_double;											// ��� json �� double ���ͣ���¼ json �� double ֵ
	std::string json_str;										// ��� json �� string ���ͣ���¼ json �� string ֵ
	std::vector<CPP_JSON> json_array;							// ��� json �� �������ͣ���¼ json ��������
	std::unordered_map<std::string, CPP_JSON> json_object;		// ��� json �Ƕ������ͣ�string ��¼��ֵ��CPP_JSON ��¼ֵ
public:

	explicit CPP_JSON()
		:json_data(""), data_length(0), json_type(JSON_TYPE::JSON_NOTTYPE), json_bool(false), json_double(0.0) {
	}

	explicit CPP_JSON(bool val)
		:CPP_JSON() {
		json_bool = val;
		json_type = JSON_TYPE::JSON_BOOL;
	}

	explicit CPP_JSON(double val)
		:CPP_JSON() {
		json_double = val;
		json_type = JSON_TYPE::JSON_DOUBLE;
	}

	explicit CPP_JSON(const std::string& val)
		:CPP_JSON() {
		json_str = val;
		json_type = JSON_TYPE::JSON_STRING;
	}

	// ���⹹�캯������¼ null ����
	explicit CPP_JSON(int, int)
		:CPP_JSON() {
		json_type = JSON_TYPE::JSON_NULL;
	}

	explicit CPP_JSON(const std::vector<CPP_JSON>& val)
		:CPP_JSON() {
		json_array = val;
		json_type = JSON_TYPE::JSON_ARRAY;
	}

	explicit CPP_JSON(const std::unordered_map<std::string, CPP_JSON>& val)
		:CPP_JSON() {
		json_object = val;
		json_type = JSON_TYPE::JSON_OBJECT;
	}
	
	void operator=(const std::string&);
	void operator=(double);
	void operator=(bool);
	void operator=(std::vector<CPP_JSON>&);
	void operator=(std::unordered_map<std::string,CPP_JSON>&);
	CPP_JSON& operator=(const CPP_JSON&) = default;

private:

	void init();

	CPP_JSON parser_null(int&) const;

	CPP_JSON parser_bool(int&) const;

	CPP_JSON parser_double(int&) const;

	CPP_JSON parser_string(int&) const;

	CPP_JSON parser_array(int&) const;

	CPP_JSON parser_object(int&) const;

private:

	void jsonArray_toStr(std::stringstream&, int) const;

	void jsonObject_toStr(std::stringstream&, int) const;

	void json_toStr(std::stringstream&, int) const;

public:
	void Parse(const std::string&);

	std::string Str() const;

	friend std::ostream& operator<<(std::ostream&, const CPP_JSON&);

	CPP_JSON& operator[](int);

	CPP_JSON& operator[](const std::string&);
};


#endif // !CPP_JSON_H