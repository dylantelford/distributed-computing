The goal of this project was to use c++ and MPI to control
the 17-node department cluster to analyze and answer queries on
4 "Bag of Words" datasets. 

Each of the 4 datasets contained 3 files: 
One file listing all of the words in the set, one file listing the 
word number, line number, and number of times it apperaed on that 
line, and a third file listing the word number and the byte number 
that that word starts to appear in the second file. 

The "head node" would distribute parts of the files to the babies, 
who would process and return their answers to the head, which would
then present the user with a menu to ask the following queries: 

    1. What percent of documents in X use any one word more than __ times?
    2. What words in X are used more than __ times in any document?
    3. In which data set does the word ____ appear most frequently?
    4. Does the word ____ appear more frequently in X or Y?

