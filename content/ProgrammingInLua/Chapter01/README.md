* 注释： 单行注释 --， 多行注释 
```
--[[
    .... 
--]]
```
* 分号可以作为分隔符，但不是必须的
* 8种基本数据类型: nil, boolean, number, string, userdata, function, thread, table
* 函数 type() 可以获取类型名称
* false 和 nil 之外的所有值都视为真
* and, or
```
x = x or v
等价于
if not x then x = v end

a and b or b
等价于 C语言的 a ? b : c
```
* lua [options] [script [args]]
    - -e 参数允许我们直接在命令行中输入代码， 如 lua -e "print(math.sin(12))"
    - -l 用于加载库
    - -i 用于在运行完其他命令行参数后进入交互模式
* 解释器在处理参数之前，会查找名为 LUA_INIT_5_3 的环境变量，如果找不到，就再找 LUA_INIT 的环境变量，如果其存在一个，且其内容为 @filename, 则解释器就会运行相应的文件，如果这两个环境变量存在一个，且不以 @ 开头，则解释器会认为其包含 lua 代码，对其解释执行。
* 全局变量 arg 可以获取解释器传入的参数，索引0保存的内容为脚本名
```
% lua -e "sin=math.sin" script a b

arg[-3] = "lua"
arg[-2] = "-e"
arg[-1] = "sin=math.sin"
arg[0] = "script"
arg[1] = "a"
arg[2] = "b"
```
* lua 支持可变长参数， 表达式 ...(3个点)表示传递给脚本的所有参数

