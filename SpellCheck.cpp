#include "QuadraticProbing.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

int
main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document file> <dictionary file> " << endl;
    return 0;
  }
  const string document_filename(argv[1]);
  const string dictionary_filename(argv[2]);

  ifstream dictionary(dictionary_filename);
  string dictionary_input_;
  HashTable<string> table;
  if(!dictionary.is_open()){
    cout << "Unable to open " << dictionary_filename << endl;
    return 0;
  }
  // TREE GENERATION
  while(getline(dictionary, dictionary_input_)){
    table.Insert(dictionary_input_);
  }

  ifstream document(document_filename);
  string document_input_, tmp;
  if(!document.is_open()){
    cout << "Unable to open " << document_filename << endl;
    return 0;
  }
  string alphabet = "abcdefghijklmnopqrstuvwxyz";
  // CASE A

  while(getline(document, document_input_, ' ')){
    if(document_input_[document_input_.size() - 1] == ','  ||
       document_input_[document_input_.size() - 1] == '.'  ||
       document_input_[document_input_.size() - 1] == ';'  ||
       document_input_[document_input_.size() - 1] == '\'' ||
       document_input_[document_input_.size() - 1] == ':')
    {
      document_input_.pop_back();
    }
    if(document_input_.front() == '"'){
      document_input_.erase(0, 1);
    }
    tmp = document_input_;
    if(!table.Contains(document_input_)){
      for(unsigned int j = 0; j < document_input_.size(); ++j){
        for(int i = 0; i < 26; ++i){
          tmp.insert(j, 1, alphabet[i]);
          if(table.Contains(tmp)){
            cout << document_input_ << " -> " << tmp << " (case a)" << endl;
          }
          tmp = document_input_;
        }
      }
    }
  }
  document.clear();
  document.seekg(0, ios::beg);
  // CASE B
  while(getline(document, document_input_, ' ')){
    if(document_input_[document_input_.size() - 1] == ','  ||
       document_input_[document_input_.size() - 1] == '.'  ||
       document_input_[document_input_.size() - 1] == ';'  ||
       document_input_[document_input_.size() - 1] == '\'' ||
       document_input_[document_input_.size() - 1] == ':')
    {
      document_input_.pop_back();
    }
    if(document_input_.front() == '"'){
      document_input_.erase(0, 1);
    }
    // Make Lowercase
    for(unsigned int i = 0; i < document_input_.size(); ++i){
      document_input_[i] = tolower(document_input_[i]);
    }
    tmp = document_input_;
    if(!table.Contains(document_input_)){
      for(unsigned int i = 0; i < document_input_.size(); ++i){
        tmp.erase(i, 1);
        if(table.Contains(tmp)){
          cout << document_input_ << "-> " << tmp << " (case b)" << endl;
        }
        tmp = document_input_;
      }
    }
  }

  // CASE C
  document.clear();
  document.seekg(0, ios::beg);
  while(getline(document, document_input_, ' ')){
    if(document_input_.size() == 1 || document_input_.size() == 0){continue;}
    if(document_input_[document_input_.size() - 1] == ','  ||
       document_input_[document_input_.size() - 1] == '.'  ||
       document_input_[document_input_.size() - 1] == ';'  ||
       document_input_[document_input_.size() - 1] == '\'' ||
       document_input_[document_input_.size() - 1] == ':')
    {
      document_input_.pop_back();
    }
    if(document_input_.size() == 1 || document_input_.size() == 0){continue;}
    if(document_input_.front() == '"'){
      document_input_.erase(0, 1);
    }
    // Make Lowercase
    for(unsigned int i = 0; i < document_input_.size(); ++i){
      document_input_[i] = tolower(document_input_[i]);
    }
    tmp = document_input_;
    char ctmp;
    if(!table.Contains(tmp)){
      for(unsigned int i = 0; i < tmp.size() - 1; ++i){
        ctmp = tmp[i];
        tmp.at(i) = tmp.at(i + 1);
        tmp.at(i + 1) = ctmp;
        if(table.Contains(tmp)){
          cout << document_input_ << "-> " << tmp << " (case c)" << endl;
        }
        tmp = document_input_;
      }
    }
  }

  return 0;
}