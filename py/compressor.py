from html.parser import HTMLParser
import codecs

def remove_html_whitespace(string):
    copy = string[:]

    return copy.replace('\n', '')


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

    # def handle_startendtag(self, tag, attrs):
    #     pass

    # def handle_entityref(self, name):
    #     pass
    #
    # def handle_charref(self, name):
    #     pass
    #
    def handle_comment(self, data):
        pass

    def handle_starttag(self, tag, attrs):
        self.output += "<" + tag
        for attr_tuple in attrs:
            for attr in attr_tuple:
                self.output += " " + attr
        self.output += ">"
        # self.output += "<" + tag + ">"

    def handle_endtag(self, tag):
        self.output += "</" + tag + ">"

    def handle_data(self, data):
        no_newlines = data.replace('\n', '')
        self.output += no_newlines

    def handle_decl(self, data):
        self.output += "<!" + self._strip_newlines(data) + ">"


def main():
    # filename = "../web-files/python - How to generate random html document - Stack Overflow.html"
    # filename = "../web-files/DOS Interrupts.html"
    filename = "../web-files/big wikipedia page.html"
    # filename = "../web-files/DOS Interrupts large version.html"
    # filename = "../web-files/hello.html"

    with open(filename, encoding="utf8") as file:
        contents_pre_comp = file.read()
        print("Pre compression size: " + str(len(contents_pre_comp)))
        compressor = HTMLWhiteSpaceRemover()
        compressor.feed(contents_pre_comp)
        contents_post_comp = compressor.output
        print("Post compression size: " + str(len(contents_post_comp)))
        print(f"Difference {len(contents_pre_comp) - len(contents_post_comp)} Bytes")
        print(f"Precent Diff: {((len(contents_pre_comp) - len(contents_post_comp)) / len(contents_pre_comp)) * 100.0}%")
        output = codecs.open("../compressed_output.html", 'w+', "utf-8")
        output.write(contents_post_comp)
        output.close()



if __name__ == '__main__':
    main()
