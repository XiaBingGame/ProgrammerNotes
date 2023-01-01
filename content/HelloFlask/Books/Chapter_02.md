# 第二章 Flask与Http
* 要点
    - wsgi
    - flask 的 request 的各项属性, 如 path, args 等属性
    - flask routes 查看定义的所有路由
    - 其他类型： /goback/<int:year>, 内置变量转换器
    - 可选值： @app.route('/colors/<any(blue, white, red):color>')
    - app 的五个钩子函数
    - Flask 会调用 make_response() 方法将视图函数返回值转换为响应对象
    - 视图函数可以返回最多由三个元素组成的元组: 响应主体, 状态码, 首部字段 
    - redirect() 函数可以生成重定向响应
    - Flask 的 abort() 函数可以手动返回错误响应, abort() 函数之后的代码将不会被执行
    - repsponse 对象的 mimetype 属性可以设置 MIME 类型, MIME 也就是 Head 内的 Content-Type
    - json.dumps() 方法将字典, 列表或者元组序列化为 json 字符串
    - Flask 更便捷的包装函数 jsonify(), 默认生成 200 响应
    - response.set_cookie() 方法支持多个参数来设置 Cookie 的选项,
    - Cookie 可以通过request对象的 cookies 属性读取
    - Flask 提供了 session 对象用来将 Cookie 数据加密存储, 将数据存储在浏览器上一个名为 session 的 cookie 里 
	- 保存密钥的安全做法是将密钥写入系统环境变量, 而后保存在 .env 文件中
	- 通过 session 模拟登录登出系统
    - Flask 的四个上下文变量: current_app, g, request, session
    - teardown_appcontext 钩子, 在程序上下文被销毁时调用, 也会在请求上下文被销毁时调用.
    - URL 的 referer 和 next 容易被篡改, 需要进行验证
    - AJAX 指异步 Javascript 和 XML (Asynchronous JavaScript And XML), AJAX 基于 XMLHttpRequest, 让我们可以在不重载页面的情况下和服务器进行数据交换
    - 使用 jQuery 发送 AJAX 请求. jQuery 是流行的 JavaScript 库. jQuery 处理了不同浏览器的 AJAX 兼容问题
    - HTTP 服务器端推送

* 服务器端: Web服务器和 WSGI 交互, WSGI 和 Flask 程序交互, Flask 程序和数据库交互
* WSGI 将 HTTP 格式的请求数据转换成 Flask 程序能够使用的 Python 数据
* URL 的查询字符串从问号?开始, 以键值对的形式写出, 多个键值对之间使用&分隔
* 访问页面, 请求方法一般是GET,填写表单并提交,请求方法通常为POST
* http://helloflask.com/hello?name=Grey 通过  request 的属性获取报文中的数据, 有属性 path, full_path, host, host_url, base_url, url, url_root
```
path: u'/hello'
full_path: u'/hello?name=Grey'
host: u'helloflask.com'
host_url: u'http://helloflask.com'
base_url: u'http://helloflask.com/hello'
url: u'http://helloflask.com/hello?name=Grey'
url_root: u'http://helloflask.com/
```
* request 的其他属性: args, blueprint, cookies, data, endpoint, files, form, values, get_data(), get_json(), headers, is_json, json, method
* 获取 URL 中的查询字符串
```
from flask import Flask, request
app = Flask(__name__)
@app.route('/hello')
def hello():
    name = request.args.get('name', 'Flask')    # 获取查询参数name的值
    return '<h1>Hello, %s!<h1>' % name       
```
  - 上面如果使用键作为索引获取数据, 如 name = request.args['name'], 如果没有对应键, 则会返回 HTTP 400 错误响应
