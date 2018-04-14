#include<iostream>
#include<algorithm>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<mutex>
#include<string>
#include<thread>
#include <sys/time.h>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <Eigen/Geometry>

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

    vector<cv::Mat> wholeDescriptors;
    vector<vector<cv::KeyPoint>> wholeKeyPoint;
    int mnp = rgb_files.size();


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

//        cout<<"size0 "<<mv_KP0.size()<<endl;
//        cout<<"size1 "<<mv_KP1.size()<<endl;
//        cout<<"size2 "<<mv_KP2.size()<<endl;
//        cout<<"size3 "<<mv_KP3.size()<<endl;
//        cout<<"size4 "<<mv_KP4.size()<<endl;
//        cout<<"size5 "<<mv_KP5.size()<<endl;
//        cout<<"size6 "<<mv_KP6.size()<<endl;
//        cout<<"size7 "<<mv_KP7.size()<<endl;

//      cv::imshow ( "image", image );
//      cv::waitKey (0);

//      if(n<5)
//      {
//          string nstr = std::to_string(n);
//          imwrite(nstr+".png",image);
//      }
    }


    // 特征匹配
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

    Mat image_match1,image_match2;//筛选后的匹配点图
    image_match1 = cv::imread ( rgb_files[0] );
    image_match2 = cv::imread ( rgb_files[n] );

    //cv::drawMatches(image_match1, wholeKeyPoint[0], image_match2, wholeKeyPoint[n], good_matches, img_goodmatch);

    for (int im=0;im<wholeKeyPoint[n].size();im++)
    {
        Point2f pointInterest;
        pointInterest.x=wholeKeyPoint[n][im].pt.x;
        pointInterest.y=wholeKeyPoint[n][im].pt.y;

        cv::circle(image_match2, pointInterest,6, cv::Scalar(255, 255, 0),-1);
    }

    vector<cv::KeyPoint> mv_KP0,mv_KP1,mv_KP2,mv_KP3,mv_KP4,mv_KP5,mv_KP6,mv_KP7;
    for (int im=0;im<ngoodmatches;im++)
    {
        int idext = good_matches[im].trainIdx;
        Point2f pointInterest;
        pointInterest.x=wholeKeyPoint[n][idext].pt.x;
        pointInterest.y=wholeKeyPoint[n][idext].pt.y;

        int octave=wholeKeyPoint[n][idext].octave;
        switch(octave)
        {
        case 0:
            mv_KP0.push_back(wholeKeyPoint[n][idext]);
            break;
        case 1:
            mv_KP1.push_back(wholeKeyPoint[n][idext]);
            break;
        case 2:
            mv_KP2.push_back(wholeKeyPoint[n][idext]);
            break;
        case 3:
            mv_KP3.push_back(wholeKeyPoint[n][idext]);
            break;
        case 4:
            mv_KP4.push_back(wholeKeyPoint[n][idext]);
            break;
        case 5:
            mv_KP5.push_back(wholeKeyPoint[n][idext]);
            break;
        case 6:
            mv_KP6.push_back(wholeKeyPoint[n][idext]);
            break;
        case 7:
            mv_KP7.push_back(wholeKeyPoint[n][idext]);
            break;
        default: cout<<"Keypoints' octave error!"<<endl;
        }

        cv::circle(image_match2, pointInterest,6, cv::Scalar(0, 0, 255),-1);
    }

            cout<<"size0 "<<(float)mv_KP0.size()/(float)ngoodmatches<<endl;
            cout<<"size1 "<<(float)mv_KP1.size()/(float)ngoodmatches<<endl;
            cout<<"size2 "<<(float)mv_KP2.size()/(float)ngoodmatches<<endl;
            cout<<"size3 "<<(float)mv_KP3.size()/(float)ngoodmatches<<endl;
            cout<<"size4 "<<(float)mv_KP4.size()/(float)ngoodmatches<<endl;
            cout<<"size5 "<<(float)mv_KP5.size()/(float)ngoodmatches<<endl;
            cout<<"size6 "<<(float)mv_KP6.size()/(float)ngoodmatches<<endl;
            cout<<"size7 "<<(float)mv_KP7.size()/(float)ngoodmatches<<endl;

    string nstr = std::to_string(n);
    imwrite(nstr+".png",image_match2);
cout<<"fs "<<endl;
    //imshow("筛选后的匹配点对", image_match2);
    waitKey(0);


    int a[]={4,3,2,7,8,2,3,1};
    int n=sizeof(a);
    int b[n];
    for(int i=0;i<n;i++)
    {
        for(int j=1;j<n-i;j++)
        {
            if(a[i]==a[i+j])
            {
                b[j]=a[i];
                continue;
            }
        }
    }
    for(int k=0;k<sizeof(b);k++)
    {
        cout<<b[k]<<endl;
    }

   cout<<"b[k]"<<endl;





    }






    return 0;
}
