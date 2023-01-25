# 第一章 开发环境配置
## 1.2 请求库的安装
* wheel 安装办法
```
pip3 install wheel
pip3 install requests-2.17.3-py2.py3-none-any.whl
```
* 源码安装
```
git clone git://github.com/kennethreitz/requests.git

cd requests
python3 setup.py install
```
* 安装自动化测试工具: selenium
* 安装 ChromeDriver, GeckoDriver
```
    GitHub：https://github.com/mozilla/geckodriver
    下载地址：https://github.com/mozilla/geckodriver/releases
```
* PhantomJS 是一个无界面的、可脚本编程的 WebKit 浏览器引擎，它原生支持多种 Web 标准：DOM 操作、CSS 选择器、JSON、Canvas 以及 SVG. Selenium 支持 PhantomJS，这样在运行的时候就不会再弹出一个浏览器了。而且 PhantomJS 的运行效率也很高，还支持各种参数配置，使用非常方便
```
    官方网站：http://phantomjs.org
    官方文档：http://phantomjs.org/quick-start.html
    下载地址：http://phantomjs.org/download.html
    API 接口说明：http://phantomjs.org/api/command-line.html
```
* aiohttp 就是这样一个提供异步 Web 服务的库，从 Python 3.5 版本开始，Python 中加入了 async/await 关键字，使得回调的写法更加直观和人性化。aiohttp 的异步操作借助于 async/await 关键字的写法变得更加简洁，架构更加清晰。使用异步请求库进行数据抓取时，会大大提高效率，下面我们来看一下这个库的安装方法。
```
    官方文档：http://aiohttp.readthedocs.io/en/stable
    GitHub：https://github.com/aio-libs/aiohttp
    PyPI：https://pypi.python.org/pypi/aiohttp

pip3 install aiohttp
```
## 1.3 解析库的安装
* lxml 是 Python 的一个解析库，支持 HTML 和 XML 的解析，支持 XPath 解析方式，而且解析效率非常高。本节中，我们了解一下 lxml 的安装方式，这主要从 Windows、Linux 和 Mac 三大平台来介绍。
```
    官方网站：http://lxml.de
    GitHub：https://github.com/lxml/lxml
    PyPI：https://pypi.python.org/pypi/lxml

pip3 install lxml
```
* Beautiful Soup 是 Python 的一个 HTML 或 XML 的解析库，我们可以用它来方便地从网页中提取数据。它拥有强大的 API 和多样的解析方式，本节就来了解下它的安装方式。
```
    官方文档：https://www.crummy.com/software/BeautifulSoup/bs4/doc
    中文文档：https://www.crummy.com/software/BeautifulSoup/bs4/doc.zh
    PyPI：https://pypi.python.org/pypi/beautifulsoup4

pip3 install beautifulsoup4
```
* pyquery 同样是一个强大的网页解析工具，它提供了和 jQuery 类似的语法来解析 HTML 文档，支持 CSS 选择器，使用非常方便。本节中，我们就来了解一下它的安装方式。
```
    GitHub：https://github.com/gawel/pyquery
    PyPI：https://pypi.python.org/pypi/pyquery
    官方文档：http://pyquery.readthedocs.io

pip3 install pyquery
```
* tesserocr 是 Python 的一个 OCR 识别库，但其实是对 tesseract 做的一层 Python API 封装，所以它的核心是 tesseract。因此，在安装 tesserocr 之前，我们需要先安装 tesseract。
```
    tesserocr GitHub：https://github.com/sirfz/tesserocr
    tesserocr PyPI：https://pypi.python.org/pypi/tesserocr
    tesseract 下载地址：http://digi.bib.uni-mannheim.de/tesseract
    tesseract GitHub：https://github.com/tesseract-ocr/tesseract
    tesseract 语言包：https://github.com/tesseract-ocr/tessdata
    tesseract 文档：https://github.com/tesseract-ocr/tesseract/wiki/Documentation

然后安装
pip3 install tesserocr pillow
```

