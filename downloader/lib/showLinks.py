# coding: utf8


"""
处理http请求
"""

import tornado.web
from tornado.httpclient import HTTPError
import json

from crawler import get_song_url


class ShowHandler(tornado.web.RequestHandler):
	def get(self):
		request_arg = self.get_query_argument('html_url')
		self.set_header("Content-Type", "application/json;charset=utf8")
		urls = get_song_url(request_arg)
		self.write(json.dumps(
			{
				"code": 200,
				"data": urls
			}
		))
		self.finish

	def post(self):
		return HTTPError(code=405)
