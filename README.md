# ros版本哨兵自瞄代码
## 问题一 摄像头模式不define SAVERESULT 会在video目录生成打不开的垃圾文件
## 问题二 launch文件只调用了摄像头曝光参数 是否录像 usb摄像头ID 其他还是写死在config里不能做到实时调参 colormode的名字白起了..
## 问题三 检测效率同样参数检测率比普通版本代码低 容易丢失目标 猜测是ros发送数据会导致视频流缺失或者不连续什么的还有调用视频视频过快我做了延时
## 简介 
 ### fabu.cpp 打开摄像头或者视频设置曝光参数发布视频流到ros topic camera/image
 ### duqu.cpp duqu2.cpp 没什么用..
 ### daqu3.cpp 接受视频流作检测找到合适的装甲信息发送到ros topic pos_info
 ### currentyawpub 接受装甲信息和陀螺仪信息发送到串口合适的转向值
 ### roslaunch只启动fabu和daqu3节点 也可以加入currentyawpub 方便启用
## 注意事项
 ### opencvyi文件夹里面的所有cmakelists的include目录和参数文件目录都需要改我写的是自己的绝对目录
## 使用方法
 ### 1 把work放到主目录下 2cd work   3 catkin_make 4  cuicheng@cuicheng-N8xEJEK:~$ echo "source ~/work/devel/setup.bash" >> ~/.bashrc
 5  cuicheng@cuicheng-N8xEJEK:~$ source ~/.bashrc  6 roslaunch opencvyi colormode.launch 7 rosrun opencvyi pb
**以上**
 

 
