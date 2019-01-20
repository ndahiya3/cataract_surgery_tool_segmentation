#include "opencv2/opencv.hpp"
#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator

#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;
using namespace cv;

/* Each tools presence in every video is saved in text files
 * as a sequence of frame numbers.
 * Open a particular video and extract and save the frames numbers
 * written in the corresponding file. Tool 1-21 and Background i.e.
 * no tool present is 22, for e.g.
 * tool_22.txt has 25 lines, one line for each video file. First number is
 * video file number (1-25) followed by csv frames numbers. Video files
 * may have 0 or more occurances of a particular tool.
 */
void extractFrames(string line,string videofilename,string saveframeprefix,float scale=1.0f);
int main(int, char **)
{
  // Process each tool in every video, optionally rescale and save
  vector<string> videoFilenames;
  vector<string> toolNumberFilenames;

  int numberOfVideos = 25;
  int numberOfTools = 22;
  float rescale = 0.25;

  for (int i=1; i<=numberOfVideos; i++) {
      string videofilename;
      if (i < 10)
        videofilename = string("./train-videos/train0") + to_string(i) + ".mp4";
      else
        videofilename =  string("./train-videos/train") + to_string(i) + ".mp4";
      videoFilenames.push_back(videofilename);
    }
  for (int i=1; i<=numberOfTools; i++) {
      string toolFilename = "./tool-files/tool_" + to_string(i) + ".txt";
      toolNumberFilenames.push_back(toolFilename);
    }

  for (int i=0; i<numberOfTools; i++) {
      // Open text file for current tool
      string toolNumberfilename = toolNumberFilenames.at(i);
      ifstream file;
      file.open(toolNumberfilename);
      if(!file.is_open()) {
          cout << "Error opening text file.\n";
          return 0;
        }

      for (int j=0; j<numberOfVideos; j++) {
          string videofile = videoFilenames.at(j);
          string line;
          getline(file,line);
          string savefileprefix = "./training-frames/tool_" + to_string(i+1) + "/tool_" + to_string(i+1) + "_video" + to_string(j+1) + "_";
          extractFrames(line,videofile,savefileprefix,rescale);
        }
     file.close();// Tool frame numbers text file
    }

  return 0;
}

void extractFrames(string line, string videofilename, string saveframeprefix, float scale) {
  VideoCapture cap;              // Open video file and read specific frames
  cap.open(videofilename);

  if (!cap.isOpened()) {
      cout << "Error opening video." << endl;
      return;
    }
  typedef tokenizer<escaped_list_separator<char>> Tokenizer;
  vector<string> vec;
  Tokenizer tok(line);
  vec.assign(tok.begin(),tok.end());

  Mat frame;
  int frame_num_2_capture;
  string frame_num;
  //string save_frame_name = "./tool_13/tool_13_" + basefilename + "_";

  for (int i=1; i<vec.size(); i++) { // First num is video file number
      frame_num = vec.at(i);
      frame_num_2_capture = stoi(frame_num);
      cap.set(CV_CAP_PROP_POS_FRAMES,frame_num_2_capture);
      cap >> frame;
      // resize
      Mat resizedframe;
      resize(frame,resizedframe,Size(),scale,scale);
      //resize(frame,resizedframe,Size(round(scale*frame.cols),round(scale*frame.rows)));
      imwrite(saveframeprefix + frame_num + ".jpeg",resizedframe);
    }

  cap.release();
}

//int main(int, char**)
//{
//  // For now extracting background frames from train01.mp4

//  string basefilename = "train01";
//  string videofilename = basefilename + ".mp4";

//  // Open and read frame numbers to extract
//  string framenumfilename = "tool_13.txt";
//  ifstream file;
//  file.open(framenumfilename);
//  if(!file.is_open()) {
//      cout << "Error opening text file.\n";
//      return 0;
//    }

//  typedef tokenizer<escaped_list_separator<char>> Tokenizer;
//  vector<string> vec;
//  string line;
//  getline(file,line);             // Train01 corresponds to 1st line of file
//  Tokenizer tok(line);
//  vec.assign(tok.begin(),tok.end());
//  file.close();

//  VideoCapture cap;              // Open video file and read specific frames
//  cap.open(videofilename);

//  if (!cap.isOpened()) {
//      cout << "Error opening video." << endl;
//      return 0;
//    }
//  Mat frame;
//  int frame_num_2_capture;
//  string frame_num;
//  string save_frame_name = "./tool_13/tool_13_" + basefilename + "_";

//  for (int i=1; i<vec.size(); i++) { // First num is video file number
//      frame_num = vec.at(i);
//      frame_num_2_capture = stoi(frame_num);
//      cap.set(CV_CAP_PROP_POS_FRAMES,frame_num_2_capture);
//      cap >> frame;
//      imwrite(save_frame_name + frame_num + ".jpg",frame);
//    }

//  cap.release();

//  return 0;

//}



