## 1.4 数据库的安装
* MySQL
```
    官方网站：https://www.mysql.com/cn
    下载地址：https://www.mysql.com/cn/downloads
    中文教程：http://www.runoob.com/mysql/mysql-tutorial.html
```
* MongoDB 是由 C++ 语言编写的非关系型数据库，是一个基于分布式文件存储的开源数据库系统，其内容存储形式类似 JSON 对象，它的字段值可以包含其他文档、数组及文档数组，非常灵活。
```
    官方网站：https://www.mongodb.com
    官方文档：https://docs.mongodb.com
    GitHub：https://github.com/mongodb
    中文教程：http://www.runoob.com/mongodb/mongodb-tutorial.html
```
  * 可视化工具: 这里推荐一个可视化工具 RoboMongo/Robo 3T，它使用简单，功能强大，官方网站为 https://robomongo.org/，三大平台都支持，下载链接为 https://robomongo.org/download。另外，还有一个简单易用的可视化工具 —— Studio 3T，它同样具有方便的图形化管理界面，官方网站为 https://studio3t.com，同样支持三大平台，下载链接为 https://studio3t.com/download/。
* Redis 是一个基于内存的高效的非关系型数据库，本节中我们来了解一下它在各个平台的安装过程。
```
    官方网站：https://redis.io
    官方文档：https://redis.io/documentation
    中文官网：http://www.redis.cn
    GitHub：https://github.com/antirez/redis
    中文教程：http://www.runoob.com/redis/redis-tutorial.html
    Redis Desktop Manager：https://redisdesktop.com
    Redis Desktop Manager GitHub：https://github.com/uglide/RedisDesktopManager
```
## 1.5 存储库的安装
* PyMySQL
```
    GitHub：https://github.com/PyMySQL/PyMySQL
    官方文档：http://pymysql.readthedocs.io/
    PyPI：https://pypi.python.org/pypi/PyMySQL

pip3 install pymysql
```
* PyMongo
```
    GitHub：https://github.com/mongodb/mongo-python-driver
    官方文档：https://api.mongodb.com/python/current/
    PyPI：https://pypi.python.org/pypi/pymongo

pip3 install pymongo
```
* redis-py 库
```
    GitHub：https://github.com/andymccurdy/redis-py
    官方文档：https://redis-py.readthedocs.io/

pip3 install redis
```
* RedisDump 是一个用于 Redis 数据导入 / 导出的工具，是基于 Ruby 实现的，所以要安装 RedisDump，需要先安装 Ruby。
```
    GitHub：https://github.com/delano/redis-dump
    官方文档：http://delanotes.com/redis-dump
    Ruby:  http://www.ruby-lang.org/zh_cn/documentation/installation

gem install redis-dump
```
## 1.6 Web 库的安装
* Flask
```
    GitHub：https://github.com/pallets/flask
    官方文档：http://flask.pocoo.org
    中文文档：http://docs.jinkan.org/docs/flask
    PyPI：https://pypi.python.org/pypi/Flask
```
    - 利用 Flask + Redis 维护动态代理池和 Cookies 池
* Tornado 是一个支持异步的 Web 框架，通过使用非阻塞 I/O 流，它可以支撑成千上万的开放连接，效率非常高。
```
    GitHub：https://github.com/tornadoweb/tornado
    PyPI：https://pypi.python.org/pypi/tornado
    官方文档：http://www.tornadoweb.org
    
pip3 install tornado
```
    - 利用 Tornado + Redis 来搭建一个 ADSL 拨号代理池。
## 1.7 App 爬取相关库的安装
* 爬虫也可以抓取 App 的数据。App 中的页面要加载出来，首先需要获取数据，而这些数据一般是通过请求服务器的接口来获取的。由于 App 没有浏览器这种可以比较直观地看到后台请求的工具，所以主要用一些抓包技术来抓取数据。
* 抓包工具有 Charles、mitmproxy 和 mitmdump。一些简单的接口可以通过 Charles 或 mitmproxy 分析，找出规律，然后直接用程序模拟来抓取了。但是如果遇到更复杂的接口，就需要利用 mitmdump 对接 Python 来对抓取到的请求和响应进行实时处理和保存。
* Appium: 它可以像 Selenium 一样对 App 进行自动化控制，如自动化模拟 App 的点击、下拉等操作。
* Charles
```
    官方网站：https://www.charlesproxy.com
    下载链接：https://www.charlesproxy.com/download
```
    - Charles 是收费软件，不过可以免费试用 30 天。如果试用期过了，其实还可以试用，不过每次试用不能超过 30 分钟，启动有 10 秒的延时，但是完整的软件功能还是可以使用的，所以还算比较友好。
    - 安装证书
