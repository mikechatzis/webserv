#include "server.hpp"

// This assumes that key exists
std::string generate_html(const std::string &key)
{
	std::string html;
	std::map<std::string, std::string> errors = http_table();
	std::stringstream ss;
	ss << "<html>\r\n<head><title>h1 {text-align: center;}p {text-align: center;}div {text-align: center;}" << key << " " << errors[key] << "</title></head>\r\n";
	ss << "<body>\r\n<h1>" << key << " " << errors[key] << "</h1>\r\n";
	ss << "<img style=\"margin-left: auto;\", src=\"https://http.cat/";
	ss << key;
	ss << "\">\r\n</body>\r\n</html>\r\n";
	std::cout << ss.str() << std::endl;
	return (ss.str());
}

std::string delete_response( void ){

	std::stringstream ss;
	ss << "<html>\r\n<body>\r\n<h1>File deleted.</h1>\r\n</body>\r\n</html>\r\n";
	std::cout << ss.str() << std::endl;
	return (ss.str());
}
