# 第1章 C++和标准库速成
* 最简单的代码
{{{c++
import <iostream>;

int main(int argc, char** argv)
{
	std::cout << "Hello, World!" << std::endl;
	return 0;
}
}}}
* C 中的标准库头文件在C++中依然存在, 但使用以下两个版本
    - 不使用 .h 后缀, 改用前缀 c, 这是推荐使用的版本, 其所有内容放在 std 名称空间内(如 <cstdio>>)
    - 使用 .h 后缀, 这是旧版本, 不使用名称空间(如 <stdio.h>)
* 从 C++20 开始, 字符串格式化推荐使用 std::format() 输出, 定义在 <format> 中
```
std::cout << std::format("There are {} way I love you.", 219) << std::endl;
```
* 不建议过度使用 endl, 如在循环中使用.
* printf() 和 scanf() 未提供类型安全, 建议使用 format() 和流库.