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

    def handle_starttag(self, tag, attrs):
        self.output += "<" + tag + ">"

    def handle_endtag(self, tag):
        self.output += "<" + tag + ">"

    def handle_data(self, data):
        self.output += data


class Server(BaseHTTPRequestHandler):

    def do_HEAD(self):
        pass

    def do_POST(self):
        pass

    def do_GET(self):
        self.respond()

    def handle_http(self, status, content_type):
        filename = "../web-files/python - How to generate random html document - Stack Overflow.html"
        filename = "../web-files/DOS Interrupts.html"
        # filename = "../web-files/hello.html"
        with open(filename, encoding="utf8") as file:
            content_type = "text/html"
            response = file.read()
            response.split()
            self.send_response(200)
            self.send_header('Content-type', content_type)
            self.end_headers()
            # response = remove_html_whitespace(response)
            return bytes(response, "UTF-8")

    def respond(self):
        content = self.handle_http(200, 'text/html')
        self.wfile.write(content)


HOST_NAME = 'localhost'
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
