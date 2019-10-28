from http.server import HTTPServer, BaseHTTPRequestHandler
from html.parser import HTMLParser

routes = {
    '/': 'html/test.html',
}


def remove_html_whitespace(string):
    copy = ""

    return copy


class HTMLWhiteSpaceRemover(HTMLParser):

    def __init__(self):
        super().__init__()
        self.output = ""

    def clear(self):
        self.output = ""

    def error(self, message):
        pass

    def _strip_newlines(self, data):
        return data.replace('\n', '')

    def handle_starttag(self, tag, attrs):
        self.output += "<" + tag + ">"

    def handle_endtag(self, tag):
        self.output += "</" + tag + ">"

    def handle_data(self, data):
        no_newlines = data.replace('\n', '')
        self.output += no_newlines

    def handle_decl(self, data):
        self.output += "<!" + self._strip_newlines(data) + ">"


class Server(BaseHTTPRequestHandler):


    def do_HEAD(self):
        pass

    def do_POST(self):
        pass

    def do_GET(self):
        self.respond()

    def handle_http(self, status, content_type):
        filename = "../web-files/python - How to generate random html document - Stack Overflow.html"
        # filename = "../web-files/DOS Interrupts.html"
        # filename = "../web-files/hello.html"
        with open(filename, encoding="utf8") as file:
            parser = HTMLWhiteSpaceRemover()
            content_type = "text/html"
            response = file.read()
            # response.split()
            parser.feed(response)
            self.send_response(200)
            self.send_header('Content-type', content_type)
            self.end_headers()
            return bytes(parser.output, "UTF-8")
            # return bytes(response, "UTF-8")

    def respond(self):
        content = self.handle_http(200, 'text/html')
        self.wfile.write(content)


HOST_NAME = '10.0.0.18'
# HOST_NAME = '172.18.55.241'
PORT_NUMBER = 8000


def main():
    httpd = HTTPServer((HOST_NAME, PORT_NUMBER), Server)
    print("start")
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    print("end")
    print("MAIN")


if __name__ == '__main__':
    main()
