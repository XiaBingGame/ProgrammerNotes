# 2. 基本库的使用
## 2.1. urllib
* urllib 库: Python 内置的 HTTP 请求库, 不需要额外安装, 有四个模块:
    - request
    - error: 异常处理模块
    - parse: 提供了许多 URL 处理方法, 如拆分,解析,合并等
    - rebotparser: 识别网站的 robots.txt 文件
* urllib
    - api: urllib.request.urlopen(url, data=None, [timeout,]*, cafile=None, capath=None, cadefault=False, context=None)
    - data 参数是可选的。如果要添加该参数，需要使用 bytes 方法将参数转化为字节流编码格式的内容，即 bytes 类型。另外，如果传递了这个参数，则它的请求方式就不再是 GET 方式，而是 POST 方式。
    - request
        - urlopen
            - timeout
        - Request 对象
            - url 参数
            - data 参数
            - headers 参数
            - method 参数
        - HTTPPasswordMgrWithDefaultRealm 认证对象
            - addpassword
        - HTTPBasicAuthHandler
          - 使用认证对象创建认证处理器
          - build_opener: 使用认证处理器打开
        - ProxyHandler 代理处理器
          - build_opener: 使用代理处理器打开
        - HTTPCookieProcessor Cookie 处理器, 参数为 http.cookiejar.CookieJar 对象, http.cookiejar.MozillaCookieJar 对象, http.cookiejar.LWPCookieJar 对象
          - build_opener: 使用 Cookie 处理器打开
    - Request 对象
    - error
        - URLError
            - reason: 可以是 socket.timeout 对象
        - HTTPError
    - parse
        - urllib.parse.urlencode 包装一个字典为二进制
        - urlparse 实现 URL 的识别和分段
          - scheme 设置默认协议
          - allow_fragments 可忽略片段
          - 返回值是一个元组
        - urlunparse 拼接一个 url
        - urlsplit 同 urlparse, 只是不再分开 path 和 param
        - urlunsplit 拼接一个 url
        - urljoin 拼接 URL, 提供一个 base_url 作为一个参数, 新的链接作为第二个参数, 使用 base_url 的 scheme, netloc, path 对第二个参数进行补充
        - urlencode 构造 GET 请求参数
        - parse_qs 请求转换为字典
        - parse_qsl 转换为元组
        - quote: 中文转换为URL编码
        - unquote: 将 URL 编码转换为其他语言
    - robotparser
        - RobotFileParser 对象
          - set_url()
          - read()
          - can_fetch()
          - parse()
        - parse
* bytes() 二进制化
* 自动创建文件目录代码片段
```
from os import makedirs
from os.path import exists

exists(RESULTS_DIR) or makedirs(RESULTS_DIR)
```
* logging 模块
    - basicConfig
* f 格式化字符串
```
index_url = f'{BASE_URL}/page/{page}'
```
* 捕获网页
```
def scrape_page(url):
    logging.info('scraping %s...', url)
    try:
        response = requests.get(url)
        if response.status_code == 200:
            return response.text
        logging.error('get invalid status code %s while scraping %s',
                      response.status_code, url)
    except requests.RequestException:
        logging.error('error occurred while scraping %s', url, exc_info=True)
```

## 2.2 requests 模块
    - get() 返回类型为 requests.models.Response 类型
        - params 参数, 自动拼接字典类型的数据作为请求的附加信息
        - 如果返回 json 数据, 可以直接调用 Response.json() 方法将其转换为字典
        - headers 参数
            - User-Agent
        - cookies 参数, 为 RequestsCookieJar 对象
        - verify 参数 可以不验证网络证书
        - cert 参数: 设置本地证书
        - timeout 参数: 设置超时时间
        - auth 参数: 设置身份认证
        - proxies 参数: 设置代理
    - post
        - data 参数
    - put
    - delete
    - patch
    - Response 对象
        - text
        - status_code
        - cookies
        - json() 字典化
        - content 二进制
        - url
        - history
        - headers
    - RequestsCookieJar --- 设置 Cookie
    - codes --- 内置的状态码查询对象
```
exit() if not r.status_code == requests.codes.ok else print('Request Successfully')
```
    - Session 对象: 模拟在同一个浏览器中打开同一站点不同页面
        - get 方法
        - prepare_request 方法: 构造一个 Prepared Request
        - send 方法: 发送一个 Prepared Request
    - prepare_request
