#include "server.hpp"

std::vector<conf_data*> *readConfFile(t_gconf *gconf, std::string const &file = "conf/default.conf"){
	if (file.compare(file.size() - 5, 5, ".conf"))
		throw std::invalid_argument("configuration file: invalid name");
	std::vector<conf_data*> *co = new std::vector<conf_data*>();

	struct {
		bool port;
		bool root;
		bool host;
	} doubles;

	doubles.port = doubles.root = doubles.host = false;

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
				while (cont[i] !='}')
					i++;
			if (!cont[i])
				throw std::invalid_argument("no matching '}'");
		}
	}

	while (!f.eof()){
		char line[1000];
		f.getline(line, 1000, '{');
		std::string l_ine(line);
		
		l_ine.erase(std::remove_if(l_ine.begin(), l_ine.end(), isspace), l_ine.end());

		/* CASE "SERVER" */
		if (l_ine == "server"){
			co->push_back(new conf_data());

			std::vector<conf_data*>::iterator it= --co->end();
			f.getline(line, 1000, '}');
			std::stringstream content(line);
			std::string const rules[6] = {"server_names", "port", "error_page", "root", "host", ""};

			while (!content.eof())
			{
				content.getline(line, 1000, ':');
				std::string li_ne(line);
				li_ne.erase(remove_if(li_ne.begin(), li_ne.end(), isspace), li_ne.end());
				int pos = 0;
				for (size_t i = 0; i < 6; ++i)
				{
					if (li_ne == rules[i]){
						pos = i + 1;
						break;
					}
				}
				switch (pos)
				{
					case 1:
						content.getline(line, 1000, ';');
						li_ne = line;
						if (content.eof())
								throw std::invalid_argument("no ';' found");
						(*it)->addServerNames(line);
						break;
					
					case 2:
						content.getline(line, 1000, ';');
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
						content.getline(line, 1000, ';');
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
						content.getline(line, 1000, ';');
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
						content.getline(line, 1000, ';');
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
						break;
					default:
						throw std::invalid_argument("invalid rule");
						break;
				}
				
			}
		}
		/* CASE "ERROR_PAGE" */
		else if(l_ine == "error_page"){
			f.getline(line, 1000, '}');
			std::stringstream content(line);
			std::string const rules[2] = {"codes_to_filepath", ""};

			while (!content.eof())
			{
				content.getline(line, 1000, ':');
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
						content.getline(line, 1000, ';');
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
		Color::Modifier b_red(Color::BG_RED);
		Color::Modifier f_black(Color::FG_BLACK);
		Color::Modifier b_def(Color::BG_DEFAULT);
		Color::Modifier f_def(Color::FG_DEFAULT);
		std::cerr << b_red << f_black << "\nError: " << e.what() << b_def << '\n';
		std::cerr << b_red << "Initiating with default settings" << b_def << f_def <<"\n\n" ;
		co = readConfFile(gconf);
	}
	
	Color::Modifier f_green(Color::FG_GREEN);
	Color::Modifier f_yellow(Color::FG_YELLOW);
	Color::Modifier f_def(Color::FG_DEFAULT);
	std::vector<conf_data*>::iterator it = co->begin();
	for (size_t i = 0; i < co->size(); i++)
	{
		std::cout << "ELEMENT: " << i + 1 << f_green << "\n\nserver_names: " << (*it)->s_names() << "\nport: " << (*it)->s_port() <<
		"\nroot_dir: " << (*it)->s_root() << "\nhost: " << (*it)->s_host() << std::endl << std::endl;
		std::cout << f_def << "elements's error codes\n\n" << f_yellow ;
		(*it)->printErrorCodes();
		std::cout << f_def << std::endl << std::endl;
		++it;
	}
	
	Color::Modifier f_blue(Color::FG_BLUE);
	std::cout << "DEFAULT ERROR CODES\n\n";
	for (std::map<size_t, std::string>::iterator i = gconf->error_pages->begin(); i != gconf->error_pages->end(); i++)
	{
		std::cout << f_blue << "default_error_code: " << i->first << " path: " << i->second << f_def << std::endl;
	}
	
	for (std::vector<conf_data*>::iterator ite = co->begin(); ite != co->end(); ++ite)
		delete *ite;
	
	delete co;
	delete gconf->error_pages;
	delete gconf;
	return 0;
}
