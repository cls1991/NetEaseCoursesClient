# coding: utf8

import lib.crawler as crawler


if __name__ == '__main__':
    # 测试样例
    # url = 'http://open.163.com/special/opencourse/algorithms.html'
    # url = 'http://open.163.com/special/opencourse/ios7.html'
    # url = 'http://open.163.com/special/opencourse/ios8.html'
    # url = 'http://open.163.com/special/opencourse/mitmaterial.html'
    url = 'http://open.163.com/special/opencourse/ipadandiphoneapplication.html'
    file_name = 'ipadandiphoneapplication.txt'
    crawler.run(url, file_name)