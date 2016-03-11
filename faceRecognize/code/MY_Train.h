#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;
using namespace face;

void my_train() {
	vector<Mat> images;
	vector<int> labels;
	Ptr<FaceRecognizer> model;
	string path_in, path_out;

	for (int i = 0; i < 89; i++) {
		path_in = "F:/data/PC_AT/LinChuting/";
		if (i > 9) {
			path_in += i / 10 + '0';
			path_in += i % 10 + '0';
		}
		else {
			path_in += i + '0';
		}
		path_in += ".pgm";
		images.push_back(imread(path_in, CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(0);
	}
	model = createLBPHFaceRecognizer();
	//model->load("F:/data/PC_TRAIN/lct_Model.xml");
	//model->update(images, labels);
	model->train(images, labels);
	path_out = "F:/data/PC_TRAIN/lct_Model.xml";
	model->save(path_out);
}

void predict() {
	Ptr<FaceRecognizer> model;
	model = createLBPHFaceRecognizer();
	model->load("F:/data/PC_TRAIN/my_Model.xml");
	Mat img = imread("F:/data/PC_AT/test/lcq0.pgm", CV_LOAD_IMAGE_GRAYSCALE);
	int predictedLabel = model->predict(img);
	if (predictedLabel == 0) cout << "prediction result: LinChuting.\n";
	else if (predictedLabel == 1) cout << "prediction result: WuJiahui.\n";
	else if (predictedLabel == 2) cout << "prediction result: LiChengquan.\n";
	else cout << "could not predict!\n";
}