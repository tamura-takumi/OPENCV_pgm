#include <opencv2/opencv.hpp>
#include <vector>

/*
* 画像の範囲選択処理で使用する。
* 
*/
// 外部で定義されている変数を参照
extern std::vector<cv::Point> points;
extern cv::Mat src, src_copy;

void mouseCallbackPoligon(int event, int x, int y, int, void*)
{
	if (event == cv::EVENT_LBUTTONDOWN) {
		points.push_back(cv::Point(x, y));  // クリックした位置にポイントを追加
		cv::circle(src, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), cv::FILLED);  // ポイントを描画
		cv::imshow("Image", src);
	}
	else if (event == cv::EVENT_RBUTTONDOWN) {
		// 右クリックでポリゴンを描画
		if (points.size() >= 3) {
			cv::polylines(src, points, true, cv::Scalar(0, 0, 0), 2);
			cv::imshow("Image", src);
		}
	}
	else if (event == cv::EVENT_MBUTTONDOWN) {
		// 中クリックでリセット
		src_copy.copyTo(src);
		points.clear();
		cv::imshow("Image", src);
	}
}

extern cv::Rect2i rectangle_value;
//左クリックした際のピクセル位置情報取得
void mouse_callback_getPixcelVal(int event, int x, int y, int flags, void* userdata)
{
	//bool* isClick = static_cast<bool*>(userdata);
	if (event == cv::EVENT_LBUTTONDOWN) {
		//*isClick = true;
		std::cout << "Draw rectangle\n"
			<< " start position (x, y) : " << x << ", " << y << std::endl;
		rectangle_value = cv::Rect2i(x, y, 0, 0);
		rectangle_value.width = x;
		rectangle_value.height = y;
	}
}


void mouse_callback(int event, int x, int y, int flags, void* userdata)
{
	bool* isClick = static_cast<bool*>(userdata);
	if (event == cv::EVENT_LBUTTONDOWN) {
		*isClick = true;
		std::cout << "Draw rectangle\n"
			<< " start position (x, y) : " << x << ", " << y << std::endl;
		rectangle_value = cv::Rect2i(x, y, 0, 0);
	}
	if (event == cv::EVENT_LBUTTONUP) {
		*isClick = false;
		std::cout << " end   position (x, y) : " << x << ", " << y << std::endl;
		rectangle_value.width = x - rectangle_value.x;
		rectangle_value.height = y - rectangle_value.y;
	}
	if (event == cv::EVENT_MOUSEMOVE) {
		if (*isClick) {
			rectangle_value.width = x - rectangle_value.x;
			rectangle_value.height = y - rectangle_value.y;
		}
	}
}