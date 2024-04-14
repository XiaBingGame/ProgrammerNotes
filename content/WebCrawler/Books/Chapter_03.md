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
* Beautiful Soup 支持的解析器, 支持第三方解析库
    - Python 解析器: BeautifulSoup(markup, 'html.parser')
    - LXML HTML 解析器: BeautifulSoup(markup, 'lxml')
    - LXML XML 解析器: BeautifulSoup(markup, 'xml')
    - html5lib: BeautifulSoup(markup, 'html5lib')
* prettify() 方法把要解析的字符串以标准的缩进格式输出
* 节点返回的类型有 bs4.element.Tag， 其有 string 属性，只返回第一个匹配的节点
* 提取节点的信息
    - 提取名称, name 属性, 如 print(soup.title.name)
    - attrs 可以获取所有属性, 如 print(soup.p.attrs), print(soup.p.attrs['name']), 运行结果如下:
```
{'class': ['title'], 'name': 'dromouse'}
dromouse
```
    - 嵌套选择, 如 print(soup.head.title.string) 则嵌套选择 head 节点内的 title 节点
* 通过 contents 获取直接子节点, children 属性也可获取所有子节点, 只是其为迭代器, descendants 获取子孙节点
* parent 获取父节点, parents 获取所有祖先节点
* next_sibling, previous_sibling 获取兄弟节点， next_siblings, previous_siblings 兄弟节点列表
* 方法选择器
    - find_all: 查找所有符合条件的元素, find_all(name, attrs, recursive, text, **kwargs)
        - name: soup.find_all(name='ul')
        - attrs: soup.find_all(attrs={'id': 'list-1'}), soup.find_all(attrs={'name': 'elements'})
        - 上面的方法可以直接用参数， 如 soup.find_all(id='list-1'), soup.find_all(class_='elements')
        - text 可以是文本，也可以是正则表达式, 如 soup.find_all(text=re.compile('link'))
    - find: 同上， 只返回第一个元素
    - find_parents, find_parent
    - find_next_siblings, find_next_sibling
    - find_previous_siblings, find_previous_sibling
    - find_all_next, find_next
    - find_all_previous, find_previous
* select 方法, CSS选择器, 如 soup.select('.panel .panel-heading'), soup.select('ul li'), soup.select('#list-2 .element')
    - select 方法支持嵌套选择
    - 除了 string, 还有 get_text() 方法获取文本

## 3.3 pyquery 的使用