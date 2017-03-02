/***************
* Dylan Telford
* CSC 330
* Project 3
*
*       This MPI program will read and answer questions about 4 bags of words. 
*       The head node will distribute the work to the different baby nodes, 
*       which will each read a portion of the data. The head will then present 
*       a user interface, allowing queries. Queries are sent to the babies and 
*       the babies send their totals to the head. The head sums the totals and 
*       presents an answer to the user.
*/

// header files 
#include <stdio.h>
#include <string>
#include <math.h>
#include <mpi.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <iterator>

// constant definitions for each data set
#define ENRON 1
#define NIPS 2
#define KOS 3
#define NYT 4
// constant definition for each query
#define Q1 5
#define Q2 6
#define Q3 7
#define Q4 8
// other constants
#define DONE 9
#define STOP 10

using namespace std;

// dataSet definition and declarations
// each of the bags of words will have their own dataSet struct containing necessary info and file paths
struct dataSet {
        int id;
        int numDocs;
        int numWords;
        int numNonZero;

        string vocabPath;
        string wordPath;
        string startPath;
};
dataSet enronData;
dataSet nipsData;
dataSet kosData;
dataSet nytData;

// dwdata is the struct that each baby will use to fill their array based on the docword files
struct dwdata {
        int docNum;
        int wordNum;
        int count;
};
// dwdata arrays declared here (head node will not use this but defining here allows access by all babies)
dwdata* enronArr;
dwdata* nipsArr;
dwdata* kosArr;
dwdata* nytArr;
// string arrays for each bag of words to hold their vocab files. Only the head node will use these. 
string* enronVocab;
string* nipsVocab;
string* kosVocab;
string* nytVocab;

// MPI variables for number of nodes and ranks
int numProcessors;
int myRank;

// size variables will be used by each baby to indicate the size of their dwdata array based on the number of entries they make
int enronSize;
int nipsSize;
int kosSize;
int nytSize;

// function declarations
void makeDataSets();
void splitAndSend(dataSet mySet);
void recieveAndRead(dataSet mySet);
void fillArray(dataSet thisSet, int startPos, int stopPos);
dwdata* resizeArray(dwdata* myArray, int size);
void menu();
void makeVocabArray(dataSet mySet);
void recieveAnyQ();
string sendQ1(int x, int num);
string sendQ2(int x, int num);
string sendQ3(string word);
string sendQ4(int x, int y, string word);
void doQ1(int x, int num);
void doQ2(int x, int num);
void doQ3(int enronWord, int nipsWord, int kosWord, int nytWord);
void doQ4(int enronWord, int nipsWord, int kosWord, int nytWord);



// main method
int main(int argc, char *argv[]){
        // all nodes make their 4 dataSets, basic info on each bag of words
        makeDataSets();

        // MPI setup
        MPI_Init(&argc, &argv);
        MPI_Comm_size(MPI_COMM_WORLD, &numProcessors);
        MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
        MPI_Status stat;

        // if I am the head node
        if(myRank == 0){
                // print the number of docs, words, and nonZero counts for the user to see
                printf(" ENRON -- docs: %d, words:%d, nonZero: %d \n", enronData.numDocs, enronData.numWords, enronData.numNonZero);
                printf(" NIPS -- docs: %d, words:%d, nonZero: %d \n", nipsData.numDocs, nipsData.numWords, nipsData.numNonZero);
                printf(" KOS -- docs: %d, words:%d, nonZero: %d \n", kosData.numDocs, kosData.numWords, kosData.numNonZero);
                printf(" NYT -- docs: %d, words:%d, nonZero: %d \n", nytData.numDocs, nytData.numWords, nytData.numNonZero);
                printf("reading in files...\n");
                // send each baby their start and stop positions for each bag of words
                splitAndSend(enronData);
                splitAndSend(nipsData);
                splitAndSend(kosData);
                splitAndSend(nytData);
                // while babies are reading docwords, fill vocab arrays
                makeVocabArray(enronData);
                makeVocabArray(nipsData);
                makeVocabArray(kosData);
                makeVocabArray(nytData);
                // before displaying menu, wait for each of the babies to send a mesage with DONE tag
                int j;
                int fin;
                for(j=0; j<numProcessors-1; j++){
                        MPI_Recv(&fin, 1, MPI_INT, MPI_ANY_SOURCE, DONE, MPI_COMM_WORLD, &stat);
                }
                // display user interface. menu() will call appropriate functions based on user input, runs until user quits.
                menu();

        } else{
                // if I am not the head node
                // initialize dwdata arrays with a starting size of 2000. These will be dynamic and the size variable will keep track.
                enronArr = new dwdata[2000];
                nipsArr = new dwdata[2000];
                kosArr = new dwdata[2000];
                nytArr = new dwdata[2000];
                // Recieve my start and stop values for each bag of words, read appropriate docstart file into dwdata arrays
                recieveAndRead(enronData);
                recieveAndRead(nipsData);
                recieveAndRead(kosData);
                recieveAndRead(nytData);
                // Send a message to the head node that I have finished reading my section
                int fin = 1;
                MPI_Send(&fin, 1, MPI_INT, 0, DONE, MPI_COMM_WORLD);
                // be ready to recieve a message with a query tag. recieveAnyQ() will call functions based on query tag. 
                recieveAnyQ();
        }
        // Finalize MPI
        MPI_Finalize();
}

