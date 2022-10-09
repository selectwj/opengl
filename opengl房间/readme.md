opengl卧室

项目简介：
本项目基于OpenGL实现，使用gl/glut.h库。
具体实现内容包含纹理映射、光照、材质、控制浏览，并显示墙壁、玻璃、卧室、客厅、厨房中二十余种物体。
关于按键控制，实现up键前进，down键后退，left左移，right右移，
w视角上转，s视角下转，a左转，d右转，
o关灯或开灯。

1. void makeCheckImage(void) //生成棋盘纹理
2. GLvoid DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
和void mySolidCylinder(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
用于生成实心圆锥或圆柱
3. void drawRoomWall(）//画四周墙壁，包含后方、上方、中间玻璃墙面、左方、右方
4. void drawBedRoom() //画卧室，包含床、枕头、柜子、窗户
5. void drawGround() //绘制地面、地毯、纹理映射
6. void drawLivingRoom() //画客厅，包含电视、桌子、桌腿、沙发、茶壶、杯子、垫圈、角落球体、圆环、钟、钟的指针
7. void drawCanteen() //画厨房，包含右侧柜子、后侧柜子、鸡蛋、圆锥体、锅、窗户
8. void drawLignt() //画灯
9. void reshape(int w, int h) //设置透视和观察点
10. void init() //初始化，包括纹理的初始化、物体显示列表、光源初始化
11. void SpecialKeys(int key, int x, int y) //方向键的事件，up键前进，down键后退，left左移，right右移
12. void processNormalKeys(unsigned char key, int x, int y) 
//一些普通键的事件，w视角上转，s视角下转，a左转，d右转，o开灯/关灯
13. void draw() //初始化显示、调用绘制的函数
14. void main(int argc, char* argv[]) //主函数，重复显示，调用上述函数