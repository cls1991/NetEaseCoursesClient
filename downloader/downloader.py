# coding: utf8
import pycurl
import re
import wget
from StringIO import StringIO
from bs4 import BeautifulSoup
from multiprocessing.dummy import Pool as ThreadPool
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


# @todo 视频名乱码
# 注意: 这里获取下载链接时, 需要设置agent为移动端; pc端并未提供下载
def get_song_url(url):
    buffers = StringIO()
    curl = pycurl.Curl()
    curl.setopt(pycurl.URL, url)
    curl.setopt(pycurl.USERAGENT, user_agent)
    curl.setopt(pycurl.WRITEDATA, buffers)
    curl.perform()
    curl.close()

    body = buffers.getvalue()
    soup = BeautifulSoup(body)

    # 找到所有课程的html链接
    url_list = []
    content = soup.find('table', {'id': 'list2'}).findAll('td', {'class': 'u-ctitle'})
    for tag in content:
        temp_content = tag.find('a')
        name = temp_content.text
        href = temp_content['href']
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
    result = search_pattern.search(content[7].text)
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
    try:
        file_object = open(fname, 'w')
        file_object.write('\n'.join(urls))
        file_object.close()
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


if __name__ == '__main__':
    # url = 'http://open.163.com/special/opencourse/algorithms.html'
    # url = 'http://open.163.com/special/opencourse/ios7.html'
    url = 'http://open.163.com/special/opencourse/ios8.html'
    # url = 'http://open.163.com/special/opencourse/ipadandiphoneapplication.html'
    list1 = get_song_url(url)
    write_urls_to_file(list1, 'ios8.txt')
    # write_urls_to_file(list1, 'ipadandiphoneapplication.txt')
    if not list1:
        print('没有视频信息!')
    else:
        print('是否下载?(y/n)')
        choice = raw_input()
        if choice == 'Y' or choice == 'y':
            print('开始下载视频...')
            pool = ThreadPool(4)
            results = pool.map(download_with_pool, list1)
            pool.close()
            pool.join()
            print('下载完毕!')
        else:
            print('已取消下载')
