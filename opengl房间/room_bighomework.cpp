#include<gl/glut.h>
#include<stdio.h>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>

GLuint drawcube, drawsphere, drawteapot;

//设置光照的开关
static bool lflag = GL_TRUE, rflag = GL_TRUE;

//控制平移旋转
static GLfloat sx = 0, sy = 0, sz = 0;
double rotate_x = 0.0;
double rotate_y = 0.0;

//整体房间大小
const int room_l = 400;
const int room_h = 120;
const int room_w = 150;

//纹理参数定义
#define checkImageWidth 64
#define checkImageHeight 64
static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLuint texName;

/*生成棋盘纹理*/
void makeCheckImage(void) {
    int i, j, c;
    for (i = 0; i < checkImageHeight; i++) {
        for (j = 0; j < checkImageWidth; j++) {
            c = ((((i & 0x8) == 0) ^ ((j & 0x8)) == 0)) * 255;
            checkImage[i][j][0] = (unsigned char)c;
            checkImage[i][j][1] = (unsigned char)c;
            checkImage[i][j][2] = (unsigned char)c;
            checkImage[i][j][3] = (unsigned char)c;
        }
    }
}

GLvoid DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
{
    GLfloat vertex[4];

    const GLfloat delta_angle = 2.0 * 3.1415926 / num_segments;
    glBegin(GL_TRIANGLE_FAN);

    vertex[0] = cx;
    vertex[1] = cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);

    //draw the vertex on the contour of the circle   
    for (int i = 0; i < num_segments; i++)
    {
        vertex[0] = cos(delta_angle * i) * r + cx;
        vertex[1] = sin(delta_angle * i) * r + cy;
        vertex[2] = cz;
        vertex[3] = 1.0;
        glVertex4fv(vertex);
    }

    vertex[0] = 1.0 * r + cx;
    vertex[1] = 0.0 * r + cy;
    vertex[2] = cz;
    vertex[3] = 1.0;
    glVertex4fv(vertex);
    glEnd();
}

void mySolidCylinder(GLUquadric* quad, GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks)
{
    glColor3f(84.0 / 255, 0.0, 125.0 / 255.0);
    gluCylinder(quad, base, top, height, slices, stacks);
    //top   
    DrawCircleArea(0.0, 0.0, height, top, slices);
    //base   
    DrawCircleArea(0.0, 0.0, 0.0, base, slices);
}