// other functions
void makeDataSets(){
        // this function will be called by every node. For each bag of words, it will construct the dataSet struct. 
        // id and paths are hard coded while numDocs, numWords, and numNonZero are read from docword file
        string docs;
        string words;
        string nonZero;
        // make enronData
        enronData.id = 1;
        enronData.vocabPath = "/home/mcconnel/BagOfWords/vocab.enron.txt";
        enronData.wordPath = "/home/mcconnel/BagOfWords/docword.enron.txt";
        enronData.startPath = "/home/mcconnel/BagOfWords/docstart.enron.txt";
        ifstream enronDW(enronData.wordPath.c_str());
        getline(enronDW, docs);
        getline(enronDW, words);
        getline(enronDW, nonZero);
        enronDW.close();
        enronData.numDocs = std::atoi(docs.c_str());
        enronData.numWords = std::atoi(words.c_str());
        enronData.numNonZero = std::atoi(nonZero.c_str());
        // make nipsData
        nipsData.id = 2;
        nipsData.vocabPath = "/home/mcconnel/BagOfWords/vocab.nips.txt";
        nipsData.wordPath = "/home/mcconnel/BagOfWords/docword.nips.txt";
        nipsData.startPath = "/home/mcconnel/BagOfWords/docstart.nips.txt";
        ifstream nipsDW(nipsData.wordPath.c_str());
        getline(nipsDW, docs);
        getline(nipsDW, words);
        getline(nipsDW, nonZero);
        nipsDW.close();
        nipsData.numDocs = std::atoi(docs.c_str());
        nipsData.numWords = std::atoi(words.c_str());
        nipsData.numNonZero = std::atoi(nonZero.c_str());
        // make kosData
        kosData.id = 3;
        kosData.vocabPath = "/home/mcconnel/BagOfWords/vocab.kos.txt";
        kosData.wordPath = "/home/mcconnel/BagOfWords/docword.kos.txt";
        kosData.startPath = "/home/mcconnel/BagOfWords/docstart.kos.txt";
        ifstream kosDW(kosData.wordPath.c_str());
        getline(kosDW, docs);
        getline(kosDW, words);
        getline(kosDW, nonZero);
        kosDW.close();
        kosData.numDocs = std::atoi(docs.c_str());
        kosData.numWords = std::atoi(words.c_str());
        kosData.numNonZero = std::atoi(nonZero.c_str());
        // make nytData
        nytData.id = 4;
        nytData.vocabPath = "/home/mcconnel/BagOfWords/vocab.nytimes.txt";
        nytData.wordPath = "/home/mcconnel/BagOfWords/docword.nytimes.txt";
        nytData.startPath = "/home/mcconnel/BagOfWords/docstart.nytimes.txt";
        ifstream nytDW(nytData.wordPath.c_str());
        getline(nytDW, docs);
        getline(nytDW, words);
        getline(nytDW, nonZero);
        nytDW.close();
        nytData.numDocs = std::atoi(docs.c_str());
        nytData.numWords = std::atoi(words.c_str());
        nytData.numNonZero = std::atoi(nonZero.c_str());
}

void makeVocabArray(dataSet mySet){
        // this function will only be called by the head node. It takes a dataSet as a parameter, and based on the
        // mySet.id value, it fills the appropriate vocab array with the words from the vocab file (who's path is found in
        // the mySet.vocabPath variable). It should be noted that the first word of each set will be at index 0 in this array
        // while it's word number in the docwords file is 1. Account for this when sending values in queries. (pos+1 = wordnum)

        // open vocab file for mySet
        ifstream voc(mySet.vocabPath.c_str());
        // if mySet is enronData, fill that array from vocab file
        if(mySet.id == 1){ 
                enronVocab = new string [enronData.numWords];
                int i;
                for(i=0; i<mySet.numWords; i++){
                        voc >> enronVocab[i];
                }
        } else if(mySet.id == 2){ // if mySet is nipsData, fill that array from vocab file
                nipsVocab = new string [nipsData.numWords];
                int i;
                for(i=0; i<mySet.numWords; i++){
                        voc >> nipsVocab[i];
                }
        } else if(mySet.id == 3){ // if mySet is kosData, fill that array from vocab file
                kosVocab = new string [kosData.numWords];
                int i;
                for(i=0; i<mySet.numWords; i++){
                        voc >> kosVocab[i];
                }
        } else if(mySet.id == 4){ // if mySet is nytData, fill that array from vocab file
                nytVocab = new string [nytData.numWords];
                int i;
                for(i=0; i<mySet.numWords; i++){
                        voc >> nytVocab[i];
                }
        }
        // close vocab file for mySet
        voc.close();
}

