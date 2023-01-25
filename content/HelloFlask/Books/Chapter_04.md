# 第四章 表单

# 4.1 HTML 表单
* 最简单的表单
```
<form method="post">
    <label for="username">Username</label><br>
    <input type="text" name="username" placeholder="Héctor Rivera"><br>
    <label for="password">Password</label><br>
    <input type="password" name="password" placeholder="19001130"><br>
    <input id="remember" name="remember" type="checkbox" checked>
    <label for="remember"><small>Remember me</small></label><br>
    <input type="submit" name="submit" value="Log in">
</form>
```
* WTForms 是一个用 Python 编写的表单库.

# 4.2 使用 Flask-WTF 处理表单
* Flask-WTF 集成了 WTForms
* 安装 flask-wtf, email-validator
* Flask-WTF 默认为每个表单启用 CSRF 保护, 它会为我们自动生成和验证 CSRF 令牌, 因此需要设置密钥, 程序使用密钥对令牌进行签名
```
app.secret_key = 'secret_string'
```
* WTForms 导入 Form 基类
* 常用的 WTForms 字段
    - BooleanField: 复选框, 处理为 True 或 False
    - DateField: 文本字段, 处理为 datetime.date 对象
    - DateTimeField: 文本字段, 处理为 datetime.datetime 对象
    - FileField: 文件上传字段
    - FloatField: 浮点数字段, 值会被处理为浮点型
    - IntegerField: 整数字段, 值会被处理为整型
    - RadioField: 一组单选按钮
    - SelectField: 下拉列表
    - SeleteMultipleField: 多选下拉列表
    - SubmitField: 提交按钮
    - StringField: 文本按钮
    - HiddenField: 隐藏文本字段
    - PasswordField: 密码文本字段
    - TextAreaField: 多行文本字段
* 实例化字段类常用参数
    - label: 字段标签 <label> 的值, 也就是渲染后显示在输入字段前的文字
    - render_kw: 用来设置对应的 HTML <input>标签的属性, 比如传入 {'placeholder':'Your Name'}
    - validators: 一个列表, 包含一系列验证器, 会在表单提交后被逐一调用验证表单数据
    - default: 字符串或可调用对象, 用来为表单字段设置默认值
* 验证器从 wtforms.validators 模块中导入, 常用的 WTForms 验证器
    - DataRequired(message=None): 验证数据是否有效
    - Email(message=None): 验证 Email 地址
    - EqualTo(fieldname, message=None): 验证两个字段值是否相同
    - InputRequired(message=None): 验证是否有数据
    - Length(min=-1, max=-1, message=None): 验证输入值长度是否在给定范围内
    - NumberRange(min=None,max=None,message=None): 验证数字是否在给定范围内
    - Optional(strip_whitespace=True): 允许输入值为空,并跳过其他验证
    - Regexp(regex, flags=0, message=None): 使用正则表达式验证输入值
    - URL(require_tld=True, message=None): 验证 URL
    - AnyOf(values, message=None, values_formatter=None): 确保输入值在可选值列表中
    - NoneOf(values, message=None, values_formatter=None): 确保输入值不在可选值列表中
* 上面的 message 参数用来传入自定义错误消息, 如没有设置则使用内置的英文错误消息.
```
name = StringField('Your Name', validators=[DataRequired(message=u'名字不能为空!')])
```
* FlaskForm 类继承自 Form 类
```
class LoginForm(FlaskForm):
    username = StringField('Username', validators=[DataRequired()])
    password = PasswordField('Password', validators=[DataRequired(), Length(8, 128)])
    remember = BooleanField('Remember me')
    submit = SubmitField('Log in')
```
* 配置键 WTF_CSRF_ENABLED 用来设置是否开启 CSRF 保护, 默认为 True. Flask-WTF 会自动在实例化表单类时添加一个包含 CSRF 令牌值得隐藏字段, 字段名为 csrf_token
* 给表单添加额外的属性
    - 使用 render_kw 设置属性
```
username = StringField('Username', render_kw={'placeholder': 'Your Username'})
```
    - 输出结果
```
<input type="text" id="username" name="username" placeholder="Your Username">
```
    - 通过调用字段时传入, 通过添加括号使用关键字参数得形式也可以传入字段额外的 HTML 属性
```
>>> form.username(style='width: 200px;', class_='bar')
u'<input class="bar" id="username" name="username" style="width: 200px;" type="text">'
```
    - class_ 代替 class, 因为后者是python的关键保留字
