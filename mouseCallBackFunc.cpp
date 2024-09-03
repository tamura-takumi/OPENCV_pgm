#include <opencv2/opencv.hpp>
#include <vector>

/*
* �摜�͈̔͑I�������Ŏg�p����B
* 
*/
// �O���Œ�`����Ă���ϐ����Q��
extern std::vector<cv::Point> points;
extern cv::Mat src, src_copy;

void mouseCallbackPoligon(int event, int x, int y, int, void*)
{
	if (event == cv::EVENT_LBUTTONDOWN) {
		points.push_back(cv::Point(x, y));  // �N���b�N�����ʒu�Ƀ|�C���g��ǉ�
		cv::circle(src, cv::Point(x, y), 3, cv::Scalar(0, 255, 0), cv::FILLED);  // �|�C���g��`��
		cv::imshow("Image", src);
	}
	else if (event == cv::EVENT_RBUTTONDOWN) {
		// �E�N���b�N�Ń|���S����`��
		if (points.size() >= 3) {
			cv::polylines(src, points, true, cv::Scalar(0, 0, 0), 2);
			cv::imshow("Image", src);
		}
	}
	else if (event == cv::EVENT_MBUTTONDOWN) {
		// ���N���b�N�Ń��Z�b�g
		src_copy.copyTo(src);
		points.clear();
		cv::imshow("Image", src);
	}
}

extern cv::Rect2i rectangle_value;
//���N���b�N�����ۂ̃s�N�Z���ʒu���擾
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