void splitAndSend(dataSet mySet){
        // this function will only be called by the head node, but it will be called 4 times (once for each bag of words)
        // It will determine a start byte and stop document for each node and send the two values in an array to their appropriate baby
        int i;
        int id;
        id = mySet.id;
        // each node will do this for loop, node ranks are equal to i+1
        for(i=0; i<numProcessors-1; i++){
                int startDoc;
                int stopDoc;
                // the first node starts at doc 1, otherwise it starts at i * (the number of documents / number of babies)
                if(i == 0){
                        startDoc = 1;
                } else{
                        startDoc = i * (mySet.numDocs / (numProcessors - 1));
                }
                // the last node needs to stop at the last document, the rest start at their start+(number of documents / number of babies)
                if(i == 15){
                        stopDoc = mySet.numDocs;
                } else if(i == 0){ // special condition for first node (started at 1 not 0)
                        stopDoc = startDoc + (mySet.numDocs / (numProcessors - 1)) - 2;
                } else{
                        stopDoc = startDoc + (mySet.numDocs / (numProcessors - 1)) - 1;
                }

                int doc = 0;
                int byte;
                // open appropriate docstart file, stored in mySet parameter
                ifstream ds(mySet.startPath.c_str());
                // find my start doc and store corresponding byte in int byte
                while(doc != startDoc){
                        ds >> doc;
                        ds >> byte;
                }
                // close docstart file
                ds.close();
                // array to be sent to babies
                int startStop [2];
                startStop[0] = byte;
                startStop[1] = stopDoc;
                // based on id of mySet, send array to i+1 (node rank) with tag denoting which set values are for
                if(id == 1){
                        MPI_Send(startStop, 2, MPI_INT, i+1, ENRON, MPI_COMM_WORLD);
                } else if(id == 2){
                        MPI_Send(startStop, 2, MPI_INT, i+1, NIPS, MPI_COMM_WORLD);
                } else if(id == 3){
                        MPI_Send(startStop, 2, MPI_INT, i+1, KOS, MPI_COMM_WORLD);
                } else if(id == 4){
                        MPI_Send(startStop, 2, MPI_INT, i+1, NYT, MPI_COMM_WORLD);
                }
        }  
}

void recieveAndRead(dataSet mySet){
        // this function will be called 4 times by each baby node (once for each bag of words) with parameter mySet
        // each time it is called, the baby will know which set it is for, and will use that recieve statement. 
        // it will then call the function fillArray() to read the files into arrays. 
        MPI_Status stat;
        int id = mySet.id;
        // array to be recieved from head node
        int startStop[2];
        int start;
        int stop;

        // based on id, will recieve message with appropriate tag
        if(id == 1){
                MPI_Recv(startStop, 2, MPI_INT, 0, ENRON, MPI_COMM_WORLD, &stat);
        } else if(id == 2){
                MPI_Recv(startStop, 2, MPI_INT, 0, NIPS, MPI_COMM_WORLD, &stat);
        } else if(id == 3){
                MPI_Recv(startStop, 2, MPI_INT, 0, KOS, MPI_COMM_WORLD, &stat);
        } else if(id == 4){
                MPI_Recv(startStop, 2, MPI_INT, 0, NYT, MPI_COMM_WORLD, &stat);
        }

        // start and stop values assigned
        start = startStop[0];
        stop = startStop[1];
        // call to fillArray()
        fillArray(mySet, start, stop);
}

void fillArray(dataSet thisSet, int startPos, int stopPos){
        // this function will be called 4 times by each baby. It takes in a dataSet, start byte, and stop doc
        // and then opens the appropriate docword file and reads its section into its dwdata array
        // the array will be dynamic, starting at a size of 2000 and calling resizeArray() as needed. 
        // the counter variable is used to know current position and will be assigned to appropriate size variable at end. 
        int id = thisSet.id;
        int counter = 0;
        int arraySize = 2000;
        // dwdata struct to hold the three values on each line. This is what the array is made of. 
        dwdata package;
        package.docNum = 0;
        // open the docword file for mySet
        ifstream dwords(thisSet.wordPath.c_str());
        // seek to the start byte for this node
        dwords.seekg(startPos);
        // continue to read until the stop doc is passed or until the file ends. 
        while (package.docNum <= stopPos && ! dwords.eof()){
                // store 3 values into 3 struct values
                dwords >> package.docNum;
                dwords >> package.wordNum;
                dwords >> package.count;
                // increment counter
                counter += 1;
                // based on id, check to see if array needs resizing, then assign package to array[counter]
                if(id == 1){
                        if(counter == arraySize){
                                enronArr = resizeArray(enronArr, arraySize);
                                arraySize += 2000;
                        }
                        enronArr[counter] = package;
                } else if(id == 2){
                        if(counter == arraySize){
                                nipsArr = resizeArray(nipsArr, arraySize);
                                arraySize += 2000;
                        }
                        nipsArr[counter] = package;
                } else if(id == 3){
                        if(counter == arraySize){
                                kosArr = resizeArray(kosArr, arraySize);
                                arraySize += 2000;
                        }
                        kosArr[counter] = package;
                } else if(id == 4){
                        if(counter == arraySize){
                                nytArr = resizeArray(nytArr, arraySize);
                                arraySize += 2000;
                        }
                        nytArr[counter] = package;
                }
        }
        // close docword file
        dwords.close();
        // based on set, make arraysize equal to counter. will be different for each node. 
        if(id == 1){
                enronSize = counter;
        } else if(id == 2){
                nipsSize = counter;
        } else if(id == 3){
                kosSize = counter;
        } else if(id == 4){
                nytSize = counter;
        }
}