* 路由表(app.url_map): 将请求分发到对应的视图函数, 如果没有找到视图函数, 则会得到404响应
* flask routes 命令可以查看程序中定义的所有路由
* static 端点是 Flask 的特殊路由, 用来访问静态文件
* GET 是最常用的 HTTP 方法, 视图函数默认监听的方法类型就是 GET, HEAD, OPTIONS 方法的请求由 Flask 处理, DELETE, PUT等方法一般不会在程序中实现
* app.route() 使用 methods 参数传入一个监听的 HTTP 方法的可迭代对象. 比如同时监听 GET 和 POST 请求
```
@app.route('/hello', methods=['GET', 'POST'])
def hello():
	return '<h1>Hello, Flask!</h1>'
```
* 如果视图函数无法处理其他方法, 比如 POST 方法, 则会返回 405 错误响应(Method Not Allowed)
* 同一个 URL 规则可以定义多个视图函数, 分别处理不同 HTTP 方法的请求
* URL 变量可以解析为其他类型, 默认为字符串, 如 /goback/<int:year>
* Flask 内置 URL 变量转换器
```
string --- 不包含斜线的字符串
int
float
path --- 包含斜线的字符串
any --- 匹配一系列给定值中的一个元素
uuid --- UUID 字符串
```
  - 格式 <转换器:变量名>
```
@app.route('goback/<int:year>')
def go_back(year):
    return '<p>Welcome to %d!</p>' % (2018 - year)
```
  - 如果传入的不是整数, 则直接返回 404 错误响应
* any 转换器, 添加括号给出可选值, "<any (value1, value2, ...): 变量名>", 或者先给出一个预先定义的列表, 而后通过格式化字符串的方式构建URL规则字符串
```
@app.route('/colors/<any(blue, white, red):color>')
def three_colors(color):
    return '<p>Love is patient and kind. Love is not jealous or boastful or p

colors = ['blue', 'white', 'red']
@app.route('/colors/<any(%s):color>' % str(colors)[1:-1])
```
* 钩子函数用于表示请求处理不同阶段执行的处理函数, 通过装饰器实现. 例如对函数附加了 app.before_request 装饰器后, 该函数就会注册微 before_request 处理函数. 五种请求钩子
```
before_first_request
before_request
after_request
teardown_request: 即使有未处理的异常抛出, 会在每个请求结束后运行. 如果发生异常, 传入异常对象作为参数到注册的函数中.
after_this_request
```
* 每个钩子可以注册任意多个处理函数, 函数名并不是必须和钩子名称相同, 例子:
```
@app.before_request
def do_something():
    pass # 这里的代码会在每个请求处理前执行
```
* 常见应用场景
	- before_first_request: 创建数据库表, 添加管理员用户等
	- before_request:记录用户最后在线的时间
	- after_request:提交数据库的更改
* after_reqeust 和 after_this_request 必须接收一个响应类对象作为参数
* 环境为 production 时, 程序出错, Flask 自动返回 500 错误响应, 调试模式则会显示调试信息和错误堆栈
* Flask 使用 Response 对象表示响应, Flask 会调用 make_response() 方法将视图函数返回值转换为响应对象
* 视图函数可以返回最多由三个元素组成的元组: 响应主体, 状态码, 首部字段
	- 可以只包含主体内容
