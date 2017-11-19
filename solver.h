#pragma once
#include <string>
#include <iostream>
using namespace std;

/*
	This class receive two DNA sequences with arbitrary length,
	  and align them by Smith-Waterman algorithm.

	The parameters needed in the algorithm are also needed in parameter table
	  of constructor, check the constructor for more information.

	This class also detect illegal character within the sequence and
	  record useful statistics about the alignment.
*/

class solver
{
private:

	//For memory concern, we require that one instance of solver
	// are used only once.
	//This will be assigned with true in method solve()
	bool computed;

protected:

	//the two DNA sequence to align  
	char * seq_a;
	char * seq_b;

	//similarity matrix
	double ** sim;
	
	//used in penalty fucntion
	double pen_a;
	double pen_b;
	
	//scoring matrix
	double ** H;

	//final result
	char * aliSeq_a;
	char * aliSeq_b;

	//record the maximum of the scoring matrix while running ComputeH()
	int mx;
	int my;
	double maxScore;



	//convert acid to number for indexing
	int acid2int(char acid);

	//penalty function: return gap penalty of k-gap
	double pen(int k);

	//this function computer H(scoring matrix) following the Smith-Waterman algorithm
	//check the detail in the README file
	void ComputeH();

	//This function return a signal for every step of BackTrack as direction
	char TrackStep(int x, int y);

	//This function implement the stage of tracking-back
	void BackTrack();


public:

	//Strings of aligned sequences, asigned by aliSeq_a and aliSeq_b after alignment
	//The pointers to aligned sequences are otherwise set in public module of this class
	//  to avoid memory leaks and exceptions in deconstractor.
	char * result_a;
	char * result_b;
	
	//length of aligned sequences, supposed to be the same for both of them
	int aliLen;
	
	//the number of matched pairs in aligned sequences
	int matchCount;

	//length of the two unaligned DNA
	int length_a;
	int length_b;

	//constructor, receiving parameters:
	//		    squence_a: char* pointer to sequence A.
	//			squence_b: char* pointer to sequence B.
	//		   similarity: char** pointer to a 4x4 matrix of similarity of 4 base.
	//    penalty_opening: parameter of gap penalty, see method pen().
	//  penalty_extension: parameter of gap penalty, see method pen().
	solver(char* sequence_a, char* sequence_b, double ** similarity, double penalty_opening, double penalty_extension);

	//deconstructor, delete H, aliSeq_a and aliSeq_b 
	~solver();
	
	//start the process of alignment, this method is called only once
	void solve();
};

int solver::acid2int(char acid)
{
	switch (acid)
	{
	case 'A':return 0;
	case 'C':return 1;
	case 'G':return 2;
	case 'T':return 3;
	default:throw("runtime error: find illegal character in DNA sequence");
	}
}

double solver::pen(int k)
{
	if (k <= 0)
		throw("runtime error: gap penalty function non-positive receive parameter.");
	return k*pen_a + pen_b;
}

void solver::ComputeH()
{
	for (int i = 1; i<length_a; i++)
		for (int j = 1; j < length_b; j++)
		{
			double * scrArr = new double[i + j + 1];
			//this array contains i+j+1 prospective maximum 
			// that will be assigned to H[i][j]


			//calculate the i+j+1 values of scrArr
			for (int k = 1; k <= i; k++)
			{
				scrArr[k] = H[i - k][j] - pen(k);
			}

			for (int k = 1; k <= j; k++)
			{
				scrArr[k + i] = H[i][j - k] - pen(k);
			}

			double similarity = sim[acid2int(seq_a[i - 1])][acid2int(seq_b[j - 1])];

			scrArr[0] = H[i - 1][j - 1] + similarity;


			//select the maximum of scrArr
			double maxScrArr = scrArr[0];

			for (int k = 1; k < i + j + 1; k++)
			{
				if (scrArr[k] >= maxScrArr)
					maxScrArr = scrArr[k];
			}


			//assign H[i][j] with maximum of scrArr
			//  and record the current maximum of H for the later BackTrack 
			H[i][j] = maxScrArr;

			if (maxScrArr > maxScore)
			{
				mx = i;
				my = j;
				maxScore = maxScrArr;
			}
			delete[] scrArr;
		}
}

void solver::solve()
{
	//As an instance of solve is for once-only-use,
	//the second call on solve() will be terminated here.
	if (computed)
	{
		return;
	}
	else
	{
		computed = true;
	}

	ComputeH();

	BackTrack();

	result_a = aliSeq_a;
	result_b = aliSeq_b;
}

