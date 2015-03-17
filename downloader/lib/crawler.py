# coding: utf8

import os
# 切换工作目录到项目根目录
project = os.path.split(os.path.realpath(__file__))[0]
os.chdir(project)

import pycurl
import re
import wget
from StringIO import StringIO
from bs4 import BeautifulSoup
from multiprocessing.dummy import Pool as ThreadPool

# 测试网页编码
import chardet

# 伪装成iPad客户端
user_agent = 'Mozilla/5.0(iPad; U; CPU iPhone OS 3_2 like Mac OS X; en-us) ' \
             'AppleWebKit/531.21.10 (KHTML, like Gecko) Version/4.0.4 Mobile/7B314 Safari/531.21.10'
# 伪造来源地址
refer_path = 'http://open.163.com/special/opencourse/algorithms.html'
# 正则匹配真实下载地址
search_pattern = re.compile('%s(.*)%s' % ('appsrc', "'"))
# 注意: 模式串包含空格
replace_pattern = re.compile('appsrc : ')
# .m3u8 => .mp4
rename_pattern = re.compile('.m3u8')


# 注意: 这里获取下载链接时, 需要设置agent为移动端; pc端并未提供下载
def get_song_url(url0):
    buffers = StringIO()
    curl = pycurl.Curl()
    curl.setopt(pycurl.URL, url0)
    curl.setopt(pycurl.USERAGENT, user_agent)
    curl.setopt(pycurl.WRITEDATA, buffers)
    curl.perform()
    curl.close()

    body = buffers.getvalue()
    # print chardet.detect(body)
    soup = BeautifulSoup(body)
    # 找到所有课程的html链接
    url_list = []
    content0 = soup.find('table', {'id': 'list2'})
    if not content0:
        return None
    content = content0.findAll('td', {'class': 'u-ctitle'})
    for tag in content:
        temp_content = tag.find('a')
        # @todo 解决视频名乱码的问题
        name = temp_content.text
        href = temp_content['href']
        real_url = get_song_real_url(href)
        url_list.append(name)
    return url_list


def get_song_real_url(html_url):
    """
    :param html_url: 接受视频html地址
    :return: 返回视频真实下载地址
    """
    buffers = StringIO()
    curl = pycurl.Curl()
    curl.setopt(pycurl.URL, html_url)
    curl.setopt(pycurl.USERAGENT, user_agent)
    curl.setopt(pycurl.REFERER, refer_path)
    curl.setopt(pycurl.WRITEDATA, buffers)
    curl.perform()
    curl.close()

    # 获得str类型的数据
    body = buffers.getvalue()
    soup = BeautifulSoup(body)

    # 获取包含视频真实地址的script
    content = soup.findAll('script', {'type': 'text/javascript'})
    """
    为了兼顾性能, 这里参考大量网页源代码, 总结出来视频url出现在倒数第二个script脚本中,
    省去了全局搜索造成的额外开销
    """
    pos = len(content) - 1
    result = search_pattern.search(content[pos].text)
    if result:
        tmp_url = result.group(0)
        real_url = rename_pattern.sub('.mp4', replace_pattern.sub('', tmp_url)).replace("'", '')
        return real_url
    else:
        return None


def write_urls_to_file(urls, fname):
    """
    将视频真实下载链接写入文件
    :param urls: list类型
    :param fname: 文件名
    :return:
    """
    base_dir = '../out/'
    try:
        file_object = open(base_dir + fname, 'w')
        file_object.write('\n'.join(urls).encode('utf8'))
        file_object.close()
        print(u'url文件已生成')
    except IOError:
        print(u'文件不存在')
        exit()


# 用pool改写for循环
def download_with_pool(url):
    print(u'开始下载%s' % url)
    try:
        wget.download(url)
        print(u'%s下载完毕' % url)
    except Exception:
        print(u'下载%s出现错误' % url)


def run(html_url, saved_name):
    """
    封装好的接口, 供外部程序调用
    :param html_url: 目标网页的url; 比如: url = 'http://open.163.com/special/opencourse/algorithms.html'
    :param saved_name: url存储的文件名
    :return:
    """
    list1 = get_song_url(html_url)
    if not list1:
        print(u'未提供相关视频资源!')
    else:
        write_urls_to_file(list1, saved_name)
        print(u'是否下载?(y/n)')
        choice = raw_input()
        if choice == 'Y' or choice == 'y':
            print(u'开始下载视频...')
            pool = ThreadPool(4)
            pool.map(download_with_pool, list1)
            pool.close()
            pool.join()
            print(u'下载完毕!')
        else:
            print(u'已取消下载')