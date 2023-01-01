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


# 模板
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