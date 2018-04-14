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
![](https://thumbnail0.baidupcs.com/thumbnail/15ec178c4b4f364b8fd16e3f24e48a9b?fid=1025299852-250528-235317209281432&time=1523674800&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-hLN9loEfByK8JFphTsAmtAkdZr4%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=2399965882319285122&dp-callid=0&size=c710_u400&quality=100&vuk=-&ft=video)    

**Experiment Procedure:** Feature point collection and distribution.    
![](https://thumbnail0.baidupcs.com/thumbnail/6264eb846c5e043ee9bbf6173c302dd4?fid=1025299852-250528-361409009198385&time=1523674800&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-MQ1cmuVLQ35ViYD%2F%2B1Mnr5Qj4KQ%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=2399998024769677400&dp-callid=0&size=c710_u400&quality=100&vuk=-&ft=video)    

**Comparison and Verification:** Comparison of Scale-space-based Uncertainty Models and Conventional Models.    
![](https://thumbnail0.baidupcs.com/thumbnail/d4ab982261f2dc8617ea80dc4cc1db8a?fid=1025299852-250528-333707749078386&time=1523674800&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-N0bo%2BoTyHh9EsCa4PWu2Pps6LZg%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=2400025285854955276&dp-callid=0&size=c710_u400&quality=100&vuk=-&ft=video)    

**Scale-space-based Uncertainty Model:** Uncertainty model distribution on the same scale.    
![](https://thumbnail0.baidupcs.com/thumbnail/4e5030006ca0bd92d78fd5bb712205e8?fid=1025299852-250528-855921119045371&time=1523674800&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-mP2JUZrUHDpwRA5HgbfBrOAE%2B%2Bs%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=2400059326141318457&dp-callid=0&size=c710_u400&quality=100&vuk=-&ft=video)    

**Uncertainty Model and Motion Mode:** Influence of Uncertainty Model on Different Motion Modes.    
![](https://thumbnail0.baidupcs.com/thumbnail/aa67c25587ff30b08f6c9ca39842b78b?fid=1025299852-250528-951569195468386&time=1523674800&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-IiaN0tWUawLetZ6hX7s9CDLXL1E%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=2400094399101137274&dp-callid=0&size=c710_u400&quality=100&vuk=-&ft=video)    

**Results in Public Datasets:** Experimental results on public datasets.    
![](https://thumbnail0.baidupcs.com/thumbnail/e195e937807625f86ae857b35baaa036?fid=1025299852-250528-550869568495772&time=1523674800&rt=sh&sign=FDTAER-DCb740ccc5511e5e8fedcff06b081203-y5EXmCut%2FKxHnQgZpNzVvBQI%2FS4%3D&expires=8h&chkv=0&chkbd=0&chkpc=&dp-logid=2400116244140367422&dp-callid=0&size=c710_u400&quality=100&vuk=-&ft=video)    

