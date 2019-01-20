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

int main(int argc, char *argv[])
{
  // Read 25 training csv files and collect tool usage frame numbers for each file
  // Only frame numbers where Tool is present by itself

  typedef tokenizer<escaped_list_separator<char>> Tokenizer;
  ofstream file[22]; // One text file for each tool, 25 lines per file, each line is csv frame numbers
  // corresponding to each training data file; One file for no tool present

  for (int i=0; i<22; i++) {
      string filename = "tool_" + to_string(i+1) + ".txt";
      file[i].open(filename.c_str());
      if (!file[i].is_open()) {
          cout << "Could not open file for tool number " << i << endl;
          return 0;
        }
    }

  for (int i=1; i<26; i++) {

      string readfilename; ifstream data;
      if (!readTrainingCSVFile(i,readfilename,data))
        continue;

      string line; getline(data,line); // Skip first line

      vector<string> vec;
      vector<string> frame_num_for_tools[22];

      while (getline(data,line)) {

          Tokenizer tok(line);
          vec.assign(tok.begin(),tok.end());
          string frame_num = vec.at(0);

          // Store frame number for tool if it is present by itself
          int num_tools_present=0;
          int which_tool=0;

          for (int j=1; j<vec.size(); j++) { // 1st token is frame number
              string toolpresent = vec.at(j);
              double isToolPresent = strtof(toolpresent.c_str(),NULL);
              if (isToolPresent==1) {
                  num_tools_present++;
                  which_tool=j-1;
                }
            }
          if (num_tools_present==1)
            frame_num_for_tools[which_tool].push_back(frame_num); // Particular tool by itself
          else if (num_tools_present==0)
            frame_num_for_tools[21].push_back(frame_num);         // No tool
        }
      data.close();
      // We have frame number for each tool in current file now. Write them; // Extra processing to avoid last comma by itself
      for (int k=0; k<22; k++) {
          file[k] << to_string(i);
          if (!frame_num_for_tools[k].empty()) {
              file[k] << ",";
              copy(frame_num_for_tools[k].begin(),frame_num_for_tools[k].end()-1,ostream_iterator<string>(file[k],","));
              file[k] << frame_num_for_tools[k].back();
            }
          file[k] << endl;
        }
    }
  // Close all files
  for (int l=0; l<22; l++)
    file[l].close();
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































