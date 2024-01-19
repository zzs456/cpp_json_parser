#### cpp_json_parser

c++ 实现的 json 解析器

**功能**

·将字符串解析为 CPP_JSON 对象

```c++
std::string str = R"(
	{ 
	"null":null,
	"name":"test",
		"number":10.987,
		"array":[
			1.0,
			3.45,
			9.80,
		{ "age":12,"name":"Tom" }	
		],
		"object":{
			"name":"Jack",
			"age":100
			"sex":"man",
	"array":[12,0,true,false,null,[12,{}]]
			}
	}
	)";

	CPP_JSON json1;
	json1.Parse(str);
```

·设置缩进

```c++
CPP_JSON::set_Indent(4);
```

·输出 CPP_JSON 对象

```
std::cout << json1 << std::endl;
```

·访问元素

```c++
std::cout << json1["object"]["array"][5][1] << std::endl;
```

·修改元素

```c++
json1["object"]["array"] = 12.0;
```

