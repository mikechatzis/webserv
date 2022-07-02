#include "server.hpp"

std::string valid_get(std::fstream &test, std::map<std::string, std::string> &map, std::string type){
	std::string resp;
	std::stringstream s;
	std::stringstream s2;
	s << test.rdbuf();
	s2 << s.str().length();
	test.close();
	resp += "200 ";
	resp += map["200"];
	resp += "Content-Type: ";
	resp += type;
	resp += "Content-Length: ";
	resp += s2.str();
	resp += "\r\n\r\n";
	resp += s.str();
	return resp;
}

std::string invalid_get(const std::string &err_cd, std::map<std::string, std::string> &map){
	std::string resp;
	std::string err_html = generate_html(err_cd);
	std::stringstream s;
	s << err_html.length();
	resp += err_cd + " ";
	resp += map[err_cd];
	resp += "Content-Type: text/html\r\nContent-Length: ";
	resp += s.str();
	resp += "\r\n\r\n";
	resp += err_html;
	return resp;
}

std::string valid_delete(std::string file_name, std::string type, std::map<std::string, std::string> &map){
	std::remove(file_name.c_str());
	std::string resp;
	std::stringstream s, s2;
	s << "{\"success\":\"true\"}\n";
	s2 << s.str().length();
	resp += "200 ";
	resp += map["200"];
	resp += "Content-Type: ";
	resp += type;
	resp += "Content-Length: ";
	resp += s2.str();
	resp += "\r\n\r\n";
	resp += s.str();
	return resp;
}

std::string invalid_delete(const std::string &err_cd, std::map<std::string, std::string> &map){
	std::string resp;
	std::stringstream s, s2;
	s << "{\"success\":\"false\"}\n";
	s2 << s.str().length();
	resp += err_cd + " ";
	resp += map[err_cd];
	resp += "Content-Type: text/html\r\nContent-Length: ";
	resp += s2.str();
	resp += "\r\n\r\n";
	resp += s.str();
	return resp;
}

std::string valid_post(std::string file_name, std::string type, std::map<std::string, std::string> &map){
	std::ofstream file(file_name);
	if (!file)
		return invalid_post("422", map);
	file.close();
	std::string resp;
	resp += "200 ";
	resp += map["200"];
	resp += "Content-Type: ";
	resp += type;
	return resp;
}

std::string invalid_post(std::string err_cd, std::map<std::string, std::string> &map){
	std::string resp;
	std::stringstream s, s2;
	s << generate_html(err_cd);
	s2 << s.str().length();
	resp += err_cd + " ";
	resp += map[err_cd];
	resp += "Content-Type: text/html\r\nContent-Length: ";
	resp += s2.str();
	resp += "\r\n\r\n";
	resp += s.str();
	return resp;
}
