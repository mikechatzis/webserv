#include "server.hpp"
#include <sstream>

// This assumes that key exists
const std::string generate_html(const std::string &key)
{
	std::string html;
	std::map<std::string, std::string> errors = http_table();
	std::stringstream ss;
	ss << "<html>\r\n<head><title>" << key << " " << errors[key] << "</title></head>\r\n";
	ss << "<body>\r\n<center><h1>" << key << " " << errors[key] << "</h1></center>\r\n";
	ss << "<hr><center>webserv</center>\r\n</body>\r\n</html>\r\n";
	std::cout << ss.str() << std::endl;
	return (ss.str());
}

int main()
{
	generate_html("400");
	generate_html("401");
	generate_html("402");
	generate_html("403");
	generate_html("404");
	generate_html("418");
}
