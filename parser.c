/*
    COSC431 Assignment 1: Parser
    Author: Max Edwards
    Date Created: 18/4/2020
    Last Date Modified: 18/4/2020
*/
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
int main() {
    long docID = 0;
    char currentWord[20];
    int c, i, j;
    int letterIndex = 0;
    int isTag = 0;
    char possibleTags[16][10] = {
        "doc",
        "docno",
        "hl",
        "dd",
        "so",
        "in",
        "dateline",
        "text",
        "/doc",
        "/docno",
        "/hl",
        "/dd",
        "/so",
        "/in",
        "/dateline",
        "/text"
    };
    for(;(c=getchar()) != EOF; letterIndex++) {
        c = tolower(c);
        //Checks if we are in the middle of a tag
        if(isTag) {
            //spaces will not occur in a tag in wsj.xml
            if(c == ' ') {
                isTag = 0;
            } else {
                currentWord[letterIndex] = (char) c;
                //Checking that it currently matches a tag
                for(i=0;i<(sizeof(possibleTags)/sizeof(possibleTags[0]));i++) {
                    for(j=0;j<letterIndex;j++) {
                        if(possibleTags[i][j] != currentWord[j]) {
                            break;
                        }
                    }
                    if(j == letterIndex) {
                        break;
                    }
                }
                //This is only true if the current 'tag' doesnt match a known tag, hence is not a tag
                //and will be treated as a normal word instead
                if(j!= letterIndex) {
                    isTag = 0;
                }
                else {
                    //Dealing with the end of the tag
                    if(c == '>') {
                        currentWord[letterIndex] = '\0';
                        if(strcmp("/doc", currentWord) == 0) {
                            printf("\n");
                        }
                        isTag = 0;
                        letterIndex = -1;
                    }
                    continue;
                }
            }
        }
        //Checks if we start looking at a tag
        if(c == '<') {
            currentWord[letterIndex] = '\0';
            //checks current word isnt empty
            if(letterIndex != 0) {
                if(currentWord[letterIndex-1] == '.') {
                    currentWord[letterIndex-1] = '\0';
                    if(letterIndex != 1) {
                        printf("%s ", currentWord);
                    }
                } else {
                    printf("%s ", currentWord);
                }
            }
            isTag=1;
            letterIndex=-1;
        //Checks word has ended
        } else if(c == ' ' || c== '\n' || c == '"' || c == ',' || c == '?' || c == '/' ||
                  c == '[' || c == ']' || c == '{' || c == '}' || c == ':' || c == ';' ||
                  c == '?' || c == '>' || c == '(' || c == ')' || c == '&' || c == '!') {
            currentWord[letterIndex] = '\0';
            //checks word isnt empty
            if(letterIndex != 0) {
                if(currentWord[letterIndex-1] == '.') {
                    currentWord[letterIndex-1] = '\0';
                    if(letterIndex != 1) {
                        printf("%s ", currentWord);
                    }
                } else {
                    printf("%s ", currentWord);
                }
            }
            letterIndex = -1;
        } else {
            currentWord[letterIndex] = (char) c;
        }
    }
    printf("\n");
}