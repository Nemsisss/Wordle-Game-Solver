// For debugging
#include <iostream>
// For std::remove

#include <map>
#include <set>
#include "wordle.h"
#include "dict-eng.h"
#include <vector>
#include <string>
using namespace std;


// Add prototypes of helper functions here
void wordle_helper(
     std::string& in,
    const std::set<std::string>& dict, 
    std::set<std::string>& combos, string word, int index);
bool isValid (std::string text,const std::set<std::string>& dict );
void generatePermutations(std::set<std::string>& combos, string floatings, string word);
// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> words;
    std::string temp=in;
    if(temp.length()==0) 
    {
        return words;
    }
    else if(temp.find('-')==string::npos )
    {
        words.insert(temp);
        return words;
    }
    std::map<int,char> fixedChars; //to store the index of the fixed letters as keys and the letters themselves as the values
    string tempFloating=floating;
    for(long unsigned int i=0; i<in.size();i++)
    {
        if(in[i]!='-')
        {
            fixedChars.insert(pair<int,char>(i,in[i])); //to store the information of fixed letters, i.e index and the letter itself
        }
    }
    int numOfDashes=in.length()-fixedChars.size()-floating.size(); // # of dashes after subtracting the # of floating and fixed letters
    tempFloating+= string(numOfDashes,'-'); //add the number of dashes needed to the floatings string
    generatePermutations(words, tempFloating,"");// recursively generate permutations of the floating string (using the floats and dashes)
    std::set<string>::iterator it;
    string newWord;
    set<string> newWordsSet;

    //add the fixed letters in their correct indices to the valid floating permutations returned from generatePermutations()
    for(it=words.begin();it!=words.end();it++)
    {
        newWord= *it;
        for(long unsigned int j=0;j<in.size();j++)
        {
            if(fixedChars.find(j)!= fixedChars.end())
            {
                newWord= newWord.substr(0,j)+ fixedChars.find(j)->second+ newWord.substr(j);
            }
        }
        wordle_helper(newWord,dict, newWordsSet,"",0); // recursively fill in the blanks left in each valid permutation
    }
     return newWordsSet;
}

// Define any helper functions here

/* recursively fills in the remaning blanks to generate valid words using the permutations which already meet the constraints */
void wordle_helper(
     std::string& in,
    const std::set<std::string>& dict, 
    std::set<std::string>& combos, string word, int index)
{
    if(word.size()==in.size() )
    {
        if (isValid(word, dict) )
        {
             combos.insert(word);
        }
        return;
    }
    if(in[index]=='-')
    {
        for(int j=0; j<26; j++) // ascii a = 97
        {
            wordle_helper(in, dict, combos, word +(char)('a'+j), index+1);
        }
    }
    else
    {
        wordle_helper(in,  dict, combos, word+in[index] , index+1);
    }
}

/* checks if text is a valid english word */
bool isValid (std::string text,const std::set<std::string>& dict )
{
    if(dict.find(text)!= dict.end()) //if the text exists in the dictionary
    {
        return true;
    }
    return false;
}

/* generates permutations with dashes and floatings */
void generatePermutations(std::set<std::string>& combos, string tempFloating, string word)
{
    if(combos.find(word)!=combos.end())
    {
        return;
    }
    if(tempFloating.length() == 0)
    {
        combos.insert(word);
        return;
    }
    for( long unsigned int i=0 ; i<tempFloating.length() ; i++)
    {
       generatePermutations(combos,tempFloating.substr(0,i)+ tempFloating.substr(i+1), word+ tempFloating[i]);
    }
}