* mitmproxy 是一个支持 HTTP 和 HTTPS 的抓包程序，类似 Fiddler、Charles 的功能，只不过它通过控制台的形式操作。此外，mitmproxy 还有两个关联组件，一个是 mitmdump，它是 mitmproxy 的命令行接口，利用它可以对接 Python 脚本，实现监听后的处理；另一个是 mitmweb，它是一个 Web 程序，通过它以清楚地观察到 mitmproxy 捕获的请求。
```
    GitHub：https://github.com/mitmproxy/mitmproxy
    官方网站：https://mitmproxy.org
    PyPi：https://pypi.python.org/pypi/mitmproxy
    官方文档：http://docs.mitmproxy.org
    MitmDump 脚本：http://docs.mitmproxy.org/en/stable/scripting/overview.html
    下载地址：https://github.com/mitmproxy/mitmproxy/releases
    DockerHub：https://hub.docker.com/r/mitmproxy/mitmproxy

pip3 install mitmproxy
```

## 爬虫框架的安装
* 爬虫框架有 pyspider 和 Scrapy
* pyspider
```
    官方文档：http://docs.pyspider.org/
    PyPi：https://pypi.python.org/pypi/pyspider
    GitHub：https://github.com/binux/pyspider
    官方教程：http://docs.pyspider.org/en/latest/tutorial
    在线实例：http://demo.pyspider.org

pip3 install pyspider
```

## 1.8　爬虫框架的安装
* Scrapy
```
    官方网站：https://scrapy.org
    官方文档：https://docs.scrapy.org
    PyPi：https://pypi.python.org/pypi/Scrapy
    GitHub：https://github.com/scrapy/scrapy
    中文文档：http://scrapy-chs.readthedocs.io

lxml
pip3 install pyOpenSSL-17.2.0-py2.py3-none-any.whl
pip3 install Twisted-17.5.0-cp36-cp36m-win_amd64.whl
PyWin32
pip3 install Scrapy
```

## 1.9　部署相关库的安装
* Docker
```
    官方网站：https://www.docker.com
    GitHub：https://github.com/docker
    Docker Hub：https://hub.docker.com
    官方文档：https://docs.docker.com
    DaoCloud：http://www.daocloud.io
    中文社区：http://www.docker.org.cn
    中文教程：http://www.runoob.com/docker/docker-tutorial.html
    推荐书籍：https://yeasy.gitbooks.io/docker_practice
```
* 如果你的是 64 位 Windows 10 系统，那么推荐使用 Docker for Windows。此时直接从 Docker 官方网站下载最新的 Docker for Windows 安装包即可：https://docs.docker.com/docker-for-windows/install/
* Scrapyd
```
    GitHub：https://github.com/scrapy/scrapyd
    PyPi：https://pypi.python.org/pypi/scrapyd
    官方文档：https://scrapyd.readthedocs.io

pip3 install scrapyd
```

# 爬虫基础
## 2.1 HTTP 基本原理
* 请求方法
    - GET: 请求页面，并返回页面内容
    - HEAD: 类似于 GET 请求，只不过返回的响应中没有具体的内容，用于获取报头
    - POST: 大多用于提交表单或上传文件，数据包含在请求体中
    - PUT: 从客户端向服务器传送的数据取代指定文档中的内容
    - DELETE: 请求服务器删除指定的页面
    - CONNECT: 把服务器当作跳板，让服务器代替客户端访问其他网页
    - OPTIONS: 允许客户端查看服务器的性能
    - TRACE: 回显服务器收到的请求，主要用于测试或诊断
