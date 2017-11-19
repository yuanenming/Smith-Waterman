#pragma once
#include <fstream>
using namespace std;

/*
	This function is called for reading DNA sequences in the file,
	  taking parameters:
			filePath: path to two DNA sequences file.
			  buffer: pointer to dynamic memory buffering the whole file text.
			   seq_a: pointer to dynamic memory to store the first DNA sequence.
               seq_b: pointer to dynamic memory to store the second DNA sequence.
	         fileEnd: character signalling the end of file.
*/
void SequenceReader(const char * filePath, char * seq_a, char * seq_b, char fileEnd);



void SequenceReader(const char * filePath, char * seq_a, char * seq_b,char fileEnd)
{
	fstream file;
	
	try
	{
		file.open(filePath);
	}
	catch (exception)
	{
		throw("resource error: failed to open the DNA sequence file");
	}	

	char * buffer;
	buffer = new char[1000000];

	//set the max of file stream to 1000000
	file.getline(buffer, 1000000, fileEnd);

	int i = 0;
	int j = 0;

	//skip all the instruction
	while (buffer[i] != '<')
		i++;

	i++;

	while (buffer[i] != '>')
	{
		if (buffer[i] == '\n'|| buffer[i] == ' ')
		{
			i++;
			continue;
		}

		seq_a[j] = buffer[i];
		j++;
		i++;
	}

	seq_a[j] = '\0';

	
	//skip until next "<"
	while (buffer[i] != '<')
		i++;
	
	i++;
	j = 0;
	
	while (buffer[i] != '>')
	{
		if (buffer[i] == '\n'|| buffer[i] == ' ')
		{
			i++;
			continue;
		}

		seq_b[j] = buffer[i];
		j++;
		i++;
	}

	seq_b[j] = '\0';

	delete[] buffer;
}