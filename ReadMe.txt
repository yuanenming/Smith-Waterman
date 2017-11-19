Welcome!

This software implement the Smith-Waterman algorithm, which aligns two 
  DNA sequences and returns two sequences with similar regions matched.
Please read the following STEPs before using.

STEP 0. About This software

  This software is developed completely with C++ language, which is 
   supported by most systems. No other framework is needed for running.
  This is a course project of Data Science Frontier of Nankai University,
   independently developed by Xie Zhouyang. Anyone can use this software
   for any purpose and make any modification(source code is available).
  This software with source code is availabel at https://github.com/XieZhouyang/SmithWaterman.git

STEP 1. Smith-Waterman Algorithm

  If you're not familiar with this algorithm and want to have an 
   overview on it, please read the introduction under this directory, 
   and it's OK that you use this software without any knowledge about the algorithm.

STEP 2. Load The Sequences

  The two DNA sequences should be written to 
   ./SmithWaterman/sequcences.txt . 
  Each of the sequences starts with a '<' and ends with a '>', and no other characters
   should be inserted into the sequences except 
   'A','T','C','G',' '(space),'\n'(next line).
  And it's recommended that you don't append other text to this file.

STEP 3. Configuration

  It's optional to set the parameters, as there is default settings.
  If you want to set the parameters, open ./SmithWaterman/config.txt and
   modify the parameters following the instruction in it.

STEP 4. Running

  You can run this software by simply double-click on the 
   ./SmithWaterman/SmithWaterman.exe , but you might miss error infomation if any.
  The best way is to run SmithWaterman.exe by system command-line.

STEP 5. Result

  The result of alignment will be output to ./SmithWaterman/result.txt, in the
   same format with the input sequence.txt .

STEP 6. Further Use

  This software is far from perfect, and, if you want to improve this, 
    the source code is available under the ./SourceCode. 
  The code is finely commented.