```
@app.route('/hello')
def hello():
    ...
    return '<h1>Hello, Flask!</h1>'
```
* 指定了状态码
```
@app.route('/hello')
def hello():
    ...
    return '<h1>Hello, Flask!</h1>', 201
```
* 状态码为3XX的重定向响应, Location字段设置为重定向的目标URL
```
@app.route('/hello')
def hello():
    ...
    return '', 302, {'Location', 'http://www.example.com'}
```
* redirect() 函数可以生成重定向响应
```
from flask import Flask, redirect
# ...
@app.route('/hello')
def hello():
    return redirect('http://www.example.com')
```
* 状态码 302 表示临时重定向, redirect() 第二个参数或者code关键字可以传入修改的状态码
* redirect() 函数使用 url_for() 函数生成目标 URL, 使得程序重定向到其他视图
```
from flask import Flask, redirect, url_for 
...
@app.route('/hi')
def hi():
    ...
    return redierct(url_for('hello'))  # 重定向到/hello
@app.route('/hello')
def hello():
    ...
```
* HTTP错误对应的异常类在 Werkzeug 的 werkzeug.exceptions 模块中定义, 抛出这些异常即可返回对应的错误响应. Flask 的 abort() 函数可以手动返回错误响应, abort() 函数之后的代码将不会被执行
```
from flask import Flask, abort
...
@app.route('/404')
def not_found():
    abort(404)
```
* MIME类型: 不同的响应数据格式需要设置不同的 MIME 类型, MIME 类型在 HEAD 的 Content-Type 字段中定义, 如 HTML 类型
```
Content-Type: text/html; charset=utf-8
```
* MIME 类型(media type 或 content type) 是一种用来标识文件类型的机制. 一般格式为"类型名/子类型名", 子类型名一般为文件扩展名. 如 "text/html", "image/png"
* repsponse 对象的 mimetype 属性可以设置 MIME 类型
```
from flask import make_response
@app.route('/foo')
def foo():
    response = make_response('Hello, World!')
    response.mimetype = 'text/plain'
    return response
```
* 常见数据格式有纯文本, HTML, XML(application/xml) , JSON(application/json)
* python 库有 json 模块, Flask 可以导入 json 对象, 而后 dumps() 方法将字典, 列表或者元组序列化为 json 字符串
```
from  flask import Flask, make_response, json
...
@app.route('/foo')
def foo():
    data = {
        'name':'Grey Li',
        'gender':'male'
    }
    response = make_response(json.dumps(data))
    response.mimetype = 'application/json'
    return response
```
* Flask 更便捷的包装函数 jsonify(), 默认生成 200 响应
```
from flask import jsonify
@app.route('/foo')
def foo():
    return jsonify(name='Grey Li', gender='male')
```
* Response 类的常用属性和方法
```
headers
status
status_code
mimetype
set_cookie()
get_json()
is_json()
json
```
* set_cookie() 方法支持多个参数来设置 Cookie 的选项, 如下:
```
key
value
max_age: 单位为秒
expires, UNIX 时间戳
path: cookie 只在给定的路径可用, 默认是整个域名
domain: cookie 可用的域名
secure, 只有 https 才可使用
httponly: 可禁止 JavaScript 获取 cookie
```
```
from flask import Flask, make_response
...
@app.route('/set/<name>')
def set_cookie(name):
    response = make_response(redirect(url_for('hello')))
    response.set_cookie('name', name)
    return response
```
* Cookie 可以通过request对象的 cookies 属性读取
```
from flask import Flask, request
@app.route('/')
@app.route('/hello')
def hello():
    name = request.args.get('name')
    if name is None:
        name = request.cookies.get('name', 'Human')  # 从Cookie中获取name值
    return '<h1>Hello, %s</h1>' % name
```
* Flask 提供了 session 对象用来将 Cookie 数据加密存储, 将数据存储在浏览器上一个名为 session 的 cookie 里
	* session 需要设置密钥. 通过 Flask.secret_key 属性或者 SECRET_KEY 设置
```
app.secret_key = 'secret string'
```
	* 安全做法是将密钥写入系统环境变量, 而后保存在 .env 文件中
