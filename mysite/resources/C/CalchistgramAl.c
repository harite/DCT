#define COLOR_STEPS 256
 
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
 
IplImage *markers = 0, *dsp_img = 0;
 
typedef struct{
  int r[COLOR_STEPS];//赤の個数
  int g[COLOR_STEPS];//緑の個数
  int b[COLOR_STEPS];//赤の個数
}histgram_elements;

 
/* マウスイベント用コールバック関数 */
void on_mouse (int event, int x, int y, int flags, void *param)
{
  int seed_rad = 20;
  static int seed_num = 0;
  CvPoint pt;
  // (1)クリックにより中心を指定し，円形のシード領域を設定する
  if (event == CV_EVENT_LBUTTONDOWN) {
    seed_num++;
    pt = cvPoint (x, y);
    cvCircle (markers, pt, seed_rad, cvScalarAll (seed_num), CV_FILLED, 8, 0);
    cvCircle (dsp_img, pt, seed_rad, cvScalarAll (255), 3, 8, 0);
    cvShowImage ("image", dsp_img);
  }
}
//ヒストグラム出力用のテスト用関数
void cvOutputHistgram(IplImage *img){
  uchar p[3];//画素値を取得する変数
  int x,y;//座標値
  int value;//画素値
  histgram_elements hist;//ヒストグラムの要素(初期化注意)
  //ヒストグラムを初期化
  for(value=0;value<COLOR_STEPS;value++){
    hist.r[value]=0;
    hist.g[value]=0;
    hist.b[value]=0;
  }
  
  //各種ファイル操作
  FILE *fp1,*fp2,*fp3;//それぞれ赤、緑、青を出力するヒストグラム
  fp1 = fopen("./histgram_red.csv", "w+");
  if(fp1 == NULL){
    printf("file1 error");
    exit(-1);
  }
  
  fp2 = fopen("./histgram_green.csv", "w+");
  if(fp2 == NULL){
    printf("file2 error");
    exit(-1);
  }
  
  fp3 = fopen("./histgram_blue.csv", "w+");
  if(fp3 == NULL){
    printf("file3 error");
    exit(-1);
  }
 
  for (y = 0; y < img->height; y++) {
    for (x = 0; x < img->width; x++) {
      p[0] = img->imageData[img->widthStep * y + x * 3];        // B
      p[1] = img->imageData[img->widthStep * y + x * 3 + 1];    // G
      p[2] = img->imageData[img->widthStep * y + x * 3 + 2];    // R
      
      for(value=0;value<COLOR_STEPS;value++){
	if(p[2] == value) hist.r[value]++;
	if(p[1] == value) hist.g[value]++;
	if(p[0] == value) hist.b[value]++;
      }
    }
  }
 
  //ファイル書き込み
  for(value=0;value<COLOR_STEPS;value++){
    fprintf(fp1,"%d,%d\n",value,hist.r[value]);
    fprintf(fp2,"%d,%d\n",value,hist.g[value]);
    fprintf(fp3,"%d,%d\n",value,hist.b[value]);
    /*
      printf("r_%d,%d\n",value,hist.r[value]);
      printf("g_%d,%d\n",value,hist.g[value]);
      printf("b_%d,%d\n",value,hist.b[value]);
    */
  }
 
  fclose(fp1);
  fclose(fp2);
  fclose(fp3);
}
 
 
 
/* メインプログラム */
int main (int argc, char **argv)
{
  int *idx, i, j;
  IplImage *src_img = 0, *dst_img = 0;
  //char imgpath[] = "Users/ryuichi64/Documents/recognize/build/Release/test.jpg";
  //char imgpath[] = "./test.jpg";
  // (2)画像の読み込み，マーカー画像の初期化，結果表示用画像領域の確保を行なう
  src_img = cvLoadImage (argv[1], CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  //src_img = cvLoadImage (imgpath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);
  
  if (src_img == NULL){
    printf("Cannot find image!\n");//読み込みエラーメッセージの表示
    exit (-1);
  }
  
  //dsp_img = cvCloneImage (src_img);
  //dst_img = cvCloneImage (src_img);
  
  cvOutputHistgram(src_img);
  //ヒストグラム機能確認の為、一旦ここで処理をストップする(2012.11.29)
  exit(-1);
  markers = cvCreateImage (cvGetSize (src_img), IPL_DEPTH_32S, 1);
  cvZero (markers);
  
  // (3)入力画像を表示しシードコンポーネント指定のためのマウスイベントを登録する
  cvNamedWindow ("image", CV_WINDOW_AUTOSIZE);
  cvShowImage ("image", src_img);
  cvSetMouseCallback ("image", on_mouse, 0);
  cvWaitKey (0);
  
  // (4)watershed分割を実行する  
  cvWatershed (src_img, markers);
  
  // (5)実行結果の画像中のwatershed境界（ピクセル値=-1）を結果表示用画像上に表示する
  
  for (i = 0; i < markers->height; i++) {
    for (j = 0; j < markers->width; j++) {
      idx = (int *) cvPtr2D (markers, i, j, NULL);
      if (*idx == -1)
	cvSet2D (dst_img, i, j, cvScalarAll (255));
    }
  }
  
  cvNamedWindow ("watershed transform", CV_WINDOW_AUTOSIZE);
  cvShowImage ("watershed transform", dst_img);
  cvWaitKey (0);
  
  cvDestroyWindow ("watershed transform");
  cvReleaseImage (&markers);
  cvReleaseImage (&dsp_img);
  cvReleaseImage (&src_img);
  cvReleaseImage (&dst_img);
  
  return 1;
}
