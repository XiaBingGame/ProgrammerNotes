* const 如果位于星号前面, 表示被指物是常量, 如果位于星号后面, 则表示指针是常量
* const 放在类型前和类型后的意义都是一样的
```
void f1(const Widget* pw);
void f1(Widget const* pw);
```
* const iterator 表示指针为常量, const iterator 表示指针所指向的内容为常量
```
std::vector<int> vec;
// ...
const std::vector<int>::iterator iter = vec.begin();
*iter = 10;     // 正确, iter 所指向的内容不为常量
++iter;         // 错误, 不能改变指针
std::vector<int>::const_iterator cIter = vec.begin();
*cIter = 10;    // 错误, cIter 所指向的内容为常量
++cIter;        // 正确, 指针可以改变
```
* 返回常量值可以避免被改变
```
class Rational;
const Rational operator*(const Rational& lhs, const Rational& rhs);

Rational a, b, c;
// ...
(a * b) = c;  // 错误, a * b 的返回值为常量.
```
* const 方法做用于 const 对象
* 如果一个函数的返回类型是一个内置类型, 则不能改动该函数的返回值.
* 方法可以有常量和非常量版本, 非常量版本调用常量版本的同名成员函数
{{{c++
char& operator[](std::size_t position)
{
	return const_cast<char&>(static_cast<const TextBlock&>(*this)[position]);
}
}}}