```
SECRET_KEY=secret string
```
* 在程序中可以使用 os 模块提供的 getenv() 方法获取:
```
import os
# ...
app.secret_key = os.getenv('SECRET_KEY', 'secret string')
```
* 用户认证模拟
```
from flask import redirect, session, url_for
@app.route('/login')
def login():
    session['logged_in'] = True  # 写入session
    return redirect(url_for('hello'))
```
* 我们向 session 中添加一个 logged-in cookie, 将它的值设为 True, 表示用户已认证.
* 修改后的 hello 视图
```
from flask import request, session
@app.route('/')
@app.route('/hello')
def hello():
    name = request.args.get('name')
    if name is None:
        name = request.cookies.get('name', 'Human')
        response = '<h1>Hello, %s!</h1>' % name
        # 根据用户认证状态返回不同的内容
        if 'logged_in' in session:
            response += '[Authenticated]'
        else:
            response += '[Not Authenticated]'
        return response
```
* session中的数据可以像字典一样通过键读取，或是使用get()方法。这里我们只是判断session中是否包含logged_in键，如果有则表示用户已经登录。通过判断用户的认证状态，我们在返回的响应中添加一行表示认证状态的信息：如果用户已经登录，显示[Authenticated]；否则显示[Not authenticated]。
* 判断用户是否认证, 模拟后台的 admin 视图
```
from flask import session, abort
@app.route('/admin')
def admin():
    if 'logged_in' not in session:
        abort(403)
    return 'Welcome to admin page.'
```
* 登出用户
```
from flask import session
@app.route('/logout')
def logout():
    if 'logged_in' in session:
        session.pop('logged_in')
    return redirect(url_for('hello'))
```
* 默认情况, session cookie 在用户关闭浏览器时删除. 设置 session.permanent 为 True, 可以将 session 的有效期延长为 Flask.permanent_session_lifetime 的属性值对应的 datetime.timedelta 对象, 或者设置 PERMANENT_SESSION_LIFETIME 变量, 默认为 31 天
* 不要在 session 中存储敏感信息
* Flask 的四个上下文变量, 这四个变量都是代理对象(proxy). 如果要获取原始对象, 可以对代理对象调用 _get_current_object() 方法
	* current_app: 程序上下文, 指向处理请求的当前程序实例
	* g: 程序上下文, 替代Python的全局变量用法, 仅在当前请求中可用, 用于存储全局数据, 每次请求都会重设.
	* request: 请求上下文, 封装客户端发出的请求报文数据
	* session: 请求上下文, 用于记住请求之间的数据, 通过签名的 Cookie 实现.
* g 随着每一个请求的进入和激活, 随着每一个请求的处理完毕而销毁. 可以保存每个请求处理前所需要的全局变量, 比如当前登入的用户对象,数据库连接等.
```
from flask import g
@app.before_request
def get_name():
    g.name = request.args.get('name')
```
* g 支持类似字典的 get(), pop() 以及 setdefault() 方法
* 激活程序上下文的情况:
	* flask run 命令启动程序时
	* 旧的 app.run() 方法启动程序时
	* 执行使用 @app.cli.command() 装饰器注册的 flask 命令时
	* 使用 flask shell 命令启动 Python shell 时.
* 在请求处理时, 程序上下文和请求上下文拥有相同的生命周期.
* 如果在没有激活上下文时使用这些变量, Flask 抛出 RuntimeError 异常.
* 手动激活上下文, 使用 app.app_context() 获取程序上下文
```
>>> from app import app
>>> from flask import current_app
>>> with app.app_context():
    ... current_app.name
'app'
```
* 使用 push() 推送(激活)上下文, pop()销毁上下文
```
>>> from app import app
>>> from flask import current_app
>>> app_ctx = app.app_context()
>>> app_ctx.push()
>>> current_app.name
'app'
>>> app_ctx.pop()
```
* test_request_context() 临时创建请求上下文
```
>>> from app import app
>>> from flask import request
>>> with app.test_request_context('/hello'):
...     request.method
'GET'
```
* teardown_appcontext 钩子, 在程序上下文被销毁时调用, 也会在请求上下文被销毁时调用.
```
@app.teardown_appcontext
def teardown_db(exception):
    ...
    db.close()
```
* 重定向至上个页面
```
return redirect(request.referrer)
```
	* 回到上个页面或者为空时的默认页面
```
return redirect(request.referrer or url_for('hello'))
```
* 返回值后添加 next 参数
```
# redirect to last page
@app.route('/foo')
def foo():
    return '<h1>Foo page</h1><a href="%s">Do something and redirect</a>' \
           % url_for('do_something', next=request.full_path)


@app.route('/bar')
def bar():
    return '<h1>Bar page</h1><a href="%s">Do something and redirect</a>' \
           % url_for('do_something', next=request.full_path)
```
* do_something 的返回值
```
return redirect(request.args.get('next'))
return redirect(request.args.get('next', url_for('hello')))
```
* 结合两者
```
def redirect_back(default='hello', **kwargs):
    for target in request.args.get('next'), request.referrer:
        if target:
            return redirect(target)
    return redirect(url_for(default, **kwargs))

@app.route('/do_something_and_redirect')
def do_something():
    # do something
    return redirect_back()
```
* 对 URL 进行安全验证
	* referer 和 next 容易被篡改, 需要进行验证
	* URL 中的 next 参数, 以查询字符串的方式写在 URL 里
