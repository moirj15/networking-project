from http.server import HTTPServer, BaseHTTPRequestHandler


class Server(BaseHTTPRequestHandler):
    def do_HEAD(self):
        pass

    def do_POST(self):
        pass

    def do_GET(self):
        self.respond()

    def handle_http(self, status, content_type):
        self.send_response(status)
        self.send_header('Content-type', content_type)
        self.end_headers()
        return bytes("Hello World", "UTF-8")

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
