* 宏不会产生名称, 所以以常量替换宏
```
const double AspectRatio = 0.653;
```
* 常量字符串指针的定义
```
// 前面的const表示指向的字符串为常量, 第二个const表示指针为常量
const char* const authorName = "Scott Meyers";
```
* 类静态常量不但要有声明式, 还需要有定义式, 其中类内设定初值只针对整数值有效
```
class GamePlayer {
private:
	static const int NumTurns = 4; // 常量声明式
	static const double FudgeFactor; // 静态常量声明
	int scores[NumTurns]; // 使用该常量
};

const int GamePlayer::NumTurns; // 常量定义式
const double GamePlayer::FudgeFactor = 0.35;
```
* 使用 enum hack 定义一个编译期使用的常量值, 其实 enum hack 是一个模板元编程(template metaprogramming).
```
class GamePlayer {
private:
	enum { NumTurns = 4 };
	int scores[NumTruns];
};
```
* enum 和 #define 一样不会导致非必要的内存分配
* 对于宏函数, 用 inline template 函数代替
```
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

// 替换为
template<typename T>
inline void callWithMax(const T& a, const T& b) {
	f(a > b ? a : b);
}
```
