# features_uncertainty
Some procedures for measuring features' uncertainty in disparity image space

**Authors**: Zhenghua.Hou HIT 16S108281

**2017.11.9**: Finish the measurement of features' uncertainty at different scales.

**2017.11.18**: The number of matching points of each scale under different degrees of motion blur is counted.

# 1. License
Only Myself and My junior of laboratory.

# 2. Prerequisites
## C++11 or C++0x Compiler
I use the new thread and chrono functionalities of C++11.
## OpenCV
I use [OpenCV](http://opencv.org).
## PCL
I use [PCL](http://pointclouds.org).
## Eigen
I use [Eigen](http://eigen.tuxfamily.org).

# 4. Result
**Uncertainty Model Principle:** The difference between Euclidean distance and Mahalanobis distance.   
   
![](https://github.com/ZhenghuaHIT/features_uncertainty/raw/master/images/f1.png)    
  

**Experiment Procedure:** Feature point collection and distribution.    
    
![](https://github.com/ZhenghuaHIT/features_uncertainty/raw/master/images/f2.png) 

**Comparison and Verification:** Comparison of Scale-space-based Uncertainty Models and Conventional Models.  
  
![](https://github.com/ZhenghuaHIT/features_uncertainty/blob/master/images/f3.jpg)    
  

**Scale-space-based Uncertainty Model:** Uncertainty model distribution on the same scale.    
  
![](https://github.com/ZhenghuaHIT/features_uncertainty/raw/master/images/f4.png)    

  
**Uncertainty Model and Motion Mode:** Influence of Uncertainty Model on Different Motion Modes.  
  
![](https://github.com/ZhenghuaHIT/features_uncertainty/raw/master/images/f5.png)    
  

**Results in Public Datasets:** Experimental results on public datasets.  
  
![](https://github.com/ZhenghuaHIT/features_uncertainty/raw/master/images/f6.png)    



