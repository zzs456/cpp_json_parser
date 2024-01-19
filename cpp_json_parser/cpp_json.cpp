#include "cpp_json.h"
#include <cctype>
#include <iostream>

// 静态变量初始化 缩进数
int CPP_JSON::indent_num = 0;

void CPP_JSON::operator=(const std::string& rhs)
{
	if (json_type == JSON_TYPE::JSON_STRING) {
		json_str = rhs;
		return;
	}
	init();
	json_type = JSON_TYPE::JSON_STRING;
	json_str = rhs;
}

void CPP_JSON::operator=(double rhs)
{
	if (json_type == JSON_TYPE::JSON_DOUBLE) {
		json_double = rhs;
		return;
	}
	init();
	json_type = JSON_TYPE::JSON_DOUBLE;
	json_double = rhs;
}

void CPP_JSON::operator=(bool rhs)
{
	if (json_type == JSON_TYPE::JSON_BOOL) {
		json_bool = rhs;
		return;
	}
	init();
	json_type = JSON_TYPE::JSON_BOOL;
	json_bool = rhs;
}

void CPP_JSON::operator=(std::vector<CPP_JSON>& rhs)
{
	if (json_type == JSON_TYPE::JSON_ARRAY) {
		json_array.clear();
		json_array = rhs;
		return;
	}
	init();
	json_type = JSON_TYPE::JSON_ARRAY;
	json_array.clear();
	json_array = rhs;
}

void CPP_JSON::operator=(std::unordered_map<std::string, CPP_JSON>& rhs)
{
	if (json_type == JSON_TYPE::JSON_OBJECT) {
		json_object.clear();
		json_object = rhs;
		return;
	}
	init();
	json_type = JSON_TYPE::JSON_OBJECT;
	json_object.clear();
	json_object = rhs;
}

void CPP_JSON::init()
{
	json_data = "";
	data_length = 0;
	json_type = JSON_TYPE::JSON_NOTTYPE;
	json_bool = false;
	json_double = 0.0;
	json_array.clear();
	json_object.clear();
}

// 解析 null
CPP_JSON CPP_JSON::parser_null(int& index) const
{
	int i = index;
	// 跳过空格等空字符
	while (i < data_length && std::isspace(json_data[i])) {
		++i;
	}
	if (i >= data_length) throw "parse null value error";
	if (json_data[i] != 'n') throw "parse null value error";
	if (i + 4 > data_length) {
		throw "parse null value error";
	}
	if (json_data[i + 1] == 'u' && json_data[i + 2] == 'l' && json_data[i + 3] == 'l') {
		index = i + 4;
		// 返回 null
		return CPP_JSON(0, 0);
	}
	else {
		throw "parse null value error";
	}
	return CPP_JSON();
}

// 解析 bool
CPP_JSON CPP_JSON::parser_bool(int& index) const
{
	int i = index;
	// 跳过空格等空字符
	while (i < data_length && std::isspace(json_data[i])) {
		++i;
	}
	if (i >= data_length) throw "parse bool value error";
	// 如果不是 t 开头或者 f 开头
	if (json_data[i] != 't' && json_data[i] != 'f') {
		throw "parse bool value error";
	}
	if (json_data[i] == 't') {
		if (i + 4 > data_length) throw "parse bool value error";
		if (json_data[i + 1] == 'r' && json_data[i + 2] == 'u' && json_data[i + 3] == 'e') {
			index = i + 4;
			// 返回 true
			return CPP_JSON(true);
		}
		else {
			throw "parse bool value error";
		}
	}
	else {
		if (i + 5 > data_length) throw "parse bool value error";
		if (json_data[i + 1] == 'a' && json_data[i + 2] == 'l' && json_data[i + 3] == 's'
			&& json_data[i + 4] == 'e') {
			index = i + 5;
			// 返回 false
			return CPP_JSON(false);
		}
		else {
			throw "parse bool value error";
		}
	}
	// return CPP_JSON();
}

// 解析 double
CPP_JSON CPP_JSON::parser_double(int& index) const
{
	// 记录正负号的 bool 变量
	bool double_sign = true;
	int i = index;
	// 跳过空格等空字符
	while (i < data_length && std::isspace(json_data[i])) {
		++i;
	}
	if (i >= data_length) throw "parse number value error";
	if (json_data[i] != '+' && json_data[i] != '-' && !std::isdigit(json_data[i])) {
		throw "parse number value error";
	}
	if (json_data[i] == '+') {
		++i;
	}
	else if (json_data[i] == '-') {
		double_sign = false;
		++i;
	}
	if (i >= data_length) {
		throw "parse number value error";
	}
	if (!std::isdigit(json_data[i])) {
		throw "parse number value error";
	}
	else {
		int begin = i;
		while (i < data_length && std::isdigit(json_data[i])) {
			++i;
		}
		// 有小数点的情况
		if (i < data_length && json_data[i] == '.') {
			++i;
			if (i >= data_length || !std::isdigit(json_data[i])) {
				throw "parse number value error";
			}
			while (i < data_length && std::isdigit(json_data[i])) {
				++i;
			}
		}
		int length = i - begin;
		index = i;
		// 根据 double_sign 判断返回正数还是负数
		if (double_sign) return CPP_JSON(std::stod(json_data.substr(begin, length)));
		else return CPP_JSON(-std::stod(json_data.substr(begin, length)));
	}
}

