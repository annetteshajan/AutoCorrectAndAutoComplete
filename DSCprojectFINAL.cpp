#include <iostream>

#include<stdio.h>

#include <map>

#include <string>

#include<cstring>

#include <vector>

#include <algorithm>

#include <cctype>

#include <fstream>

#include <cstdlib>

using namespace std;

//Linked list for suggestions

struct suggestion

{

string word;

int count;

struct suggestion* next;

};

// Trie's node

struct trie

{ typedef map<char, trie*> point; //map for memory optimization

point next;

// If word is equal to "" is because there is no word in the

// dictionary which ends here.

string word;

int cnt; //priority count

bool endof; //marks end of word

trie() : next(map<char, trie*>()) //initialse

{

trie* n=this;

n->endof=false;

n->cnt=0;

}

void insert(string w,int choice)

{ //insert in trie

w = string("$") + w; //common root

int sz = w.size();

int i=0;

while (i<sz)

{ if (choice==1)

break;

else

{ w[i]=tolower(w[i]);

i++;

}

}

int p=word.length();

i=0;

//remove non characters

while(i < p){

if (!isalpha(word[i]) || word[i] == ' '){

word.erase(i,1);

p--;

}else

i++;

}

trie* n = this; //n refers to the structure

for (int i = 0; i < sz; ++i) {

if (n->next.find(w[i]) == n->next.end()) {

n->next[w[i]] = new trie();

}

n = n->next[w[i]];

}

n->word = w;

n->cnt=1;

if (choice==2) //user input

n->cnt+=2;

n->endof=true;

}

int search(string w)

{ //search in trie, returns count or 0

trie* n = this; //n refers to the structure

w=string("$")+w;

int sz=w.size();

int i;

for ( i = 0; i < sz; ++i)

{

if (n->next.find(w[i])!=n->next.end())

{

n = n->next[w[i]];

}

else

return 0;

}

if (n!=NULL && n->endof)

{ n->cnt++; //customisation

return n->cnt;

}

else

return 0;

}

};

struct suggestion* destroy(struct suggestion* start)

{ //deallocates the suggestions list

struct suggestion* cur=start;

struct suggestion* temp=start;

while (temp!=NULL)

{

cur=temp;

temp=temp->next;

free(cur);

}

return NULL;

};

struct suggestion* insend(struct suggestion* start,string word,int count)

{ //insert at end of suggestions list

if (start==NULL)

{

struct suggestion* temp= new struct suggestion;

temp->count=count;

temp->word=word;

temp->next=NULL;

return temp;

}

struct suggestion* temp=start;

while (temp!=NULL && temp->count>count)

{ temp=temp->next;

}

if (temp==NULL)

{

struct suggestion* newn= new struct suggestion;

newn->word=word;

newn->count=count;

newn->next=NULL;

temp->next=newn;

return start;

}

struct suggestion* newn=new struct suggestion;

newn->word=word;

newn->count=count;

newn->next=temp->next;

temp->next=newn;

return start;

};

void search_impl(struct suggestion** start,trie* tree, char ch, vector<int> last_row, const string& word,int min_cost)

{ //recursvie function to find minimum edit distance/cost

int sz = last_row.size();

vector<int> current_row(sz);

current_row[0] = last_row[0] + 1;

// Calculate the min cost of insertion, deletion, match or substution

int insert_or_del, replace;

for (int i = 1; i < sz; ++i) {

insert_or_del = min(current_row[i-1] + 1, last_row[i] + 1);

replace = (word[i-1] == ch) ? last_row[i-1] : (last_row[i-1] + 1);

current_row[i] = min(insert_or_del, replace);

}

// When we find a cost that is less than the min_cost, is because

// it is the minimum until the current row, so we update

if ((current_row[sz-1] <= min_cost) && (tree->word != "")) {

min_cost = current_row[sz-1];

*start=insend(*start,tree->word,tree->cnt);

}

// If there is an element wich is smaller than the current minimum cost,

// we can have another cost smaller than the current minimum cost

if (*min_element(current_row.begin(), current_row.end()) <= min_cost) {

for (trie::point::iterator it = tree->next.begin(); it != tree->next.end(); ++it) {

//cout<<"yay";

search_impl(start,it->second, it->first, current_row, word,min_cost);

}

}

}

