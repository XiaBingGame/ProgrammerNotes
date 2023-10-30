# 条款02 尽量以 const, enum, inline 替换 #define
* [条款02](ch02/README.md)


# 条款03: 尽可能使用 const
* [条款03](ch03/README.md)


# 条款04: 确定对象被使用前已先被初始化
* 确保每一个构造函数都将对象的每一个成员初始化 
* 基类的成员初始化早于派生类的成员初始化,类的成员变量总是以其声明顺序被初始化
* 函数内的 static 对象称为 local static, 其他 static 对象则为 non-local static
* 创建自身的局部 static 对象, 用函数返回值代替静态全局变量
{{{
class FileSystem { ... };
FileSystem& tfs()
{
	static FileSystem fs;
	return fs;
}
}}}
* 为了确保多线程确定性,在单线程启动阶段手工调用这些引用返回函数.

# 条款05: 了解C++默默编写并调用哪些函数
* 所有编译器生成的函数都是 public 和 inline 的, 编译器生成的析构函数为 non-virtual 的
* 如果基类的拷贝赋值构造函数为 private, 则派生类不会自动生成拷贝赋值构造函数.

# 条款06: 若不想使用编译器自动生成的函数,就该明确拒绝
* 将拷贝构造函数和拷贝赋值构造函数设置为 private 且不定义它, 可以阻止人们调用它们.
* 创建一个基类, 其私有化拷贝构造函数和拷贝赋值构造函数, 且不定义这些函数, 可实现在编译期间就阻止调用这些拷贝函数
{{{c++
class Uncopyable {
protected:
	Uncopyable() {}
	~Uncopyable() {}
	
private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);
};
}}}

# 条款07: 为多态基类声明 virtual 析构函数
* 只有当 class 内含至少一个 virtual 函数, 才为它声明 virtual 析构函数.
* 可以实现一个纯虚析构函数, 同时定义该纯虚析构函数. 这样可以实现一个抽象类.

# 条款08: 别让异常逃离析构函数
