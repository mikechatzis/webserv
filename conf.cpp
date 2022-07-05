#include "server.hpp"

std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file = "conf/default.conf"){
	if (file.compare(file.size() - 5, 5, ".conf"))
		throw std::invalid_argument("configuration file: invalid name");
	std::vector<conf_data*> *co = new std::vector<conf_data*>();

	std::ifstream f(file, std::ifstream::in);
	if (f.fail())
		throw std::runtime_error("configuration file not found or corrupt");

	//CHECK FOR INVALID "{}"
	{
		std::ifstream c(file, std::ifstream::in);
		if (c.fail())
			throw std::runtime_error("configuration file not found or corrupt");
		char cont[10000];
		bzero(cont, 10000);
		c.read(cont, 10000);
		for (size_t i = 0; cont[i]; i++)
		{
			if (cont[i] == '{')
				while (cont[i] !='}' && cont[i])
					i++;
			if (!cont[i])
				throw std::invalid_argument("no matching curly brackets");
		}
	}

	while (!f.eof()){
		char line[100];
		f.getline(line, 100, '{');
		std::string l_ine(line);
		
		l_ine.erase(std::remove_if(l_ine.begin(), l_ine.end(), isspace), l_ine.end());

		/* CASE "SERVER" */
		if (l_ine == "server"){
			co->push_back(new conf_data());

			std::vector<conf_data*>::iterator it= --co->end();
			f.getline(line, 100, '}');
			std::stringstream content(line);
			std::string const rules[3] = {"server_names", "port", ""};

			while (!content.eof())
			{
				content.getline(line, 100, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 3; i++)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 100, ';');
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->setServerNames(line);
						break;
					
					case 2:
						char *end;
						content.getline(line, 100, ';');
						if (content.eof())
							throw std::invalid_argument("no ';' found");
						li_ne = line;
						li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
						(*it)->setPort(strtol(li_ne.c_str(), &end, 10));
						if (!(*it)->port || *end){
							std::cerr << "invalid port value: '" << li_ne << "', port set to default" << std::endl;
							(*it)->setPort(4242);
						}
						break;
					case 3:
						break;
					default:
						throw std::invalid_argument("invalid rule");
						break;
				}
				
			}
		}
		/* CASE "ERROR_PAGE" */
		else if(l_ine == "error_page"){
			f.getline(line, 100, '}');
			std::stringstream content(line);
			std::string const rules[2] = {"codes_to_filepath", ""};

			while (!content.eof())
			{
				content.getline(line, 100, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 2; i++)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 100, ';');
						{
							if (content.eof())
									throw std::invalid_argument("no ';' found");
							std::vector<size_t> codes;
							li_ne = line;
							char *end;
							for(size_t j = 0; line[j]; j++)
							{
								if (isdigit(line[j])){
									size_t c = strtol(&line[j], &end, 10);
									if (!std::isspace(*end)|| std::to_string(c).length() != 3)
										throw std::invalid_argument("invalid error code");
									j += 3;
									codes.push_back(c);
								}
								else if(isalpha(line[j])){
									for (std::vector<size_t>::iterator it = codes.begin(); it != codes.end(); it++)
										gconf->error_pages->insert(std::make_pair(*it, &line[j]));
									break;
								}
							}
						}
						break;
					case 2:
						break;
					default:
						throw std::invalid_argument("invalid rule");
						break;
				}
			}
		}
		else if (l_ine == ""){}
		else
			throw std::invalid_argument(l_ine);
	}
	return co;
}

int main(int ac, char **av){

	std::vector<conf_data*> *co;
	t_gconf *gconf = new t_gconf;
	gconf->error_pages = new std::map<size_t, std::string>();

	if (ac == 2)
		co = readConfFile(gconf, av[1]);
	else
		co = readConfFile(gconf);
	std::vector<conf_data*>::iterator it = co->begin();
	for (size_t i = 0; i < co->size(); i++)
	{
		std::cout << "element: " << i + 1 << "\nserver_names: " << (*it)->s_names() << "\nport: " << (*it)->s_port() << std::endl;
		++it;
	}
	for (std::map<size_t, std::string>::iterator i = gconf->error_pages->begin(); i != gconf->error_pages->end(); i++)
	{
		std::cout << "code: " << i->first << " path: " << i->second << std::endl;
	}
	
	for (std::vector<conf_data*>::iterator ite = co->begin(); ite != co->end(); ite++)
		delete *ite;
	
	delete co;
	delete gconf->error_pages;
	delete gconf;
	return 0;
}