* 常用请求头信息
    - Accept：请求报头域，用于指定客户端可接受哪些类型的信息。
    - Accept-Language：指定客户端可接受的语言类型。
    - Accept-Encoding：指定客户端可接受的内容编码。
    - Host：用于指定请求资源的主机 IP 和端口号，其内容为请求 URL 的原始服务器或网关的位置。从 HTTP 1.1 版本开始，请求必须包含此内容。
    - Cookie：也常用复数形式 Cookies，这是网站为了辨别用户进行会话跟踪而存储在用户本地的数据。它的主要功能是维持当前访问会话。例如，我们输入用户名和密码成功登录某个网站后，服务器会用会话保存登录状态信息，后面我们每次刷新或请求该站点的其他页面时，会发现都是登录状态，这就是 Cookies 的功劳。Cookies 里有信息标识了我们所对应的服务器的会话，每次浏览器在请求该站点的页面时，都会在请求头中加上 Cookies 并将其发送给服务器，服务器通过 Cookies 识别出是我们自己，并且查出当前状态是登录状态，所以返回结果就是登录之后才能看到的网页内容。
    - Referer：此内容用来标识这个请求是从哪个页面发过来的，服务器可以拿到这一信息并做相应的处理，如做来源统计、防盗链处理等。
    - User-Agent：简称 UA，它是一个特殊的字符串头，可以使服务器识别客户使用的操作系统及版本、浏览器及版本等信息。在做爬虫时加上此信息，可以伪装为浏览器；如果不加，很可能会被识别出为爬虫。
    - Content-Type：也叫互联网媒体类型（Internet Media Type）或者 MIME 类型，在 HTTP 协议消息头中，它用来表示具体请求中的媒体类型信息。例如，text/html 代表 HTML 格式，image/gif 代表 GIF 图片，application/json 代表 JSON 类型，更多对应关系可以查看此对照表：http://tool.oschina.net/commons。
* Content-Type 内容
    - application/x-www-form-urlencoded 	表单数据
    - multipart/form-data 	表单文件上传
    - application/json 	序列化 JSON 数据
    - text/xml 	XML 数据
* 响应状态码
    - 100 	继续 	请求者应当继续提出请求。服务器已收到请求的一部分，正在等待其余部分
    - 101 	切换协议 	请求者已要求服务器切换协议，服务器已确认并准备切换
    - 200 	成功 	服务器已成功处理了请求
    - 201 	已创建 	请求成功并且服务器创建了新的资源
    - 202 	已接受 	服务器已接受请求，但尚未处理
    - 203 	非授权信息 	服务器已成功处理了请求，但返回的信息可能来自另一个源
    - 204 	无内容 	服务器成功处理了请求，但没有返回任何内容
    - 205 	重置内容 	服务器成功处理了请求，内容被重置
    - 206 	部分内容 	服务器成功处理了部分请求
    - 300 	多种选择 	针对请求，服务器可执行多种操作
    - 301 	永久移动 	请求的网页已永久移动到新位置，即永久重定向
    - 302 	临时移动 	请求的网页暂时跳转到其他页面，即暂时重定向
    - 303 	查看其他位置 	如果原来的请求是 POST，重定向目标文档应该通过 GET 提取
    - 304 	未修改 	此次请求返回的网页未修改，继续使用上次的资源
    - 305 	使用代理 	请求者应该使用代理访问该网页
    - 307 	临时重定向 	请求的资源临时从其他位置响应
    - 400 	错误请求 	服务器无法解析该请求
    - 401 	未授权 	请求没有进行身份验证或验证未通过
    - 403 	禁止访问 	服务器拒绝此请求
    - 404 	未找到 	服务器找不到请求的网页
    - 405 	方法禁用 	服务器禁用了请求中指定的方法
    - 406 	不接受 	无法使用请求的内容响应请求的网页
    - 407 	需要代理授权 	请求者需要使用代理授权
    - 408 	请求超时 	服务器请求超时
    - 409 	冲突 	服务器在完成请求时发生冲突
    - 410 	已删除 	请求的资源已永久删除
    - 411 	需要有效长度 	服务器不接受不含有效内容长度标头字段的请求
    - 412 	未满足前提条件 	服务器未满足请求者在请求中设置的其中一个前提条件
    - 413 	请求实体过大 	请求实体过大，超出服务器的处理能力
    - 414 	请求 URI 过长 	请求网址过长，服务器无法处理
    - 415 	不支持类型 	请求格式不被请求页面支持
    - 416 	请求范围不符 	页面无法提供请求的范围
    - 417 	未满足期望值 	服务器未满足期望请求标头字段的要求
    - 500 	服务器内部错误 	服务器遇到错误，无法完成请求
    - 501 	未实现 	服务器不具备完成请求的功能
    - 502 	错误网关 	服务器作为网关或代理，从上游服务器收到无效响应
    - 503 	服务不可用 	服务器目前无法使用
    - 504 	网关超时 	服务器作为网关或代理，但是没有及时从上游服务器收到请求
    - 505 	HTTP 版本不支持 	服务器不支持请求中所用的 HTTP 协议版本
