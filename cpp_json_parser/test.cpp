#include "cpp_json.h"
#include <iostream>
#include <string>

int main() {

		std::string str = R"(
	{ 
	"name":"test",
		"number":10.987,
		"array":[
			1.0,
			3.45,
			9.80,
		{ "age":12,"name":"Tom" }	
		],
		"object":{
			"name":"yushi",
			"age":100
			"sex":"man",
	"array":[12,0,true,false,null,[12,{}]]
			}
	}
	)";

	// std::string str = R"({"name":12})";


	CPP_JSON json1;
	json1.Parse(str);

	CPP_JSON::set_Indent(4);

	std::cout << json1 << std::endl;

	// std::cout << json1["object"]["array"][5][1] << std::endl;

	json1["object"]["array"] = 12.0;

	std::cout << json1 << std::endl;
	system("pause");
	return 0;
}