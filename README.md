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
   
![](https://github.com/ZhenghuaHIT/features_uncertainty/master/images/f1.png)    
  

**Experiment Procedure:** Feature point collection and distribution.    
    
![](http://i1.bvimg.com/641465/b83bd969c23c067c.jpg) 

**Comparison and Verification:** Comparison of Scale-space-based Uncertainty Models and Conventional Models.  
  
![](http://i1.bvimg.com/641465/61f2dc8617ea80dc.jpg)    
  

**Scale-space-based Uncertainty Model:** Uncertainty model distribution on the same scale.    
  
![](http://i1.bvimg.com/641465/6ca0bd92d78fd5bb.png)    

  
**Uncertainty Model and Motion Mode:** Influence of Uncertainty Model on Different Motion Modes.  
  
![](http://i1.bvimg.com/641465/87ff30b08f6c9ca3.png)    
  

**Results in Public Datasets:** Experimental results on public datasets.  
  
![](http://i1.bvimg.com/641465/807625f86ae857b3.png)    



