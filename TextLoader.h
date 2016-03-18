#ifndef TEXTLOADER_H
#define TEXTLOADER_H

#include "Trackable.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class TextLoader : public Trackable
{
public:
	TextLoader();
	~TextLoader();

	vector<string> loadFile(string fileName);
private:
};
#endif