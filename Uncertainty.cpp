#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<mutex>
#include<string>
#include<thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>

#include <ORBextractor.h>

using namespace std;
using namespace cv;

int main (int argc, char** argv)
{
    string dataset_dir = argv[1];
    ifstream fin ( dataset_dir+"/associations.txt" );
    if ( !fin )
    {
        cout<<"please generate the associate file called associate.txt!"<<endl;
        return 1;
    }

    vector<string> rgb_files, depth_files;
    vector<double> rgb_times, depth_times;
    vector<vector<Point2f>> wholePoint;
    wholePoint.clear();
    rgb_files.clear();
    while ( !fin.eof() )
    {
        string rgb_time, rgb_file, depth_time, depth_file;
        fin>>rgb_time>>rgb_file>>depth_time>>depth_file;
        rgb_times.push_back ( atof ( rgb_time.c_str() ) );
        depth_times.push_back ( atof ( depth_time.c_str() ) );
        rgb_files.push_back ( dataset_dir+"/"+rgb_file );
        depth_files.push_back ( dataset_dir+"/"+depth_file );

        if ( fin.good() == false )
            break;
    }

    int mnp = rgb_files.size();

    vector<cv::Mat> wholeDescriptors;
    vector<vector<cv::KeyPoint>> wholeKeyPoint;
    for(int n=0;n<mnp-1;n++)
    {
        // 读取图片
        cv::Mat image;
        cv::Mat image_gray;
        image = cv::imread ( rgb_files[n] );

        if ( image.type() != CV_8UC1 && image.type() != CV_8UC3 )
        {
            // 图像类型不符合要求
            cout<<"请输入一张彩色图或灰度图."<<endl;
            return 0;
        }

        //1.彩色图转灰度图
        if(image.channels()==3)
            cvtColor(image, image_gray, CV_RGB2GRAY);
        else if(image.channels()==4)
            cvtColor(image, image_gray, CV_RGBA2GRAY);

        if ( image.data == nullptr )
        {
            cerr<<"文件"<<argv[1]<<"不存在."<<endl;
            return 0;
        }

        // 2.——加载 ORB parameters——
        int nFeatures = 1000; // 每一帧提取的特征点数
        float fScaleFactor = 1.2; // 图像建立金字塔时的变化尺度 1.2
        int nLevels = 8; // 尺度金字塔的层数 8
        int fIniThFAST = 20; // 提取fast特征点的默认阈值 20
        int fMinThFAST = 8; // 如果默认阈值提取不出足够fast特征点，则使用最小阈值 8

        // 特征提取器
        ORB_SLAM2::ORBextractor* mp_ORBextractor;
        mp_ORBextractor = new ORB_SLAM2::ORBextractor(nFeatures,fScaleFactor,nLevels,fIniThFAST,fMinThFAST);

        // 对图像提取特征点
        vector<cv::KeyPoint> mv_KP;
        cv::Mat m_Descriptors;
        (*mp_ORBextractor)(image_gray,cv::Mat(),mv_KP,m_Descriptors);
        int mn = mv_KP.size();
        wholeDescriptors.push_back(m_Descriptors);
        wholeKeyPoint.push_back(mv_KP);

        vector<Point2f> pointInterest(mn);//特征点，用以画在图像中
        pointInterest.clear();

        for(int i=0;i<mn;i++)
        {
            pointInterest[i].x= mv_KP[i].pt.x;//特征点在图像中横坐标         
            pointInterest[i].y = mv_KP[i].pt.y;//特征点在图像中纵坐标 
        }
        wholePoint.push_back( pointInterest);

        vector<cv::KeyPoint> mv_KP0,mv_KP1,mv_KP2,mv_KP3,mv_KP4,mv_KP5,mv_KP6,mv_KP7;

        for(int i=0;i<mn;i++)
        {
            int octave = mv_KP[i].octave;

            switch(octave)
            {
            case 0:
                mv_KP0.push_back(mv_KP[i]);
                break;
            case 1:
                mv_KP1.push_back(mv_KP[i]);
                break;
            case 2:
                mv_KP2.push_back(mv_KP[i]);
                break;
            case 3:
                mv_KP3.push_back(mv_KP[i]);
                break;
            case 4:
                mv_KP4.push_back(mv_KP[i]);
                break;
            case 5:
                mv_KP5.push_back(mv_KP[i]);
                break;
            case 6:
                mv_KP6.push_back(mv_KP[i]);
                break;
            case 7:
                mv_KP7.push_back(mv_KP[i]);
                break;
            default: cout<<"Keypoints' octave error!"<<endl;
            }

    //        cv::Point pointInterest;//特征点，用以画在图像中
    //        pointInterest.x = mv_KP[i].pt.x;//特征点在图像中横坐标
    //        pointInterest.y = mv_KP[i].pt.y;//特征点在图像中纵坐标
            cv::circle(image, pointInterest[i], mv_KP[i].octave*2+3, cv::Scalar(0, 255-15*mv_KP[i].octave, 0),1);

        }

    //    cout<<"size0 "<<mv_KP0.size()<<endl;
    //    cout<<"size1 "<<mv_KP1.size()<<endl;
    //    cout<<"size2 "<<mv_KP2.size()<<endl;
    //    cout<<"size3 "<<mv_KP3.size()<<endl;
    //    cout<<"size4 "<<mv_KP4.size()<<endl;
    //    cout<<"size5 "<<mv_KP5.size()<<endl;
    //    cout<<"size6 "<<mv_KP6.size()<<endl;
    //    cout<<"size7 "<<mv_KP7.size()<<endl;

      cv::imshow ( "image", image );
      //cv::waitKey (0);

      if(n<5)
      {
          string nstr = std::to_string(n);
          imwrite(nstr+".png",image);
      }
    }


    // 特征匹配
    vector<vector<double>> errorxx;
    vector<vector<double>> erroryy;
    for(int n=1;n<mnp-1;n++)
    {
    vector<DMatch> matches;
    BFMatcher matcher (NORM_HAMMING);
    matcher.match(wholeDescriptors[0], wholeDescriptors[n], matches);

    double min_dist=0, max_dist=0;//定义距离

    for (int i = 0; i < wholeDescriptors[0].rows; ++i)//遍历
    {
        double dist = matches[i].distance;
        if(dist<min_dist) min_dist = dist;
        if(dist>max_dist) max_dist = dist;
    }

    //当描述自之间的距离大于两倍的min_dist，即认为匹配有误，舍弃掉。
    //但是有时最小距离非常小，比如趋近于0了，所以这样就会导致min_dist到2*min_dist之间没有几个匹配。
    // 所以，在2*min_dist小于30的时候，就取30当上限值，小于30即可，不用2*min_dist这个值了
    std::vector<DMatch> good_matches;
    for (int j = 0; j < wholeDescriptors[0].rows; ++j)
    {
        if (matches[j].distance <= max(2*min_dist, 30.0))
            good_matches.push_back(matches[j]);
    }
    int ngoodmatches = good_matches.size();
    cout<<"good matches number is: "<<good_matches.size()<<endl;

    Mat img_goodmatch,image_match1,image_match2;//筛选后的匹配点图
    image_match1 = cv::imread ( rgb_files[0] );
    image_match2 = cv::imread ( rgb_files[n] );

    cv::drawMatches(image_match1, wholeKeyPoint[0], image_match2, wholeKeyPoint[n], good_matches, img_goodmatch);

    //imshow("筛选后的匹配点对", img_goodmatch);
    //waitKey(0);

    int nsize = wholeKeyPoint[0].size();

    vector<double> errorx(nsize,0);
    vector<double> errory(nsize,0);

    for(int k=0;k<ngoodmatches;k++)
    {
        int idexq = good_matches[k].queryIdx;
        int idext = good_matches[k].trainIdx;
        errorx[idexq] = wholeKeyPoint[n][idext].pt.x;
        errory[idexq] = wholeKeyPoint[n][idext].pt.y;
    }

    if(n==1)
    {
        vector<double> errorxt(nsize,0);
        vector<double> erroryt(nsize,0);
        for(int s=0;s<nsize;s++)
        {
            errorxt[s]=wholeKeyPoint[0][s].pt.x;
            erroryt[s]=wholeKeyPoint[0][s].pt.y;
        }
        errorxx.push_back(errorxt);
        erroryy.push_back(erroryt);
    }

    errorxx.push_back(errorx);
    erroryy.push_back(errory);
    }


   // 分尺度存储 kpo[哪个图片][哪个尺度][哪个特征点x/y] （1000个点，非当前尺度为-1）
   // 遍历图片
   vector<vector<vector<double>>> xkp;
   vector<vector<vector<double>>> ykp;
   xkp.clear();ykp.clear();

   for(int i=0;i<errorxx.size();i++)
   {
       // 遍历尺度
       vector<vector<double>> xkpo;
       vector<vector<double>> ykpo;
       xkpo.clear();ykpo.clear();
       for(int j=0;j<8;j++)
       {
           // 遍历点
           vector<double> xkpoi(errorxx[i].size(),0);
           vector<double> ykpoi(errorxx[i].size(),0);
           for(int k=0;k<errorxx[i].size();k++)
           {
               int octave = wholeKeyPoint[0][k].octave;
               if(octave == j)
               {
                   xkpoi[k]=errorxx[i][k]; // [i][j][k]
                   ykpoi[k]=erroryy[i][k];
               }

           }

          xkpo.push_back(xkpoi);
          ykpo.push_back(ykpoi);
       }
       xkp.push_back(xkpo);
       ykp.push_back(ykpo);
   }

   cout<<"xkp "<<xkp.size()<<endl;
   cout<<"xkp[] "<<xkp[2].size()<<endl;
   cout<<"xkp[][] "<<xkp[2][2].size()<<endl;

   // 求均值

   vector<vector<double>> kpmean;
   vector<vector<double>> ykpmean;// kpmean[尺度][特征点] kpmean kpmeanoct mean
   kpmean.clear();ykpmean.clear();
   for(int oct=0;oct<8;oct++)
   {
       vector<double> kpmeankp;
       vector<double> ykpmeankp;
       for(int nkp=0;nkp<xkp[0][oct].size();nkp++)
       {
           double sum = 0; double unzero = 0; double mean = 0;
           double ysum = 0; double ymean = 0;
           for(int file=0;file<mnp-1;file++)
           {
               sum = sum+ xkp[file][oct][nkp];
               ysum = ysum+ ykp[file][oct][nkp];

               if(xkp[file][oct][nkp]!=0)
               {
                   unzero = unzero +1;
               }

           }
           if(unzero>mnp-3)
           {
               mean = sum/unzero;
               ymean = ysum/unzero;
           }
           else
           {
               mean = 0;
               ymean = 0;
           }
           kpmeankp.push_back(mean);
           ykpmeankp.push_back(ymean);
       }
       kpmean.push_back(kpmeankp);
       ykpmean.push_back(ykpmeankp);
   }

   cout<<"kpmean "<<kpmean.size()<<endl;
   cout<<"kpmean[] "<<kpmean[3].size()<<endl;

   vector<vector<double>> kpvar;// kpmean[尺度][特征点] kpmean kpmeanoct mean
   kpvar.clear();
   for(int oct=0;oct<8;oct++)
   {
       vector<double> kpvarkp;
       for(int nkp=0;nkp<xkp[0][oct].size();nkp++)
       {
           double unzero = 0; double variance = 0;
           double rsum = 0;
           for(int file=0;file<mnp-1;file++)
           {
               //kpmean[oct][nkp]
               if(xkp[file][oct][nkp]!=0)
               {
                   unzero = unzero +1;
                   rsum = rsum + (xkp[file][oct][nkp]-kpmean[oct][nkp])*(xkp[file][oct][nkp]-kpmean[oct][nkp]);
               }
           }
           if(unzero>mnp-3)
           {
               double vt = rsum/unzero;
               if(vt<5)
                   variance = rsum/unzero;
               else
                   variance = 0;
           }

           else
               variance = 0;
           kpvarkp.push_back(variance);
       }
       kpvar.push_back(kpvarkp);
   }

   cout<<"kpvar "<<kpmean.size()<<endl;
   cout<<"kpvar[] "<<kpvar[0].size()<<endl;

//   for(int var=0;var<kpmean[6].size();var++)
//   {
//       if(kpmean[6][var]!=0)
//           cout<<"kpmean[][] "<<var<<"  "<<kpmean[6][var]<<endl;
//   }

   for(int var=0;var<kpvar[3].size();var++)
   {
       if(kpvar[3][var]!=0)
           cout<<"kpvar[][] "<<var<<"  "<<kpvar[3][var]<<endl;
   }








    return 0;
}
