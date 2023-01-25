# 第3章 网页数据的解析提取
## 3.1 XPath 的使用
* XPath: XML Path Language
* XPath: 常用规则
    - nodename: 选取此节点的所有子节点
    - /: 从当前节点选取直接子节点
    - //: 从当前节点选取子孙节点
    - .: 选取当前节点
    - ..: 选取当前节点的父节点
    - @: 选取属性
```
//title[@lang='eng']
```
    - 表示选择所有名称为 'title', 属性 lang 为 eng 的节点
```
from lxml import etree
text = '''
<div>
    <ul>
         <li class="item-0"><a href="link1.html">first item</a></li>
         <li class="item-1"><a href="link2.html">second item</a></li>
         <li class="item-inactive"><a href="link3.html">third item</a></li>
         <li class="item-1"><a href="link4.html">fourth item</a></li>
         <li class="item-0"><a href="link5.html">fifth item</a>
     </ul>
 </div>
'''
html = etree.HTML(text)
result = etree.tostring(html)
print(result.decode('utf-8'))
```
```
<html><body><div>
    <ul>
         <li class="item-0"><a href="link1.html">first item</a></li>
         <li class="item-1"><a href="link2.html">second item</a></li>
         <li class="item-inactive"><a href="link3.html">third item</a></li>
         <li class="item-1"><a href="link4.html">fourth item</a></li>
         <li class="item-0"><a href="link5.html">fifth item</a>
     </li></ul>
 </div>
</body></html>
```
* // 取所有子孙节点， / 取直接子节点, .. 取父节点, parent:: 属性获取父节点
```
result = html.xpath('//ul//a')

result = html.xpath('//a[@href="link4.html"]/../@class')  

result = html.xpath('//a[@href="link4.html"]/parent::*/@class')  
```
* 属性匹配: @ 实现属性过滤
```
result = html.xpath('//li[@class="item-0"]')
```
* text() 方法获取节点中的文本
```
result = html.xpath('//li[@class="item-0"]/text()')
```
* @ 可以获取属性
```
result = html.xpath('//li/a/@href')
```
* contain 解决多属性问题
```
result = html.xpath('//li[contains(@class, "li")]/a/text()')
```
* and 连接多个属性
* 运算符
    - or
    - and
    - mod
    - | 计算两个节点集    //book|//cd  所有拥有 book 和 cd 的节点
    - +
    - -
    - *
    - div 除法
    - =
    - !=
    - <
    - <=
    - >
    - >=
* 中括号传入索引可以获取特定次序的节点
```
    html = etree.HTML(text)
    result = html.xpath('//li[1]/a/text()')
    print(result)
    result = html.xpath('//li[last()]/a/text()')
    print(result)
    result = html.xpath('//li[position()<3]/a/text()')
    print(result)
    result = html.xpath('//li[last()-2]/a/text()')
    print(result)
```
* ancestor 祖先轴, attribute 轴， child 轴, descendant 轴， following 轴(当前节点后所有的节点轴), following-sibling 轴(当前节点之后的所有同级节点)

## 3.2 Beautiful Soup 的使用
* Beautiful Soup 支持的解析器
    - Python 解析器: BeautifulSoup(markup, 'html.parser')
    - LXML HTML 解析器: BeautifulSoup(markup, 'lxml')
    - LXML XML 解析器: BeautifulSoup(markup, 'xml')
    - html5lib: BeautifulSoup(markup, 'html5lib')
* prettify() 方法把要解析的字符串以标准的缩进格式输出