* 传入表单类实例
```
@app.route('/basic', methods=['GET', 'POST'])
def basic():
    form = LoginForm()
    return render_template('basic.html', form=form)
```

# 模板
```
<form method="post">
    {{ form.csrf_token }}
    {{ form.username.label }}<br>
    {{ form.username }}<br>
    {{ form.password.label }}<br>
    {{ form.password }}<br>
    {{ form.remember }}{{ form.remember.label }}<br>
    {{ form.submit }}<br>
</form>
```
* Flask-WTF 为表单类实例提供了一个 form.hidden_tag() 方法, 其会依次渲染表单中所有的隐藏字段, 如 csrf_token 字段
* 通过 render_kw 实现 Bootstrap 风格的表单

## 4.3 处理表单数据
* 从获取数据到保存数据大致经历以下步骤:
    - 解析请求, 获取表单数据
    - 对数据进行必要的转换, 比如将勾选框的值转换成Python的布尔值
    - 验证数据是否符合要求, 同时验证 CSRF 令牌
    - 如果验证未通过则需要生成错误消息，并在模板中显示错误消息。
    - 如果通过验证，就把数据保存到数据库或做进一步处理
### 4.3.1 提交表单
* HTML表单中控制提交行为的属性
    - action: 当前 URL, 即页面对应的 URL
    - method: get, 请求方法, 仅支持 get 和 post
    - enctype: application/x-www-form-urlencoded, 表单数据的编码类型
* GET方式仅适用于长度不超过3000个字符，且不包含敏感信息的表单。
* 出于安全的考虑，我们一般使用POST方法提交表单。
* Flask为路由设置默认监听的HTTP方法为GET。为了支持接收表单提交发送的POST请求，我们必须在app.route（）装饰器里使用methods关键字为路由指定HTTP方法
```
@app.route('/', methods=['GET', 'POST'])
def basic():
    form = LoginForm()
    return render_template('basic.html', form=form)
```

### 4.3.2 验证表单数据
* 使用HTML5内置的验证属性即可实现基本的客户端验证（type、required、min、max、accept等）
```
<input type="text" name="username" required>
```
    - 如果用户没有输入,则弹出浏览器内置错误.
    - 和其他附加HTML属性相同, 我们可以在定义表单时通过 render_kw 传入这些属性, 或是在渲染表单时传入. 类似 required 这类布尔值属性, 值可以为空或是任意 ASCII 字符.
```
{{ form.username(required='') }}
```
* 可以使用各种 JavaScript 表单验证库, 如 jQuery Validation Plugin, Parsley.js, Bootstrap Validator.
* Flask 程序使用 WTForms 实现服务器端验证.
* WTForms 验证表单字段的方式是在实例化表单类时传入表单数据, 然后对表单实例调用 validate() 方法. 其会逐个对字段调用字段实例化时定义的验证器, 返回表示验证结果的布尔值. 如果验证失败, 就把错误消息存储到表单实例的 errors 属性对应的字典中.
```
>>> from wtforms import Form, StringField, PasswordField, BooleanField 
>>> from wtforms.validators import DataRequired, Length                
>>> class LoginForm(Form):                                             
...     username = StringField('Username', validators=[DataRequired()])
...     password = PasswordField('Password', validators=[DataRequired(), Length(8, 128)])
>>> form = LoginForm(username='', password='123')                      
>>> form.data  # 表单数据字典
{'username': '', 'password': '123'}
>>> form.validate()
False
>>> form.errors  # 错误消息字典
{'username': [u'This field is required.'], 'password': [u'Field must be at least 6 characters long.']}
>>> form2 = LoginForm(username='greyli', password='123456')            
>>> form2.data
{'username': 'greyli', 'password': '123456'}
>>> form2.validate()
True
>>> form2.errors
{}
```
* 使用 Flask-WTF 时, 表单类继承的 FlaskForm 基类默认会从 request.form 获取表单数据, 不需要手动传入.
* 通过 POST 方法提的表单, 其数据会被Flask解析为一个字典, 可以通过 request.form 获取, 使用 GET 方法提交的表单数据同样会被解析为字典, 不过要通过 request.args 获取.
* 因为现在的 basic_form 视图同时接收两种类型的请求: GET请求和POST请求. 我们根据请求方法的不同执行不同的代码. 具体来说, 首先实例化表单, 如果是 GET 请求, 那么就渲染模板, 如果是POST请求, 那就调用 validate() 方法验证表单数据.
* request.method 属性可以得到请求的 HTTP 方法.
```
from flask import request
...
@app.route('/basic', methods=['GET', 'POST'])
def basic():
    form = LoginForm()  # GET + POST
    if request.method == 'POST' and form.validate():
        ...  # 处理POST请求
    return render_template('forms/basic.html', form=form)  # 处理GET请求
```
    - if 语句等价于
