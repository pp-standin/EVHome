#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <fstream>
#include <ctime>
using namespace cv;
using namespace std;
char current[20];
std::fstream temp;
int temperature = 0;

//hide the local functions in an anon namespace
namespace {
    void help(char** av) {
        cout << "The program captures frames from a  camera ." << endl
             << "Usage:\n" << av[0] << " device number>" << endl
             << "\tTo capture from a camera pass the device number. To find the device number, try ls /dev/video*" << endl
             << "\texample: " << av[0] << " 0" << endl
    }

    int process(VideoCapture& capture) {
        int n = 0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        cout << "press space to save a picture. q or esc to quit" << endl;
        //namedWindow(window_name, WINDOW_KEEPRATIO); //resizable window;
        Mat frame;

        for (;;) {
            capture >> frame;
            if (frame.empty())
                break;
		time_t t = time(0);   // get time now
		tm* now = localtime(&t);

            //imshow(window_name, frame);
            char key = (char)waitKey(30000); //delay N millis, usually long enough to display and capture input
    	if(now->tm_min % 5 ==0)
		{	
		temp.open("/sys/class/thermal/thermal_zone0/temp",std::ios::in);
    		temp >> current;
		temperature=atoi(current)/1000;
		
                sprintf(filename,"trials-%d%d%d_%d.jpg",now->tm_hour,now->tm_min,now->tm_sec,temperature);
                imwrite(filename,frame);
                cout << "Saved " << filename << endl;
  		}
      }
        return 0;
    }
}

int main(int ac, char** av) {
    cv::CommandLineParser parser(ac, av, "{help h||}{@input||}");
    if (parser.has("help"))
    {
        help(av);
        return 0;
    }
    std::string arg = parser.get<std::string>("@input");
    if (arg.empty()) {
        help(av);
        return 1;
    }
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        cerr << "Failed to open the video device, video file or image sequence!\n" << endl;
        help(av);
        return 1;
    }
    return process(capture);
}
