# 第1章 重构，第一个案例
* 概括：本章讲述了 Extract Method、Move Method、Replace Temp with Query、Replace Type Code with State/Stragy 的几个例子。
* 重构之前要做好测试工作
* Extract Method --- 将复杂的代码段包装成一个方法。
* 给变量和函数重命名，使代码更清晰。
* Move Mthod --- 将与某个类密切相关的代码包装为该类的方法。
* Replace Temp with Query --- 临时变量尽量使用查询相关的方法代替。
* 不要将另外一个类对象的成员数据或成员方法作为一个 switch 语句的条件。
* Replace Type Code with State/Stragy: 主要在设置条件码的方法内创建派生类的对象
{{{c++
	void setPriceCode(int arg) {
		switch (arg):
			case REGULAR:
				_price # new RegularPrice();
				break;
			case CHILDRENS:
				_price # new ChildrenPrice();
				break;
			...
	}
}}}
* 使用多态代替 switch/case 语句。

# 第2章 重构原则
* 重构在不改变软件功能的前提下, 增加代码易读性和维护性.
* 两顶帽子:增加新功能和重构.
* 事不过三, 三则重构
* 增加间接层:将大型对象拆成多个小型对象, 大型函数拆成多个小函数. 间接层的价值:
	* 逻辑共享: 也就是函数和方法复用.
	* 通过类名和函数名解释要做的功能, 内部则实现这些功能.
	* 通过派生类重写函数隔离变化的部分.
	* 多态实现条件逻辑.
* 编写工具检测程序运行耗时耗性能的代码部分.

# 第3章 代码的坏味道
* 形成自己的重构直觉判断能力, 判断类的成员是否过多, 函数代码是否过长
* 重复代码
	- Extract Method 提炼出相同的代码
	- Extract Method / Pull Up Method: 构造超类共有函数
	- Template Method: 分开代码相同和不同的地方
	- Substitue Algorithm: 实现相同代码逻辑中一些调用了不同算法部分的重构.
	- 两个不相关的类包含相同的代码, 单独在一个类的方法内保存该相同代码.
* 过长的函数
	- 不同的逻辑部分提取为函数, 注意函数名称要易懂且有意义.
	- Replace Temp with Query 消除临时变量
	- Introduce Parameter Object, Preserve Whole Object 减少参数.
	- Replace Method with Method Object
	- Decompose Condition
	- 当一个地方含有注释时, 表示应该抽取为一个函数或方法.
* 过大的类
	- Extract Class / Extract subclass: 抽取出类或子类.
	- 对于 GUI 类, 数据和行为移动到一个独立的类中. Duplicate Observed Data 保持两边的重复数据同步.
* 过长的参数列表
	- Replace Parameter with Method: 通过一个对象的方法获取具体的参数, 对象可以是函数本身所属类对象,或者函数参数所指的对象
	- Preserve Whole Object: 来自同一对象的数据替换为该对象.
	- Introduce Parameter Object: 构造一个参数对象
* 功能的每一次变化, 都只能发生在一个类内.
* 一个类受多种功能变化的影响(Divergent Change: 发散式变化)
* 如果功能的一次修改, 在许多类里进行(Shotgun Surgery: 霰弹式修改), 抽出变化部分放到同一个类里.
	- Move Method / Move Field / Inline Class
* Feature Envy(依恋情节): 某个函数或函数的一部分对另外一个类对象过度依赖, 将该函数或该函数的一部分移动到该类处.
	- 一些设计模式除外, 如 Strategy / Visitor
* Data Clumps(数据泥团): 多个类中相同的数据成员, 多个函数中相同的参数, 将它们提取到一个类中.
	- Extract Class / Introduce Parameter Object / Preserve Whole Object
* Primitive Obsession(基本类型偏执): 不需要对基本类型的偏执. 尽量包装基本类型为新的类型
	- Replace Data Value With Object / Replace Type Code with Class / Replace Type Code with Subclass / Replace Type Code with State/Stragy / Replace Array with Object / Introduce Parameter Object
* Switch Statements(switch 惊悚现身)
	- 多态代替
	- Replace Parameter with Explicit Methods
* Parallel Inheritance Hierarchies(平行继承体系): 一个类增加子类时,另一个类也要增加子类, 解决方法是一个继承体系的实例引用另一个继承体系的实例.
* Lazy Class(冗赘类): 多余的类删掉.
	- Collapse Hierarchy / Inline Class
* Speculative Generality(夸夸其谈未来性): 未来可能用到的功能.
	- Collapse Hierarchy / Inline Class / Remove Parameter / Rename Method
* Temporary Field(令人迷惑的暂时字段). 某些字段特定时候才用, 和类其他字段有区别.
	- Extract Class
* Message Chain(多度耦合的消息链), 请求一个对象,再在这个对象上请求一个对象, 如此多次.
	- Hide Delegate
* Middle Man(中间人): 类一半的接口委托给其他类
	- Remove Middle Man / Inline Method / Replace Delegation with Inheritance
* Inappropriate Intimacy(狎昵关系): 两个类过多的访问私有成员, 或子类对基类了解过多
	- Move Method / Move Field / Change Bidirectional Association to Unidirectional / Hide Delegate / Replace Inheritance with Delegation
* Alternative Classes with Different interface(异曲同工的类): 两个方法函数做的是同样的事情.
	- Rename Method / Move Method / Extract Superclass
* Incomplete Library Class(不完美的库类): 修改库的一些功能使其完善.
	- Introduce Foreign Method
	- Introduce Local Expression
* Data Class(纯粹的数据类)
	- 找出其他类设置该类成员的地方, 看能否将这些修改移动到该类内, 以便可以隐藏那些取值/设值的函数
	- Move Method / Extract Method / Hide Method
* Refused Bequest(被拒绝的馈赠): 子类不需要继承基类的一些成员
	- 可以重构子类的另一个兄弟类, 保存所有不希望被继承的那些基类成员(Push down Method)
	- Replace Inheritance with Delegation 当不想要继承接口时使用.
* Comments(过多的注视)
	- Extract Method / Rename Method / Introduce Assertion

# 第4章 构筑测试体系
* 测试要自动化, 且需要时刻进行测试
* 编写新功能前先写测试,这样可明确新功能的需求
* 尽可能测试最可能出错的地方

# 第6章 重新组织函数
## 6.1 Etract Method 提炼函数
* 做法要点
	* 新函数内修改了源函数内的局部变量.
		* 新函数改为查询代码, 然后返回修改后的局部变量
		* Replace Temp with Query 消灭掉局部变量
	* 需要读取的源函数内的局部变量, 作为新函数的参数
	* 新函数尽量最多只返回一个值

## 6.2 Inline Method 内联函数
* 使用函数内的代码代替函数
