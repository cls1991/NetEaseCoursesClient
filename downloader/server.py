# coding: utf8


"""
实现简单的http server功能
"""

import os
project_path = os.path.split(os.path.realpath(__file__))[0]
os.chdir(project_path)

import tornado.ioloop
import tornado.web
import tornado.httpserver

from lib.showLinks import ShowHandler


class MainHandler(tornado.web.RequestHandler):
	"""
	处理get请求
	"""
	def get(self):
		print 'hello, world!'
		
	
application = tornado.web.Application([
	(r"/", MainHandler),
	(r"/showLinks", ShowHandler)
])

if __name__ == '__main__':
	port = 9001
	server = tornado.httpserver.HTTPServer(application)
	server.bind(port)
	server.start(1)
	tornado.ioloop.IOLoop.instance().start()
