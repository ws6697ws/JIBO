#ifndef PREDO_H
#define PREDO_H

#include "opencv.hpp"
#include "core.hpp"
#include "highgui.h"

#include <iostream>
using namespace std;

char* string_to_char(string str) {
	char* ch = new char[str.length() + 1];
	for (int i = 0; i < str.length(); i++) ch[i] = str[i];
	ch[str.length()] = '\0';
	return ch;
}

int predo()
{
	int count = 89;
	string path, out;
	for (int i = 0; i < count; i++) {
		path = "C:/Users/Administrator/Pictures/Camera Roll/LCT/";
		if (i > 9) {
			path += i / 10 + '0';
			path += i % 10 + '0';
		}
		else {
			path += i + '0';
		}
		path += ".jpg";
		IplImage * img = cvLoadImage(string_to_char(path));
		if (!img)
		{
			fprintf(stderr, "Can not load image %s\n", path);
			return -1;
		}
		if (!img->imageData)
			return -1;
		IplImage* img1 = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
		cvCvtColor(img, img1, CV_BGR2GRAY);//cvCvtColor(src,des,CV_BGR2GRAY)
		out = "F:/data/PC_AT/LinChuting/";
		if (i > 9) {
			out += i / 10 + '0';
			out += i % 10 + '0';
		}
		else {
			out += i + '0';
		}
		out += ".pgm";
		cvSaveImage(string_to_char(out), img1);
		cvReleaseImage(&img);
		cvReleaseImage(&img1);
	}
	cvWaitKey();
	cvDestroyAllWindows();
	return 0;
}

#endif
