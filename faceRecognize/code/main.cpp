#include "opencv.hpp"
#include "cv.h"
#include "highgui.h"
#include "core.hpp"
#include "face.hpp"
#include "predo.h"
#include "ORL_Train.h"
#include "MY_Train.h"

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;
using namespace face;

void UI(int ID, double Conf, int change) {
	cout << "########## JiBo ##########\n";
	cout << "#   =====  |  |  =====   #\n";
	if(!change) cout << "#     o    |  |     o    #\n";
	else cout << "#     v    |  |     v    #\n";
	cout << "#           \\/           #\n";
	cout << "#                        #\n";
	cout << "  #       \\____/       #\n";
	cout << "     ################\n\n";
	if (Conf < 50.0) {
		cout << ">> Prediction: ";
		if (ID == 0) cout << "LinChuting.\n";
		else if (ID == 1) cout << "WuJiahui.\n";
		else if (ID == 2) cout << "LiChengquan.\n";
		else cout << "could not predict!\n";
	} else cout << "Nobody is detected!\n";
	cout << ">> Confidence: " << Conf << endl;
}

void camera() {
	Ptr<FaceRecognizer> model;
	model = createLBPHFaceRecognizer();
	cout << "loading model...\n";
	model->load("F:/data/PC_TRAIN/lct_Model.xml");

	vector<Mat> images;
	VideoCapture cap(0);
	if (!cap.isOpened()) return;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	Mat frame, frame_gray;
	string path;
	bool stop = false;
	int predictedLabel;
	double predictedConfidence;
	int count = 0;
	while (!stop)
	{
		predictedLabel = -1;
		predictedConfidence = 0.0;
		cap >> frame;
		cvtColor(frame, frame_gray, CV_BGR2GRAY);
		//GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
		//Canny(edges, edges, 0, 30, 3);
		imshow("JiBo-Eye", frame_gray);
		model->predict(frame_gray, predictedLabel, predictedConfidence);
		system("cls");
		UI(predictedLabel, predictedConfidence, (count++) % 2);
		if (waitKey(30) >= 0)
			stop = true;
	}
}

int main() {
	// take pictures
	camera();

	// ###### MY FACE #####
	// prefix the picture to '*.pgm'
	//predo();

	// train my '*.pgm' at 'F:/data/PC_AT/LinChuting/*.pgm'
	/*cout << "start training models...\n";
	my_train();
	cout << "complete training.\n\n";*/

	// predictiton my faces
	/*cout << "loading model...\n";
	predict();*/

	// ###### ORL FACE #####
	/*// train 40 persons' face datasets (each with 9 small pictures)
	cout << "start training models...\n";
	train_generator();
	cout << "complete training.\n\n";

	// load models from dataset
	// 40 tests (each '*.xml' has a picture to test, that picture is without training)
	cout << "start testing model(loading model...)\n";
	test_generator();
	cout << "complete all tests.\n\n";*/

	// wait for response
	char anyKey; cin >> anyKey;

	return 0;
}
