import http.server, urllib

h = 0

class MyWebServer(http.server.BaseHTTPRequestHandler):
    #info = 0
    def do_POST(self):
        content_length = int(self.headers['Content-Length'])
        post_data_bytes = self.rfile.read(content_length)
        post_data_str = post_data_bytes.decode("UTF-8")
        list_of_post_data = post_data_str.split('&')
        post_data_dict = {}
        for item in list_of_post_data:
            variable, value = item.split('=')
            post_data_dict[variable] = value
        print(post_data_dict)
        htmltext = "<html><body>This is Post method</body></html>"
        respbytes = bytes(htmltext, "utf-8")
        self.send_response(200)
        self.send_header("content-type","text/html; charset=utf-8")
        self.send_header("content-length", str(len(respbytes)))
        self.end_headers()
        self.wfile.write(respbytes)
        
    def do_GET(self):
        global h
        params = dict(urllib.parse.parse_qsl(urllib.parse.urlsplit(self.path).query))
        if "p" in params and "q" in params and "r" in params:
            r = params["r"]
            a = int(params["p"])
            b = int(params["q"])
            match(r):
                case "+": c = a + b
                case "-": c = a - b
                case "*": c = a * b
                case "/": c = a / b
                case "%": c = a % b
                case _: c = 0
            htmltext = "<html><body><strong>" + str(c) +"</strong></body></html>"
        else:
            c = 0
            htmltext="""<html><body>
                <form method=get>
                <input type="number" name="p" value="0"/><br/>
                <input type="number" name="q" value="0"/><br/>
                <input type="submit" name="r" value="+"/>
                <input type="submit" name="r" value="-"/>
                <input type="submit" name="r" value="*"/>
                <input type="submit" name="r" value="/"/>
                <input type="submit" name="r" value="%"/>
                </form></body></html>"""

        h = h + c
        #self.info = self.info + c
        
        respbytes = bytes(htmltext, "utf-8")
        self.send_response(200)
        self.send_header("content-type","text/html; charset=utf-8")
        self.send_header("content-length", str(len(respbytes)))
        self.end_headers()
        self.wfile.write(respbytes)

serverside = ("0.0.0.0", 8500)

httpd = http.server.ThreadingHTTPServer(serverside, MyWebServer)
httpd.serve_forever()
