# coding: utf8

import pycurl
import re
from StringIO import StringIO
from bs4 import BeautifulSoup

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
    soup.decompose()
    content = content0.findAll('td', {'class': 'u-ctitle'})
    for tag in content:
        temp_content = tag.find('a')
        # @todo 解决视频名乱码的问题
        # name = temp_content.text
        href = temp_content['href']
        print href
        real_url = get_song_real_url(href)
        url_list.append(real_url)
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
    soup.decompose()
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