struct suggestion* autocorrect(struct suggestion** start,trie tree,string word,int cost,int* flag)

{ //iterates through trie and calls search_impl for finding min cost

word = string("$") + word;

if (tree.search(word.substr(1,word.length())))

{ *flag=1;

return NULL;

}

int sz = word.size();

int min_cost = cost;

vector<int> current_row(sz + 1);

// Naive DP initialization

for (int i = 0; i < sz; ++i) current_row[i] = i;

current_row[sz] = sz;

// For each letter in the root map which matches with a

// letter in word, we must call the search

for (int i = 0 ; i < sz; ++i) {

if (tree.next.find(word[i]) != tree.next.end()) {

search_impl(start,tree.next[word[i]], word[i], current_row, word,min_cost);

}

}

return *start;

}

void suggestion(struct suggestion** start,string prefix,trie* n)

{ if (n->endof)

{ //end of word reached

n->cnt++;

*start=insend(*start,n->word,n->cnt);

return;

}

for (int i = 0; i < 26; i++)

{ char ch='a'+i;

if (n!=NULL && n->next[ch])

{ prefix.push_back(ch);

suggestion(start,prefix,n->next[ch]); //recursive call with prefix appended

}

}

}

struct suggestion* autocomplete(struct suggestion** start,string prefix,trie* n)

{ //autocomplete function with base cases

prefix="$"+prefix;

int sz=prefix.size();

int i;

for ( i = 0; i < sz; ++i)

{

if (n->next.find(prefix[i])!= n->next.end())

{

n = n->next[prefix[i]];

}

else

{

//Cannot autocomplete

return NULL;

}

}

if (n->word!="" && n->endof)

{ //given prefix is a wod in trie

n->cnt++;

char c=toupper(prefix[0]);

cout<<c;

cout<<prefix.substr(1,prefix.length());

return NULL;

}

if (!n->endof)

{

//given prefix can be autocompleted

suggestion(start,prefix,n);

}

return *start;

}

int fact(int n)

{ //utility function to find factorial

if (n==1)

return 1;

return (n*fact(n-1));

}

int checkifpresent(char a, string word)

{ //utility function to check if letter present in word

int n=sizeof(word);

int i;

for(i=0;i<n;i++)

{

if (word[i]==a)

return 1;

}

return 0;

}

string remove_redundancy(string word)

{ //autocorrect preprocessor

string newword;

int j,n;

n=word.length();

int i=0,duplicate_index;

n=word.length();

//remove non characters

while(i < n){

if (!isalpha(word[i]) || word[i] == ' '){

word.erase(i,1);

n--;

}else

i++;

}

i=0;

n=word.length();

//removing triple duplicates

while (i<(n))

{ newword.push_back(word[i]);

//cout<<newword;

if ((word[i]==word[i+1]) && (word[i+1]==word[i+2]))

{ i+=2;

if (i==n)

return newword;

}

else

i++;

}

int m=newword.length();

i=0;

while (i<m)

{

newword[i]=tolower(newword[i]);

i++;

}

return newword;

}

void swap(char& a, char& b)

{ //utility function to swap characters

char temp;

temp = a;

a = b;

b = temp;

}

void permutation(vector<string> *permute,string s,int i,int n,trie* root,int* flag)

{ //function to generate all permutations and check if present in trie

int j;

if (i == n)

{ if (root->search(s))

{ if (*flag==0)

*flag=1;

if (*flag==1)

printf("Did you mean:\n");

*flag=2;

if (!(std::find((*permute).begin(), (*permute).end(), s)!=(*permute).end()))

{

(*permute).push_back(s);

char c=toupper(s[0]);

cout<<c;

cout<<s.substr(1,s.length())+" ";

}

}

}

else

{for (j = i; j < s.length(); j++)

{

swap(s[i],s[j]);

permutation(permute,s, i + 1, n,root,flag);

swap(s[i],s[j]);

}

}

}

void display(struct suggestion *start)

{ //dislays the suggestions list

if (start==NULL)

printf("No words here \n");

else

{ int i=0;

struct suggestion *temp=start;

while (temp!=NULL && i<5)

{

string word=temp->word.substr(1,temp->word.length());

char c=toupper(word[0]);

cout<<c;

cout<<word.substr(1,word.length())+" ";

temp=temp->next;

i++;

}

}

}