void drawRoomWall() {

    //绘制后侧墙壁；
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.5, 0.5, 0.5, 0.0);
    glTranslatef(0, 0, -75);
    glScalef(400, 120, 1);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制上方墙壁；
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.5, 0.5, 0.5, 0.0);
    glTranslatef(0, 60, 0);
    glScalef(400, 1, 150);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制中间墙壁；  //玻璃
    glPushMatrix();
    glEnable(GL_BLEND);       //启用颜色混合，例如实现半透明效果
    glEnable(GL_DEPTH_TEST); //启用 深度测试（根据坐标的远近自动隐藏被遮住的图形）
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//使用源颜色的alpha值来作为因子，得到的颜色混和效果

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.4, 0.4, 0.4, 0.85);
    glTranslatef(-75, 0, -15);
    glScalef(1, 120, 120);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glDisable(GL_BLEND);

    //绘制左侧墙面；
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.5, 0.5, 0.5, 0);
    glTranslatef(-200, 0, 0);
    glScalef(1, 120, 150);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制右侧墙面；
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.5, 0.5, 0.5, 0.0);
    glTranslatef(200, 0, 0);
    glScalef(1, 120, 150);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void drawBedRoom() {
    //绘制床  
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(188.0 / 255.0, 172.0 / 255.0, 77.0 / 255.0, 0.0);
    glTranslatef(61 / 2 + 0.5 - room_l / 2, 21 / 2 - room_h / 2, 88 / 2 - room_w / 2);
    glScalef(61, 21, 88);
    glCallList(drawcube);
    glColor4f(200.0 / 255.0, 97.0 / 255.0, 20.0 / 255.0, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制枕头
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(158.0 / 255.0, 77.0 / 255.0, 77.0 / 255.0, 0.0);
    glTranslatef(61 / 2 + 0.5 - room_l / 2 + 1, 21 / 2 - room_h / 2 + 15, 88 / 2 - room_w / 2 - 30);
    glScalef(45, 8, 18);
    glCallList(drawcube);
    glColor4f(200.0 / 255.0, 97.0 / 255.0, 20.0 / 255.0, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制柜子
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(64.0 / 255.0, 44.0 / 255.0, 16.0 / 255.0, 0.0);
    glTranslatef(-85, -20, -35);
    glScalef(20, 80, 80);
    glCallList(drawcube);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制窗
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.4, 0.4, 0.8, 0);
    glTranslatef(-200, 0, 20);
    glScalef(2, 30, 50);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void drawGround() {
    //绘制地面；
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(34.0 / 255.0, 29.0 / 255.0, 15.0 / 255.0, 0.0);
    glTranslatef(0, -60, 0);
    glScalef(400, 1, 150);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -69, 0);
    /*启动映射*/
    glEnable(GL_TEXTURE_2D);
    /*说明映射方式*/
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    // glBindTexture(GL_TEXTURE_2D, texName);
    glBegin(GL_QUADS);
    /*设置纹理坐标*/
    glTexCoord2f(0, 0); glVertex3f(95, 10, 75);
    glTexCoord2f(0, 1); glVertex3f(95, 10, -75);
    glTexCoord2f(1, 1); glVertex3f(200, 10, -75);
    glTexCoord2f(1, 0); glVertex3f(200, 10, 75);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.5, 0.5, 0.5, 0.0);
    glTranslatef(0, -60, 0);
    glScalef(400, 1, 150);
    glCallList(drawcube);
    glColor4f(0.8, 0.8, 0.8, 0.0);
    glutWireCube(1);

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);

    //装饰地毯
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor3f(6.0 / 255.0, 66.0 / 255.0, 47.0 / 255.0);
    glTranslatef(15, -40, 0);

    glBegin(GL_LINES);

    float y = -19.5, z1 = 50;
    for (float x1 = -70; x1 < 70; ++x1)
    {
        glVertex3f(x1, y, z1);
        glVertex3f(x1, y, -z1);

    }
    float x2 = 70;
    for (float z2 = -50; z2 < 50; ++z2)
    {
        glVertex3f(x2, y, z2);
        glVertex3f(-x2, y, z2);

    }

    glEnd();
    glPopMatrix();
}