dwdata* resizeArray(dwdata* myArray, int size){
        // this function will be called by babies when they need to increase the size of their arrays. 
        // it takes an array and size and returns an identical array with 2000 more spots. 
        int newSize = size + 2000;
        // new array declared and initialized with new size
        dwdata* newArray;
        newArray = new dwdata[newSize];
        int i;
        // old values are put into new array
        for(i=0; i<size; i++){
                newArray[i] = myArray[i];
        }
        // delete old array
        delete myArray;
        // return new array
        return newArray;
}

// Send Query Functions
// these functions (one for each query) will send an int array of size 7 that holds the following values:
// params = [x, y, num, enronWordNum, nipsWordNum, kosWordNum, nytWordNum]
// the only difference in send and recieve statements will be the tag (ie. Q1 for query 1)
// these functions then print the answer and return a string to the menu so the menu knows to print again. 
string sendQ1(int x, int num){
        // this function sends out the params array for query 1, recieves and totals the responses, and prints answer
        MPI_Status stat;
        // params = [x, y, num, enronWord, nipsWord, kosWord, nytWord]
        int params[7];
        params[0] = x;
        params[1] = 0;
        params[2] = num;
        params[3] = 0;
        params[4] = 0;
        params[5] = 0;
        params[6] = 0;
        int i;
        // send array to each baby (rank = i+1)
        for(i=0; i<numProcessors-1; i++){
                MPI_Send(params, 7, MPI_INT, i+1, Q1, MPI_COMM_WORLD);
        }
        int j;
        int total = 0;
        int myTotal;
        // recieve myTotal from each baby and increment int total for final answer
        for(j=0; j<numProcessors-1; j++){
                MPI_Recv(&myTotal, 1, MPI_INT, MPI_ANY_SOURCE, Q1, MPI_COMM_WORLD, &stat);
                total += myTotal;
        }
        int totalDocs;
        string name;
        // based on value for x, assign totalDocs the total number of documents in that set and assign a name for set
        if(x == ENRON){
                totalDocs = enronData.numDocs;
                name = "Enron Emails";
        } else if(x == NIPS){
                totalDocs = nipsData.numDocs;
                name = "NIPS Papers";
        } else if(x == KOS){
                totalDocs = kosData.numDocs;
                name = "Daily KOS";
        } else if(x == NYT){
                totalDocs = nytData.numDocs;
                name = "New York Times";
        }
        // calculate percentage of documents that have a word more than num times
        float percent;
        percent = ((float)total / (float)totalDocs) * 100;
        // print final answer to user
        printf("%f percent of %s documents use any one word more than %d times.", percent, name.c_str(), num);
        // return string to menu
        string finished = "finished";
        return finished;
}

string sendQ2(int x, int num){
        // send function for query 2.
        MPI_Status stat;
        // params = [x, y, num, enronWord, nipsWord, kosWord, nytWord]
        int params[7];
        params[0] = x;
        params[1] = 0;
        params[2] = num;
        params[3] = 0;
        params[4] = 0;
        params[5] = 0;
        params[6] = 0;
        int i;
        for(i=0; i<numProcessors-1; i++){
                MPI_Send(params, 7, MPI_INT, i+1, Q2, MPI_COMM_WORLD);
        }
        int j;
        int wordArray[500];
        int k;
        for(k=0; k<500; k++){
                wordArray[k] = 0;
        }
        int entries = 0;
        for(j=0; j<numProcessors-1; j++){
                int myArray[400];
                MPI_Recv(myArray, 400, MPI_INT, MPI_ANY_SOURCE, Q2, MPI_COMM_WORLD, &stat);

                int pos = 0;
                while(myArray[pos] != 0){
                        int duplicate = 0;
                        int pos1 = 0;
                        while(wordArray[pos1] != 0){
                                if(wordArray[pos1] == myArray[pos]){
                                        duplicate = 1;
                                }
                                pos1 += 1;
                        }
                        if(duplicate == 0){
                                wordArray[entries] = myArray[pos];
                                entries += 1;
                        }
                        pos += 1;
                }
        }
        string stringArray[entries];
        string name;
        if(x == ENRON){
                int l;
                for(l=0; l<entries; l++){
                        stringArray[l] = enronVocab[(wordArray[l])-1].c_str();
                }
                name = "Enron Emails";
        }
        
        printf("The following words appeared more than %d times in %s:\n", num, name.c_str());
        int n;
        for(n=0; n<entries; n++){
                // use cout instead of printf becuase strings are not in c
                cout << stringArray[n] << "\n";
        }

        string answer = "";
        return answer;
}

