#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/opencv_lib.hpp>

//#include "opencv.hpp"
#include "core/core.hpp"
#include "highgui/highgui.hpp"
//#include <opencv_lib.hpp>
#include <string>
#include <string.h>
//#include <unistd.h>
#include <cassert>
#include <filesystem>
#include <fstream>


#include <Windows.h>
#include <stdexcept>
#include <iostream>
#include <vector>


#include<iostream>
#include<fstream>




#pragma comment(lib,"opencv_world300.lib")


using namespace cv;
using namespace std;

Rect2i rectangle_value;





void mouse_callback_getPixcelVal(int event, int x, int y, int flags, void* userdata);
void mouse_callback(int event, int x, int y, int flags, void* userdata);


/*
�Q�l�����N
https://www.qoosky.dev/techs/e064b34c9c
https://af-e.net/c-language-fgets-line-by-line/
*/

/*
20240825
���L�R�[�h��mouseCallBackFunc.cpp�ֈړ��Bmain.cpp�փv���g�^�C�v�錾��ǋL�B
�Evoid mouse_callback(int event, int x, int y, int flags, void* userdata)
�Evoid mouse_callback_getPixcelVal(int event, int x, int y, int flags, void* userdata)
�E

*/
//�R�[���o�b�N�֐�
//void mouse_callback(int event, int x, int y, int flags, void* userdata)
//{
//	bool* isClick = static_cast<bool*>(userdata);
//	if (event == EVENT_LBUTTONDOWN) {
//		*isClick = true;
//		cout << "Draw rectangle\n"
//			<< " start position (x, y) : " << x << ", " << y << endl;
//		rectangle_value = Rect2i(x, y, 0, 0);
//	}
//	if (event == EVENT_LBUTTONUP) {
//		*isClick = false;
//		cout << " end   position (x, y) : " << x << ", " << y << endl;
//		rectangle_value.width = x - rectangle_value.x;
//		rectangle_value.height = y - rectangle_value.y;
//	}
//	if (event == EVENT_MOUSEMOVE) {
//		if (*isClick) {
//			rectangle_value.width = x - rectangle_value.x;
//			rectangle_value.height = y - rectangle_value.y;
//		}
//	}
//}

////���N���b�N�����ۂ̃s�N�Z���ʒu���擾
//void mouse_callback_getPixcelVal(int event, int x, int y, int flags, void* userdata)
//{
//	//bool* isClick = static_cast<bool*>(userdata);
//	if (event == EVENT_LBUTTONDOWN) {
//		//*isClick = true;
//		cout << "Draw rectangle\n"
//			<< " start position (x, y) : " << x << ", " << y << endl;
//		rectangle_value = Rect2i(x, y, 0, 0);
//		rectangle_value.width = x;
//		rectangle_value.height = y;
//	}
//}


//TEMP
void ImgShowBK(string inputImgPath, string outputImgPath) {

	Mat src;

	//�E�B���h�E�l�[��
	string src_window = "ImgIn";
	string roi_window = "ImgOut";

	//src = imread("C:\makeSoftWare\ImgFld\sampleInput1.PNG");
	src = imread(inputImgPath);
	imshow(src_window, src);
}
//TEMP
void sample(string inputImgPath, string outputImgPath) {
	Mat3b draw_img = imread(inputImgPath);
	for (int imgRow = 0; draw_img.rows; imgRow++) {
		for (int imgCols = 0; draw_img.cols; imgCols++) {
			draw_img(cv::Point(imgRow, imgCols)) = cv::Vec3b(255, 0, 0);
		}
	}
	imshow("test", draw_img);
	waitKey(0);
}

