#include <cv.h>
#include <highgui.h>
#include "ml.h"
#include <stdio.h>
#include"iostream.h"
float a[2500];
int main(int argc, char **argv)
{
int i, j, ii, jj;
int width= 50, height= 50;      /*����ͼ��ĳߴ��С*/
int image_dim= width* height;
int pimage_num= 41;         /*��������*/ 
int nimage_num= 61;        /*��������*/ 
int all_image_num= pimage_num+ nimage_num;
IplImage *img_org;
IplImage *sample_img;
//int res[all_image_num];
int res[102];
//float data[all_image_num* image_dim];
float data[255000];
CvMat data_mat, res_mat;
CvTermCriteria criteria;
CvSVM svm= CvSVM ();
CvSVMParams param;
char filename[65];







// (1) ��ȡ����������
for (i= 0; i< pimage_num; i++) {
    sprintf(filename, "�Ҷ�����\\������\\1 (%03d).jpg",i);     //�������������ļ�·��
   printf(filename);
    img_org= cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
   if(img_org!=NULL)
      cout<<"�ɹ�"<<endl;
   else
      cout<<"ʧ��"<<endl;
    sample_img= cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    cvResize(img_org, sample_img);
    cvSmooth(sample_img, sample_img, CV_GAUSSIAN, 3, 0, 0, 0);
   for (ii= 0; ii< height; ii++) {
     for (jj= 0; jj< width; jj++) {
        data[i* image_dim+ (ii* width) + jj] =
         float ((int) ((uchar) (sample_img->imageData[ii* sample_img->widthStep+ jj])) / 255.0);
     }
   }
    res[i] = 1;
}
    //��ȡ��������ͼ��Ⱥ����������ֵת����float�������͡�Ϊ�˷��㣬Ԥ����"positive/Ŀ¼��׼����������ͼ��ͼ������3λ��������������ʶ��000,001,002�������������������ȣ�����ȡ�ĸ�ͼ��ת����ͬһ�ߴ��С��28��30����Ϊ�˼���������Ӱ�죬��ͼ������ƽ��������Ȼ��Ϊ�����ø���������ֵ(�����ͼ����Ϊ�ȼ�ͼ�񱻶�ȡ)�����������������任�������顣��֮������һ��ͼ�����������(ͼ����Xͼ�񳤶�)��׼���˺�����ͼ��������ͬ��������"1"��ʾ���ô������������б���ֵ�����⻹ʹ��500������ͼ���������(����������������ͼ��û�в����)��
    //��OpenCV��ʵװ������haar-like�������������㷨��������������������ľ��Ⱥʹ����ٶȶ��ܲ�����Ȼ����ͼ����û��̫�����壬���ӻ�ȡ���������׳̶Ⱥ����̶ȿ��ǣ��˴���������ͼ�����ѧϰ��

// (2) ��ȡ����������
  j= i;
for (i= j; i< j+ nimage_num; i++) {
    sprintf(filename, "�Ҷ�����\\������\\1 (%03d).jpg", i-j);
    img_org= cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);
    sample_img= cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 1);
    cvResize(img_org, sample_img);
    cvSmooth(sample_img, sample_img, CV_GAUSSIAN, 3, 0, 0, 0);
   for (ii= 0; ii< height; ii++) {
     for (jj= 0; jj< width; jj++) {
        data[i* image_dim+ (ii* width) + jj] =
         float ((int) ((uchar) (sample_img->imageData[ii* sample_img->widthStep+ jj])) / 255.0);
     }
   }
    res[i] = 0;
}
//��ȡ��������ͼ��Ⱥ����������һ����������ת�������飬����"0"��ʾ���ø������������б���ֵ������ʹ��1000������ͼ��ĸ�����(ȫ�������������ͼ��)��

// (3)SVMѧϰ���ݺͲ����ĳ�ʼ��
  cvInitMatHeader(&data_mat, all_image_num, image_dim, CV_32FC1, data);
  cvInitMatHeader(&res_mat, all_image_num, 1, CV_32SC1, res);
  criteria= cvTermCriteria(CV_TERMCRIT_EPS, 1000, FLT_EPSILON);
  param= CvSVMParams (CvSVM::C_SVC, CvSVM::RBF, 10.0, 0.09, 1.0, 10.0, 0.5, 1.0, NULL, criteria);
