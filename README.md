# 163CoursesDownloader
###客户端查询并观看网易公开课的视频
####环境配置
#####1.requirements.txt列举出了依赖的python库
#####2.	GUI部分需要QT库的支持, 见第二部分关于GUI的说明
####使用说明：程序分为服务器和客户端两部分
#####1.服务器：基于tornado的httpserver，启动服务，监听端口9001（当然，可以自定义端口号）；通过不同的url转发到对应的RequestHandler处理，服务器启动指令如下所示：
#####2.客户端：基于QT框架，搜索网易公开课视频，模拟httpclient与服务器交互，并获取视频真实链接；至于视频播放，QT搭建的其实只是一个前端的mplayer壳，借助QProcess与后台的mplayer播放器传递控制命令，完成播放器的基本操作；