//�摜��̃}�E�X�N���b�N����HSV�F��Ԃ̒l��CMD�ɏo�͂���B
void ImgShow(string inputImgPath, string outputImgPath) {
	bool isClick = false;
	int key = 0;
	int H, S, V;
	Mat src,srcRGB;
	Mat3b draw_img = src;

	//�E�B���h�E�l�[��
	string src_window = "ImgIn";
	string roi_window = "ImgOut";

	src = imread(inputImgPath);
	srcRGB = src.clone();
	cv::cvtColor(src, src, cv::COLOR_BGR2HSV);//�F��ԕύX
	draw_img = src;

	namedWindow(src_window,1);
	//setMouseCallback(src_window, mouse_callback, &isClick);
	setMouseCallback(src_window, mouse_callback_getPixcelVal, NULL);
	imshow(src_window, srcRGB);

	bool mouseOnFlg = false;
	int pointX = 0, pointY = 0;
	for (;;) {
		key = 0;

		// ���{�^���������ꂽ��`��J�n
		if (isClick == true) {
			rectangle(srcRGB, rectangle_value, Scalar(255, 0, 0), 1, CV_AA);
			mouseOnFlg = true;
		}

		imshow(src_window, srcRGB);
		draw_img = src;

		int pointX = rectangle_value.width;
		int pointY = rectangle_value.height;
		//cv::Mat3b draw_img = src;

		// �l�̎擾
		// (0,0)���Ԃ������ꍇ�ABGR�̏���"0,0,255"�Əo�͂����B
		cv::Vec3b hsv = draw_img(cv::Point(pointX, pointY));
		printf("%d,%d,%d\n", hsv[0], hsv[1], hsv[2]);
		H = hsv[0];
		S = hsv[1];
		V = hsv[2];

		// c������HSV�o�^�AX�����ŏI���B
		key = waitKey(1);
		if (key == 'c') {
			FILE* fp = NULL;
			fopen_s(&fp, "pxInfo.txt", "a");
			fprintf(fp, "%d,%d,%d\n", H, S, V);
			fclose(fp);
		}
		if (key == 'x') {
			break;
		}		
	}
}

//�摜�������̐F�̈�}�X�N�쐬�A�}�X�N������HSV�F��ԕύX
cv::Mat iroKukanCnv(string inputImgPath, string outputImgPath,int H,int S,int V,int hiLowRng, cv::Mat src) {
	
	//cv::Mat src = cv::imread(inputImgPath, cv::IMREAD_COLOR);
	cv::Mat hsv;
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);// �F��ԕύX

	// inRange �ɂ���� Hue ������͈̔͂ɂ���̈�� mask ���擾���܂��B
	cv::Mat mask;
	//163,39,251
	//178,213,180 (�ԁj
	//139,185,207 (�ނ炳���F)
	//H = 178, S = 213, V = 180;
	//H = 139, S = 185, V = 207;
	hiLowRng = 10;
	cv::inRange(hsv, cv::Scalar(H - hiLowRng, S - hiLowRng, V - hiLowRng),
		cv::Scalar(H + hiLowRng, S + hiLowRng, V + hiLowRng), mask);

	// src �� src �� and ���Z���Amask �̗̈� (0 �ȊO�̗̈�) �ɂ��Ă̂ݍs���摜��؂�o���B
	double min, max;
	cv::minMaxLoc(mask, &min, &max);

	cv::Mat dst;
	cv::bitwise_and(src, src, dst, mask);

	//cv::imshow("src", src);
	//cv::imshow("mask", mask);
	//cv::imshow("res", dst);

	//dst�̐F���ϊ�
	for (int imgRow = 0; imgRow < dst.rows; imgRow++) {
		for (int imgCol = 0; imgCol < dst.cols; imgCol++) {
			cv::Vec3b& dstPixel = dst.at<cv::Vec3b>(imgRow, imgCol);//dst
			cv::Vec3b& hsvPixel = src.at<cv::Vec3b>(imgRow, imgCol);//src

			//���ȊO�̖��x�̏ꍇ�AHSV�C�ӂ̒l�ɕύX
			if (dstPixel[2] != 0) {
				hsvPixel[0] = H+20;
				hsvPixel[1] = S+20;
				hsvPixel[2] = V+20;
			}
		}
	}
	//cv::imshow("dst", src);

	//cv::waitKey(0);
	return src;
}


