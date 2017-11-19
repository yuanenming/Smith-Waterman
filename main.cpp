#include <fstream>
#include <iostream>
#include <cstring>
#include "SmithWater.h"
using namespace std;

int main()
{
	try {

		//The max length of two sequences set here is 5000(in step 2), adjust it for your need.
		const int maxSequenceLength = 5000;

		//step1: read config file
		cout << "step 1: read the config file." << endl;
		config con("config.txt");
		/*
			After the declaration of config con,
			  public attributes of this object are:

				double ** scoringMatrix
				double penaltyA
				double penaltyB
				  bool reportOn
		*/
		cout << "step 1: the config file is read successfully." << endl;


		//step2: read DNA sequences
		cout << "step 2: read the DNA sequences." << endl;
		char * seq_a=new char[maxSequenceLength];
		char * seq_b=new char[maxSequenceLength];
		SequenceReader("sequences.txt", seq_a, seq_b, '#');
		/*
			After this function call, sequences are now stored in seq_a and seq_b, with '\0' at the end.
		*/
		cout << "step 2: the DNA sequence is read successfully."<<endl;


		//step 3: alignment
		cout << "step 3: align the sequences."<<endl;
		solver aliSol(seq_a, seq_b, con.scoringMatrix, con.penaltyB, con.penaltyA);
		cout << "running..." << endl;
		aliSol.solve();
		
		/*
			After aliSol.solve(), solve.{
				aliLen,
				matchCount,
				result_a,
				result_b
				}
			are available now.
		*/
		cout << "step 3: alignment is done."<<endl;

		//step 4: output
		cout << "step 4: output the result." << endl;
		outputSeq("result.txt", seq_a, seq_b, con.reportOn, &aliSol);
		cout << "step 4: the result is successfully written to the file." << endl;

		delete[] seq_a;
		delete[] seq_b;
	}
	catch (const char * error)
	{
		cout << error << endl<< "The program is shut down." <<endl;
	}
	catch (exception)
	{
		cout<<"runtime error: unknow error ocurred"<<endl 
			<< "The program is shut down." << endl;
	}

	return 0;
}