#ifndef FILEMANAGERHPP
#define FILEMANAGERHPP

#include <fstream>

static class FileManager
{
public:
	FileManager();
	FileManager(std::string fileName);
	~FileManager();

	std::ofstream outputStream;
	std::string fileName;
};

#endif // !FILEMANAGERHPP