//�摜����̈�𒊏o
cv::Mat extrImg(cv::Mat src_img) {
	
	// 2�l��
	cv::Mat gray_img, bin_img;
	cv::cvtColor(src_img, gray_img, CV_BGR2GRAY);
	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	/// �֊s�̌��o
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	//�֊s�̕`��
	cv::findContours(bin_img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	cv::drawContours(src_img, contours, 0, cv::Scalar(0, 0, 200), 3, CV_AA, hierarchy, 2);
	cv::imshow("Contours1", src_img);
	cv::waitKey(0);

	return src_img;
}


// �v���g�^�C�v�錾
void mouseCallbackPoligon(int event, int x, int y, int, void*);
std::vector<cv::Point> points;  // �A���J�[�|�C���g��ێ�����x�N�^�[
cv::Mat img, img_copy, src, src_copy;

//int main()
//{
//	// �摜��ǂݍ��݂܂�
//	img = cv::imread("test.PNG");
//	if (img.empty()) {
//		std::cerr << "�摜��������܂���" << std::endl;
//		return -1;
//	}
//
//	img.copyTo(img_copy);  // ������Ԃ�ۑ�
//
//	cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
//	cv::setMouseCallback("Image", mouseCallbackPoligon);
//	cv::imshow("Image", img);
//	cv::waitKey(0);
//
//	if (points.size() >= 3) {
//		cv::Mat mask = cv::Mat::zeros(img.size(), CV_8UC1);
//		std::vector<std::vector<cv::Point>> pts{ points };
//		cv::fillPoly(mask, pts, cv::Scalar(255));
//
//		cv::Mat result;
//		img.copyTo(result, mask);
//
//		cv::imshow("Result", result);
//		cv::imwrite("Result.PNG", result);
//
//		cv::waitKey(0);
//	}
//
//	return 0;
//}

//cv::Mat src, src_copy, result;

int main() { //main_HSVkakunin

	//�摜�i�[��
	//string Strarg = "XXXX";

	//�摜��
	//string inputImg = "red.PNG";
	string inputImg = "test.PNG";
	//string inputImg = "defg.PNG";
	//string outputImg = "\sampleOutput1.PNG";


	//�t�@�C���p�X
	string inputImgPath = "";
    string oututImgPath = "";

	//inputImgPath.append(Strarg);
	inputImgPath.append(inputImg);
	int num = 0;
	//mkTxt();

	//printf("���[�h���́@1�F�摜��N���b�N��A�L�[�́uc�v�����Ńs�N�Z���l��pxInfo.txt�ɏo�́@2�FpxInfo.txt�̏������ƂɃs�N�Z���l��ϊ�\n�L�[�́ux�v�����ŏI�����܂��B");
	//scanf("%d", &num);
	num = 2;
	if (num == 1) {
		//�摜��̃N���b�N�������̃s�N�Z���l��TXT�ɏo�́B
		ImgShow(inputImgPath, oututImgPath);
	}
	
	if (num == 2) {
		//pxInfo.txt�L�ړ��e�̎w��s�N�Z���l��ϊ�
		//cv::Mat src, src_copy, result;
		cv::Mat result;
		src= cv::imread(inputImgPath, cv::IMREAD_COLOR);
		printf("�����̊m�F\n");
		//�w��͈͉摜�̃}�X�N�摜�o�͏����̂��ߒǋL�B
		src.copyTo(src_copy);  // ������Ԃ�ۑ� 
		cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
		cv::setMouseCallback("Image", mouseCallbackPoligon);
		cv::imshow("Image", src);
		cv::waitKey(0);
		printf("�����̊m�F\n");
		if (points.size() >= 3) {
			cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);
			std::vector<std::vector<cv::Point>> pts{ points };
			cv::fillPoly(mask, pts, cv::Scalar(255));

			src.copyTo(result, mask);
			cv::imshow("Result", result);
			cv::imwrite("Result.PNG", result);
			cv::waitKey(0);
		}


		//�F�ύX����
		char buffer[100];
		FILE* fp;
		fp = fopen("pxInfo.txt", "r");
		while (fgets(buffer, sizeof(buffer), fp) != NULL) {
			int H, S, V;
			//printf("%s\n", buffer);
			sscanf(buffer, "%d,%d,%d", &H, &S, &V);
			//src = iroKukanCnv(inputImgPath, oututImgPath, H, S, V, 10, src);
			src = iroKukanCnv(inputImgPath, oututImgPath, H, S, V, 10, result);
			printf("%d:%d:%d\n", H, S, V);
		}

		imshow("window", src);
		cv::imwrite("Result_CUT.PNG", src);
		fclose(fp);
		cv::waitKey(0);
		cv::destroyAllWindows();
	}
		

	return 0;
}


