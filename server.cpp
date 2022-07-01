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
	fd_set master; // master fd list
	fd_set read_fds; // temp fd list for select()
	int fdmax; // maximum file descriptor number

	int listener; // listening socket descriptor
	int newfd; // newly accepted socket descriptor
	struct sockaddr_storage remoteaddr; // client address
	socklen_t addrlen;

	char buff[1024]; // buffer for client data
	int nBytes;

	char http_header[] = "HTTP/1.1 ";

	// char remoteIP[INET6_ADDRSTRLEN];
	int yes = 1; // for setsockopt() SO_REUSEADDR
	int i, rv;

	struct addrinfo hints, *ai, *p;

	FD_ZERO(&master); // clear the fd_sets
	FD_ZERO(&read_fds);

	// get a socket and bind it
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	rv = getaddrinfo(NULL, PORT, &hints, &ai);
	if (rv != 0)
	{
		perror("getaddrinfo");
		exit(1);
	}

	for (p = ai; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		fcntl(listener, F_SETFL, O_NONBLOCK);
		if (listener < 0)
			continue ;
		
		setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0)
		{
			close(listener);
			continue ;
		}

		break ;
	}

	// if we got here, it means we failed to bind
	if (p == NULL)
	{
		std::cerr << "webserv: failed to bind" << std::endl;
		exit(2);
	}

	// listen
	if (listen(listener, 10) == -1)
	{
		perror("listen");
		exit(3);
	}

	// add listener to the master set
	FD_SET(listener, &master);

	//keep track of biggest fd
	fdmax = listener;

	//main loop
	while (true)
	{
		read_fds = master; // copy the master set
		if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1)
		{
			perror("select");
			exit(4);
		}

		// run through existing connections looking for data to read
		for (i = 0; i <= fdmax; i++)
		{
			// if below condition enters then we found one
			if (FD_ISSET(i , &read_fds))
			{
				if (i == listener)
				{
					// handle new connections
					addrlen = sizeof(remoteaddr);
					newfd = accept(listener, reinterpret_cast<struct sockaddr *>(&remoteaddr), &addrlen);
					if (newfd == -1)
						perror("accept");
					else
					{
						// add to master set
						FD_SET(newfd, &master);
						// keep track of max fd
						if (newfd > fdmax)
							fdmax = newfd;
						std::cout << "webserv: new connection" << std::endl;
					}
				}
				else
				{
					// handle data from a client
					nBytes = recv(i, buff, sizeof(buff) - 1, 0);
					buff[nBytes] = 0;
					if (nBytes <= 0)
					{
						// either error or connection closed by client
						if (nBytes == 0)
							std::cout << "webserv: socket " << i << " hung up" << std::endl;
						else
							perror("recv");
						close(i);
						// remove from master set
						FD_CLR(i, &master);
					}
					else
					{
						std::map<std::string, std::string> types = initialize_mime_types();
						std::map<std::string, std::string> http_code = http_table();
						char *parse_string_method = parse_method(buff, " ");
						std::cout << "Client method: " << parse_string_method << std::endl;
						char *parse_string = parse(buff, " ");
						if(!parse_string)
						{
							delete[] parse_string_method;
							delete[] parse_string;
							continue ;
						}
						std::cout << "Client path request: " << parse_string << std::endl;
						char *copy = strdup(parse_string);
						if (!strcmp(copy, "/"))
						{
							delete[] copy;
							copy = strdup("index.html");
						}
						char *parse_ext = parse(copy, ".");
						if(!parse_ext)
						{
							delete[] copy;
							delete[] parse_string_method;
							delete[] parse_string;
							delete[] parse_ext;
							continue ;
						}
						std::cout << "extention: " << parse_ext << std::endl;

						if (!strcmp(parse_string_method, "GET")){
							std::map<std::string, std::string>::iterator it = types.find("." + std::string(parse_ext));
							if (it != types.end()){

								std::string copy_head = http_header;
								std::string str = parse_string + 1;
								std::fstream test(str);

								if(test)
									copy_head += handle_valid_get(test, parse_string, http_code, it->second);
								else
									copy_head += handle_invalid_get("404", http_code);
								send(i, copy_head.c_str(), copy_head.length(), 0);
							}
							else{
								std::string head = http_header;
								head += handle_invalid_get("415", http_code);
								send(i, head.c_str(), head.length(), 0);
							}
						}
						else if (!strcmp(parse_string_method, "POST"))
						{
							// std::map<std::string, std::string>::iterator it = types.find("." + std::string(parse_ext));
							// if (it != types.end()){

							// 	std::string copy_head = http_header;
							// 	std::string str = parse_string + 1;
							// 	std::ofstream test(str);

							// 	if(test)
							// 		copy_head += handle_valid_post(test, parse_string, http_code, it->second);
							// 	else
							// 		copy_head += handle_invalid_post("404", http_code);
							// 	send(i, copy_head.c_str(), copy_head.length(), 0);
							// }
							// else{
							// 	std::string head = http_header;
							// 	head += handle_invalid_post("415", http_code);
							// 	send(i, head.c_str(), head.length(), 0);
							// }
						}
						else if (!strcmp(parse_string_method, "DELETE"))
						{
							std::map<std::string, std::string>::iterator it = types.find("." + std::string(parse_ext));
							if (it != types.end()){
								std::string copy_head = http_header;
								copy_head += "200 ";
								copy_head += http_code["200"];
								copy_head += it->second;
								send(i, copy_head.c_str(), copy_head.length(), 0);
							}
							else{
								std::string head = http_header;
								head += "415 ";
								head += http_code["415"];
								send(i, head.c_str(), head.length(), 0);
							}
						}
						else
						{
							std::string head = http_header;
							head += "400 ";
							head += http_code["400"];
							send(i, head.c_str(), head.length(), 0);
						}
						delete[] copy;
						delete[] parse_string_method;
						delete[] parse_string;
						delete[] parse_ext;
					}
				}
			}
		}
	}
	return (0);
}