void drawLivingRoom() {
    //电视
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0, 0, 0, 0);
    glTranslatef(-73, 0, 11);
    glScalef(1, 40, 50);
    glCallList(drawcube);
    glColor4f(0.7, 0.7, 0.7, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制四个桌腿：
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(64.0 / 255.0, 44.0 / 255.0, 16.0 / 255.0, 0.0);
    glTranslatef(20, -50, 0);
    glScalef(1, 20, 1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(64.0 / 255.0, 44.0 / 255.0, 16.0 / 255.0, 0.0);
    glTranslatef(-20, -50, 0);
    glScalef(1, 20, 1);
    glCallList(drawcube);

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(64.0 / 255.0, 44.0 / 255.0, 16.0 / 255.0, 0.0);
    glTranslatef(-20, -50, 40);
    glScalef(1, 20, 1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(64.0 / 255.0, 44.0 / 255.0, 16.0 / 255.0, 0.0);
    glTranslatef(20, -50, 40);
    glScalef(1, 20, 1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //画出桌面：
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(64.0 / 255.0, 44.0 / 255.0, 16.0 / 255.0, 0.0);
    glTranslatef(0, -40, 20);
    glScalef(50, 1, 50);
    glCallList(drawcube);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glutWireCube(1);
    // glFlush();
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制沙发
    glPushMatrix();// 右边第一级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(200.0 / 255.0, 97.0 / 255.0, 20.0 / 255.0, 0.0);
    glTranslatef(51, -53, 20);
    glScalef(20, 14, 45);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//右边第二级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(200.0 / 255.0, 97.0 / 255.0, 20.0 / 255.0, 0.0);
    glTranslatef(57, -41, 20);
    glScalef(8, 10, 45);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();// 后边第一级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(200.0 / 255.0, 97.0 / 255.0, 20.0 / 255.0, 0.0);
    glTranslatef(0, -53, -35);
    glScalef(50, 14, 20);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//后边第二级台阶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(200.0 / 255.0, 97.0 / 255.0, 20.0 / 255.0, 0.0);
    glTranslatef(0, -41, -41);
    glScalef(50, 10, 8);
    glCallList(drawcube);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//茶壶
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.78, 38.0 / 255.0, 88.0 / 255.0, 0.0);
    glTranslatef(0, -32, 10);
    glScalef(5, 10, 5);
    glCallList(drawteapot);

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    GLUquadricObj* pObj;
    glPushMatrix();//第一个杯子  左边
    pObj = gluNewQuadric();
    gluQuadricNormals(pObj, GLU_SMOOTH);
    glTranslatef(-8, -34, 18);
    glRotatef(90, 1, 0, 0);
    gluCylinder(pObj, 1.50f, 1.0f, 5.0f, 26, 13);
    glPopMatrix();

    glPushMatrix();  //第2个杯子 右边
    glTranslatef(8, -34, 18);
    glRotatef(90, 1, 0, 0);
    gluCylinder(pObj, 1.50f, 1.0f, 5.0f, 26, 13);
    glPopMatrix();


    glPushMatrix();  //桌面上垫圈
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.21, 0.21, 0.21, 0.0);

    pObj = gluNewQuadric();
    //gluQuadricDrawStyle(pObj3,GLU_LINE);
    glTranslatef(-8, -39, 18);
    glRotatef(90, 1, 0, 0);
    gluDisk(pObj, 0.50f, 3.0f, 10, 10);
    glPopMatrix();

    glPushMatrix();  //桌面上垫圈

    pObj = gluNewQuadric();
    glTranslatef(8, -39, 18);
    glRotatef(90, 1, 0, 0);
    gluDisk(pObj, 0.50f, 3.0f, 30, 30);
    glPopMatrix();

    glPushMatrix();//角落球体
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.30, 0.11, 0.15, 0.8);
    glTranslatef(-65, -55, -60);
    glScalef(5, 5, 5);
    glCallList(drawsphere);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//角落圆环
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    //glColor4f(0.90, 0.01, 0.95, 0.8);
    glColor4f(1, 0, 0, 0);
    glTranslatef(-52, -53, -60);
    //glRotatef(1,0,0, 45);
    glutSolidTorus(1, 6, 20, 20);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制钟
    GLUquadricObj* obj;
    glPushMatrix(); //地面上放置 圆锥体；
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    obj = gluNewQuadric();
    gluQuadricDrawStyle(obj, GLU_LINE);
    glTranslatef(40, 20, -75);
    glRotatef(90, 0, 0, 1);
    mySolidCylinder(obj, 14.0, 14.0, 2.0, 32.0, 5.0);
    glPopMatrix();

    //钟的指针
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glTranslatef(40, 25, -73);
    glScalef(1, 12, 1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //钟的指针
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.1, 0.1, 0.1, 0.0);
    glTranslatef(43, 17, -73);
    glRotatef(60, 0, 0, 1);
    glScalef(2, 8, 1);
    glCallList(drawcube);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

}

void drawCanteen() {
    //绘制右侧柜子
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(40.00 / 255.00, 100.00 / 255.00, 1, 0);
    glTranslatef(190, -40, -35);
    glScalef(20, 40, 80);
    glCallList(drawcube);
    glColor4f(0.2, 0.2, 0.2, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    //绘制后侧柜子
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(40.00 / 255.00, 100.00 / 255.00, 1, 0);
    glTranslatef(140, -40, -65);
    glScalef(80, 40, 20);
    glCallList(drawcube);
    glColor4f(0.2, 0.2, 0.2, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    glPushMatrix();//鸡蛋
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(1.00, 0.91, 0.05, 0.8);
    glTranslatef(120, -15, -61);
    glScalef(2, 5, 2);
    glCallList(drawsphere);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

    GLUquadricObj* pObj;
    glPushMatrix(); //地面上放置 圆锥体；
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.01, 0.01, 0.81, 0.0);
    pObj = gluNewQuadric();
    gluQuadricDrawStyle(pObj, GLU_LINE);
    glTranslatef(113, -10, -64);
    glRotatef(90, 1, 0, 0);
    gluCylinder(pObj, 0.0f, 2.0f, 10.0f, 26, 13);
    glPopMatrix();

    //锅
    GLUquadricObj* obj;
    glPushMatrix(); //地面上放置 圆锥体；
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(1.01, 0.01, 0.81, 0.0);
    obj = gluNewQuadric();
    glTranslatef(150, -14, -65);
    glRotatef(90, 1, 0, 0);
    gluCylinder(obj, 8.0, 8.0, 6.0, 32.0, 5.0);
    glPopMatrix();

    //绘制窗
    glPushMatrix();
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT);
    glColor4f(0.4, 0.4, 0.8, 0);
    glTranslatef(160, 25, -75);
    glScalef(50, 28, 2);
    glCallList(drawcube);
    glColor4f(0.7, 0.7, 0.7, 0.0);
    glutWireCube(1);
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();

}