* 写入二进制文件
```
    with open('favicon.ico', 'wb') as f:
        f.write(r.content)
```
* 返回码以及相应的查询条件
```
# 信息性状态码  
100: ('continue',),  
101: ('switching_protocols',),  
102: ('processing',),  
103: ('checkpoint',),  
122: ('uri_too_long', 'request_uri_too_long'),  

# 成功状态码  
200: ('ok', 'okay', 'all_ok', 'all_okay', 'all_good', '\\o/', '✓'),  
201: ('created',),  
202: ('accepted',),  
203: ('non_authoritative_info', 'non_authoritative_information'),  
204: ('no_content',),  
205: ('reset_content', 'reset'),  
206: ('partial_content', 'partial'),  
207: ('multi_status', 'multiple_status', 'multi_stati', 'multiple_stati'),  
208: ('already_reported',),  
226: ('im_used',),  

# 重定向状态码  
300: ('multiple_choices',),  
301: ('moved_permanently', 'moved', '\\o-'),  
302: ('found',),  
303: ('see_other', 'other'),  
304: ('not_modified',),  
305: ('use_proxy',),  
306: ('switch_proxy',),  
307: ('temporary_redirect', 'temporary_moved', 'temporary'),  
308: ('permanent_redirect',  
      'resume_incomplete', 'resume',), # These 2 to be removed in 3.0  

# 客户端错误状态码  
400: ('bad_request', 'bad'),  
401: ('unauthorized',),  
402: ('payment_required', 'payment'),  
403: ('forbidden',),  
404: ('not_found', '-o-'),  
405: ('method_not_allowed', 'not_allowed'),  
406: ('not_acceptable',),  
407: ('proxy_authentication_required', 'proxy_auth', 'proxy_authentication'),  
408: ('request_timeout', 'timeout'),  
409: ('conflict',),  
410: ('gone',),  
411: ('length_required',),  
412: ('precondition_failed', 'precondition'),  
413: ('request_entity_too_large',),  
414: ('request_uri_too_large',),  
415: ('unsupported_media_type', 'unsupported_media', 'media_type'),  
416: ('requested_range_not_satisfiable', 'requested_range', 'range_not_satisfiable'),  
417: ('expectation_failed',),  
418: ('im_a_teapot', 'teapot', 'i_am_a_teapot'),  
421: ('misdirected_request',),  
422: ('unprocessable_entity', 'unprocessable'),  
423: ('locked',),  
424: ('failed_dependency', 'dependency'),  
425: ('unordered_collection', 'unordered'),  
426: ('upgrade_required', 'upgrade'),  
428: ('precondition_required', 'precondition'),  
429: ('too_many_requests', 'too_many'),  
431: ('header_fields_too_large', 'fields_too_large'),  
444: ('no_response', 'none'),  
449: ('retry_with', 'retry'),  
450: ('blocked_by_windows_parental_controls', 'parental_controls'),  
451: ('unavailable_for_legal_reasons', 'legal_reasons'),  
499: ('client_closed_request',),  

# 服务端错误状态码  
500: ('internal_server_error', 'server_error', '/o\\', '✗'),  
501: ('not_implemented',),  
502: ('bad_gateway',),  
503: ('service_unavailable', 'unavailable'),  
504: ('gateway_timeout',),  
505: ('http_version_not_supported', 'http_version'),  
506: ('variant_also_negotiates',),  
507: ('insufficient_storage',),  
509: ('bandwidth_limit_exceeded', 'bandwidth'),  
510: ('not_extended',),  
511: ('network_authentication_required', 'network_auth', 'network_authentication')
```
* 上传文件
```
    files = { 'file': open('favicon.ico', 'rb') }
    r = requests.post('https://www.httpbin.org/post', files=files)
```
* 获取 Cookies
```
    r = requests.get('https://www.baidu.com')
    print(r.cookies)
    for key, value in r.cookies.items():
        print(key + ' = ' + value)
```
* Headers 设置 Cookie
```
    headers = {
        'Cookie': '_octo=GH1.1.1346789172.1632022770; _device_id=7f350d0bc1075168ef9cfd316188d870; tz=Asia/Shanghai; user_session=iclnztZBpgVMn-vQz_ngQuCIGiOecNLHxJAVWQfKweV_Nn8E; __Host-user_session_same_site=iclnztZBpgVMn-vQz_ngQuCIGiOecNLHxJAVWQfKweV_Nn8E; tz=Asia/Shanghai; color_mode={"color_mode":"light","light_theme":{"name":"light","color_mode":"light"},"dark_theme":{"name":"dark","color_mode":"dark"}}; logged_in=yes; dotcom_user=XiaBingGame; has_recent_activity=1; _gh_sess=yT95dmi3GNexO3VNJ2iQHVpu4cqpn7gHByoVvwL8vIDeoyLyohDvAOb8KsI9aJl7f7DlTmg/1XYWTKBZsPu3VIZ3hrFD9S/2NZvoVQ4pOHJXc0OAedFmbftjNorori3HoBi5GAX9cU/TieXEwgmG0QpgwQChdRRXHEOeALehJO6IP06+IiM4Z3E3NadTosvWlPRX2f8d/dNApZd5TLYCGlcw6Bchhq/VH8Vg4xEclQbkUahhcExAsqFCpnz1BJHU+06Z6/Ulm+uUuH+BN6OHZSRxySxEX/JlfN4YKpmseYSrMkfY4Q0GFLRcWr8cp+eYmSYVmtglNwBD6xa9hqBSGkPwmw4miPxa1qcTOE55DyvUKpjaTDtUtFTNwDD1P6pyaoxXyT12vysYp5a+tgLMr7PfMoSnU85FU7X5ChwvpzakpTH3/vZEeV4m+KO1HkVDMigWrlT8H3E6rx6Kv+pzhY+6CUNLIC6wXHKs6eNiqsvfxB5SOil6ciPDqaS5WX0S+FvnZ0C7Rz+Ezoy31jVjjhKGkyct5fwWMHsk6vjiEIEK3WHhkXZinzzWMGCaiCLk8Ye0joXYFFIPnVKZCFTQQJRaALquJAHGmPtXulMt5hhV9vBKxR4QwnFUWMGKIzJkd8QP84TxFsEjSbFfACwTcD+W++CGGgOV4J/qPvQJZbGwF5Mtqbc1dyPDRbqT7se/50nykrgRqcMwBSR4k1ZwXb/7aerbnjwxLxYem+3seicAwUZ6+JDRZA7p3nupv6ue5038+O8EBna/xZ8eWYUHfREv6odb7Fg49+6xAJUjPe4iBo3gG7x6F0CtMi+7L+AiFEV90M/hmuRfGeief/aT3FO7evFB1o9FVUEAG75HTDwKDgLCYbMRCzjYKOLlaKgmosNFw7qv4ZTm6rWsSqOfwjsvahSLp8Q6Y1fXzQ==--5kRKTPK3OhowLIYq--zkVIC3FEroDVrqijImVClA==',
    }
    r = requests.get('https://github.com/', headers=headers)
    print(r.text)
```
* RequestsCookieJar 对象设置 cookies 参数
```
import requests

cookies = 'q_c1=31653b264a074fc9a57816d1ea93ed8b|1474273938000|1474273938000; d_c0="AGDAs254kAqPTr6NW1U3XTLFzKhMPQ6H_nc=|1474273938"; __utmv=51854390.100-1|2=registration_date=20130902=1^3=entry_date=20130902=1;a_t="2.0AACAfbwdAAAXAAAAso0QWAAAgH28HQAAAGDAs254kAoXAAAAYQJVTQ4FCVgA360us8BAklzLYNEHUd6kmHtRQX5a6hiZxKCynnycerLQ3gIkoJLOCQ==";z_c0=Mi4wQUFDQWZid2RBQUFBWU1DemJuaVFDaGNBQUFCaEFsVk5EZ1VKV0FEZnJTNnp3RUNTWE10ZzBRZFIzcVNZZTFGQmZn|1474887858|64b4d4234a21de774c42c837fe0b672fdb5763b0'
jar = requests.cookies.RequestsCookieJar()
headers = {
    'Host': 'www.zhihu.com',
    'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_4) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/53.0.2785.116 Safari/537.36'
}
for cookie in cookies.split(';'):
    key, value = cookie.split('=', 1)
    jar.set(key, value)
r = requests.get('http://www.zhihu.com', cookies=jar, headers=headers)
print(r.text)
```
* 屏蔽网络证书警告
```
requests.packages.urllib3.disable_warnings()
```
* 警告写入日志
```
logging.captureWarnings(True)
```
* socks5 代理, 要安装 requests[socks]


## 2.3 re 模块
    - match
    - search
    - findall
    - sub: 替换
    - compile 保存模式

## 2.4 httpx 模块
* 支持 HTTP/2.0 协议的库有 hyper 和 httpx
* 支持 HTTP 2.0 协议
    - get
        - headers
        - text
    - post
    - put
    - delet
    - patch
    - Client: 可设置 HTTP/2.0, 和 requests 的 Session 类似
```
    client = httpx.Client(http2=True)
    response = client.get('https://spa16.scrape.center/')
    print(response.text)
```
    - AsyncClient: 异步请求
* 异步设置
```
    asyncio.get_event_loop().run_until_complete
```