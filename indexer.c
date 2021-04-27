/*
    COSC431 Assignment 1: Indexer
    Author: Max Edwards
    Date Created: 19/4/2020
    Last Date Modified: 21/4/2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
void insert(char** dictionary, char* word, int index, int size) {
    //updating dictionary
    int j;
    //moving elements over one to make room
    for(j=size;j>index;j--) {
        dictionary[j] = dictionary[j-1];
    }
    strcpy(dictionary[index], word);
}

int searchDictionary(char** dictionary, char* word, int size) {
    int i, result, min, max;
    if(size == 0) {
        return -1;
    }
    i = size/2;
    min = 0;
    max = size-1;
    while((result = strcmp(dictionary[i], word)) != 0) {
        if(result<0) {
            min = i+1;
        } else {
            max = i-1;
        }
        if(max<min) {
            //changes i to the value that represents where we insert new word
            if(min > i) {
                i++;
            }
            break;
        }
        i = (max+min)/2;
    }
    if(max<min) {
        return (i * -1) - 1;
    }
    return i;
}

int main() {
    FILE *fp;
    char currentWord[30];
    int c, i, j, k, index, size;
    int letterIndex = 0;
    int length;
    int docID=0;
    int initialSize = 100;
    int dictionarySize = 0;
    char** dictionary = (char**) malloc(sizeof(char **) * initialSize);
    float** frequencies = (float**) malloc(sizeof(float *) * initialSize);
    int** postings = (int**) malloc(sizeof(int *) * initialSize);
    char** docDictionary = (char**) malloc(sizeof(char **) * initialSize);
    int docDictionarySize = 0;
    int* indexes = (int*) malloc(sizeof(int) * initialSize);
    int* counts = (int*) malloc(sizeof(int) * initialSize);

    //going through the output of the parser
    for(;(c=getchar()) != EOF; letterIndex++) {
        //printf("%d", letterIndex);
        //signals end of a word
        if(c == ' ') {
            currentWord[letterIndex] = '\0';
            //binary search to find word in docDictionary
            i = searchDictionary(docDictionary, currentWord, docDictionarySize);
            letterIndex = -1;
            //i is negative means that it is not in the docDictionary
            if(i < 0) {
                i = (i+1) * -1;
                j = searchDictionary(dictionary, currentWord, dictionarySize);
                //if j is negative the word is not in the main dictionary either
                if(j<0) {
                    
                    j = (j+1) * -1;
                    //reallocating size if dictionary is too small
                    if(dictionarySize == (sizeof(dictionary)/sizeof(dictionary[0]))) {
                        dictionary = (char **) realloc(dictionary, sizeof(char *) * dictionarySize*2);
                        frequencies = (float **) realloc(frequencies, sizeof(float *) * dictionarySize*2);
                        postings = (int **) realloc(postings, sizeof(int *) * dictionarySize*2);
                    }
                    //adding to dictionary
                    insert(dictionary, currentWord, j, dictionarySize);
                    //adjust frequencies and postings
                    for(k=dictionarySize;k>j;k--) {
                        frequencies[k] = frequencies[k-1];
                        postings[k] = postings[k-1];
                    }
                    //allocating size for positngs and frequencies of new word
                    frequencies[j] = (float*) malloc(sizeof(float));
                    postings[j] = (int*) malloc(sizeof(int));
                    postings[j][0] = -1;
                    dictionarySize++;
                }
                //adding to the docDictionary
                //reallocating size if the docDictionary is too small
                if(docDictionarySize == (sizeof(docDictionary)/sizeof(docDictionary[0]))) {
                    docDictionary = (char **) realloc(docDictionary, sizeof(char *) * docDictionarySize*2);
                    counts = (int *) realloc(counts, sizeof(int) * docDictionarySize*2);
                    indexes = (int *) realloc(indexes, sizeof(int) * docDictionarySize*2);
                }
                insert(docDictionary, currentWord, i, docDictionarySize);
                //adjust counts and indexes
                for(k=docDictionarySize;k>i;k--) {
                    counts[k] = counts[k-1];
                    indexes[k] = indexes[k-1];
                }
                docDictionarySize++;
                //adding to counts and indexes
                counts[i] = 1;
                indexes[i] = j;
            //in the case the word is already in docDictionary
            } else {
                counts[i]++;
            }
            letterIndex = -1;
        //new line means new document
        } else if(c == '\n') {
            //updating frequencies and postings
            for(i=0;i<docDictionarySize;i++) {
                //index stores the value of the index in the dictionary
                index = indexes[i];
                size = (sizeof(postings[index])/sizeof(int));
                for(j=0;j<size;j++) {
                    if(postings[index][j]==-1) {
                        break;
                    }
                }
                //reallocate size
                if(j==size) {
                    postings[index] = (int *) realloc(postings[index], sizeof(int) * size*2);
                    frequencies[index] = (float *) realloc(frequencies[index], sizeof(float) * size*2);
                    size = 2*size;
                }
                //append to frequencies and postings
                postings[index][j] = docID;
                if(j!=size-1) {
                    postings[index][j+1] = -1;
                }
                frequencies[index][j] = ((float) counts[i]) / ((float) length);
            }
            docDictionarySize = 0;
            printf("%d\n", docID);
            docID++;
            letterIndex = -1;
        } else {
            //append letter to currentWord
            currentWord[letterIndex] = (char) c;
        }
    }
    /*
    free(dictionary);
    free(frequencies);
    free(postings);
    free(docDictionary);
    free(indexes);
    free(counts);
    */
    printf("Hello");
    return 0;
}