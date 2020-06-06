# 第5章 着色基础
* Gooch 着色模型： 法线与光线方向越趋于一致，色调越暖，否则色调越冷
![](media/DragonGoothShadingModel.png)
图1 带高亮效果的Gooth着色模型
* 主要公式如下
![](media/Equation_05_01.png)
![](media/Equation_05_02.png)
x带加减符号类似 opengl 的 clamp(0.0, 1.0)， 将x的值裁剪至0.0到1.0之间的范围。
r 向量计算出太阳光方向在表面上反射的向量