```
if 用户提交表单 and 数据通过验证:
    获取表单数据并保存
```
* WTForms会自动对CSRF令牌字段进行验证, 如果没有渲染该字段会导致验证出错, 错误消息为“CSRF token is missing”.
* Flask-WTF提供的 validate_on_submit() 方法合并了这两个操作, 因此上面的代码可以简化为:
```
@app.route('/basic', methods=['GET', 'POST'])
def basic():
    form = LoginForm()
    if form.validate_on_submit():
        ... 
    return render_template('basic.html', form=form)
```
* 除了POST方法，如果请求的方法是PUT、PATCH和DELETE方法，form.validate_on_submit() 也会验证表单数据。
* 如果form.validate_on_submit（）返回True，则表示用户提交了表单，且表单通过验证，那么我们就可以在这个if语句内获取表单数据
```
from flask import Flask, render_template, redirect, url_for, flash
...
@app.route('/basic', methods=['GET', 'POST'])
def basic():
    form = LoginForm()
    if form.validate_on_submit():
        username = form.username.data
        flash('Welcome home, %s!' % username)
        return redirect(url_for('index'))
    return render_template('basic.html', form=form)
```
* 表单类的data属性是一个匹配所有字段与对应数据的字典，我们一般直接通过“form.字段属性名.data”的形式来获取对应字段的数据。例如，form.username.data返回username字段的值。
* 尽量不要让提交表单的POST请求作为最后一个请求。这就是为什么我们在处理表单后返回一个重定向响应，这会让浏览器重新发送一个新的GET请求到重定向的目标URL。最终，最后一个请求就变成了GET请求。这种用来防止重复提交表单的技术称为PRG（Post/Redirect/Get）模式，即通过对提交表单的POST请求返回重定向响应将最后一个请求转换为GET请求。
* 如果form.validate_on_submit（）返回False，那么说明验证没有通过。对于验证未通过的字段，WTForms会把错误消息添加到表单类的errors属性中，这是一个匹配作为表单字段的类属性到对应的错误消息列表的字典。我们一般会直接通过字段名来获取对应字段的错误消息列表，即“form.字段名.errors”。比如，form.name.errors返回name字段的错误消息列表。
```
<form method="post">
    {{ form.csrf_token }}
    {{ form.username.label }}<br>
    {{ form.username() }}<br>
    {% for message in form.username.errors %}
        <small class="error">{{ message }}</small><br>
    {% endfor %}
    {{ form.password.label }}<br>
    {{ form.password }}<br>
    {% for message in form.password.errors %}
        <small class="error">{{ message }}</small><br>
    {% endfor %}
    {{ form.remember }}{{ form.remember.label }}<br>
    {{ form.submit }}<br>
</form>
```
* InputRequired验证器和DataRequired很相似，但InputRequired仅验证用户是否有输入，而不管输入的值是否有效。

## 4.4 表单进阶实践
### 4.4.1 设置错误消息语言
* WTForms内置了多种语言的错误消息，如果你想改变内置错误消息的默认语言，可以通过自定义表单基类实现（Flask-WTF版本>0.14.2）。
```
from flask_wtf import FlaskForm
app = Flask(__name__)
app.config['WTF_I18N_ENABLED'] = False
class MyBaseForm(FlaskForm):
    class Meta:
        locales = ['zh']
class HelloForm(MyBaseForm):
    name = StringField('Name', validators=[DataRequired()])
    submit = SubmitField()
```
* 首先，我们需要将配置变量WTF_I18N_ENABLED设为False，这会让Flask-WTF使用WTForms内置的错误消息翻译。然后我们需要在自定义基类中定义Meta类，并在locales列表中加入简体中文的地区字符串。
* 也可以在实例化表单类时通过meta关键字传入locales值
```
form = MyForm(meta={'locales': ['en_US', 'en']})
```
### 4.4.2 使用宏渲染表单
* 在模板中渲染表单的工作:
    - 调用字段属性，获取<input>定义
    - 调用对应的label属性，获取<label>定义
    - 渲染错误消息
