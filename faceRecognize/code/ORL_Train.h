#include "opencv.hpp"
#include "core.hpp"
#include "face.hpp"
#include "predo.h"

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;
using namespace face;

Mat testSample[200];
int testLabel[200];

void train_model(string in, string out, int type, int outside) {
	// holds images and labels
	vector<Mat> images;
	vector<int> labels;
	Ptr<FaceRecognizer> model;

	// images for 40 person
	string path_1, path_tmp, path_end;

	for (int i = 1; i <= 40; i++) {
		path_1 = in;
		if (i > 9) {
			path_1 += i / 10 + '0';
			path_1 += i % 10 + '0';
		}
		else {
			path_1 += i + '0';
		}
		path_1 += "/";
		for (int j = 1; j <= 10; j++) {
			path_tmp = "";
			if (j == 10) path_tmp += "10";
			else path_tmp += j + '0';
			path_tmp += ".pgm";
			path_end = path_1 + path_tmp;
			images.push_back(imread(path_end, CV_LOAD_IMAGE_GRAYSCALE)); labels.push_back(i);
		}
		if (i == outside) {
			testSample[i + type * 40] = images[images.size() - 1];
			testLabel[i + type * 40] = labels[labels.size() - 1];
			images.pop_back();
			labels.pop_back();
		}
	}
	if (type == 0) {
		model = createEigenFaceRecognizer();
	}
	else if (type == 1) {
		model = createFisherFaceRecognizer();
	}
	else if (type == 2) {
		model = createLBPHFaceRecognizer();
	}
	else {
		cout << "no such faceRecogizer!\n";
		exit(0);
	}
	//model->train(images, labels);
	//model->save(out);
}

void train_generator() {
	string eigen_p, fisher_p, LBPH_p;
	for (int i = 1; i <= 40; i++) {
		eigen_p = "F:/data/train/eigen/eigen_";
		if (i > 9) {
			eigen_p += i / 10 + '0';
			eigen_p += i % 10 + '0';
		}
		else {
			eigen_p += i + '0';
		}
		eigen_p += ".xml";
		train_model("F:/data/at/s", eigen_p, 0, i);
	}
	for (int i = 1; i <= 40; i++) {
		fisher_p = "F:/data/train/fisher/fisher_";
		if (i > 9) {
			fisher_p += i / 10 + '0';
			fisher_p += i % 10 + '0';
		}
		else {
			fisher_p += i + '0';
		}
		fisher_p += ".xml";
		train_model("F:/data/at/s", fisher_p, 1, i);
	}
	for (int i = 1; i <= 40; i++) {
		LBPH_p = "F:/data/train/LBPH/LBPH_";
		if (i > 9) {
			LBPH_p += i / 10 + '0';
			LBPH_p += i % 10 + '0';
		}
		else {
			LBPH_p += i + '0';
		}
		LBPH_p += ".xml";
		train_model("F:/data/at/s", LBPH_p, 2, i);
	}
}

bool simple_test(string load_p, int type, int outside) {
	Ptr<FaceRecognizer> model;
	string output = "";
	stringstream classNum1, classNum2;

	if (type == 0) {
		if (outside == 1) output = "eigen-model:\n";
		model = createEigenFaceRecognizer();
	}
	else if (type == 1) {
		if (outside == 1) output = "fisher-model:\n";
		model = createFisherFaceRecognizer();
	}
	else if (type == 2) {
		if (outside == 1) output = "LBPH-model:\n";
		model = createLBPHFaceRecognizer();
	}
	else {
		cout << "no such faceRecogizer!\n";
		exit(0);
	}
	model->load(load_p);
	int predictedLabel = model->predict(testSample[outside + type * 40]);
	FILE *fw = fopen("F:/data/predicted_result.txt", "a+");
	output += "Predicted class = ";
	classNum1 << predictedLabel;
	classNum2 << testLabel[outside + type * 40];
	output += classNum1.str() + " / Actual class = " + classNum2.str() + ".\n";
	//cout << "Predicted class = " << predictedLabel << " / Actual class = " << testLabel[outside + type * 40] << ".\n";
	fputs(string_to_char(output), fw);
	fclose(fw);
	return predictedLabel == testLabel[outside + type * 40];
}

void test_generator() {
	string eigen_p, fisher_p, LBPH_p;
	bool b_e, b_f, b_l;
	double sum_e, sum_f, sum_l;
	sum_e = sum_f = sum_l = 0.0;
	for (int i = 1; i <= 40; i++) {
		eigen_p = "F:/data/train/eigen/eigen_";
		if (i > 9) {
			eigen_p += i / 10 + '0';
			eigen_p += i % 10 + '0';
		}
		else {
			eigen_p += i + '0';
		}
		eigen_p += ".xml";
		b_e = simple_test(eigen_p, 0, i);
		if (b_e) sum_e++;
	}
	cout << "eigenFaceRecognizer accuracy: " << sum_e / 40.0 << endl;

	for (int i = 1; i <= 40; i++) {
		fisher_p = "F:/data/train/fisher/fisher_";
		if (i > 9) {
			fisher_p += i / 10 + '0';
			fisher_p += i % 10 + '0';
		}
		else {
			fisher_p += i + '0';
		}
		fisher_p += ".xml";
		b_f = simple_test(fisher_p, 1, i);
		if (b_f) sum_f++;
	}
	cout << "fisherFaceRecognizer accuracy: " << sum_f / 40.0 << endl;

	for (int i = 1; i <= 40; i++) {
		LBPH_p = "F:/data/train/LBPH/LBPH_";
		if (i > 9) {
			LBPH_p += i / 10 + '0';
			LBPH_p += i % 10 + '0';
		}
		else {
			LBPH_p += i + '0';
		}
		LBPH_p += ".xml";
		b_l = simple_test(LBPH_p, 2, i);
		if (b_l) sum_l++;
	}
	cout << "LBPHFaceRecognizer accuracy: " << sum_l / 40.0 << endl;
}