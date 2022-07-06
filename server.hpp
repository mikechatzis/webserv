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
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <stdlib.h>

#define PORT "4242"

//TEXT COLOR CHANGE
namespace Color {
    enum Code {
		FG_BLACK    = 30,
        FG_RED      = 31,
        FG_GREEN    = 32,
		FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };
    class Modifier {
        Code code;
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream& operator<<(std::ostream& os, const Modifier& mod) {
            return os << "\033[" << mod.code << "m";
        }
    };
}
///////////////////

typedef struct s_gconf{
	std::map<size_t, std::string> *error_pages;
}t_gconf;

class conf_data{

	private:
		std::string server_names;
		std::string root;
		std::string host;
		std::map<size_t, std::string> error_pages;
		size_t port;
	public:

		conf_data() :root("root"), host("localhost"), error_pages(std::map<size_t, std::string>()), port(4242){}
		conf_data(conf_data const & other){
			this->server_names = other.server_names;
			port = other.port;
			error_pages = other.error_pages;
			root = other.root;
		}
		virtual~conf_data(){}

		//SET CONFIGURATION DATA//
		void addServerNames(std::string const &s){
			server_names += s;
		}
		void setPort(size_t p){
			port = p;
		}
		void setRoot(std::string const &r){
			root = r;
		}
		void setHost(std::string const &s){
			host = s;
		}

		//ACCESS CONFIGURATION DATA//
		std::string const &s_names() const {
			return server_names;
		}
		std::string const &s_root() const {
			return root;
		}
		size_t s_port() const {
			return this->port;
		}
		std::string const &s_host() const {
			return this->host;
		}
		std::map<size_t, std::string> const &s_errorCodes() const {
			return error_pages;
		}
		//////////////////

		void printErrorCodes(){
			for (std::map<size_t, std::string>::iterator i = error_pages.begin(); i != error_pages.end(); i++)
				std::cout << "server_block_error_code: " << i->first << " page: " << i->second << std::endl;
		}

	friend std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file);
};


char* parse_method(char line[], const char symbol[]);
char* parse(char line[], const char symbol[]);
std::map<std::string, std::string> initialize_mime_types(void);
std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file);

#endif