```
http://localhost:5000/do-something?next=http://helloflask.com
```
	* 需要验证 next 变量值是否属于程序内部 URL, 确保只有属于程序内部的URL才会被返回
```
from urlparse import urlparse, urljoin  # Python3需要从urllib.parse导入
from flask import request
def is_safe_url(target):
    ref_url = urlparse(request.host_url)
    test_url = urlparse(urljoin(request.host_url, target))
    return test_url.scheme in ('http', 'https') and \
           ref_url.netloc == test_url.netloc


def redirect_back(default='hello', **kwargs):
    for target in request.args.get('next'), request.referrer:
        if not target:
            continue
        if is_safe_url(target):
            return redirect(target)
    return redirect(url_for(default, **kwargs))
```
* AJAX 指异步 Javascript 和 XML  (Asynchronous JavaScript And XML), AJAX 基于 XMLHttpRequest, 让我们可以在不重载页面的情况下和服务器进行数据交换. 加上 JavaScript 和 DOM, 就可以在接收到响应数据后局部更新页面. XML 指的是数据交互格式, 其也可以是纯文本, HTML 或 JSON. XMLHttpRequest 不仅支持 HTTP 协议, 还支持 FILE 和 FTP 协议.
* 使用 jQuery 发送 AJAX 请求. jQuery 是流行的 JavaScript 库. jQuery 处理了不同浏览器的 AJAX 兼容问题, 只需要编写一套代码, 就可以在所有主流的浏览器正常运行.
* 使用 jQuery 实现 AJAX 并不是必须的, 可以使用原生的 XMLHttpRequest, 其他 JavaScript 框架内置的 AJAX 接口, 或者使用更新的 fetch API 来发送异步请求.
* jQuery 函数 ajax() 发送 AJAX 请求. ajax 函数是底层函数, 有丰富的自定义配置, 支持的主要参数如下所示:
	* url: 字符串, 默认为当前页面地址, 表示请求的地址
	* type: 字符串, 默认为 'GET', 表请求的方式.
	* data: 字符串, 无默认值. 发送到服务器的数据, jQuery 自动转换为查询字符串
	* dataType: 字符串, 期待服务器返回的数据类型, 有 "xml", "html", "script", "json", "jsonp", "text"
	* contentType: 字符串, 默认为 "application/x-www-form-urlencoded;charset=UTF-8", 发送请求时使用的内容类型
	* complete: 函数, 请求完成后调用的回调函数
	* success: 函数, 请求成功后调用的回调函数
	* error: 函数, 请求失败后调用的回调函数
* jQuery 其他方法:
	* get(): 发送 GET 请求
	* post(): 发送 POST 请求
	* getjson(): 获取 json 数据
	* getscript(): 获取脚本
* AJAX 返回局部数据
	* 纯文本或局部 HTML 模板(纯文本替换页面中的文本值, 局部 HTML 可以插入到页面中, 比如返回评论列表)
```
@app.route('/comments/<int:post_id>')
def get_comments(post_id):
	...
	return render_template('comments.html')
```
	* JSON 数据, json 数据可以在 JavaScript 中操作
```
@app.route('/profile/<int:user_id>')
def get_profile(user_id):
	...
	return jsonify(username=username, bio=bio)
```
	* 空值. 可以返回空值,  将状态码指定为 204(表示无内容)
```
@app.route('/post/delete/<int:post_id>', method=['DELETE'])
def delete_post(post_id):
	...
	return '', 204

```
* AJAX 例子
```
from jinja2.utils import generate_lorem_ipsum

# AJAX
@app.route('/post')
def show_post():
    post_body = generate_lorem_ipsum(n=2)
    return '''
<h1>A very long post</h1>
<div class="body">%s</div>
<button id="load">Load More</button>
<script src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
<script type="text/javascript">
$(function() {
    $('#load').click(function() {
        $.ajax({
            url: '/more',
            type: 'get',
            success: function(data){
                $('.body').append(data);
            }
        })
    })
})
</script>''' % post_body


@app.route('/more')
def load_post():
    return generate_lorem_ipsum(n=1)

```

* HTTP 服务器端推送
