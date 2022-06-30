#include "server.hpp"

std::map<std::string, std::string> initialize_mime_types(void)
{
    std::map<std::string, std::string> mime_types;

	mime_types[".aac"]      = "audio/aac\r\n\r\n";
	mime_types[".abw"]      = "application/x-abiword\r\n\r\n";
	mime_types[".arc"]      = "application/octet-stream\r\n\r\n";
	mime_types[".avi"]      = "video/x-msvideo\r\n\r\n";
	mime_types[".azw"]      = "application/vnd.amazon.ebook\r\n\r\n";
	mime_types[".bin"]      = "application/octet-stream\r\n\r\n";
	mime_types[".bz"]       = "application/x-bzip\r\n\r\n";
	mime_types[".bz2"]      = "application/x-bzip2\r\n\r\n";
	mime_types[".csh"]      = "application/x-csh\r\n\r\n";
	mime_types[".css"]      = "text/css\r\n\r\n";
	mime_types[".csv"]      = "text/csv\r\n\r\n";
	mime_types[".doc"]      = "application/msword\r\n\r\n";
	mime_types[".epub"]     = "application/epub+zip\r\n\r\n";
	mime_types[".gif"]      = "image/gif\r\n\r\n";
	mime_types[".htm"]      = "text/html\r\n\r\n";
	mime_types[".html"]     = "text/html\r\n\r\n";
	mime_types[".ico"]      = "image/x-icon\r\n\r\n";
	mime_types[".ics"]      = "text/calendar\r\n\r\n";
	mime_types[".jar"]      = "Temporary Redirect\r\n\r\n";
	mime_types[".jpeg"]     = "image/jpeg\r\n\r\n";
	mime_types[".jpg"]      = "image/jpeg\r\n\r\n";
	mime_types[".js"]       = "application/js\r\n\r\n";
	mime_types[".json"]     = "application/json\r\n\r\n";
	mime_types[".mid"]      = "audio/midi\r\n\r\n";
	mime_types[".midi"]     = "audio/midi\r\n\r\n";
	mime_types[".mpeg"]     = "video/mpeg\r\n\r\n";
	mime_types[".mpkg"]     = "application/vnd.apple.installer+xml\r\n\r\n";
	mime_types[".odp"]      = "application/vnd.oasis.opendocument.presentation\r\n\r\n";
	mime_types[".ods"]      = "application/vnd.oasis.opendocument.spreadsheet\r\n\r\n";
	mime_types[".odt"]      = "application/vnd.oasis.opendocument.text\r\n\r\n";
	mime_types[".oga"]      = "audio/ogg\r\n\r\n";
	mime_types[".ogv"]      = "video/ogg\r\n\r\n";
	mime_types[".ogx"]      = "application/ogg\r\n\r\n";
	mime_types[".png"]      = "image/png\r\n\r\n";
	mime_types[".pdf"]      = "application/pdf\r\n\r\n";
	mime_types[".ppt"]      = "application/vnd.ms-powerpoint\r\n\r\n";
	mime_types[".rar"]      = "application/x-rar-compressed\r\n\r\n";
	mime_types[".rtf"]      = "application/rtf\r\n\r\n";
	mime_types[".sh"]       = "application/x-sh\r\n\r\n";
	mime_types[".svg"]      = "image/svg+xml\r\n\r\n";
	mime_types[".swf"]      = "application/x-shockwave-flash\r\n\r\n";
	mime_types[".tar"]      = "application/x-tar\r\n\r\n";
	mime_types[".tif"]      = "image/tiff\r\n\r\n";
	mime_types[".tiff"]     = "image/tiff\r\n\r\n";
	mime_types[".ttf"]      = "application/x-font-ttf\r\n\r\n";
	mime_types[".txt"]      = "text/plain\r\n\r\n";
	mime_types[".vsd"]      = "application/vnd.visio\r\n\r\n";
	mime_types[".wav"]      = "audio/x-wav\r\n\r\n";
	mime_types[".weba"]     = "audio/webm\r\n\r\n";
	mime_types[".webm"]     = "video/webm\r\n\r\n";
	mime_types[".webp"]     = "image/webp\r\n\r\n";
	mime_types[".woff"]     = "application/x-font-woff\r\n\r\n";
	mime_types[".xhtml"]    = "application/xhtml+xml\r\n\r\n";
	mime_types[".xls"]      = "application/vnd.ms-excel\r\n\r\n";
	mime_types[".xml"]      = "application/xml\r\n\r\n";
	mime_types[".xul"]      = "application/vnd.mozilla.xul+xml\r\n\r\n";
	mime_types[".zip"]      = "application/zip\r\n\r\n";
	mime_types[".3gp"]      = "video/3gpp audio/3gpp\r\n\r\n";
	mime_types[".3g2"]      = "video/3gpp2 audio/3gpp2\r\n\r\n";
	mime_types[".7z"]		= "application/x-7z-compressed\r\n\r\n";

	return (mime_types);
}

char* parse(char line[], const char symbol[])
{
    char *copy = strdup(line);
    char* message = NULL;
    char* token = strtok(copy, symbol);
	token = strtok(NULL, " ");
	if (token == NULL)
	{
		delete[] copy;
		return (message);
	}
	message = strdup(token);
	delete[] copy;
	return message;
}

char* parse_method(char line[], const char symbol[])
{
    char *copy = strdup(line);
    char *message = NULL;
    char * token = strtok(copy, symbol);
      
	//token = strtok(NULL, " ");
	message = strdup(token);
	delete[] copy;
	return message;
}
