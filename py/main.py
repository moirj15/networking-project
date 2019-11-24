from http.server import HTTPServer, BaseHTTPRequestHandler
from .compressor import HTMLWhiteSpaceRemover, remove_html_whitespace


routes = {
    '/': 'html/test.html',
}




class Server(BaseHTTPRequestHandler):


    def do_HEAD(self):
        pass

    def do_POST(self):
        pass

    def do_GET(self):
        self.respond()

    def handle_http(self, status, content_type):
        # filename = "../web-files/python - How to generate random html document - Stack Overflow.html"
        # filename = "../web-files/DOS Interrupts.html"
        filename = "../web-files/big wikipedia page.html"
        # filename = "../web-files/DOS Interrupts large version.html"
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
            print(len(response))
            response = remove_html_whitespace(response)
            print(len(response))
            # return bytes(parser.output, "UTF-8")
            return bytes(response, "UTF-8")

    def respond(self):
        content = self.handle_http(200, 'text/html')
        self.wfile.write(content)


HOST_NAME = 'localhost'
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