// 解析 string
CPP_JSON CPP_JSON::parser_string(int& index) const
{
	int i = index;
	while (i < data_length && std::isspace(json_data[i])) {
		++i;
	}
	if (i >= data_length) throw "parse string value error";
	if (json_data[i] != '\"') throw "parse string value error";
	++i;
	int begin = i;
	while (i < data_length && json_data[i] != '\"') {
		++i;
	}
	if (i >= data_length) throw "parse string value error";
	int length = i - begin;
	++i;
	index = i;
	return CPP_JSON(json_data.substr(begin, length));
}

// 解析数组
CPP_JSON CPP_JSON::parser_array(int& index) const
{
	std::vector<CPP_JSON> vs;
	int i = index;
	while (i < data_length && std::isspace(json_data[i])) {
		++i;
	}
	if (i >= data_length) throw "parse array value error";
	if (json_data[i] != '[') throw "parse array value error";
	++i;
	bool new_elem = true;
	bool is_first = true;
	while (i < data_length) {
		if (std::isspace(json_data[i])) {
			++i;
			continue;
		}
		if (json_data[i] == ',') {
			if (new_elem) {
				throw "parse array value error";
			}
			else {
				is_first = false;
				new_elem = true;
				++i;
				continue;
			}
		}
		if (json_data[i] == 'n') {
			if (new_elem) {
				new_elem = false;
				vs.push_back(parser_null(i));
				continue;
			}
			else {
				throw "parse array value error";
			}
		}
		if (json_data[i] == 't' || json_data[i] == 'f') {
			if (new_elem) {
				new_elem = false;
				vs.push_back(parser_bool(i));
				continue;
			}
			else {
				throw "parse array value error";
			}
		}
		if (json_data[i] == '+' || json_data[i] == '-' || std::isdigit(json_data[i])) {
			if (new_elem) {
				new_elem = false;
				vs.push_back(parser_double(i));
				continue;
			}
			else {
				throw "parse array value error";
			}
		}
		if (json_data[i] == '\"') {
			if (new_elem) {
				new_elem = false;
				vs.push_back(parser_string(i));
				continue;
			}
			else {
				throw "parse array value error";
			}
		}
		if (json_data[i] == '[') {
			if (new_elem) {
				new_elem = false;
				vs.push_back(parser_array(i));
				continue;
			}
			else {
				throw "parse array value error";
			}
		}
		if (json_data[i] == ']') {
			if (new_elem && !is_first) throw "parse array value error";
			++i;
			index = i;
			return CPP_JSON(vs);
		}
		if (json_data[i] == '{') {
			if (new_elem) {
				new_elem = false;
				vs.push_back(parser_object(i));
				continue;
			}
			else {
				throw "parse array value error";
			}
		}
		throw "parse array value error";
	}
}

// 解析对象
CPP_JSON CPP_JSON::parser_object(int& index) const
{
	std::unordered_map<std::string, CPP_JSON> ums;
	int i = index;
	while (i < data_length && std::isspace(json_data[i])) {
		++i;
	}
	if (i >= data_length) throw "parse object value error";
	if (json_data[i] != '{') throw "parse object value error";
	++i;
	bool new_elem = true;
	bool is_first = true;
	while (i < data_length) {
		if (std::isspace(json_data[i])) {
			++i;
			continue;
		}
		if (json_data[i] == ',') {
			if (new_elem) {
				throw "parse array value error";
			}
			else {
				is_first = false;
				new_elem = true;
				++i;
				continue;
			}
		}
		if (json_data[i] == '\"') {
			new_elem = false;
			++i;
			int key_begin = i;
			while (i < data_length && json_data[i] != '\"') {
				++i;
			}
			if (i == data_length) throw "parse object value error";
			int length = i - key_begin;
			std::string key = json_data.substr(key_begin, length);
			++i;
			while (i < data_length && json_data[i] != ':') {
				++i;
			}
			if (i == data_length) throw "parse object value error";
			++i;
			while (i < data_length && std::isspace(json_data[i])) {
				++i;
			}
			if (json_data[i] == 'n') {
				ums.insert({ key , parser_null(i) });
				continue;
			}
			if (json_data[i] == 't' || json_data[i] == 'f') {
				ums.insert({ key,parser_bool(i) });
				continue;
			}
			if (json_data[i] == '+' || json_data[i] == '-' || std::isdigit(json_data[i])) {
				ums.insert({ key,parser_double(i) });
				continue;
			}
			if (json_data[i] == '\"') {
				ums.insert({ key,parser_string(i) });
				continue;
			}
			if (json_data[i] == '[') {
				ums.insert({ key,parser_array(i) });
				continue;
			}
			if (json_data[i] == '{') {
				ums.insert({ key,parser_object(i) });
				continue;
			}
			throw "parse object value error";
		}
		if (json_data[i] == '}') {
			if (new_elem && !is_first) throw "parse object value error";
			++i;
			index = i;
			return CPP_JSON(ums);
		}
	}
}