* 响应头包含的内容
    - Date：标识响应产生的时间。
    - Last-Modified：指定资源的最后修改时间。
    - Content-Encoding：指定响应内容的编码。
    - Server：包含服务器的信息，比如名称、版本号等。
    - Content-Type：文档类型，指定返回的数据类型是什么，如 text/html 代表返回 HTML 文档，application/x-javascript 则代表返回 JavaScript 文件，image/jpeg 则代表返回图片。
    - Set-Cookie：设置 Cookies。响应头中的 Set-Cookie 告诉浏览器需要将此内容放在 Cookies 中，下次请求携带 Cookies 请求。
    - Expires：指定响应的过期时间，可以使代理服务器或浏览器将加载的内容更新到缓存中。如果再次访问时，就可以直接从缓存中加载，降低服务器负载，缩短加载时间。

## 2.2 网页基础
* 在 CSS 中，我们使用 CSS 选择器来定位节点。例如，div 节点的 id 为 container，那么就可以表示为 #container，其中 # 开头代表选择 id，其后紧跟 id 的名称。另外，如果我们想选择 class 为 wrapper 的节点，便可以使用.wrapper，这里以点（.）开头代表选择 class，其后紧跟 class 的名称。另外，还有一种选择方式，那就是根据标签名筛选，例如想选择二级标题，直接用 h2 即可。这是最常用的 3 种表示，分别是根据 id、class、标签名筛选。
* CSS 选择器还支持嵌套选择，各个选择器之间加上空格分隔开便可以代表嵌套关系，如 #container .wrapper p 则代表先选择 id 为 container 的节点，然后选中其内部的 class 为 wrapper 的节点，然后再进一步选中其内部的 p 节点。另外，如果不加空格，则代表并列关系，如 div#container .wrapper p.text 代表先选择 id 为 container 的 div 节点，然后选中其内部的 class 为 wrapper 的节点，再进一步选中其内部的 class 为 text 的 p 节点(不是p节点内部的class为text的节点)。
* CSS 选择器的语法规则
```
.class              .intro 	选择 class="intro" 的所有节点
#id                 #firstname 	选择 id="firstname" 的所有节点
*                   * 	选择所有节点
element             p 	选择所有 p 节点
element,element     div,p 	选择所有 div 节点和所有 p 节点
element element     div p 	选择 div 节点内部的所有 p 节点
element>element     div>p 	选择父节点为 div 节点的所有 p 节点
element+element     div+p 	选择紧接在 div 节点之后的所有 p 节点
[attribute]         [target] 	选择带有 target 属性的所有节点
[attribute=value]   [target=blank] 	选择 target="blank" 的所有节点
[attribute~=value]  [title~=flower] 	选择 title 属性包含单词 flower 的所有节点
:link               a:link 	选择所有未被访问的链接
:visited            a:visited 	选择所有已被访问的链接
:active             a:active 	选择活动链接
:hover              a:hover 	选择鼠标指针位于其上的链接
:focus              input:focus 	选择获得焦点的 input 节点
:first-letter       p:first-letter 	选择每个 p 节点的首字母
:first-line         p:first-line 	选择每个 p 节点的首行
:first-child        p:first-child 	选择属于父节点的第一个子节点的所有 p 节点
:before             p:before 	在每个 p 节点的内容之前插入内容
:after              p:after 	在每个 p 节点的内容之后插入内容
:lang(language)     p:lang 	选择带有以 it 开头的 lang 属性值的所有 p 节点
element1~element2   p~ul 	选择前面有 p 节点的所有 ul 节点
[attribute^=value]  a[src^="https"] 	选择其 src 属性值以 https 开头的所有 a 节点
[attribute$=value]  a[src$=".pdf"] 	选择其 src 属性以.pdf 结尾的所有 a 节点
[attribute*=value]  a[src*="abc"] 	选择其 src 属性中包含 abc 子串的所有 a 节点
:first-of-type      p:first-of-type 	选择属于其父节点的首个 p 节点的所有 p 节点
:last-of-type       p:last-of-type 	选择属于其父节点的最后 p 节点的所有 p 节点
:only-of-type       p:only-of-type 	选择属于其父节点唯一的 p 节点的所有 p 节点
:only-child         p:only-child 	选择属于其父节点的唯一子节点的所有 p 节点
:nth-child(n)       p:nth-child 	选择属于其父节点的第二个子节点的所有 p 节点
:nth-last-child(n)  p:nth-last-child 	同上，从最后一个子节点开始计数
:nth-of-type(n)     p:nth-of-type 	选择属于其父节点第二个 p 节点的所有 p 节点
:nth-last-of-type(n)    p:nth-last-of-type 	同上，但是从最后一个子节点开始计数
:last-child         p:last-child 	选择属于其父节点最后一个子节点的所有 p 节点
:root               :root 	选择文档的根节点
:empty              p:empty 	选择没有子节点的所有 p 节点（包括文本节点）
:target             #news:target 	选择当前活动的 #news 节点
:enabled            input:enabled 	选择每个启用的 input 节点
:disabled           input:disabled 	选择每个禁用的 input 节点
:checked            input:checked 	选择每个被选中的 input 节点
:not(selector)      :not 	选择非 p 节点的所有节点
::selection         ::selection 	选择被用户选取的节点部分
```

