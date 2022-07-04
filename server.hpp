#ifndef server_hpp
#define server_hpp

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read
#include <fcntl.h> // For fcntl
#include <netdb.h> // For addrinfo
#include <arpa/inet.h> // For inet_ntop
#include <map>
#include <vector>
#include <list>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>

#define PORT "4242"

class conf_data{

	private:
		std::string server_names;
		size_t port;
	public:

		conf_data() : port(4242){}
		conf_data(conf_data const & other){
			this->server_names = other.server_names;
		}
		virtual~conf_data(){}

		void setServerNames(std::string const &s){
			server_names = s;
		}
		void setPort(size_t p){
			port = p;
		}

	friend std::vector<conf_data*> *readConfFile(std::string const &file);
};

char* parse_method(char line[], const char symbol[]);
char* parse(char line[], const char symbol[]);
std::map<std::string, std::string> initialize_mime_types(void);
std::vector<conf_data*> *readConfFile(std::string const &file);

#endif
