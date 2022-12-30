# 第三章 模板

## 3.1 模板基本用法
* larr 后退符号, copy 版权符号， 具体可见 https://dev.w3.org/html5/html-author/charref
* 三种定界符
```
语句(if判断, for循环等)
{% ... %}

表达式(字符串，变量，函数调用等)
{{ ... }}

注释
{# ... #}
```
* Jinja2 for 循环特殊变量
    - loop.index: 当前迭代数， 从 1 开始
    - loop.index0: 当前迭代数， 从 0 开始
    - loop.revindex: 当前反向迭代数， 从 1 开始
    - loop.revindex0: 当前反向迭代数， 从 0 开始
    - loop.first
    - loop.last
    - loop.previtem
    - loop.nextitem
    - loop.length
* render_template() 渲染函数
* Flask 还提供了 render_template_string() 来渲染模板字符串

## 3.2 模板辅助工具
* 模板中定义变量, 使用 set 标签
```
{% set navigation = [('/', 'Home'), ('/about', 'About')] %}
```
* set 和 endset 标签声明开始和结束, 将一部分模板数据定义为变量
```
{% set navigation %}
    <li><a href="/">Home</a>
    <li><a href="/about">About</a>
{% endset %}
```
* 可以在模板中直接使用的内置变量
    - config: 当前的配置对象
    - request: 当前的请求对象
    - session: 当前的会话对象
    - g: 与请求绑定的全局变量
* app.context_processor 装饰器, 可以用来注册模板上下文处理函数, 帮我们完成统一传入变量的工作. 模板上下文处理函数需要返回一个包含变量键值对的字典.
* 处理模板上下文处理函数
```
@app.context_processor
def inject_foo():
    foo = 'I am foo.'
    return dict(foo=foo)  # 等同于 return { 'foo': foo }
```
* 调用 render_template() 函数渲染任何一个模板时,所有使用 app.context_processor 装饰器注册的模板上下文处理函数(包括Flask内置的上下文处理函数)都会被执行. 这些函数的返回值会被添加到模板中.
* 不使用装饰器传入模板上下文处理函数的方法
```
def inject_foo():
    foo = 'I am foo.'
    return dict(foo=foo)  # 等同于 return { 'foo': foo }

app.context_processor(inject_foo)
```
* 使用 lambda 简化
```
app.context_processor(lambda: dict(foo='I am foo.'))
```
* Jinja2 内置模板全局函数
    - range([start, ]stop[, step]): 类似于 Python 的 range()
    - lipsum(n = 5, html=True, min=20, max=100): 生成随即文本
    - dict(**items): 类似于 Python 的 dict()
* Flask 内置模板全局函数
    - url_for(): 用于生成 URL 的函数
    - get_flashed_message(): 用于获取 flash 消息的函数
* app.template_global 装饰器直接将函数注册为模板全局函数
```
@app.template_global()
def bar():
    return ‘I am bar.’
```
    - app.template_global() 装饰器使用 name 参数可以指定一个自定义名称.
* app.add_template_global() 方法注册自定义全局函数, 传入函数对象和可选的自定义名称(name), 比如 app.add_template_global(your_global_function).
* 过滤器(filter)是一些可以用来修改和过滤变量值的特殊函数, 过滤器和变量用一个竖线(管道符号)隔开. 需要参数的过滤器可以像函数一样使用括号传递.
```
{{ name|title }}
```
    - 相当于 name.title()
* filter 标签和 endfilter 标签声明开始和结束
```
{% filter upper %}
    This text becomes uppercase.
{% endfilter %}
```
* 内置过滤器
    - default
    - escape: 转义 HTML 文本
    - first: 返回序列的第一个元素
    - last: 返回序列的最后一个元素
    - length: 返回变量的长度
    - random: 返回序列中的随即元素
    - safe: 将变量值标记为安全, 避免转义
    - trim: 清除变量值前后的空格
    - max: 返回序列中的最大值
    - min: 返回序列中的最小值
    - unique: 返回序列中不重复的值
    - striptags: 清除变量值内的 HTML 标签
    - urlize: 将 URL 文本转换为可单击的 HTML 链接
    - wordcount: 计算单词数量
    - tojson: 将变量值转换为 JSON 格式
    - truncate: 截断字符串, 常用于显示文章摘要.