//����ͼ�������ֵ������б�ֵ����������б任��Ϊ������ѧϰ�Բ������˳�ʼ����������ָ���˷ǳ��ʺϵĲ�����Ҳ���б�Ҫ�趨��Ӧ���ʵĲ�����

// (4)SVMѧϰ�����ݱ���
  svm.train(&data_mat, &res_mat, NULL, NULL, param);
  //svm.save("svm_image.xml");
  cvReleaseImage(&img_org);
  cvReleaseImage(&sample_img);



//������������������ֵ�ͱ�ָ���Ĳ���������svm.train()��ʽ����SVMѧϰ��������:������500��������1000����������: 28��30=840ά��ѧϰ��SVM�����ø���svm.save()������XML��ʽ���ļ����档���ҳ��ʼ���ֽ�����������Ϊ��ʹ�ñ�������ع��ܣ��б�Ҫ��OpenCVԴ�������޸ġ�
//������  ����ͼ�������ֵת��������������SVM�������(����)
//��ȡ����ѧϰ��SVM�����������ͼ���м�����塣

CvMat m;
//float a[2500];
float ret= -1.0;
float scale;
IplImage *src, *src_color, *src_tmp;
int sx, sy, tw, th;
int stepx= 3, stepy= 3;
double steps= 1.2;
int iterate;

// (1)ͼ��Ķ�ȡ
src_color= cvLoadImage("�����ͼ��//11.jpg", CV_LOAD_IMAGE_COLOR);
src= cvLoadImage("�����ͼ��//11.jpg", CV_LOAD_IMAGE_GRAYSCALE);
if (src == 0||src_color==0) 
{
      cout<<"fail fail  fail"<<endl;
}
else
cout<<"OKOKOKOKOKOKOKOKOKOK"<<endl;
// (2)SVM���ݵĶ�ȡ
  ///svm.load("svm_image.xml");
/*�Զ�ȡͼ���ÿ���ֽ��д��� */
cvInitMatHeader(&m, 1, image_dim, CV_32FC1, NULL);
  tw= src->width;
  th= src->height;

for (iterate= 0; iterate< 1; iterate++) {
// (3) ��Сͼ�񣬲��Ե�ǰͼ�񲿷������б任
    src_tmp= cvCreateImage(cvSize((int) (tw/ steps), (int) (th/ steps)), IPL_DEPTH_8U, 1);
    cvResize(src, src_tmp);
    tw= src_tmp->width;
    th= src_tmp->height;
   for (sy= 0; sy<= src_tmp->height- height; sy+= stepy) {
     for (sx= 0; sx<= src_tmp->width- width; sx+= stepx) {
       for (i= 0; i< height; i++) {
         for (j= 0; j< width; j++) {
            a[i* width+ j] =
             float ((int) ((uchar) (src_tmp->imageData[(i+ sy) * src_tmp->widthStep+ (j+ sx)])) / 255.0);
         cout<<j<<endl;
         }
       }
        cvSetData(&m, a, sizeof (float) * image_dim);
// (4)����SVM���ж��ͽ����ͼ
        ret= svm.predict(&m);
      cout<<"ret is "<<ret<<endl;
       if ((int) ret== 1) {
          scale= (float) src->width/ tw;
          cvRectangle(src_color, cvPoint((int) (sx* scale), (int) (sy* scale)),
                       cvPoint((int) ((sx+ width) * scale), (int) ((sy+ height) * scale)), CV_RGB(255, 0, 0), 2);
       }
     }
   }
    cvReleaseImage(&src_tmp);
}

// (5)��ʾ�����Ľ��ͼ��
  cvNamedWindow("svm_predict", CV_WINDOW_AUTOSIZE);
  cvShowImage("svm_predict", src_color);
  cvWaitKey(0);
  cvDestroyWindow("svm_predict");
  cvReleaseImage(&src);
  cvReleaseImage(&src_color);
  cvReleaseImage(&src_tmp);


return 0;
}