#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
bool readTrainingCSVFile(int i, string& readfilename, ifstream& file);
void writeToolUsageStatisticsFile(unsigned int toolcount[21],unsigned int toolstogether[21][21]) ;

int main(int argc, char *argv[])
{
  // Read 25 training csv files and collect tool usage statistics

  unsigned int toolcount[21] = {0};
  unsigned int toolstogether[21][21] = {{0,0}};

  typedef tokenizer<escaped_list_separator<char>> Tokenizer;

  for (int i=1; i<26; i++) {

      string readfilename; ifstream data;
      if (!readTrainingCSVFile(i,readfilename,data))
        continue;

      string line; getline(data,line); // Skip first line

      int linenum=1;
      vector<string> vec;

      while (getline(data,line)) {

          Tokenizer tok(line);
          vec.assign(tok.begin(),tok.end());

          unsigned int toolpresence[21] = {0};
          // Count occurance of each individual tool
          for (int j=1; j<vec.size(); j++) { // 1st token is frame number
              string toolpresent = vec.at(j);
              double isToolPresent = strtof(toolpresent.c_str(),NULL);
              if (isToolPresent==1) {
                  toolcount[j-1]++;
                  toolpresence[j-1] = 1;
                }
            }
          // Count simultaneous occurance of each possible tool pair
          for (int k=0; k<21; k++) {
              for (int l=0; l<21; l++) {
                  if (toolpresence[k] && toolpresence[l])
                    toolstogether[k][l] += 1;
                }
            }
          linenum++;
        }
      data.close();
    }
  writeToolUsageStatisticsFile(toolcount,toolstogether);
  return 0;
}

bool readTrainingCSVFile(int i, string& readfilename, ifstream& file) {
  // Read training data csv file indexed by i
  // Filenames are ./train-labels/train[01--25].csv

  string basefilename = "./train-labels/train";
  if (i<10)
    basefilename = "./train-labels/train0";

  readfilename = basefilename + to_string(i) + ".csv";
  file.open(readfilename.c_str());

  if (!file.is_open()) {
      cout << "Could not open " << readfilename << endl;
      return false;
    } else {
      return true;
    }
}

void writeToolUsageStatisticsFile(unsigned int toolcount[21], unsigned int toolstogether[21][21]) {
  // Write tool usage statistics to text files
  string toolpresencestatsfile = "toolstats.txt";
  ofstream file;
  file.open(toolpresencestatsfile.c_str());

  if (!file.is_open()) {
      cout << "Could not open tool usage count file for writing.\n";
      return;
    }

  for (int i=0; i<21; i++) {
      file << toolcount[i] << endl;
    }
  file.close();

  string toolstogetherfile = "toolspresenttogether.txt";
  file.open(toolstogetherfile.c_str());

  if (!file.is_open()) {
      cout << "Could not open tools together file\n";
      return;
    }
  for (int i=0; i<21; i++) {
    for (int k=0; k<21; k++) {
        file << toolstogether[i][k] << ",";
      }
    file << endl;
    }
  file.close();
}

































