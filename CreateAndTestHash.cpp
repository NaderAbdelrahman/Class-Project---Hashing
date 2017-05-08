#include "QuadraticProbing.h"
#include "LinearProbing.h"
#include "DoubleHash.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

template <typename HashTableType>
void
TestFunctionForHashTable(HashTableType &hash_table, const string &words_filename, const string &query_filename) {
  cout << "TestFunctionForHashTables..." << endl;
  cout << "Words filename: " << words_filename << endl;
  cout << "Query filename: " << query_filename << endl;
  hash_table.MakeEmpty();
  ifstream file(words_filename);
  string file_input;
  if(file.is_open()){
    while(getline(file, file_input)){
      hash_table.Insert(file_input);
    }
  }
  else{
    cout << "Unable to open " << words_filename <<endl;
  }
  cout << "Collisions: " << hash_table.NumberOfCollisions() << endl;
  cout << "Number of items: " << hash_table.NumberOfItems() << endl;
  cout << "Size of hash table: " << hash_table.TableSize() << endl;
  cout << "Load factor: " << hash_table.LoadFactor() << endl;
  cout << "Avg. number of collisions: " << hash_table.AverageNumberOfCollisions() << endl;
  ifstream file2(query_filename);
  string file2_input;
  if(!file2.is_open()){
    cout << "Unable to open " << query_filename << endl;
    return;
  }
  while(getline(file2, file2_input)){
    if(hash_table.Contains(file2_input)){
      cout << "Found, " << hash_table.NumberOfProbes() << endl;
    }else{
      cout << "Not Found, " << hash_table.NumberOfProbes() << endl;
    }
  }
}
// Sample main for program CreateAndTestHash
int
main(int argc, char **argv) {
  if (argc != 4) {
    cout << "Usage: " << argv[0] << " <wordsfilename> <queryfilename> <flag>" << endl;
    return 0;
  }
  
  const string words_filename(argv[1]);
  const string query_filename(argv[2]);
  const string param_flag(argv[3]);

  if (param_flag == "linear") {
    LinearTable<string> linear_probing_table;
    TestFunctionForHashTable(linear_probing_table, words_filename, query_filename);    
  } else if (param_flag == "quadratic") {
    HashTable<string> quadratic_probing_table;
    TestFunctionForHashTable(quadratic_probing_table, words_filename, query_filename);    
  } else if (param_flag == "double") {
    HashTableDouble<string> double_probing_table;
    TestFunctionForHashTable(double_probing_table, words_filename, query_filename);    
  } else {
    cout << "Uknown tree type " << param_flag << " (User should provide linear, quadratic, or double)" << endl;
  }
  return 0;
}
