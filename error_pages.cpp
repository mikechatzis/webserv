#include "server.hpp"
#include <sstream>

// This assumes that key exists
const std::string generate_html(const std::string &key)
{
	std::string html;
	std::map<std::string, std::string> errors = http_table();
	std::stringstream ss;
	ss << "<html>\r\n<head><title><style>h1 {text-align: center;}p {text-align: center;}div {text-align: center;}</style>" << key << " " << errors[key] << "</title></head>\r\n";
	ss << "<body>\r\n<h1>" << key << " " << errors[key] << "</h1>\r\n";
	ss << "<hr>webserv\r\n</body>\r\n</html>\r\n";
	std::cout << ss.str() << std::endl;
	return (ss.str());
}