string sendQ3(string word){
        // send function for query 3. will send params to babies, recieve their 4 myTotals, sum them
        // and compare them to then print final answer and return string to menu
        MPI_Status stat;
        // params = [x, y, num, enronWord, nipsWord, kosWord, nytWord]
        int params[7];
        int i;
        // initialize params to 0. If params[3]-[6] remain 0, we know that word does not appear in that bag of words
        for(i=0; i<7; i++){
                params[i] = 0;
        }
        // 4 while loops (one for each set) will read vocab file, incrementing int pos as they go. 
        // if they find the word, they assign pos + 1 as the word number (spot in params array) for that set 
        // if they don't find, value in params remains 0
        int pos = 0;
        while(pos < enronData.numWords){
                if(enronVocab[pos] == word){
                        params[3] = pos + 1;
                        break;
                }
                pos += 1;
        }
        pos = 0;
        while(pos < nipsData.numWords){
                if(nipsVocab[pos] == word){
                        params[4] = pos + 1;
                        break;
                }
                pos += 1;
        }
        pos = 0;
        while(pos < kosData.numWords){
                if(kosVocab[pos] == word){
                        params[5] = pos + 1;
                        break;
                }
                pos += 1;
        }
        pos = 0;
        while(pos < nytData.numWords){
                if(nytVocab[pos] == word){
                        params[6] = pos + 1;
                        break;
                }
                pos += 1;
        }
        // send params to each baby with Q3 tag
        int j;
        for(j=0; j<numProcessors-1; j++){
                MPI_Send(params, 7, MPI_INT, j+1, Q3, MPI_COMM_WORLD);
        }
        // initialize totals to 0
        int enronTotal = 0;
        int nipsTotal = 0;
        int kosTotal = 0;
        int nytTotal = 0;
        // recive array of 4 myTotals from babies, add their myTotal values to total 
        int k;
        for(k=0; k<numProcessors-1; k++){
                // myTotals = [enronTotal, nipsTotal, kosTotal, nytTotal]
                int myTotals[4];
                MPI_Recv(myTotals, 4, MPI_INT, MPI_ANY_SOURCE, Q3, MPI_COMM_WORLD, &stat);
                enronTotal += myTotals[0];
                nipsTotal += myTotals[1];
                kosTotal += myTotals[2];
                nytTotal += myTotals[3];
        }
        // put all totals in array, compare them and find the largest
        int totals[4];
        totals[0] = enronTotal;
        totals[1] = nipsTotal;
        totals[2] = kosTotal;
        totals[3] = nytTotal;
        int largest = 0;
        int max;
        int n;
        for(n=0; n<4; n++){
                if(totals[n] > largest){
                        max = n;
                        largest = totals[n];
                }
        }
        // based on which was the largest value, determine name to be printed
        string name;
        if(max == 0){
                name = "Enron Emails";
        } else if(max == 1){
                name = "NIPS Papers";
        } else if(max == 2){
                name = "Daily KOS";
        } else if(max == 3){
                name = "New York Times";
        }
        // print answer
        printf("The word %s appeared the most in %s with %d entries. \n", word.c_str(), name.c_str(), largest);
        printf("Enron Emails = %d entries \n", totals[0]);
        printf("NIPS Papers = %d entries \n", totals[1]);
        printf("Daily KOS = %d entries \n", totals[2]);
        printf("New York Times = %d entries \n", totals[3]);
        // return name to menu to know this finished
        return name;
}