void drawLignt() {
    glPushMatrix();//角落圆环
    glTranslatef(60, 55, 0);
    glRotatef(90, 1, 0, 0);
    glColor4f(0.90, 0.9, 0.3, 0);
    glutSolidTorus(6, 11, 20, 20);
    glPopMatrix();
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90, w / h, 0.5, 800);  //透视效果
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(-0, 10, 130, 0, 0, 0, 0, 10, 0);//设置观察点
}

void init() {
    glClearColor(0, 0, 0, 0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);

    makeCheckImage();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    /*定义纹理*/
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    /*控制滤波*/
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

    //画出正方体显示列表
    drawcube = glGenLists(1);
    glNewList(drawcube, GL_COMPILE);
    glutSolidCube(1);
    glEndList();
    //画出球体显示列表
    drawsphere = glGenLists(1);
    glNewList(drawsphere, GL_COMPILE);
    glutSolidSphere(1, 20, 20);
    glEndList();
    //画出茶壶显示列表
    drawteapot = glGenLists(1);
    glNewList(drawteapot, GL_COMPILE);
    glutSolidTeapot(1);
    glEndList();

    //设置普通灯光照0位置及参数；
    GLfloat position0[] = { 60,30,40,1 };
    GLfloat light0s[] = { 0.10,0.10,0.10,0.0 };
    GLfloat light0d[] = { 0.6,0.7,0.7 };
    GLfloat light0a[] = { 0.9,0.9,0.9 };

    glLightfv(GL_LIGHT0, GL_POSITION, position0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0s);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0d);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0a);

    //设置探照灯光照1位置及参数
    GLfloat position1[] = { -60,60,0,1 };
    GLfloat light1s[] = { 1.0,1.0,1.0,1.0 };
    GLfloat light1d[] = { 0.06,0.1,0.1 };
    GLfloat light1a[] = { 0.5,0.5,0.5 };
    GLfloat direction[] = { 0,-60,0,1 };
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 30.0);
    glLightfv(GL_LIGHT1, GL_POSITION, position1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light1s);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1d);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1a);

    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void SpecialKeys(int key, int x, int y)
{
    if (key == GLUT_KEY_UP) {
        sz += 5.50f;
        glutPostRedisplay();
    }
    if (key == GLUT_KEY_DOWN) {
        sz -= 5.50f;
        glutPostRedisplay();
    }
    if (key == GLUT_KEY_LEFT) {
        sx += 5.50f;
        glutPostRedisplay();
    }
    if (key == GLUT_KEY_RIGHT) {
        sx -= 5.50f;
        glutPostRedisplay();
    }
}

void processNormalKeys(unsigned char key, int x, int y) {
    switch (key)
    {
    case 'w':
        rotate_x -= 5;
        break;
    case 's':
        rotate_x += 5;
        break;
    case 'a':
        rotate_y -= 1;
        break;
    case 'd':
        rotate_y += 1;
        break;
    case 'o':
        lflag = !lflag;
        break;
    default:
        break;
    }
    // 刷新
    glutPostRedisplay();
}

void draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (lflag)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    if (rflag)
        glEnable(GL_LIGHT1);

    glPushMatrix();
    glRotatef(rotate_x, 1.0, 0.0, 0.0);
    glRotatef(rotate_y, 0.0, 1.0, 0.0);
    glTranslatef(sx, sy, sz);

    drawBedRoom();
    drawGround();
    drawLivingRoom();
    drawCanteen();
    drawLignt();
    drawRoomWall();

    glPopMatrix();

    glutSwapBuffers();
}

void main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("opengl room");
    init();
    glutReshapeFunc(reshape);
    glutSpecialFunc(SpecialKeys);
    glutKeyboardFunc(processNormalKeys);
    glutDisplayFunc(draw);
    glutMainLoop();
}
