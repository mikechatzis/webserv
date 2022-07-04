#include "server.hpp"

std::vector<conf_data*> *readConfFile(std::string const &file){
	std::vector<conf_data*> *co = new std::vector<conf_data*>();

	std::ifstream f(file, std::ifstream::in);
	if (f.fail())
		throw std::runtime_error("configuration file not found or corrupt");

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

		if (l_ine == "server"){
			co->push_back(new conf_data());

			std::vector<conf_data*>::iterator it= --co->end();
			f.getline(line, 100, '}');
			std::stringstream content(line);
			std::string rules[2] = {"server_names", "port"};

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
					default:
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

// int main(){
// 	std::vector<conf_data*> *co = readConfFile("../config");
// 	// std::vector<conf_data*>::iterator it = co->begin();
// 	// for (size_t i = 0; i < co->size(); i++)
// 	// {
// 	// 	std::cout << "element: " << i + 1 << "\nserver_names: " << (*it)->server_names << "\nport: " << (*it)->port << std::endl;
// 	// 	++it;
// 	// }
// 	for (std::vector<conf_data*>::iterator ite = co->begin(); ite != co->end(); ite++)
// 		delete *ite;
	
// 	delete co;
// 	return 0;
// }