int main()

{

trie tree;

//reading file to create dictionary trie

std::ifstream file("Dictionary.txt");

struct suggestion* start1=NULL;

int i=0;

if (file.is_open()) {

std::string line;

while (getline(file, line)) {

tree.insert(line.c_str(),1);

}

file.close();

}

//utility variables

int permutationflag=0;

int alreadyexistsflag=0;

int ch;

int min_cost;

//user interface loop

while(1)

{ printf("\n1. Autocorrect 2.Autocomplete 3.Insert a new word 4.Exit \n");

cin>>ch;

if (ch==1)

{ printf("Enter word- \n");

struct suggestion* start1=NULL;

string str; //represents user input

cin.ignore();

getline(cin,str);

string newstr; //represents preprocessed string

newstr=remove_redundancy(str);

if (newstr.length()==0) //str contains no alphabets

{ cout<<"Invalid";

}

else{

if (tree.search(newstr)) //if preprocessed string exists in trie

{

if ((str.compare(newstr)!=0) && (str[str.length()-1]!='!' || str[str.length()-1]!='.')) //if str does not end in ! or .

{ str[0]=tolower(str[0]);

if (str.compare(newstr)==0) //if str is the same as newstr but first letter is capital

cout<<"That's right";

else{

printf("Did you mean:\n");

char cr=toupper(newstr[0]);

cout<<cr;

cout<<newstr.substr(1,newstr.length());

//break;

char c;

printf("\nWould you like your word to be part of the recommendations? (y/n) ");

cin>>c;

if (c=='y')

{ tree.insert(str,2); //2 is for priority for user input

printf("\nNew word inserted\n");

}

}

}

else

{

string special="!*()&^%$#@<>?;=+{}[]~123457890:\\/";

//special characters

int i=0,flag=0;

int n=special.length()-1;

while (i<n)

{

if (checkifpresent(special[i],str))

{flag=1;

cout<<"this1";

cout<<"Did you mean:\n";

char chr=toupper(newstr[0]);

cout<<chr;

cout<<newstr.substr(1,newstr.length());

break;

}

i++;

}

if (flag==0)

cout<<"That's right!";

}

}

else

{

if (newstr.length()<=5) //check permutations

{ vector<string> permute;

permutation(&permute,newstr,1,newstr.length(),&tree,&permutationflag);

}

if (permutationflag>=1)

{

permutationflag=0;

}

else

{

struct suggestion* start1=NULL;

start1=autocorrect(&start1,tree,newstr,1,&alreadyexistsflag); //autocorrect for edit distance 1

if (start1==NULL) //if no word within edit distance 1

start1=autocorrect(&start1,tree,newstr,3,&alreadyexistsflag); //words with edit distance 2 and 3

//start1=removeextra(start1);

if (start1!=NULL)

{ printf("Did you mean: \n");

display(start1);

}

else

cout<<"No reasonable match\n"; //word too far

start1=destroy(start1);

start1=NULL;

char c;

printf("\nWould you like your word to be part of the recommendations? (y/n) ");

cin>>c;

if (c=='y')

{ tree.insert(str,2); //2 is for priority for user input

printf("\nNew word inserted\n");

}

}

else

{ printf("Did you mean: \n");

display(start1);

start1=destroy(start1);

char c;

printf("\nWould you like your word to be part of the recommendations? (y/n) ");

cin>>c;

if (c=='y')

{

tree.insert(str,2); //2 is for priority for user input

printf("\nNew word inserted\n");

}

}

}

}

}

}

if (ch==2)

{ struct suggestion* start2=NULL;

string prefix;

printf("Enter prefix-\n");

cin>>prefix;

prefix=remove_redundancy(prefix); //preprocess prefix

start2=autocomplete(&start2,prefix,&tree);

display(start2);

start2=destroy(start2);

}

if (ch==3)

{

printf("\nEnter new word to insert-\n");

string newword;

cin>>newword;

if (tree.search(newword)==0)

{ tree.insert(newword,2); //2 for priority for user input

printf("New word inserted\n");

}

else

printf("Word already exists\n");

}

if (ch==4)

break;

}

return 0;

}
