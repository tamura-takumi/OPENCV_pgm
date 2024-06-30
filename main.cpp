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




//コールバック関数
void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	bool* isClick = static_cast<bool*>(userdata);
	if (event == EVENT_LBUTTONDOWN) {
		*isClick = true;
		cout << "Draw rectangle\n"
			<< " start position (x, y) : " << x << ", " << y << endl;
		rectangle_value = Rect2i(x, y, 0, 0);
	}
	if (event == EVENT_LBUTTONUP) {
		*isClick = false;
		cout << " end   position (x, y) : " << x << ", " << y << endl;
		rectangle_value.width = x - rectangle_value.x;
		rectangle_value.height = y - rectangle_value.y;
	}
	if (event == EVENT_MOUSEMOVE) {
		if (*isClick) {
			rectangle_value.width = x - rectangle_value.x;
			rectangle_value.height = y - rectangle_value.y;
		}
	}
}


void mouse_callback_getPixcelVal(int event, int x, int y, int flags, void* userdata)
{
	//bool* isClick = static_cast<bool*>(userdata);
	if (event == EVENT_LBUTTONDOWN) {
		//*isClick = true;
		cout << "Draw rectangle\n"
			<< " start position (x, y) : " << x << ", " << y << endl;
		rectangle_value = Rect2i(x, y, 0, 0);
		rectangle_value.width = x;
		rectangle_value.height = y;
	}
}


//画像を読込み、表示させる
void ImgShowBK(string inputImgPath, string outputImgPath) {

	Mat src;

	//ウィンドウネーム
	string src_window = "ImgIn";
	string roi_window = "ImgOut";

	//src = imread("C:\makeSoftWare\ImgFld\sampleInput1.PNG");
	src = imread(inputImgPath);
	imshow(src_window, src);
}

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

//画像上のマウスクリックしたHSV色空間の値をCMDに出力する。
int B = 0, G = 0, R = 0;
void ImgShow(string inputImgPath, string outputImgPath) {
	bool isClick = false;
	int key = 0;
	Mat src;
	Mat3b draw_img = src;

	//ウィンドウネーム
	string src_window = "ImgIn";
	string roi_window = "ImgOut";

	//src = imread("C:\makeSoftWare\ImgFld\sampleInput1.PNG");
	src = imread(inputImgPath);
	cv::cvtColor(src, src, cv::COLOR_BGR2HSV);//色空間変更
	draw_img = src;

	namedWindow(src_window,1);
	//setMouseCallback(src_window, mouse_callback, &isClick);
	setMouseCallback(src_window, mouse_callback_getPixcelVal, NULL);
	imshow(src_window, src);

	bool mouseOnFlg = false;
	int pointX = 0, pointY = 0;
	for (;;) {
		key = 0;

		// 左ボタンが押されたら描画開始
		if (isClick == true) {
			rectangle(draw_img, rectangle_value, Scalar(255, 0, 0), 1, CV_AA);
			mouseOnFlg = true;
		}

		imshow(src_window, draw_img);
		draw_img = src;

		int pointX = rectangle_value.width;
		int pointY = rectangle_value.height;
		//cv::Mat3b draw_img = src;

		// 値の取得
		// (0,0)が赤だった場合、BGRの順に"0,0,255"と出力される。
		cv::Vec3b bgr = draw_img(cv::Point(pointX, pointY));
		printf("%d,%d,%d\n", bgr[0], bgr[1], bgr[2]);
		B = bgr[0];
		G = bgr[1];
		R = bgr[2];

		// qキーが押されたら終了
		key = waitKey(1);
		if (key == 'q')
			break;
	}


Mat draw_imgR, draw_imgG, draw_imgB;
cvtColor(draw_img, draw_imgR, COLOR_BGR2GRAY);

//二値化
threshold(draw_imgR, draw_imgR, 50, 100, cv::THRESH_BINARY);
imshow("src_windowR", draw_imgR);
waitKey(0);


}

//画像から特定の色領域マスク作成、マスク部分をHSV色空間変更
//https://www.qoosky.dev/techs/e064b34c9c
void iroKukanCnv(string inputImgPath, string outputImgPath) {
	// BGR 色空間
	cv::Mat src = cv::imread(inputImgPath, cv::IMREAD_COLOR);

	// HSV 色空間での表現に変換
	cv::Mat hsv;
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

	// inRange によって Hue が特定の範囲にある領域の mask を取得します。
	cv::Mat mask;
	//163,39,251
	//178,213,180 (赤）
	//139,185,207 (むらさき色)
	int H, S, V, highAndLow;
	//H = 178, S = 213, V = 180;
	H = 139, S = 185, V = 207;
	highAndLow = 10;
	cv::inRange(hsv, cv::Scalar(H - highAndLow, S - highAndLow, V - highAndLow),
		cv::Scalar(H + highAndLow, S + highAndLow, V + highAndLow), mask);

	// src と src の and 演算を、mask の領域 (0 以外の領域) についてのみ行うことで、画像を切り出します。
	double min, max;
	cv::minMaxLoc(mask, &min, &max);

	cv::Mat dst;
	cv::bitwise_and(src, src, dst, mask);

	cv::imshow("src", src);
	cv::imshow("mask", mask);
	//cv::imshow("res", dst);

	//dstの色情報変換
	for (int imgRow = 0; imgRow < dst.rows; imgRow++) {
		for (int imgCol = 0; imgCol < dst.cols; imgCol++) {
			cv::Vec3b& dstPixel = dst.at<cv::Vec3b>(imgRow, imgCol);//dst
			cv::Vec3b& hsvPixel = src.at<cv::Vec3b>(imgRow, imgCol);//src

			//黒以外の明度の場合、HSV任意の値に変更
			if (dstPixel[2] != 0) {
				hsvPixel[0] = 135;
				hsvPixel[1] = 90;
				hsvPixel[2] = 88;
			}
		}
	}
	cv::imshow("dst", src);

	cv::waitKey(0);
}


int main() {

	//画像格納先
	//string Strarg = "C:\\makeSoftWare\\ImgFld\\";

	//画像名
	//string inputImg = "red.PNG";
	string inputImg = "test.PNG";
	//string inputImg = "defg.PNG";
	//string outputImg = "\sampleOutput1.PNG";


	//ファイルパス
	string inputImgPath = "";
    string oututImgPath = "";

	//inputImgPath.append(Strarg);
	inputImgPath.append(inputImg);


	//ImgShow(inputImgPath, oututImgPath);
	iroKukanCnv(inputImgPath, oututImgPath);

	cv::destroyAllWindows();

	return 0;
}


