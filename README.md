#### Question 3:

1.	记录并报告所有Rover做过的所有路线，这是为了能够最终计算出Rover探索过的区域，以及确认还有哪些区域没有被探索过

2.	随着图像技术的发展，新型NBRover能够通过图像发现周边是否有障碍物（包括Rover或者非Rover），为此NBRover除了报告障碍物位置以外，不再停止于发现障碍物的位置，而是会试图选择一条最短的路绕过障碍物，继续原来规划的线路。
例子：

    指令计划路线：1NB – 2NB – 3NB 
                            |
                        4NB
    由于第三个点有障碍物，所以NB实际行动路线是：1NB –2NB   Obstacle
                                            |
                                        3NB- 4NB 

#### Question 2:

1. Implement wrapping from one edge of the grid to another. (planets are spheres after all)

搜索区域可以认为是一个球形，也就是说从西边出界相当于从东边进入；从北边出界相当于从南边进入 

2. Implement obstacle detection before each move to a new square. If a given sequence of commands encounters an obstacle, the rover moves up to the last possible point and reports the obstacle.

当漫游者发现下一步存在障碍物，就停止在当前位置，并报告障碍物的位置。


#### Question 1:

一个班的机器人漫游者被NASA发送到火星，准备登陆一个高原。这是一个矩形的高原，漫游者要保证照相机

可以完整的将周围的地形影像拍下来，传回地球。
 
 
漫游者的位置由x，y和一个表示方向的字母的组合表示。为了方便控制行驶，高原被分割成网格。例如：0,0，N

表示漫游者处于左下角，面朝北。

 
为了控制漫游者，NASA发送一串简单字符串，由‘L’，‘R’和‘M’组成。‘L'和’R'分别表示原地向左或者向右旋转

90度。‘M’表示在网格上前进一步，并保持朝向。
 
假设方格中从（x, y）到（x,y+1）表示向北移动

 
输入：

第一行输入高原右上方坐标，假设左下方坐标为0,0
 
剩下的输入是相关漫游者部署的信息。每两行描述一个漫游者：第一行表示漫游者的位置，第二行是告诉漫游者
探索高原的指令序列。
 
位置由分开的两个数字和一个字母组成，分别表示x轴，y轴和漫游者的朝向
 
每个漫游者都会序列化的完成动作，也就是说，第二个漫游者不会在第一个漫游者完成移动之前开始移动。
 
    Output:
    输出每个漫游者最终的坐标和朝向
    
    Test Input：
    5 5
    1 2 N
    LMLMLMLMM
    3 3 E
    MMRMMRMRRM
    
    Expected Output:
    1 3 N
    5 1 E