string sendQ4(int x, int y, string word){
        // send function for query 4. Very similar to above function for query 3, but only finds values for x and y
        // if the set is not selected, that params[7] value is made -1
        MPI_Status stat;
        // params = [x, y, num, enronWord, nipsWord, kosWord, nytWord]
        int params[7];
        int i;
        // initialize params to 0
        for(i=0; i<7; i++){
                params[i] = 0;
        }
        // 4 if statements check to see if that set was selected, then finds word number same way as above.
        // if not selected, set value to -1 
        int pos;
        if(x == 1 || y == 1){
                pos = 0;
                while(pos < enronData.numWords){
                        if(enronVocab[pos] == word){
                                params[3] = pos + 1;
                                break;
                        }
                        pos += 1;
                }
        } else{
                params[3] = -1;
        }
        if(x == 2 || y == 2){
                pos = 0;
                while(pos < nipsData.numWords){
                        if(nipsVocab[pos] == word){
                                params[4] = pos + 1;
                                break;
                        }
                        pos += 1;
                }
        } else{
                params[4] = -1;
        }

        if(x == 3 || y == 3){
                pos = 0;
                while(pos < kosData.numWords){
                        if(kosVocab[pos] == word){
                                params[5] = pos + 1;
                                break;
                        }
                        pos += 1;
                }
        } else{
                params[5] = -1;
        }
        if(x == 4 || y == 4){
                pos = 0;
                while(pos < nytData.numWords){
                        if(nytVocab[pos] == word){
                                params[6] = pos + 1;
                                break;
                        }
                        pos += 1;
                }
        } else{
                params[6] = -1;
        }
        // send params to babies
        int j;
        for(j=0; j<numProcessors-1; j++){
                MPI_Send(params, 7, MPI_INT, j+1, Q4, MPI_COMM_WORLD);
        }

        // initialize totals to 0
        int enronTotal = 0;
        int nipsTotal = 0;
        int kosTotal = 0;
        int nytTotal = 0;
        // recieve the same way as sendQ3(). the values for those sets not selected will total to -16
        int k;
        for(k=0; k<numProcessors-1; k++){
                // myTotals = [enronTotal, nipsTotal, kosTotal, nytTotal]
                int myTotals[4];
                MPI_Recv(myTotals, 4, MPI_INT, MPI_ANY_SOURCE, Q4, MPI_COMM_WORLD, &stat);
                enronTotal += myTotals[0];
                nipsTotal += myTotals[1];
                kosTotal += myTotals[2];
                nytTotal += myTotals[3];
        }
        // variables for answer
        string xName;
        string yName;
        int xTotal;
        int yTotal;
        // two if else statements to determine x and y names and totals
        if(x == ENRON){
                xName = "Enron Emails";
                xTotal = enronTotal;
        } else if(x == NIPS){
                xName = "NIPS Paper";
                xTotal = nipsTotal;
        } else if(x == KOS){
                xName = "Daily KOS";
                xTotal = kosTotal;
        } else if(x == NYT){
                xName = "New York Times";
                xTotal = nytTotal;
        }

        if(y == ENRON){
                yName = "Enron Emails";
                yTotal = enronTotal;
        } else if(y == NIPS){
                yName = "NIPS Paper";
                yTotal = nipsTotal;
        } else if(y == KOS){
                yName = "Daily KOS";
                yTotal = kosTotal;
        } else if(y == NYT){
                yName = "New York Times";
                yTotal = nytTotal;
        }
        // print final answer
        if(xTotal > yTotal){
                printf("The word %s appeared more (%d times) in %s than it did in %s (%d times)", 
                        word.c_str(), xTotal, xName.c_str(), yName.c_str(), yTotal);
        } else if(yTotal > xTotal){
                printf("The word %s appeared more (%d times) in %s than it did in %s (%d times)", 
                        word.c_str(), yTotal, yName.c_str(), xName.c_str(), xTotal);
        }
        // return string to menu
        return xName;
}

