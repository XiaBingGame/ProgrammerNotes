<!--
 * @Author: summericeyl summericeyl@gmail.com
 * @Date: 2023-05-21 01:48:45
 * @LastEditors: summericeyl summericeyl@gmail.com
 * @LastEditTime: 2024-09-13 07:09:16
 * @FilePath: \ProgrammerNotes\content\ProgrammingInLua\Chapter03\README.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
* 5.3 之后的 Lua 支持两种数值, 64位的 integer 和 64 位的 float
* 双精度浮点数最大表示 2^53 次方的整型值。
* 支持科学计数法, 如 4.57e-3, 5E+20, 0.3e12
* 使用函数 math.type() 区分整型值和浮点型值
```
> math.type(3)      --> integer
> math.type(3.0)    --> float
```
* 0x 开头表示十六进制常量, 还支持十六进制的浮点数, 其以 p 或 P 开头的指数部分组成
```
> 0xff      --> 255
> 0x1A3     --> 419
> 0x0.2     --> 0.125
> 0x1p-1    --> 0.5
> 0xa.bp2   --> 42.75
```
* 使用 %a 格式化字符串
```
> string.format("%a", 419)      --> 0x1.a3p+8
> string.format("%a", 0.1)      --> 0x1.99999999999ap-4
```
* 加,减,乘,除,负,取整除法,取模,指数运算
* 除法运算操作的结果是浮点数
* floor 除法(//) 向负无穷取整
```
> 3 // 2        --> 1
> 3.0 // 2      --> 1.0
> 6 // 2        --> 3
> 6.0 // 2.0    --> 3.0
> -9 // 2       --> -5
> 1.5 // 0.5    --> 3.0
```
* 取模运算的定义
```
a % b == a - ((a // b) * b)
```
* 取模运算结果的符号永远与第二个操作数的符号保持一致.  对于任意指定的正常量K, 即使 x 是负数, 表达式 x%K 的结果也永远在 [0, K-1] 之间.
* 对于实数类型, 有这样的结果, x - x%0.01 恰好是 x 保留两位小数的结果, x - x%0.001 恰好是 x 保留三位小数的结果.
```
> x = math.pi
> x - x%0.01        --> 3.14
> x - x%0.001       --> 3.141
```
* 模可以用于角度
```
local tolerance = 0.17
function isturnback(angle)
    angle = angle % (2*math.pi)
    return (math.abs(angle-math.pi) < tolerance)
end
```
* 可以使用 ^ 求幂
* 关系运算 < > <= >= == ~=
* 数学库: 三角函数(sin, cos, tan, asin等), 指数函数, 取整函数, 最大和最小函数max和min, 用于生成随机数的伪随机数函数(random)以及常量pi和huge(最大可表示数值, 在大多数平台上代表 inf), 所有的三角函数都以弧度为单位. deg 和 rad 进行角度和弧度的转换.