// 解析字符串
void CPP_JSON::Parse(const std::string& str)
{
	init();
	json_data = str;
	data_length = str.size();
	int begin = 0;
	while (begin < data_length && std::isspace(json_data[begin])) {
		++begin;
	}
	if (begin == data_length) {
		std::cout << "解析失败，格式存在问题!" << std::endl;
		return;
	}
	if (json_data[begin] == 'n') {
		json_type = JSON_TYPE::JSON_NULL;
		parser_null(begin);
	}
	else if (json_data[begin] == 't' || json_data[begin] == 'f') {
		json_type = JSON_TYPE::JSON_BOOL;
		json_bool = parser_bool(begin).json_bool;
	}
	else if (json_data[begin] == '+' || json_data[begin] == '-' || std::isdigit(json_data[begin])) {
		json_type = JSON_TYPE::JSON_DOUBLE;
		json_double = parser_double(begin).json_double;
	}
	else if (json_data[begin] == '\"') {
		json_type = JSON_TYPE::JSON_STRING;
		json_str = parser_string(begin).json_str;
	}
	else if (json_data[begin] == '[') {
		json_type = JSON_TYPE::JSON_ARRAY;
		json_array = parser_array(begin).json_array;
	}
	else if (json_data[begin] == '{') {
		json_type = JSON_TYPE::JSON_OBJECT;
		json_object = parser_object(begin).json_object;
	}
	else {
		throw "parse init string value error";
	}
}

void CPP_JSON::jsonArray_toStr(std::stringstream& out, int depth) const
{
	// 如果没有缩进，则不进行换行
	if (!indent_num) {
		out << "[";
	}
	else {
		out << "[\n";
	}
	std::string spaces((depth + 1) * indent_num, ' ');
	int elem_nums = json_array.size();
	int index = 1;
	for (const auto& elem : json_array) {
		out << spaces;
		elem.json_toStr(out, depth + 1);
		if (index != elem_nums) {
			if (!indent_num) {
				out << ",";
			}
			else {
				out << ",\n";
			}
		}
		else {
			if (indent_num) {
				out << "\n";
			}
		}
		++index;
	}
	spaces.resize(depth * indent_num);
	out << spaces;
	out << "]";
}

void CPP_JSON::jsonObject_toStr(std::stringstream& out, int depth) const
{
	// 如果没有缩进，则不进行换行
	if (!indent_num) {
		out << "{";
	}
	else {
		out << "{\n";
	}
	std::string spaces((depth + 1) * indent_num, ' ');
	int elem_nums = json_object.size();
	int index = 1;
	for (const auto& elem : json_object) {
		out << spaces;
		out << "\"" << elem.first << "\"" << ":";
		elem.second.json_toStr(out, depth + 1);
		if (index != elem_nums) {
			if (!indent_num) {
				out << ",";
			}
			else {
				out << ",\n";
			}
		}
		else {
			if (indent_num) {
				out << "\n";
			}
		}
		++index;
	}
	spaces.resize(depth * indent_num);
	out << spaces;
	out << "}";
}

// JSON 输出为字符串
void CPP_JSON::json_toStr(std::stringstream& out, int depth) const {
	switch (json_type) {
	case JSON_TYPE::JSON_NULL:
		out << "null";
		break;
	case JSON_TYPE::JSON_BOOL:
		if (json_bool) {
			out << "true";
		}
		else {
			out << "false";
		}
		break;
	case JSON_TYPE::JSON_DOUBLE:
		out << json_double;
		break;
	case JSON_TYPE::JSON_STRING:
		out << "\"" << json_str << "\"";
		break;
	case JSON_TYPE::JSON_ARRAY:
		jsonArray_toStr(out, depth);
		break;
	case JSON_TYPE::JSON_OBJECT:
		jsonObject_toStr(out,depth);
		break;
	default:
		std::cout << "json error" << std::endl;
		break;
	}
}

// 公开的接口
std::string CPP_JSON::Str() const
{
	std::stringstream out;
	json_toStr(out, 0);
	return out.str();
}

CPP_JSON& CPP_JSON::operator[](int index)
{
	if (json_type != JSON_TYPE::JSON_ARRAY) {
		throw "type error";
	}
	if (index < 0) {
		throw "index is invalid";
	}
	if (index > json_array.size()) {
		throw "out of range";
	}
	return json_array[index];
}

CPP_JSON& CPP_JSON::operator[](const std::string& key)
{
	if (json_type != JSON_TYPE::JSON_OBJECT) {
		throw "type error";
	}
	if (!json_object.count(key)) {
		throw "key not exit";
	}
	return json_object[key];
}

// 重载 << 运算符
std::ostream& operator<<(std::ostream& cout, const CPP_JSON& json)
{
	cout << json.Str();
	return cout;
}
