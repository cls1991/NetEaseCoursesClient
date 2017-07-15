NetEaseCoursesClient
======

## 客户端查询并观看网易公开课的视频

### 环境配置
-------------------------------
- 服务器基于python构建，requirements.txt文件列举出了依赖的python库
- 客户端界面搭建需要QT库的支持
- 客户端与服务器交互的数据类型为json格式，服务器(python环境)有内置的json库，而客户端则需要第三方库的支持，这里选取的是jsoncpp，具体API可以自行上网查询
- 客户端要模拟httpclient发送http请求，与服务器进行数据交互，这里选择用curl进行httpclient模拟，因此需要安装curl及curl-devel相关软件
- 客户端播放器后台采用的是开源的mplayer，因此需要安装mplayer软件
### 目录说明
-------------------------------
#### NetEaseCoursesClient/

- crawler/
	- lib/
		- showLinks.py: 处理http请求
		- crawler.py: 负责找到视频真实下载链接
    - out/: 视频下载链接存储文件，用于测试
	- server.py: 服务器启动文件
	- main.py: 测试文件
- mplayer/：客户端界面布局文件，以及播放器逻辑控制代码
- pictures/：程序截图
 
### 使用说明：程序分为服务器和客户端两部分
-----------------------------------------------
#### 1. 服务器：基于tornado的httpserver，启动服务，监听端口9001（当然，可以自定义端口号）；通过不同的url转发到对应的RequestHandler处理，服务器启动指令如下所示：
![server_run_pic](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/server_run_pic.png)
####当然，可以直接通过浏览器发送http请求，查看服务器返回的数据，结果显示如下：
![httpclient_result_pic](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/httpclient_result_pic.png)
#### 2. 客户端：基于QT框架，搜索网易公开课视频，模拟httpclient与服务器交互，并获取视频真实链接；至于视频播放，QT搭建的其实只是一个前端的mplayer壳，借助QProcess与后台的mplayer播放器传递控制命令，完成播放器的基本操作；
* step1: 搜索
![search_pic](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/search_pic.png)
![search_result_1](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/search_result_1.png)
![search_result_2](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/search_result_2.png)
* step2: 播放
![play_pic](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/play_pic.png)
* step3: 音量调节
![volum_pic](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/volum_pic.png)
* step4: 进度条
![play_progress_pic_1](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/play_progress_pic_1.png)
![play_progress_pic_2](https://github.com/cls1991/163CoursesDownloader/raw/master/pictures/play_progress_pic_2.png)

### TODO
------------------------------------------------
- 客户端搜索条件改善，目前搜索源为视频在网易公开课上对应的网页url地址。以ios8为例，正常情况下用户输入ios8查询即可，这里，搜索条件其实是[http://open.163.com/special/opencourse/ios8.html](http://open.163.com/special/opencourse/ios8.html)，因此程序还需要为用户选择搜索的网页集
- 客户端搜索结果显示的是视频真实url，可以改进为视频缩略图以及视频信息显示的数据集合
- 客户端需要添加视频缓冲动画，用于用户交互
- 服务器需要添加搜索结果缓存，这样客户端直接从缓存拉取数据，而不是重新抓取网页，分析处理网页，可以省去不必要的服务器开销
- 服务器搭建在本机上，局限性比较大，后期考虑租一台服务器，部署后台程序到远程服务器上，作为常驻进程



