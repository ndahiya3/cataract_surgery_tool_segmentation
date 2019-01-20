#include "opencv2/opencv.hpp"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

int main(int, char**)
{
  string basefilename = "train01";
  string videofilename = basefilename + ".mp4";
  VideoCapture cap;
  cap.open(videofilename);

  if (!cap.isOpened()) {
      cout << "Error opening video." << endl;
      return 0;
    }

  // Print video properties
  int codec = static_cast<int>(cap.get(CV_CAP_PROP_FOURCC));
  double fps = cap.get(CV_CAP_PROP_FPS);
  int fwidth = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_WIDTH));
  int fheight = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_HEIGHT));
  int numFrames = static_cast<int>(cap.get(CV_CAP_PROP_FRAME_COUNT));

  cout << "Frame Width: " << fwidth << "  Frame Height: " << fheight << "  FPS: " << fps << endl;
  cout << "Number of frames: " << numFrames << endl;

  // Capture and save frames
  int framenum=1;
  while (1){
      Mat frame;
      cap >> frame;
      if (frame.empty())
        break;
      string savefilename = "./train01-frames/" + basefilename + "-" + to_string(framenum) + ".jpg";
      imwrite(savefilename,frame);

      imshow("Frame", frame);
      char c=(char)waitKey(25);
      if (c==27)
        break;
      framenum++;
    }
  cap.release();
  destroyAllWindows();
  cout << "Num frames extracted: " << --framenum << endl;
  return 0;

}