* 过滤器可以叠加使用
```
<h1>Hello, {{ name|default(‘陌生人’)|title }} </h1>
```
* 避免转义, 将变量作为 HTML 解析, 可以对变量使用 safe 过滤器
```
{{ sanitized_text|safe }}
```
* 渲染前将变量转换为 Markup 对象也可以将文本标记为安全
```
from flask import Markup

@app.route('/hello'):
def hello():
    text = Markup('<h1>Hello, Flask</h1>')
    return render_template('index.html', text=text)
```
* 不要对用户输入的内容使用 safe 过滤器, 否则容易被植入恶意代码, 导致 XSS 攻击
* app.template_filter() 自定义过滤器, 可以使用 name 关键字设置过滤器的名字
```
from flask import Markup

@app.template_filter()
def muscial(s)
    return s + Markup(' &#9834;')
```
* 直接使用 app.add_template_filter() 方法注册自定义过滤器， 传入函数对象和可选的自定义名称(name), 如 app.add_template_filter(your_filter_function)
* 用 is 连接变量和测试器, 如 number 测试器
```
{% if age is number %}
    {{ age * 365 }}
{% else %}
    无效的数字
{% endif %}
```
* 常用测试器
    - callable: 是否可被调用
    - defined: 是否已定义
    - undefined: 是否未定义
    - none: 变量是否 None
    - number: 是否是数字
    - string: 是否是字符串
    - sequence: 是否是序列
    - iterable: 是否可迭代
    - mapping: 是否匹配对象
    - sameas: 是否指向相同的内存地址
* 测试器可以括号或者空格传入其他的参数
```
{% if foo is sameas(bar) %}
{% if foo is sameas bar %}
```
* app.template_test() 装饰器注册一个自定义测试器, name 关键字指定自定义名称
```
@app.template_test()
def bar(n):
    if n == ‘bar’:
        return True
    return False
```
* app.add_template_test() 方法注册自定义测试器.
* Flask 创建 Environment 对象, 存储在 app.jinja_env 属性上
* app.jinja_env 更改 Jinja2 设置. 如自定义所有的定界符
```
app = Flask(__name__)
app.jinja_env.variable_start_string = '[['
app.jinja_env.variable_end_string = ']]'
```
* 模板环境中的全局函数、过滤器和测试器分别存储在 Environment 对象的 globals, filters, tests 属性中, 这三个属性都是字典对象.
* 添加自定义全局变量
```
def bar():
    return 'I am bar.'
foo = 'I am foo.'

app.jinja_env.globals['bar'] = bar
app.jinja_env.globals['foo'] = foo
```
* 添加自定义过滤器
```
def smiling(s):
    return s + ' :)'

app.jinja_env.filters['smiling'] = smiling

```
* 添加自定义测试器
```
def bar(n):
    if n == 'bar':
        return True
    return False

app.jinja_env.tests['bar'] = bar
```

## 3.3 模板组织结构
* DRY 原则: Don't Repeat Yourself
* 局部模板: 仅包含部分代码, 不会在视图函数中直接渲染它, 而是插入到其他独立模板中.
```
{% include '_banner.html' %}
```
    - 局部模板的命名通常以一个下划线开始
* 封装一部分模板代码
```
{% macro qux(amount=1) %}
    {% if amount == 1 %}
        I am qux.
    {% elif amount > 1 %}
        We are quxs.
    {% endif %}
{% endmacro %}
```
* 通过 import 语句使用宏
```
{% from 'macros.html' import qux %}
...
{{ qux(amount=5) }}
···
* include 会传递当前上下文， import 则不会, 可以hi用 with context 传递当前上下文
```
{% from "macros.html" import foo with context %}
```
* 基模板存储页面的固定部分
```
<!DOCTYPE html>
<html>
<head>
    {% block head %}
        <meta charset="utf-8">
        <title>{% block title %}Template - HelloFlask{% endblock %}</title>
        {% block styles %}{% endblock %}
    {% endblock %}
