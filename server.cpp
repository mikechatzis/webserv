#include "server.hpp"

// get sockaddr
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET)
		return (&(reinterpret_cast<struct sockaddr_in *>(sa)->sin_addr));
	return (&(reinterpret_cast<struct sockaddr_in6 *>(sa)->sin6_addr));
}

int main(void)
{
	fd_set master;
	fd_set read_fds;

	int fdmax;
	int listener;
	int newfd;

	struct sockaddr_storage remoteaddr;
	socklen_t addrlen;

	char buff[1024];
	int nBytes;

	char http_header[] = "HTTP/1.1 ";

	int yes = 1;
	int i, rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&master);
	FD_ZERO(&read_fds);

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	rv = getaddrinfo(NULL, PORT, &hints, &ai);

	if (rv != 0){
		perror("getaddrinfo");
		exit(1);
	}

	for (p = ai; p != NULL; p = p->ai_next){
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		fcntl(listener, F_SETFL, O_NONBLOCK);
		if (listener < 0)
			continue ;
		
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0){
			close(listener);
			continue ;
		}
		break ;
	}

	if (p == NULL){
		std::cerr << "webserv: failed to bind" << std::endl;
		exit(2);
	}

	if (listen(listener, 10) == -1){
		perror("listen");
		exit(3);
	}

	FD_SET(listener, &master);
	fdmax = listener;

	while (true){
		read_fds = master;
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1){
			perror("select");
			exit(4);
		}

		for (i = 0; i <= fdmax; i++){
			if (FD_ISSET(i , &read_fds)){
				if (i == listener){

					addrlen = sizeof(remoteaddr);
					newfd = accept(listener, reinterpret_cast<struct sockaddr *>(&remoteaddr), &addrlen);
					if (newfd == -1)
						perror("accept");
					else{
						FD_SET(newfd, &master);
						if (newfd > fdmax)
							fdmax = newfd;
						std::cout << "webserv: new connection" << std::endl;
					}
				}
				else{

					nBytes = recv(i, buff, sizeof(buff) - 1, 0);
					buff[nBytes] = 0;
					if (nBytes <= 0){

						if (nBytes == 0)
							std::cout << "webserv: socket " << i << " hung up" << std::endl;
						else
							perror("recv");
						close(i);
						FD_CLR(i, &master);
					}
					else{
						std::map<std::string, std::string> types = initialize_mime_types();
						std::map<std::string, std::string> http_code = http_table();
						char *parse_string_method = parse_method(buff, " ");
						char *parse_string = parse(buff, " ");
						if(!parse_string){

							delete[] parse_string_method;
							delete[] parse_string;
							continue ;
						}
						char *copy = strdup(parse_string);
						if (!strcmp(copy, "/")){

							delete[] copy;
							copy = strdup("index.html");
						}
						char *parse_ext = parse(copy, ".");
						if(!parse_ext){

							delete[] copy;
							delete[] parse_string_method;
							delete[] parse_string;
							delete[] parse_ext;
							continue ;
						}
						// std::cout << "Client method: " << parse_string_method << std::endl;
						// std::cout << "Client path request: " << parse_string << std::endl;
						// std::cout << "extention: " << parse_ext << std::endl;

						std::string response = http_header;
						if (!strcmp(parse_string_method, "GET")){
							std::map<std::string, std::string>::iterator it = types.find("." + std::string(parse_ext));
							if (it != types.end()){

								std::string str = parse_string + 1;
								std::fstream test(str);

								if(test)
									response += valid_get(test, http_code, it->second);
								else
									response += invalid_get("404", http_code);
								send(i, response.c_str(), response.length(), 0);
							}
							else{
								response += invalid_get("415", http_code);
								send(i, response.c_str(), response.length(), 0);
							}
						}
						else if (!strcmp(parse_string_method, "POST")){
							// std::map<std::string, std::string>::iterator it = types.find("." + std::string(parse_ext));
							// if (it != types.end()){

							// 	std::string str = parse_string + 1;
							// 	std::fstream test(str);
							// 	if(!test)
							// 		response += valid_post(test, it->second, http_code);
							// 	else
							// 		response += invalid_post("204", http_code);
							// 	send(i, response.c_str(), response.length(), 0);
							// }
							// else{
							// 	response += invalid_post("415", http_code);
							// 	send(i, response.c_str(), response.length(), 0);
							// }
						}
						else if (!strcmp(parse_string_method, "DELETE")){
							std::map<std::string, std::string>::iterator it = types.find("." + std::string(parse_ext));
							if (it != types.end()){

								std::string str = parse_string + 1;
								if(std::FILE *file = fopen(str.c_str(), "r"))
									response += valid_delete(file, it->second, http_code);
								else
									response += invalid_delete("204", http_code);
								send(i, response.c_str(), response.length(), 0);
							}
							else{
								response += invalid_delete("415", http_code);
								send(i, response.c_str(), response.length(), 0);
							}
						}
						else
						{
							response += "400 ";
							response += http_code["400"];
							send(i, response.c_str(), response.length(), 0);
						}
						delete[] copy;
						delete[] parse_string_method;
						delete[] parse_string;
						delete[] parse_ext;
						exit(0);
					}
				}
			}
		}
	}
	return (0);
}