void recieveAnyQ(){
        // this is called by all of the babies. While the head node is in the menu function, they need to 
        // be able to recieve any query. So they call the appropriate functions with different params[] values
        // based on the tag they got with the message
        MPI_Status stat;
        
        int good = 1;
        while(good != 0){
                // params = [x, y, num, enronWord, nipsWord, kosWord, nytWord]
                int params[7];
                MPI_Recv(params, 7, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
                int tag;
                tag = stat.MPI_TAG;

                if(tag == Q1){
                        doQ1(params[0], params[2]);
                } else if(tag == Q2){
                        doQ2(params[0], params[2]);
                } else if(tag == Q3){
                        doQ3(params[3], params[4], params[5], params[6]);
                } else if(tag == Q4){
                        doQ4(params[3], params[4], params[5], params[6]);
                } else if(tag == STOP){
                        good = 0;
                }
        }
}

void doQ1(int x, int num){
        // babies call this function for query 1. increment myTotal as they find words used more than the num passed in
        int i;
        int current = 1;
        int myTotal = 0;
        int thisDoc = 0;

        if(x == ENRON){
                for(i=0; i<enronSize; i++){
                        if(enronArr[i].docNum > current){
                                thisDoc = 0;
                                current = enronArr[i].docNum;
                        }
                        if(thisDoc == 0){
                                if(enronArr[i].count > num){
                                        myTotal += 1;
                                        thisDoc = 1;
                                }
                        }
                }
        } else if(x == NIPS){
                for(i=0; i<nipsSize; i++){
                        if(nipsArr[i].docNum > current){
                                thisDoc = 0;
                                current = nipsArr[i].docNum;
                        }
                        if(thisDoc == 0){
                                if(nipsArr[i].count > num){
                                        myTotal += 1;
                                        thisDoc = 1;
                                }
                        }
                }
        } else if(x == KOS){
                for(i=0; i<kosSize; i++){
                        if(kosArr[i].docNum > current){
                                thisDoc = 0;
                                current = kosArr[i].docNum;
                        }
                        if(thisDoc == 0){
                                if(kosArr[i].count > num){
                                        myTotal += 1;
                                        thisDoc = 1;
                                }
                        }
                }
        } else if(x == NYT){
                for(i=0; i<nytSize; i++){
                        if(nytArr[i].docNum > current){
                                thisDoc = 0;
                                current = nytArr[i].docNum;
                        }
                        if(thisDoc == 0){
                                if(nytArr[i].count > num){
                                        myTotal += 1;
                                        thisDoc = 1;
                                }
                        }
                }
        }
        // send myTotal to head node
        MPI_Send(&myTotal, 1, MPI_INT, 0, Q1, MPI_COMM_WORLD);
}

void doQ2(int x, int num){
        // babies construct their own arrays of words used more than num times to send to head.
        int myArray[400];
        int j;
        for(j=0; j<400; j++){
                myArray[j] = 0;
        }
        if(x == ENRON){
                int i;
                for(i=0; i<enronSize; i++){
                        if(enronArr[i].count > num){
                                int duplicate = 0;
                                int pos = 0;
                                while(myArray[pos] != 0){
                                        if(myArray[pos] == enronArr[i].wordNum){
                                                duplicate = 1;
                                        }
                                        pos += 1;
                                }
                                if(duplicate == 0){
                                        myArray[arrayEntries] = enronArr[i].wordNum;
                                        arrayEntries += 1;
                                }
                        }
                }
        } else if(x == NIPS){
                int i;
                for(i=0; i<nipsSize; i++){
                        if(nipsArr[i].count > num){
                                int duplicate = 0;
                                int pos = 0;
                                while(myArray[pos] != 0){
                                        if(myArray[pos] == nipsArr[i].wordNum){
                                                duplicate = 1;
                                        }
                                        pos += 1;
                                }
                                if(duplicate == 0){
                                        myArray[arrayEntries] = nipsArr[i].wordNum;
                                        arrayEntries += 1;
                                }
                        }
                }
        } else if(x == KOS){
                int i;
                for(i=0; i<kosSize; i++){
                        if(kosArr[i].count > num){
                                int duplicate = 0;
                                int pos = 0;
                                while(myArray[pos] != 0){
                                        if(myArray[pos] == kosArr[i].wordNum){
                                                duplicate = 1;
                                        }
                                        pos += 1;
                                }
                                if(duplicate == 0){
                                        myArray[arrayEntries] = kosArr[i].wordNum;
                                        arrayEntries += 1;
                                }
                        }
                }
        } else if(x == NYT){
                int i;
                for(i=0; i<nytSize; i++){
                        if(nytArr[i].count > num){
                                int duplicate = 0;
                                int pos = 0;
                                while(myArray[pos] != 0){
                                        if(myArray[pos] == nytArr[i].wordNum){
                                                duplicate = 1;
                                        }
                                        pos += 1;
                                }
                                if(duplicate == 0){
                                        myArray[arrayEntries] = nytArr[i].wordNum;
                                        arrayEntries += 1;
                                }
                        }
                }
        }

        MPI_Send(myArray, 400, MPI_INT, 0, Q2, MPI_COMM_WORLD);
}

void doQ3(int enronWord, int nipsWord, int kosWord, int nytWord){
        // babies search for word num in arrays if it is in that set, increment total by the number stored in count
        // myTotals = [enronTotal, nipsTotal, kosTotal, nytTotal]
        int myTotals[4];

        int i;
        int enronTotal = 0;
        if(enronWord != 0){
                for(i=0; i<enronSize; i++){
                        if(enronArr[i].wordNum == enronWord){
                                enronTotal += enronArr[i].count;
                        }
                }
        }
        int j;
        int nipsTotal = 0;
        if(nipsWord != 0){
                for(j=0; j<nipsSize; j++){
                        if(nipsArr[j].wordNum == nipsWord){
                                nipsTotal += nipsArr[j].count;
                        }
                }
        }
        int k;
        int kosTotal = 0;
        if(kosWord != 0){
                for(k=0; k<kosSize; k++){
                        if(kosArr[k].wordNum == kosWord){
                                kosTotal += kosArr[k].count;
                        }
                }
        }
        int l;
        int nytTotal = 0;
        if(nytWord != 0){
                for(l=0; l<nytSize; l++){
                        if(nytArr[l].wordNum == nytWord){
                                nytTotal += nytArr[l].count;
                        }
                }
        }
        myTotals[0] = enronTotal;
        myTotals[1] = nipsTotal;
        myTotals[2] = kosTotal;
        myTotals[3] = nytTotal;

        MPI_Send(myTotals, 4, MPI_INT, 0, Q3, MPI_COMM_WORLD);
}

void doQ4(int enronWord, int nipsWord, int kosWord, int nytWord){
        // babies search for word num in arrays if it is in that set, increment total by the number stored in count
        // also check to see if word is -1, if it is it is not selected and that is returned as its total
        // myTotals = [enronTotal, nipsTotal, kosTotal, nytTotal]
        int myTotals[4];

        int i;
        int enronTotal = 0;
        if(enronWord != 0 && enronWord != -1){
                for(i=0; i<enronSize; i++){
                        if(enronArr[i].wordNum == enronWord){
                                enronTotal += enronArr[i].count;
                        }
                }
        } else if(enronWord == -1){
                enronTotal = -1;
        }

        int j;
        int nipsTotal = 0;
        if(nipsWord != 0 && nipsWord != -1){
                for(j=0; j<nipsSize; j++){
                        if(nipsArr[j].wordNum == nipsWord){
                                nipsTotal += nipsArr[j].count;
                        }
                }
        } else if(nipsWord == -1){
                nipsTotal = -1;
        }
        int k;
        int kosTotal = 0;
        if(kosWord != 0 && kosWord != -1){
                for(k=0; k<kosSize; k++){
                        if(kosArr[k].wordNum == kosWord){
                                kosTotal += kosArr[k].count;
                        }
                }
        } else if(kosWord == -1){
                kosTotal = -1;
        }
        int l;
        int nytTotal = 0;
        if(nytWord != 0 && nytWord != -1){
                for(l=0; l<nytSize; l++){
                        if(nytArr[l].wordNum == nytWord){
                                nytTotal += nytArr[l].count;
                        }
                }
        } else if(nytWord == -1){
                nytTotal = -1;
        }
        myTotals[0] = enronTotal;
        myTotals[1] = nipsTotal;
        myTotals[2] = kosTotal;
        myTotals[3] = nytTotal;

        MPI_Send(myTotals, 4, MPI_INT, 0, Q4, MPI_COMM_WORLD);
}

void menu(){
        // main menu called by the head node. sends DONE tag to recieve any to tell babies to stop if answer == 5
        int answer1;
        answer1 = 0;
        while(answer1 != 5){
                printf("\nChoose one of the following queries:\n");
                printf("\t1- What percent of documents in X use any one word more than __ times?\n");
                printf("\t2- What words in X are used more than __ times in any document?\n");
                printf("\t3- In which data set does the word ____ appear most frequently?\n");
                printf("\t4- Does the word ____ appear more frequently in X or Y?\n");
                printf("\t5- Quit.\n");
                scanf("%d", &answer1);

                if(answer1 == 1){
                        int q1x;
                        int q1num;
                        printf("\nSelect one of the following for X:\n");
                        printf(" 1- Enron Emails\n 2- NIPS Papers\n 3- Daily KOS\n 4- New York Times\n");
                        scanf("%d", &q1x);
                        printf("\nEnter a value for __\n");
                        scanf("%d", &q1num);
                        printf("you entered %d and %d \n", q1x, q1num);
                        string finished;
                        finished = sendQ1(q1x, q1num);
                }
                else if(answer1 == 2){
                        int q2x;
                        int q2num;
                        printf("\nSelect one of the following for X:\n");
                        printf(" 1- Enron Emails\n 2- NIPS Papers\n 3- Daily KOS\n 4- New York Times\n");
                        scanf("%d", &q2x);
                        printf("\nEnter a value for __\n");
                        scanf("%d", &q2num);
                        printf("you entered %d and %d \n", q2x, q2num);
                        string answer;
                        answer = sendQ2(q2x, q2num);
                }
                else if(answer1 == 3){
                        string q3word;
                        printf("\nEnter a word for your search: \n");
                        char theWord[50];
                        scanf("%s", theWord);
                        q3word = theWord;
                        printf("You entered: %s \n", q3word.c_str());
                        string answer;
                        answer = sendQ3(q3word);
                }
                else if(answer1 == 4){
                        string q4word;
                        int q4x;
                        int q4y;
                        printf("\nEnter a word for your search: \n");
                        char theWord[50];
                        scanf("%s", theWord);
                        q4word = theWord;
                        printf("\nSelect one of the following for X:\n");
                        printf(" 1- Enron Emails\n 2- NIPS Papers\n 3- Daily KOS\n 4- New York Times\n");
                        scanf("%d", &q4x);
                        printf("\nSelect one of the following for Y:\n");
                        printf(" 1- Enron Emails\n 2- NIPS Papers\n 3- Daily KOS\n 4- New York Times\n");
                        scanf("%d", &q4y);
                        printf("You entered %s and selected %d and %d \n", q4word.c_str(), q4x, q4y);
                        string answer;
                        answer = sendQ4(q4x, q4y, q4word);
                }
                else if(answer1 == 5){
                        int params[7];
                        int i;
                        for(i=0; i<7; i++){
                                params[i] = 0;
                        }
                        int j;
                        for(j=0; j<numProcessors-1; j++){
                                MPI_Send(params, 7, MPI_INT, j+1, STOP, MPI_COMM_WORLD);
                        }
                        
                }
        }
}