</head>
<body>
<nav>
    <ul><li><a href="{{ url_for('index') }}">Home</a></li></ul>
</nav>

<main>
    {% block content %}{% endblock %}
</main>
<footer>
    {% block footer %}
    {% endblock %}
</footer>
{% block scripts %}{% endblock %}
</body>
</html>
```
* 子模版中可以通过定义同名的块来执行继承操作
* 块的结束标签可以指明块名
```
{% block body %}
...
{% endblock body %}
```
* 使用 extends 标签声明扩展基模板, extends 必须是子模版的第一个标签
    - 覆盖
    - 追加内容， 原内容 用 {{ super() }} 继承


## 3.4 模板进阶实践
* 模板中的 Jinja2 语句、表达式和注释会保留移除后的空行，通过在定界符内侧添加减号自动去掉这些空行。 如 {%- endfor%}
* 两个控制空白的属性
```
app.jinja_env.trim_blocks = True
app.jinja_env.lstrip_blocks = True
```
    - 前者用来删除 Jinja2 语句后的第一个空行, 后者用来删除 Jinja2 语句所在行之前的空格和制表符(tabs).
    - trim_blocks 中的 block 指的是使用 {% ... %} 定界符的代码块.
    - 宏内的空白控制行为不受 trim_blocks 和 lstrip_blocks 属性控制.
* 静态文件默认在主脚本同级目录的 static 文件夹中.
* 静态文件的视图函数, 端点值为 static, 默认 URL 规则为 static/<path:filename>
* 使用其他文件夹存储静态文件, 可以在实例化 Flask 类时使用 static_folder 参数指定, 静态文件的URL路径中的 static 也会自动跟随文件夹名称变化. 再实例化 Flask 类时使用 static_url_path 参数可以自定义静态文件的 URL 路径.
```
url_for('static', filename='avatar.jpg')
<link rel="stylesheet" type="text/css" href="{{ url_for('static', filename='style.css' ) }}">
<link rel="icon" type="image/x-icon" href="{{ url_for('static', filename='favicon.ico') }}">
```
* css，js, 从 Bootstrap 下载，而后分类放在 static 目录下
* Bootstrap 依赖 jQuery, Popper.js, 按照 jQuery->Popper.js->Bootstrap 的顺序引入
* CDN 下载， 不过个人觉得可能有访问问题。
* 方便设置的宏
```
{% macro static_file(type, filename_or_url, local=True) %}
    {% if local -%}
        {% set filename_or_url = url_for('static', filename=filename_or_url) %}
    {%- endif %}
    {% if type == 'css' -%}
        <link rel="stylesheet" href="{{ filename_or_url }}" type="text/css">
    {%- elif type == 'js' -%}
        <script type="text/javascript" src="{{ filename_or_url }}"></script>
    {%- elif type == 'icon' -%}
        <link rel="icon" href="{{ filename_or_url }}">
    {%- endif %}
{% endmacro %}
```
* 消息闪现 flash() 函数。flash() 发送的消息存储在 session 中, 模板使用全局函数 get_flashed_messages() 获取消息
* HTML 文件 head 标签添加编码生命 <meta charset="utf-8">
* 模板设置 flash 消息
```
<main>
    {% for message in get_flashed_messages() %}
        <div class="alert">{{ message }}</div>
    {% endfor %}
    {% block content %}{% endblock %}
</main>
```
* 错误处理函数需要使用 app.errorhandler() 装饰器。
```
# 404 error handler
@app.errorhandler(404)
def page_not_found(e):
    return render_template('errors/404.html'), 404
```
* HTML元素的  data-* 属性可以存储用于 javascript 获取的数据.
* CSS 可以用 var() 函数获取存储的Jinja2变量。