* 创建宏节省代码
```
{% macro form_field(field) %}
    {{ field.label }}<br>
    {{ field(**kwargs) }}<br>
    {% if field.errors %}
        {% for error in field.errors %}
            <small class="error">{{ error }}</small><br>
        {% endfor %}
    {% endif %}
{% endmacro %}
```
* 这个form_field() 宏接收表单类实例的字段属性和附加的关键字参数作为输入，返回包含<label>标签、表单字段、错误消息列表的HTML表单字段代码。
```
{% from 'macros.html' import form_field %}
...
<form method="post">
    {{ form.csrf_token }}
    {{ form_field(form.username)}}<br>
    {{ form_field(form.password) }}<br>
    ...
</form>
```
### 4.4.3 自定义验证器
#### 1. 行内验证器
```
from wtforms import IntegerField, SubmitField
from wtforms.validators import ValidationError
class FortyTwoForm(FlaskForm):
    answer = IntegerField('The Number')
    submit = SubmitField()
    def validate_answer(form, field):
        if field.data != 42:
            raise ValidationError('Must be 42.')
```
* 当表单类中包含以“validate_字段属性名”形式命名的方法时，在验证字段数据时会同时调用这个方法来验证对应的字段，这也是为什么表单类的字段属性名不能以validate开头。验证方法接收两个位置参数，依次为form和field，前者为表单类实例，后者是字段对象，我们可以通过field.data获取字段数据，这两个参数将在验证表单时被调用传入。验证出错时抛出从wtforms.validators模块导入的ValidationError异常，传入错误消息作为参数。因为这种方法仅用来验证特定的表单类字段，所以又称为行内验证器（in-line validator）。
#### 2. 全局验证器
```
from wtforms.validators import ValidationError
def is_42(form, field):
    if field.data != 42:
        raise ValidationError('Must be 42')
class FortyTwoForm(FlaskForm):
    answer = IntegerField('The Number', validators=[is_42])
    submit = SubmitField()
```
* 当使用函数定义全局的验证器时，我们需要在定义字段时在validators列表里传入这个验证器。因为在validators列表中传入的验证器必须是可调用对象，所以这里传入了函数对象，而不是函数调用。
```
from wtforms.validators import ValidationError
def is_42(message=None):
    if message is None:
        message = 'Must be 42.'
    def _is_42(form, field):
        if field.data != 42:
            raise ValidationError(message)
    return _is_42
class FortyTwoForm(FlaskForm):
    answer = IntegerField('The Number', validators=[is_42()])
    submit = SubmitField()
```
* 在现在的is_42（）函数中，我们创建了另一个_is_42（）函数，这个函数会被作为可调用对象返回。is_42（）函数接收的message参数用来传入自定义错误消息，默认为None，如果没有设置就使用内置消息。在validators列表中，这时需要传入的是对工厂函数is_42（）的调用。

### 4.4.4 文件上传
* 在HTML中，渲染一个文件上传字段只需要将<input>标签的type属性设为 file，即<input type="file">。这会在浏览器中渲染成一个文件上传字段，单击文件选择按钮会打开文件选择窗口，选择对应的文件后，被选择的文件名会显示在文件选择按钮旁边。
* 在服务器端，可以和普通数据一样获取上传文件数据并保存。不过我们需要考虑安全问题，文件上传漏洞也是比较流行的攻击方式。除了常规的CSRF防范，我们还需要重点注意下面的问题：
    - 验证文件类型。
    - 验证文件大小。
    - 过滤文件名。
1. 定义上传表单
* 在Python表单类中创建文件上传字段时，我们使用扩展Flask-WTF提供的FileField类，它继承WTForms提供的上传字段FileField，添加了对Flask的集成。
* 创建上传表单
```
from flask wtf.file import FileField, FileRequired, FileAllowed
class UploadForm(FlaskForm):
    photo = FileField('Upload Image', validators=[FileRequired(), FileAllowed(['jpg', 'jpeg', 'png', 'gif'])])
    submit = SubmitField()
```
* Flask-WTF提供的上传文件验证器
    - FileRequired
    - FileAllowed
* 扩展Flask-Uploads（https://github.com/maxcountryman/flask-uploads）内置了在Flask中实现文件上传的便利功能。
* Flask-WTF提供的 FileAllowed() 也支持传入Flask-Uploads中的上传集对象（Upload Set）作为 upload_set参数的值。另外，同类的扩展还有 Flask-Transfer(https://github.com/justanr/Flask-Transfer)。