solver::~solver()
{
	//We don't delete double ** sim here
	// because its memory is allocated outside this class

	//delete scoring matrix
	for (int i = 0; i < length_a; i++)
		delete[] H[i];

	delete[] H;


	//try this part as it's possible that user have deleted the aligned sequence strings
	//  by char * result_a or char * result_b before deconstruction
	try
	{
		delete[] aliSeq_a;
		delete[] aliSeq_b;
	}
	catch (exception)
	{
		throw("runtime error: the aligned sequence strings are already delete before solver's deconstruction");
	}
}

solver::solver(char* sequence_a, char* sequence_b, double ** similarity, double penalty_opening, double penalty_extension)
{
	//check parameters
	length_a = strlen(sequence_a);
	length_b = strlen(sequence_b);

	if (length_a <= 0 || length_b <= 0 || penalty_extension <= 0 || penalty_opening < 0)
		throw("initilization error: parameter is wrongly set!");

	//initialize class attributes

	computed = false;

	aliLen = 0;

	matchCount = 0;

	//It's unlikely to validate the char ** similarity.
	//Exception should be catched, if any
	sim = similarity;


	if (sequence_a == nullptr || sequence_b == nullptr)
		throw("initilization error: pointer is null!");
	seq_a = sequence_a;
	seq_b = sequence_b;

	pen_a = penalty_extension;
	pen_b = penalty_opening;

	//The lengths increase by one for convenience of constructing the scoring matrix and
	length_a += 1;
	length_b += 1;

	mx = 0;
	my = 0;
	maxScore = 0;

	H = new double *[length_a];
	for (int i = 0; i < length_a; i++)
	{
		H[i] = new double[length_b];
		memset(H[i], 0, length_b * sizeof(double));
	}

	aliSeq_a = new char[length_a + length_b];
	aliSeq_b = new char[length_a + length_b];
	memset(aliSeq_a, 0, (length_a + length_b) * sizeof(char));
	memset(aliSeq_b, 0, (length_a + length_b) * sizeof(char));
	/*
	we set the length of aligned sequence to the sum of two DNA,
	to handle the worst situation where no pairs are matched.
	*/

	result_a = nullptr;
	result_b = nullptr;
}

void solver::BackTrack()
{
	//load the maximum and its coordinate of H
	//  x,y are dynamical in the process of BackTrack
	int x = mx;
	int y = my;

	// i pointing to current blank position of aliSeq_a and aliSeq_b
	int i = 0;

	//step dynamically denotes the direction of next step
	char step = 'D';


	//The process of BackTrack
	while (step!='E')
	{
		step = TrackStep(x, y);

		switch (step)
		{
		case 'D':aliSeq_a[i] = seq_a[x - 1];
			aliSeq_b[i] = seq_b[y - 1];
			i++;
			x--; y--;
			break;

		case 'L':aliSeq_a[i] = '-';
			aliSeq_b[i] = seq_b[y - 1];
			i++;
			y--;
			break;

		case 'U':aliSeq_a[i] = seq_a[x - 1];
			aliSeq_b[i] = '-';
			i++;
			x--;
			break;

		case 'E':aliSeq_a[i] = seq_a[x - 1];
			aliSeq_b[i] = seq_b[y - 1];
			i++;
			break;
		}

	}

	aliSeq_a[i] = '\0';
	aliSeq_b[i] = '\0';

	aliLen = i;
	//The length of aligned sequences are determined by current i
	i--;
	//make i pointing to the last character of aligned sequences


	//reverse the two aligned strings and calculate matchCount
	for (int j = 0; j <= i; j++, i--)
	{
		if (aliSeq_a[j] == aliSeq_b[j] && i != j)
			matchCount++;

		if (aliSeq_a[i] == aliSeq_b[i])
			matchCount++;

		swap(aliSeq_a[j], aliSeq_a[i]);
		swap(aliSeq_b[j], aliSeq_b[i]);
	}

}

char solver::TrackStep(int x, int y)
{
	if (H[x-1][y-1]==0|| H[x][y - 1] == 0 || H[x - 1][y] == 0)
		return 'E';
	//'E' means ending the process of BackTrak as it comes a 0 in the neighbourhood

	if (H[x - 1][y - 1] >= H[x - 1][y] && H[x - 1][y - 1] >= H[x][y - 1])
		return 'D';
	//'D' means go upper-left (diagonally)

	if (H[x - 1][y] > H[x][y - 1])
		return 'U';
	//'U' means go up
	else
		return 'L';
	//'L' means go left
}