//face_authentication_for_picture.cpp : 入力画像から顔認識して、デバック表示に出す

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <vector>;

#define HIST_FLAT_ON //ヒストグラムの平滑化：デフォルトON
//#define EDGE_DETECTION_ON //エッジ検出：デフォルトOFF

using namespace cv;
using namespace std;

int main()
{

	//使用する顔認識学習ファイルを設定する
	//OpenCVのdataフォルダーの中にあるやつをお好みで設定
	//string cascade_path = "haarcascade_frontalface_alt.xml";
	//string cascade_path = "haarcascade_frontalface_default.xml";
	string cascade_path = "haarcascade_frontalface_alt2.xml";
	
	//入力画像
	string filename = "test.png";
	Mat src_image = imread( filename, CV_LOAD_IMAGE_COLOR);
	namedWindow("元画像");
	imshow("元画像", src_image);
	
	//グレー画像の領域作成
	Mat gray_image;
	#ifdef EDGE_DETECTION_ON
	Mat laplacian_img, temp_img;
	#endif /* EDGE_DETECTION_ON */

	//グレー画像作成
	cvtColor(src_image, gray_image, CV_BGR2GRAY);

	#ifdef HIST_FLAT_ON
	//ヒストグラム平坦化
	equalizeHist(gray_image, gray_image);
	#endif /* HIST_FLAT_ON */

	#ifdef EDGE_DETECTION_ON
	//ラプラシアンフィルタでエッジ検出
	Laplacian(gray_image, temp_img,CV_16S,3);
	convertScaleAbs(temp_img, laplacian_img, 1, 0);
	threshold(laplacian_img, laplacian_img, 0, 255, THRESH_OTSU);
	#endif /* EDGE_DETECTION_ON */

	//グレー画像出力
	namedWindow("グレー画像");
	imshow("グレー画像", gray_image);
	#ifdef EDGE_DETECTION_ON
	//エッジ画像出力
	namedWindow("エッジ画像");
	imshow("エッジ画像", laplacian_img);
	#endif /* EDGE_DETECTION_ON */
	
	//カスケード分類器の特徴量を取得
	vector <Rect> faces;
	CascadeClassifier cascade;
	cascade.load(cascade_path);
	#ifdef HIST_FLAT_ON
	cascade.detectMultiScale(gray_image, faces, 1.1, 2, 2);
	#endif /* HIST_FLAT_ON */
	#ifdef EDGE_DETECTION_ON
	cascade.detectMultiScale(laplacian_img, faces, 1.1, 2, 2);
	#endif /* EDGE_DETECTION_ON */

	//検出した顔を描画する&出力する
	for (auto it = faces.begin(); it != faces.end(); ++it)
	rectangle(src_image, it->tl(), it->br(), Scalar(0, 255, 255), 2, 8, 0);
	namedWindow("検出画像");
	imshow("検出画像",src_image);
	imwrite("検出画像.png", src_image);

	waitKey(0);
	destroyAllWindows();
	return 0;
}