## 2.4 会话和 Cookies
* 两个用于保持 HTTP 连接状态的技术，它们分别是会话和 Cookies。会话在服务端，也就是网站的服务器，用来保存用户的会话信息；Cookies 在客户端，也可以理解为浏览器端，有了 Cookies，浏览器在下次访问网页时会自动附带上它发送给服务器，服务器通过识别 Cookies 并鉴定出是哪个用户，然后再判断用户是否是登录状态，然后返回对应的响应。

## 2.5 代理的基本原理
* 根据协议区分的代理类型
    - FTP 代理服务器，主要用于访问 FTP 服务器，一般有上传、下载以及缓存功能，端口一般为 21、2121 等。
    - HTTP 代理服务器，主要用于访问网页，一般有内容过滤和缓存功能，端口一般为 80、8080、3128 等。
    - SSL/TLS 代理，主要用于访问加密网站，一般有 SSL 或 TLS 加密功能（最高支持 128 位加密强度），端口一般为 443。
    - RTSP 代理，主要用于 Realplayer 访问 Real 流媒体服务器，一般有缓存功能，端口一般为 554。
    - Telnet 代理，主要用于 telnet 远程控制（黑客入侵计算机时常用于隐藏身份），端口一般为 23。
    - POP3/SMTP 代理，主要用于 POP3/SMTP 方式收发邮件，一般有缓存功能，端口一般为 110/25。
    - SOCKS 代理，只是单纯传递数据包，不关心具体协议和用法，所以速度快很多，一般有缓存功能，端口一般为 1080。SOCKS 代理协议又分为 SOCKS4 和 SOCKS5，SOCKS4 协议只支持 TCP，而 SOCKS5 协议支持 TCP 和 UDP，还支持各种身份验证机制、服务器端域名解析等。简单来说，SOCK4 能做到的 SOCKS5 都可以做到，但 SOCKS5 能做到的 SOCK4 不一定能做到。
* 根据匿名程度区分的代理类型
    - 高度匿名代理，高度匿名代理会将数据包原封不动的转发，在服务端看来就好像真的是一个普通客户端在访问，而记录的 IP 是代理服务器的 IP。
    - 普通匿名代理，普通匿名代理会在数据包上做一些改动，服务端上有可能发现这是个代理服务器，也有一定几率追查到客户端的真实 IP。代理服务器通常会加入的 HTTP 头有 HTTP_VIA 和 HTTP_X_FORWARDED_FOR。
    - 透明代理，透明代理不但改动了数据包，还会告诉服务器客户端的真实 IP。这种代理除了能用缓存技术提高浏览速度，能用内容过滤提高安全性之外，并无其他显著作用，最常见的例子是内网中的硬件防火墙。
    - 间谍代理，间谍代理指组织或个人创建的，用于记录用户传输的数据，然后进行研究、监控等目的代理服务器。
* 常见代理
    - 使用网上的免费代理，最好使用高匿代理，使用前抓取下来筛选一下可用代理，也可以进一步维护一个代理池。
    - 使用付费代理服务，互联网上存在许多代理商，可以付费使用，质量比免费代理好很多。
    - ADSL 拨号，拨一次号换一次 IP，稳定性高，也是一种比较有效的解决方案。
