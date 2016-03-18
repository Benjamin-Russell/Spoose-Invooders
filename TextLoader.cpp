#include "TextLoader.h"

TextLoader::TextLoader()
{

}

TextLoader::~TextLoader()
{

}

vector<string> TextLoader::loadFile(string fileName)
{
	ifstream dataFile;
	dataFile.open(fileName);
	string tmpStr;
	vector<string> fileLines;

	if (!dataFile.is_open())
	{
		cout << "Error loading " << fileName << "." << endl;
		return fileLines;
	}

	while (getline(dataFile, tmpStr))
	{
		if (tmpStr.compare(0, 1, "#"))	// Comments in data files start with #
			fileLines.push_back(tmpStr);
	}

	dataFile.close();

	return fileLines;
}