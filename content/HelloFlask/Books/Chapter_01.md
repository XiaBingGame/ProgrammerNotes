# 第一章 搭建开发环境
- HTTP 服务器端推送

* 要点:
	- git
	- pipenv
	- app.route() 装饰器
	- python-dotenv, .env(私密), .flaskenv(公开)
	- --host --port
	- app.config
	- url_for
* git tag -n  查看所有标签
* git reset --hard 撤销改动
* git diff 比较两个版本
* git fetch -all / git fetch --tags / git reset --hard origin/master 更新
* pipenv --version 查看 pipenv 是否安装
* 工作目录下创建虚拟环境 pipenv install
* 激活虚拟环境 pipenv shell
* 不激活虚拟环境运行 pipenv run python hello.py
* 一般情况 pip 搭配一个 requirements.txt 来记录项目库依赖
* pipfile 替代难于管理的 requirements.txt, 以后通过 pipenv install 可以自动安装
* pipenv graph 查看当前环境下的依赖情况, pip list 查看依赖列表
* pipenv install 包名 --- 虚拟环境下安装包
* app = Flask(__name__) 实例化该类, __name__ 为模块名
* app.route() 装饰器, 传入 URL 规则作为参数, 让 URL 与函数建立关联, 这个过程为注册路由(route), 路由管理URL和函数之间的映射, 函数为视图函数(view function)
* route() 装饰器的第一个参数是 URL 规则, 以斜杠(/)开始
* 可以多个 URL 绑定到多个视图函数上
```
@app.route('/hi')
@app.route('/hello')
def say_hello():
	return '<h1>Hello, Flask!</h1>'
```
* 添加变量 "<变量名>"
```
@app.route('/greet/<name>')
def greet(name):
	return '<h1>Hello, %s!</h1>' % name
```
* 使用 defaults 参数设置 URL 变量的默认值
```
@app.route('/greet', defaults={'name':Programmer})
@app.route('/greet/<name>')
def greet(name):
	return '<h1>Hello, %s!</h1>' % name
```
* 等同于
```
@app.route('/greet')
@app.route('/greet/<name>')
def greet(name='Programmer'):
	return '<h1>Hello, %s!</h1>' %s name
```
* flask run / pipenv run flask run 启动 flask 内置的开发服务器, 默认 http://127.0.0.1:5000/
* flask 自动探测规则:
	- 当前目录查找 app.py 和 wsgi.py 模块, 并从中查找 app 或 application 的程序实例
	- 环境变量 FLASK_APP 对应的值查找 app 或 application 的实例, FLASK_APP 为包含模块的名称
	- 如安装了 python-dotenv, 自动从 .flaskenv 文件和 .env 文件中加载环境变量
* python-dotenv 管理项目的环境变量, 安装: pipenv install python-dotenv, 生成文件 .env(私密) 和 .flaskenv(公开), 键对形式, # 表示注释
```
SOME_VAR = 1
# 注释
FOO = "BAR"
```
* 一般 .env 不要提交到 git 仓库中
* pycharm 的 flask 运行配置支持
* --host 选项将主机设置为 0.0.0.0 使其对外可见
```
$ flask run --host=0.0.0.0
```
* ngrok、Localtunnel 等内网穿透/端口转发工具
* 修改默认端口
```
flask run --port=8000
```
* FLASK_RUN_HOST 和 FLASK_RUN_PORT 可以设置host和端口
* FLASK_ENV 设置环境, 默认为 production, 可以设置 development, 可在 .flaskenv 写入
* FLASK_DEBUG: 1 开启, 0 关闭调试
* 默认使用 Werkzeug 内置的 stat 重载器, 安装用于监测文件变动的 python 库 watchdog, 安装后 Werkzeug 自动用它监测文件变动
```
$ pipenv install watchdog --dev
```
	- --dev 将这个包声明为开发依赖
* flask shell 启动
* flask 扩展, 使用 flask 提供的 API 编写的 Python 库
```
from flask import Flask
from flask_foo import Foo

app = Flask(__name__)
foo = Foo(app)
```
* Flask对象的app.config属性作为统一的接口来设置和获取配置变量, 其指向的 Config 类是字典的子类
	- 添加一个键值, 注意配置名称必须是全大写形式.
```
app.config['ADMIN_NAME'] = 'Peter'
```
	- update() 一次加载多个值
```
app.config.update(
	TESTING=True,
	SECRET_KEY='_5#YF4Q8z\n\xec]'
)
```
* 用 url_for() 函数获取 URL, 当路由中定义的URL规则被修改时, 通过该函数返回正确的URL. url_for() 第一个参数为端点值(endpoint). 端点用来标记一个视图函数以及对应的URL规则, 端点的默认值为视图函数的名称
```
@app.route('/')
def index():
	return 'Hello Flask!'
```
	- 端点即视图函数的名称为 index, 调用 url_for('index')即可以获取对应的URL, 即"/"
```
@app.route('/hello/<name>')
def greet(name):
	return 'Hello %s!' % name
```
	- url_for('greet', name='Jack') 为 "/hello/Jack"
* url_for 设置参数 _external 为 True, 即可以得到完整的 URL
* 创建任意一个函数, 并为其添加 app.cli.command() 装饰器, 我们就可以注册一个 flask 命令
```
@app.cli.command()
def hello():
	click.echo('Hello, Human!')
```
	- 函数名称即为命令名称, 使用 flask hello 命令触发函数
	- 装饰器可以传入参数来设置命令名称, 如 app.cli.command('say-hello'), 则命令为 flask say-hello
* flask --help 查看 flask 的命令帮助文档
* 默认情况下, 模板文件存放在项目根目录的 templates 文件夹中, 静态文件存放在 static 文件夹下, 这两个文件夹需要和包含程序实例的模块处于同一个目录下
```
hello/
	- templates/
	- static/
	- app.py
```
* 不推荐从CDN下载资源. CDN指分布式服务器系统. 服务商吧你需要的资源存储在分布于不同地理位置的多个服务器,它会根据用户的地理位置就近分配服务器提供服务. 对于开源的CSS和JavaScript库,CDN提供商通常会免费提供服务.
* MVC架构中,程序被分为三个组件: 数据处理(Model), 用户界面(View), 交互逻辑(Controller)
