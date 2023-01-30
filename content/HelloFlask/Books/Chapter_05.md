# 第5章 数据库
* 常见的DBMS有MySQL、PostgreSQL、SQLite、MongoDB等。为了便于理解，我们可以把数据库看作一个大仓库，仓库里有一些负责搬运货物（数据）的机器人，而DBMS就是操控机器人搬运货物的程序。
* 本章新涉及的Python库如下所示：
    - SQLAlchemy（1.2.7）
      - 主页：http://www.sqlalchemy.org/
      - 源码：https://github.com/zzzeek/sqlalchemy
      - 文档：http://docs.sqlalchemy.org/en/latest/
    - Flask-SQLAlchemy（2.3.2）
      - 主页：https://github.com/mitsuhiko/flask-sqlalchemy
      - 文档：http://flask-sqlalchemy.pocoo.org/2.3/
    - Alembic（0.9.9）
      - 主页：https://bitbucket.org/zzzeek/alembic
      - 文档：http://alembic.zzzcomputing.com/en/latest/
    - Flask-Migrate（2.1.1）
      - 主页：https://github.com/miguelgrinberg/Flask-Migrate
      - 文档：https://flask-migrate.readthedocs.io/en/latest/
* 在第一次运行新的示例程序后按下Crtl+F5或Shift+F5清除缓存。

## 5.1 数据库的分类
* 数据库一般分为两种，SQL（Structured Query Language，结构化查询语言）数据库和NoSQL（Not Only SQL，泛指非关系型）数据库。
### 5.1.1 SQL
* SQL数据库指关系型数据库，常用的SQL DBMS主要包括SQL Server、Oracle、MySQL、PostgreSQL、SQLite等。关系型数据库使用表来定义数据对象，不同的表之间使用关系连接。
* 几个基本概念：
    - 表（table）：存储数据的特定结构。
    - 模式（schema）：定义表的结构信息。
    - 列/字段（column/field）：表中的列，存储一系列特定的数据，列组成表。
    - 行/记录（row/record）：表中的行，代表一条记录。
    - 标量（scalar）：指的是单一数据，与之相对的是集合（collection）。

### 5.1.2 NoSQL
* 最常用的两种NoSQL数据库如下所示：
    - 文档存储（document store）, 文档存储是NoSQL数据库中最流行的种类，它可以作为主数据库使用。文档存储使用的文档类似SQL数据库中的记录，文档使用类JSON格式来表示数据。常见的文档存储DBMS有MongoDB、CouchDB等。表5-1的身份信息表中的第一条记录使用文档可以表示为：
```
{
    id: 1,
    name: "Nick",
    sex: "Male"
    occupation: "Journalist"
}
```
    - 键值对存储（key-value store）, 键值对存储在形态上类似Python中的字典，通过键来存取数据，在读取上非常快，通常用来存储临时内容，作为缓存使用。常见的键值对DBMS有Redis、Riak等，其中Redis不仅可以管理键值对数据库，还可以作为缓存后端（cachebackend）和消息代理（message broker）。
* 还有列存储（column store，又被称为宽列式存储）、图存储（graph store）等类型的NoSQL数据库，这里不再展开介绍。

## 5.2 ORM魔法
* 尽管ORM非常方便，但如果你对SQL相当熟悉，那么自己编写SQL代码可以获得更大的灵活性和性能优势。就像是使用IDE一样，ORM对初学者来说非常方便，但进阶以后你也许会想要自己掌控一切。
* ORM把底层的SQL数据实体转化成高层的Python对象，这样一来，你甚至不需要了解SQL，只需要通过Python代码即可完成数据库操作，ORM主要实现了三层映射关系：
    - 表→Python类。
    - 字段（列）→类属性。
    - 记录（行）→类实例。
* 比如，我们要创建一个contacts表来存储留言，其中包含用户名称和电话号码两个字段。在SQL中，下面的代码用来创建这个表：
```
CREATE TABLE contacts(
    name varchar(100) NOT NULL,
    phone_number varchar(32),
);
```
* 如果使用ORM，我们可以使用类似下面的Python类来定义这个表：
```
from foo_orm import Model, Column, String
class Contact(Model):
    __tablename__ = 'contacts'
    name = Column(String(100), nullable=False)
    phone_number = Column(String(32))
```
* 要向表中插入一条记录, 需要使用下面的 SQL 语句:
```
INSERT INTO contacts(name, phone_number) 
VALUES('Grey Li', '12345678');
```
* 使用ORM则只需要创建一个Contact类的实例，传入对应的参数表示各个列的数据即可。下面的代码和使用上面的SQL语句效果相同：
```
contact = Contact(name='Grey Li', phone_number='12345678')
```
* 使用Python实现的ORM有SQLAlchemy、Peewee、PonyORM等。

## 5.3 使用Flask-SQLAlchemy管理数据库
```
pipenv install flask-sqlalchemy
```
```
from flask import Flask
from flask_sqlalchemy import SQLAlchemy
app = Flask(__name__)
db = SQLAlchemy(app)
```
