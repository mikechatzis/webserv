#include "server.hpp"

std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file = "conf/default.conf"){
	if (file.compare(file.size() - 5, 5, ".conf"))
		throw std::invalid_argument("configuration file: invalid name");
	std::vector<conf_data*> *co = new std::vector<conf_data*>();

	struct {
		bool port;
		bool root;
		bool host;
		bool methods;
	} doubles;

	std::ifstream f(file, std::ifstream::in);
	if (f.fail())
		throw std::runtime_error("configuration file not found or corrupt");

	//CHECK FOR INVALID "{}"
	{
		std::ifstream c(file, std::ifstream::in);
		if (c.fail())
			throw std::runtime_error("configuration file not found or corrupt");
		char cont[100000];
		bzero(cont, 100000);
		c.read(cont, 100000);
		if (!strchr(cont, '{'))
			throw std::invalid_argument("missing token '{'");
		for (size_t i = 0; cont[i]; i++)
		{
			if (cont[i] == '{')
				while (cont[i] !='}' && cont[i])
					i++;
			if (!cont[i])
				throw std::invalid_argument("no matching '}'");
		}
	}

	while (!f.eof()){
		char line[10000];
		f.getline(line, 10000, '{');
		std::string l_ine(line);
		
		l_ine.erase(std::remove_if(l_ine.begin(), l_ine.end(), isspace), l_ine.end());

		/* CASE "SERVER" */
		if (l_ine == "server"){
			doubles.port = doubles.root = doubles.host = doubles.methods = false;
			co->push_back(new conf_data());

			std::vector<conf_data*>::iterator it= --co->end();
			f.getline(line, 10000, '}');
			std::stringstream content(line);
			std::string const rules[7] = {"server_names", "port", "error_page", "root", "host", "allowed_methods", ""};

			while (!content.eof())
			{
				content.getline(line, 10000, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 7; ++i)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 10000, ';');
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->addServerNames(line);
						break;
					
					case 2:
						content.getline(line, 10000, ';');
						if (doubles.port)
							break;
						else
							doubles.port = true;
						char *end;
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
						content.getline(line, 10000, ';');
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
									for (std::vector<size_t>::iterator i = codes.begin(); i != codes.end(); ++i)
										(*it)->error_pages.insert(std::make_pair(*i, &line[j]));
									break;
								}
							}
						}
						break;
					case 4:
						content.getline(line, 10000, ';');
						if (doubles.root)
							break;
						else
							doubles.root = true;
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->setRoot(line);
						break;
					case 5:
						content.getline(line, 10000, ';');
						if (doubles.host)
							break;
						else
							doubles.host = true;
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->setHost(line);
						break;
					case 6:
						{
							content.getline(line, 10000, ';');
							if (doubles.methods)
								break;
							else
								doubles.methods = true;
							std::string const methods[3] = {"GET", "POST", "DELETE"};
							if (content.eof())
									throw std::invalid_argument("no ';' found");
							li_ne = line;
							char *token = strtok(line, " \n\t");
							size_t j = 0;
							for (size_t i = 0; token; ++i){
								for (j = 0; j < 3; j++)
									if (token == methods[j])
										break;
								token = strtok(NULL, " \n\t");
							}
							if (j == 3)
								throw std::invalid_argument("Invalid HTTP method");
							(*it)->setMethods(li_ne);
						}
						break;
					case 7:
						break;
					default:
						throw std::invalid_argument("invalid rule");
						break;
				}
				
			}
		}
		/* CASE "ERROR_PAGE" */
		else if(l_ine == "error_page"){
			f.getline(line, 10000, '}');
			std::stringstream content(line);
			std::string const rules[2] = {"codes_to_filepath", ""};

			while (!content.eof())
			{
				content.getline(line, 10000, ':');
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
						content.getline(line, 10000, ';');
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

	try
	{
		if (ac == 2)
			co = readConfFile(gconf, av[1]);
		else
			throw std::invalid_argument("Invalid argument");
	}
	catch(const std::exception& e)
	{
		Color::Modifier f_red(Color::Red);
		Color::Modifier f_white(Color::White);
		std::cerr << f_red << "\nError: " << e.what() << '\n';
		std::cerr << "Initiating with default settings" << f_white <<"\n\n" ;
		co = readConfFile(gconf);
	}
	
	Color::Modifier f_green(Color::SeaGreen2);
	Color::Modifier f_magenta(Color::Magenta2);
	Color::Modifier f_yellow(Color::Yellow2);
	Color::Modifier f_grey_b(Color::Grey93, 1);
	Color::Modifier f_grey_lighter(Color::Grey100);
	Color::Modifier reset(Color::White, 0, 1);
	std::vector<conf_data*>::iterator it = co->begin();
	for (size_t i = 0; i < co->size(); i++)
	{
		std::cout << f_grey_b << "\nELEMENT: " << i + 1 << " {" << reset
		<< f_magenta << "\n\n\tserver_names: " << f_green << (*it)->s_names() 
		<< f_magenta << "\n\tport: " << f_green << (*it)->s_port()
		<< f_magenta << "\n\troot_dir: " << f_green << (*it)->s_root()
		<< f_magenta << "\n\tallowed methods: " << f_green << (*it)->s_methods()
		<< f_magenta << "\n\thost: " << f_green << (*it)->s_host() << std::endl << std::endl;
		(*it)->printErrorCodes();
		std::cout << f_grey_b << "}" << std::endl << std::endl;
		++it;
	}
	
	Color::Modifier f_blue(Color::DeepSkyBlue2);
	std::cout << f_grey_b << "DEFAULT ERROR CODES\n\n" << reset;
	for (std::map<size_t, std::string>::iterator i = gconf->error_pages->begin(); i != gconf->error_pages->end(); i++)
	{
		std::cout << f_blue << "default_error_code: " << reset << i->first 
				  << f_blue << " path: " << reset << i->second << std::endl;
	}
	
	for (std::vector<conf_data*>::iterator ite = co->begin(); ite != co->end(); ++ite)
		delete *ite;
	
	delete co;
	delete gconf->error_pages;
	delete gconf;
	return 0;
}
