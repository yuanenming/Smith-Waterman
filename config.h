#pragma once
#include <fstream>
using namespace std;

/*
    This class is used to read config file of SmithWaterman.
	
	Make sure the config file is in correct format before running.
	The file is read and processed immediately the class of object is constructed.    
*/

class config
{
protected:
	
	//config file stream
	fstream file;

	//handle the scoring matrix input
	void getScoringMatrix(char * line);

	//handle the penalty factor A
	void getPenaltyA(char * line);

	//handle the penalty factor B
	void getPenaltyB(char * line);

	//handle the option on report
	void getReportOn(char * line);

public:

	//scoring matrix, pointing to a 4x4 matrix
	double ** scoringMatrix;

	//penalty parameter A
	double penaltyA;

	//penalty parameter B
	double penaltyB;

	//signal whether to append report or not
	bool reportOn;

	//constructor, receiving the path of config file as parameter
	config(const char * ConfigFileName);

};


void config::getScoringMatrix(char * line)
{
	for (int i = 0; i<4; i++)
		for (int j = 0; j < 4; j++)
		{
			file >> scoringMatrix[i][j];
		}
}

void config::getPenaltyA(char * line)
{
	file >> penaltyA;
}

void config::getPenaltyB(char * line)
{
	file >> penaltyB;
}

void config::getReportOn(char * line)
{
	file.getline(line, 1000);
	int i = 0;
	while (i < 1000 && line[i] == ' ')
		i++;
	if (i == 1000)
		throw("config error:can not read parameter \"report\"!");

	if (line[i] == 'o'&&line[i + 1] == 'n')
		reportOn = true;
	else
	{
		if (line[i] == 'o'&&line[i + 1] == 'f'&&line[i + 2] == 'f')
			reportOn = false;
		else
			throw("config error:can not read parameter \"report\"!");
	}
}

config::config(const char * ConfigFileName)
{

	file.open(ConfigFileName);
	if (!file.is_open())
		throw("config error:can not open the config file!");

	char line[1000];

	scoringMatrix = new double*[4];
	for (int i = 0; i < 4; i++)
		scoringMatrix[i] = new double[4];
    
int count = 0;
	while (!file.eof())
	{
		memset(line, 0, sizeof(line));
		file.getline(line, 1000);
		if ((line[0] == '/'&&line[1] == '/') || (line[0] == '\0') || (line[0] == '\n'))
			continue;

		int i = 0;
		while (line[i] == ' ')
			i++;

		switch (line[i])
		{
		case 's':getScoringMatrix(line);
			break;
		case 'a':getPenaltyA(line);
			break;
		case 'b':getPenaltyB(line);
			break;
		case 'r':getReportOn(line);
			break;
		default: throw("config error:error in config file, please correct!");
		}
	}

	file.close();

}