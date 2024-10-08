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
参考リンク
https://www.qoosky.dev/techs/e064b34c9c
https://af-e.net/c-language-fgets-line-by-line/
*/

/*
20240825
下記コードをmouseCallBackFunc.cppへ移動。main.cppへプロトタイプ宣言を追記。
・void mouse_callback(int event, int x, int y, int flags, void* userdata)
・void mouse_callback_getPixcelVal(int event, int x, int y, int flags, void* userdata)
・

*/
//コールバック関数
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

////左クリックした際のピクセル位置情報取得
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

	//ウィンドウネーム
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

//画像上のマウスクリックしたHSV色空間の値をCMDに出力する。
void ImgShow(string inputImgPath, string outputImgPath) {
	bool isClick = false;
	int key = 0;
	int H, S, V;
	Mat src,srcRGB;
	Mat3b draw_img = src;

	//ウィンドウネーム
	string src_window = "ImgIn";
	string roi_window = "ImgOut";

	src = imread(inputImgPath);
	srcRGB = src.clone();
	cv::cvtColor(src, src, cv::COLOR_BGR2HSV);//色空間変更
	draw_img = src;

	namedWindow(src_window,1);
	//setMouseCallback(src_window, mouse_callback, &isClick);
	setMouseCallback(src_window, mouse_callback_getPixcelVal, NULL);
	imshow(src_window, srcRGB);

	bool mouseOnFlg = false;
	int pointX = 0, pointY = 0;
	for (;;) {
		key = 0;

		// 左ボタンが押されたら描画開始
		if (isClick == true) {
			rectangle(srcRGB, rectangle_value, Scalar(255, 0, 0), 1, CV_AA);
			mouseOnFlg = true;
		}

		imshow(src_window, srcRGB);
		draw_img = src;

		int pointX = rectangle_value.width;
		int pointY = rectangle_value.height;
		//cv::Mat3b draw_img = src;

		// 値の取得
		// (0,0)が赤だった場合、BGRの順に"0,0,255"と出力される。
		cv::Vec3b hsv = draw_img(cv::Point(pointX, pointY));
		printf("%d,%d,%d\n", hsv[0], hsv[1], hsv[2]);
		H = hsv[0];
		S = hsv[1];
		V = hsv[2];

		// c押下でHSV登録、X押下で終了。
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

//画像から特定の色領域マスク作成、マスク部分をHSV色空間変更
cv::Mat iroKukanCnv(string inputImgPath, string outputImgPath,int H,int S,int V,int hiLowRng, cv::Mat src) {
	
	//cv::Mat src = cv::imread(inputImgPath, cv::IMREAD_COLOR);
	cv::Mat hsv;
	cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);// 色空間変更

	// inRange によって Hue が特定の範囲にある領域の mask を取得します。
	cv::Mat mask;
	//163,39,251
	//178,213,180 (赤）
	//139,185,207 (むらさき色)
	//H = 178, S = 213, V = 180;
	//H = 139, S = 185, V = 207;
	hiLowRng = 10;
	cv::inRange(hsv, cv::Scalar(H - hiLowRng, S - hiLowRng, V - hiLowRng),
		cv::Scalar(H + hiLowRng, S + hiLowRng, V + hiLowRng), mask);

	// src と src の and 演算を、mask の領域 (0 以外の領域) についてのみ行い画像を切り出し。
	double min, max;
	cv::minMaxLoc(mask, &min, &max);

	cv::Mat dst;
	cv::bitwise_and(src, src, dst, mask);

	//cv::imshow("src", src);
	//cv::imshow("mask", mask);
	//cv::imshow("res", dst);

	//dstの色情報変換
	for (int imgRow = 0; imgRow < dst.rows; imgRow++) {
		for (int imgCol = 0; imgCol < dst.cols; imgCol++) {
			cv::Vec3b& dstPixel = dst.at<cv::Vec3b>(imgRow, imgCol);//dst
			cv::Vec3b& hsvPixel = src.at<cv::Vec3b>(imgRow, imgCol);//src

			//黒以外の明度の場合、HSV任意の値に変更
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


//画像から領域を抽出
cv::Mat extrImg(cv::Mat src_img) {
	
	// 2値化
	cv::Mat gray_img, bin_img;
	cv::cvtColor(src_img, gray_img, CV_BGR2GRAY);
	cv::threshold(gray_img, bin_img, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	/// 輪郭の検出
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;

	//輪郭の描写
	cv::findContours(bin_img, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
	cv::drawContours(src_img, contours, 0, cv::Scalar(0, 0, 200), 3, CV_AA, hierarchy, 2);
	cv::imshow("Contours1", src_img);
	cv::waitKey(0);

	return src_img;
}


// プロトタイプ宣言
void mouseCallbackPoligon(int event, int x, int y, int, void*);
std::vector<cv::Point> points;  // アンカーポイントを保持するベクター
cv::Mat img, img_copy, src, src_copy;

//int main()
//{
//	// 画像を読み込みます
//	img = cv::imread("test.PNG");
//	if (img.empty()) {
//		std::cerr << "画像が見つかりません" << std::endl;
//		return -1;
//	}
//
//	img.copyTo(img_copy);  // 初期状態を保存
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

	//画像格納先
	//string Strarg = "XXXX";

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
	int num = 0;
	//mkTxt();

	//printf("モード入力　1：画像上クリック後、キーの「c」押下でピクセル値をpxInfo.txtに出力　2：pxInfo.txtの情報をもとにピクセル値を変換\nキーの「x」押下で終了します。");
	//scanf("%d", &num);
	num = 2;
	if (num == 1) {
		//画像上のクリックした所のピクセル値をTXTに出力。
		ImgShow(inputImgPath, oututImgPath);
	}
	
	if (num == 2) {
		//pxInfo.txt記載内容の指定ピクセル値を変換
		//cv::Mat src, src_copy, result;
		cv::Mat result;
		src= cv::imread(inputImgPath, cv::IMREAD_COLOR);
		printf("処理の確認\n");
		//指定範囲画像のマスク画像出力処理のため追記。
		src.copyTo(src_copy);  // 初期状態を保存 
		cv::namedWindow("Image", cv::WINDOW_AUTOSIZE);
		cv::setMouseCallback("Image", mouseCallbackPoligon);
		cv::imshow("Image", src);
		cv::waitKey(0);
		printf("処理の確認\n");
		if (points.size() >= 3) {
			cv::Mat mask = cv::Mat::zeros(src.size(), CV_8UC1);
			std::vector<std::vector<cv::Point>> pts{ points };
			cv::fillPoly(mask, pts, cv::Scalar(255));

			src.copyTo(result, mask);
			cv::imshow("Result", result);
			cv::imwrite("Result.PNG", result);
			cv::waitKey(0);
		}


		//色変更処理
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


