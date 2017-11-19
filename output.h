#pragma once
#include <fstream>
#include "solver.h"
using namespace std;

/*
		This function write the aligned sequences to a specific file,
		  with option to also write a report to the file.
*/

void outputSeq(const char * path, char * seq_a, char * seq_b, bool reportOn = true, const solver * aliSol=nullptr)
{
	fstream file;
	try 
	{
		file.open(path,ios::app);
	}
	catch (exception)
	{
		throw("output error: cannot open result file.");
	}

    //Set the pointer to the end of file
	file.seekp(ios::end);

	try
	{
		file << "<" << endl;

		int length = strlen(aliSol->result_a);

        //These variables are for iteratively outputing, the default is 50 characters per line
		char matchLine[51];
		matchLine[50] = '\0';

		char tempA[51];
		tempA[50] = '\0';

		char tempB[51];
		tempB[50] = '\0';

		int i;
		for (i = 0; i < length; i++)
		{
			if((i+1) % 50 == 0)
			{
				strncpy(tempA, (aliSol->result_a)+ i - 49, 50);
				strncpy(tempB, (aliSol->result_b)+ i - 49, 50);

				file << tempA << endl 
					 << matchLine << endl 
					 << tempB << endl
					 <<endl;
			}

			matchLine[i % 50] = aliSol->result_a[i]== aliSol->result_b[i] ? '|' : ' ';
		}

		if ( i % 50 != 0)
		{
			strncpy(tempA, (aliSol->result_a) + i - (i%50) , i%50 );
			strncpy(tempB, (aliSol->result_b) + i - (i%50) , i%50 );

			tempA[i%50] = '\0';
			tempB[i%50] = '\0';

			matchLine[i % 50] = '\0';

			file << tempA << endl 
				 << matchLine << endl 
				 << tempB << endl
				 <<endl;
		}

		file << ">" << endl;
	}
	catch (exception)
	{
		throw("output error: failed to output the result.");
	}

	if (reportOn)
	{
		file << "----------------------------------------" << endl
			<< "report:"<<endl
			<< "sequence A length : " << aliSol->length_a << endl
			<< "sequence B length : " << aliSol->length_b << endl
			<< "aligned sequence length : " << aliSol->aliLen << endl
			<< "matched pairs in aligned sequences : " << aliSol->matchCount << endl
			<< "----------------------------------------\n" << endl;
	}

	file.close();
}