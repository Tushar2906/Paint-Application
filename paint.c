#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int H, W;

int brushSize=5;
char keystroke, input[1024]="", temp[1024]="";
int X_coord=0, line_on=0;
char ctool[100]="IDLE";

int textx, texty;

int r=0,g=0,b=0;

unsigned char color[3];

int ml_x = 0, ml_y = 0;
int mc_x = 0, mc_y = 0;

float m;

int point_no = 0;
int X1, Y1, X2, Y2, c;
int radius;

struct Mouse {
	int x;
	int y;
	int lmb;
	int mmb;
	int rmb;
	int xpress;
	int ypress;
};
typedef struct Mouse Mouse;

int tool=0;

Mouse TheMouse = {0,0,0,0,0};

typedef void (*ButtonCallback)();

struct Button {
	int   x;
	int   y;
	int   w;
	int   h;
	int state;
	int highlighted;
	char* label;
	char* discription;
	ButtonCallback callbackFunction;
};
typedef struct Button Button;

void clrscr(){
	X1=0;
	Y1=0;
	point_no=-1;
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

void freedraw(){
	if (tool!=1){
		tool=1;
		memset(ctool, 0, sizeof(ctool));
		strcpy(ctool, "PENCIL");
	}
}

void text(){
	if (tool!=2){
		tool=2;
		memset(ctool, 0, sizeof(ctool));
		strcpy(ctool, "TEXT");
	}
}

void line(){
	if (tool!=3){
		tool=3;
		memset(ctool, 0, sizeof(ctool));
		strcpy(ctool, "LINE");
	}
}

void erase(){
	if (tool!=4){
		tool=4;
		memset(ctool, 0, sizeof(ctool));
		strcpy(ctool, "ERASER");
	}
}

void drawcir(){
	if (tool!=5){
		tool=5;
		memset(ctool, 0, sizeof(ctool));
		strcpy(ctool, "CIRCLE");
	}
}

void drawrect(){
	if (tool!=6){
		tool=6;
		memset(ctool, 0, sizeof(ctool));
		strcpy(ctool, "RECTANGLE");
	}
}

void incpoint(){
	if(brushSize<15)
		brushSize++;
}

void decpoint(){
	if(brushSize>1)
		brushSize--;
}

Button MyButton0 = {10, 40, 110, 25,0,0,"CLEAR", "Clears the whole screen", clrscr};
Button MyButton1 = {10, 70, 50,25, 0,0, "PENCIL", "", freedraw};
Button MyButton2 = {10, 100, 50,25, 0,0, "TEXT", "", text};
Button MyButton3 = {70, 100, 50,25, 0,0, "LINE", "", line};
Button MyButton4 = {70, 70, 50,25, 0,0, "ERASER", "", erase};
Button MyButton5 = {10, 130, 50,25, 0,0, "CIRCLE", "", drawcir};
Button MyButton6 = {70, 130, 50,25, 0,0, "RECTANGLE", "", drawrect};
Button MyButton7 = {10, 465, 50,25, 0,0, "+", "", incpoint};
Button MyButton8 = {70, 465, 50,25, 0,0, "-", "", decpoint};

void show(int x, int y, char *string, void *font){
	int len, i;
	glRasterPos2f(x, y);
	len = (int) strlen(string);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(font, string[i]);
}

void drawLine(int x, int y, int r1, int g1, int b1, int axis){
	glBegin(GL_POINTS);
    glColor3f(r1, g1, b1);
    glVertex2f(x, y);
		if(axis==1){
			for(int i=0;i<100;i++){
				glVertex2f(x+i,y);
			}
		}else if(axis==2){
			for(int i=0;i<100;i++){
				glVertex2f(x,y+i);
			}
		}else if(axis==3){
			for(int i=0;i<100;i++){
				glVertex2f(x-i,y);
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
        glEnd();
}

void draw_pixel(int x, int y) {
	glBegin(GL_POINTS);
		glColor3ub(r,g,b);
		glVertex2i(x, y);
		//glColor3f(1.0,1.0,1.0);
	glEnd();
}

void circle(int ax, int bx,int ay, int by){
	int cx = (ax+bx)/2;
	int cy = (ay+by)/2;
	int dx = bx-cx;
	int dy = by-cy;
	int g = dx*dx + dy*dy;
	radius = sqrt(g);
	int x = radius, y = 0;
	float p = 1-radius;
	while(x>y){
		y++;
		if(p<=0){
			p+=2*y+1;
		}else{
			x--;
			p+=2*y+1-2*x;
		}
		if(x<y){
			break;
		}

		if(x+cx>130)
			draw_pixel(x+cx,(y+cy));
		if(-x+cx>130)
			draw_pixel(-x+cx,(y+cy));
		if(x+cx>130)
			draw_pixel(x+cx,(-y+cy));
		if(-x+cx>130)
			draw_pixel(-x+cx,(-y+cy));
		if(y+cx>130)
			draw_pixel(y+cx,(x+cy));
		if(-y+cx>130)
			draw_pixel(-y+cx,(x+cy));
		if(y+cx>130)
			draw_pixel(y+cx,(-x+cy));
		if(-y+cx>130)
			draw_pixel(-y+cx,(-x+cy));
	}
}

void draw_line(int x1, int x2, int y1, int y2) {
	int dx, dy, i, e;
	int incx, incy, inc1, inc2;
	int x,y;

	dx = x2-x1;
	dy = y2-y1;
	if(dx!=0&&dy!=0){
		if (dx < 0)
			dx = -dx;
		if (dy < 0)
			dy = -dy;
		incx = 1;
		if (x2 < x1)
			incx = -1;
		incy = 1;
    		if (y2 < y1)
			incy = -1;
    		x = x1;
		y = y1;

	if (dx > dy) {
  	draw_pixel(x, y);
    e = 2 * dy-dx;
    inc1 = 2*(dy-dx);
    inc2 = 2*dy;
    for (i=0; i<dx; i++) {
      if (e >= 0) {
				y += incy;
        e += inc1;
      }else
      	e += inc2;
      x += incx;
      draw_pixel(x, y);
    }
	}else {
  	draw_pixel(x, y);
    e = 2*dx-dy;
  	inc1 = 2*(dx-dy);
  	inc2 = 2*dx;
    for (i=0; i<dy; i++) {
    	if (e >= 0) {
    		x += incx;
       	e += inc1;
    	}else
    		e += inc2;
    		y += incy;
      	draw_pixel(x, y);
    	}
  	}
	}else if(dx==0){				//For slope = infinite
		m=1.0;
		int pdy;
		if (dy<0){
			pdy=-dy;
			for(int i=0;i<=pdy;i++)
				draw_pixel(x1,y1-i);
		}else{
			pdy=dy;
			for(int i=0;i<=pdy;i++)
				draw_pixel(x1,y1+i);
		}
	}else if(dy==0){				//For slope = 0
		int pdx;
		if (dx<0){
			pdx=-dx;
			for(int i=0;i<=pdx;i++)
				draw_pixel(x1-i,y1);
		}else{
			pdx=dx;
			for(int i=0;i<=pdx;i++)
				draw_pixel(x1+i,y1);
		}
	}
}

void draw_rect(int x1, int x2, int y1, int y2) {
	glColor3ub(r,g,b);
	glBegin(GL_LINE_LOOP);
	glPointSize(brushSize);

	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);
	glEnd();
	glFlush();
}

int ButtonClickTest(Button* b,int x,int y) {
	if( b) {
	    if( x > b->x && x < b->x+b->w && y > b->y && y < b->y+b->h ) {
				return 1;
	    }
	}
	return 0;
}

void ButtonRelease(Button *b,int x,int y){
	if(b) {
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) && ButtonClickTest(b,x,y)){
			if (b->callbackFunction) {
				b->callbackFunction();
			}
		}
		b->state = 0;
	}
	glutPostRedisplay();
}

void ButtonPress(Button *b,int x,int y){
	if(b){
		if(ButtonClickTest(b,x,y)){
			b->state = 1;
		}
	}
	glutPostRedisplay();
}

void ButtonPassive(Button *b,int x,int y){
	if(b){
		if( ButtonClickTest(b,x,y) ){
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				glutSwapBuffers();
				glutPostRedisplay();
			}
		}else
			if( b->highlighted == 1 )
			{
				b->highlighted = 0;
				glutSwapBuffers();
				glutPostRedisplay();
			}
	}
}

void ButtonDraw(Button *b){
	int fontx;
	int fonty;
	if(b){
		if (b->highlighted)
			glColor3f(0.921f,0.458f,0.188f);
		else
		glColor3f(0.341f,0.341f,0.31f);

		glBegin(GL_QUADS);
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();
		glLineWidth(3);

		if (b->state)
			glColor3f(0.3f,0.3f,0.3f);
		else
			glColor3f(0.5f,0.5f,0.5f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state)
			glColor3f(0.5f,0.5f,0.5f);
		else
			glColor3f(0.3f,0.3f,0.3f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);
		fontx = b->x + (b->w-glutBitmapLength(GLUT_BITMAP_HELVETICA_10,b->label))/2;
		fonty = b->y + (b->h+10)/2;

		if (b->state) {
			fontx+=2;
			fonty+=2;
		}
		if(b->highlighted){
			glColor3f(0,0,0);
			show(fontx,fonty, b->label, GLUT_BITMAP_HELVETICA_10);
			glColor3f(0,0,0);
			char disc[100];
			sprintf(disc, "DISCRIPTION: %s", b->discription);
			show(140, H-6, disc, GLUT_BITMAP_HELVETICA_10);
			fontx--;
			fonty--;
		}
		glColor3f(1,1,1);
		show(fontx,fonty, b->label, GLUT_BITMAP_HELVETICA_10);
	}
}

void colorPicker(void){
	glBegin(GL_POLYGON);
	glColor3f(0,0,0);
	glVertex2f(65,H-40);
	glColor3f(1,0,0);
	glVertex2f(120,H-70);
	glColor3f(1,1,0);
	glVertex2f(120,H-130);
	glColor3f(0,1,0);
	glVertex2f(65,H-160);
	glColor3f(0,1,1);
	glVertex2f(10,H-130);
	glColor3f(0,0,1);
	glVertex2f(10,H-70);
	glEnd();
	glFlush();
}

void Draw2D(){
	glColor3f(1,1,1);
	char menu[100] = "MENU";
	show(40, 25, menu, GLUT_BITMAP_HELVETICA_18);
	ButtonDraw(&MyButton0);
	ButtonDraw(&MyButton1);
	ButtonDraw(&MyButton2);
	ButtonDraw(&MyButton3);
	ButtonDraw(&MyButton4);
	ButtonDraw(&MyButton5);
	ButtonDraw(&MyButton6);
	glColor3f(1,1,1);
	char psize[100] = "POINT SIZE";
	show(10, 450, psize, GLUT_BITMAP_HELVETICA_18);
	ButtonDraw(&MyButton7);
	ButtonDraw(&MyButton8);
	glColor3f(1,1,1);
	char cpick1[100] = "COLOR", cpick2[100]="SPECTRUM";
	show(32, H-170, cpick1, GLUT_BITMAP_HELVETICA_18);
	show(15, H-20, cpick2, GLUT_BITMAP_HELVETICA_18);
	colorPicker();
	glColor3f(0,0,0);
	char dtool[100];
	sprintf(dtool, "TOOL: %s", ctool);
	show(W-350, H-6, dtool, GLUT_BITMAP_HELVETICA_10);
	char dpsize[100];
	sprintf(dpsize, "POINT SIZE: %d", brushSize);
	show(W-250, H-6, dpsize, GLUT_BITMAP_HELVETICA_10);
	char dcolor[100];
	sprintf(dcolor, "COLOR-> R:%d G:%d B:%d", r,g,b);
	show(W-150, H-6, dcolor, GLUT_BITMAP_HELVETICA_10);
}

void welcome(){
	char welcstr1[100]="WELCOME TO REAL-TIME";
	char welcstr2[100]="COLLABORATIVE PAINT";
	char welcstr3[100]="LOADING...3";

	glColor3f(1,1,1);

	show((W/2)-150,(H/2)-20,welcstr1, GLUT_BITMAP_TIMES_ROMAN_24);
	show((W/2)-145,(H/2)+5,welcstr2, GLUT_BITMAP_TIMES_ROMAN_24);
	show(W-150,H-20,welcstr3, GLUT_BITMAP_HELVETICA_18);
	glClearColor(1,0,0,1);
	int i=0;
	while(i<=W){
		for (int z=5;z<=H;z+=5){
			drawLine(i,z,1,0,0,1);
		}
		i+=5;
		glFlush();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glColor3f(0,0,0);
	char welcstr4[100]="LOADING...2";
	show((W/2)-150,(H/2)-20,welcstr1, GLUT_BITMAP_TIMES_ROMAN_24);
	show((W/2)-145,(H/2)+5,welcstr2, GLUT_BITMAP_TIMES_ROMAN_24);
	show(W-150,H-20,welcstr4, GLUT_BITMAP_HELVETICA_18);
	glClearColor(1,1,0,1);
	i=0;
	while(i<=H){
		for (int z=5;z<=W;z+=5){
			drawLine(z,i,0,1,0,2);
		}
		i+=5;
		glFlush();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
	glColor3f(0,0,0);
	char welcstr5[100]="LOADING...1";
	show((W/2)-150,(H/2)-20,welcstr1, GLUT_BITMAP_TIMES_ROMAN_24);
	show((W/2)-145,(H/2)+5,welcstr2, GLUT_BITMAP_TIMES_ROMAN_24);
	show(W-150,H-20,welcstr5, GLUT_BITMAP_HELVETICA_18);
	glClearColor(1,1,1,1);

	i=W;
	while(i>=0){
		for (int z=5;z<=H;z+=5){
			drawLine(i,z,0,0,1,3);
		}
		i-=5;
		glFlush();
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void drawSection(){
	glBegin(GL_QUADS);
		glColor3f(0.341f,0.341f,0.31f);
		glVertex2i( 0, 0);
		glVertex2i( 130, 0);
		glColor3f(0.203f,0.207f,0.187f);
		glVertex2i( 130, H);
		glVertex2i( 0, H);
	glEnd();
	glLineWidth(3);
	glColor3f(0.2f,0.2f,0.2f);
	glBegin(GL_LINES);
		glVertex2i( 130, 0);
		glVertex2i( 130, H);
	glEnd();
	glBegin(GL_QUADS);
	glColor3f(0.8f,0.8f,0.8f);
		glVertex2i( 131.5, H);
		glVertex2i( 131.5, H-20);
		glColor3f(0.7f,0.7f,0.7f);
		glVertex2i( W, H-20);
		glVertex2i( W, H);
	glEnd();
	glutSwapBuffers();
	glFlush();
}

void myDisplay(void){
	drawSection();
	Draw2D();
	glutSwapBuffers();
}

void reshape(int w, int h){
	H=h; W=w;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	//welcome();
	glClearColor(1,1,1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void MouseButton(int button,int state,int x, int y){
	TheMouse.x = x;
	TheMouse.y = y;
	if (state == GLUT_DOWN) {
		if ( !(TheMouse.lmb ) ) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		}
		if(tool==2){
			textx=x;
			texty=y;
		}
		switch(button) {
			case GLUT_LEFT_BUTTON:
				TheMouse.lmb = 1;
				ButtonPress(&MyButton0,x,y);
				ButtonPress(&MyButton1,x,y);
				ButtonPress(&MyButton2,x,y);
				ButtonPress(&MyButton3,x,y);
				ButtonPress(&MyButton4,x,y);
				ButtonPress(&MyButton5,x,y);
				ButtonPress(&MyButton6,x,y);
				ButtonPress(&MyButton7,x,y);
				ButtonPress(&MyButton8,x,y);
				break;
			case GLUT_MIDDLE_BUTTON:
				TheMouse.mmb = 1;
				break;
			case GLUT_RIGHT_BUTTON:
				TheMouse.rmb = 1;
				break;
		}
		if(x > 10 && x < 120 && y > H-160 && y < H-40){
			glReadPixels(x,H-y,1,1,GL_RGB, GL_UNSIGNED_BYTE, color);
			if(color[0]!=255 && color[1]!=255 && color[2]!=255){
				r=color[0];
				g=color[1];
				b=color[2];
				glColor3ub(r,g,b);
			//printf("r:%d\ng:%d\nb:%d\n", color[0], color[1], color[2]);
			}
		}
	}else{
		switch(button) {
			case GLUT_LEFT_BUTTON:
			TheMouse.lmb = 0;
			ButtonRelease(&MyButton0,x,y);
			ButtonRelease(&MyButton1,x,y);
			ButtonRelease(&MyButton2,x,y);
			ButtonRelease(&MyButton3,x,y);
			ButtonRelease(&MyButton4,x,y);
			ButtonRelease(&MyButton5,x,y);
			ButtonRelease(&MyButton6,x,y);
			ButtonRelease(&MyButton7,x,y);
			ButtonRelease(&MyButton8,x,y);
			break;
		case GLUT_MIDDLE_BUTTON:
			TheMouse.mmb = 0;
			break;
		case GLUT_RIGHT_BUTTON:
			TheMouse.rmb = 0;
			break;
		}
	}
	if(tool==3 && TheMouse.x>130 && TheMouse.y<H-20){
		if (button==GLUT_LEFT_BUTTON){
			if (state==GLUT_DOWN){
				if(point_no!=0)
					point_no=0;
				ml_x = x;
				ml_y = y;
				point_no=1;
			}else if(state==GLUT_UP && point_no==1){
				glPointSize(brushSize);
				draw_line(ml_x, x, ml_y, y);
				point_no=0;
				glFlush();
			}
		}
	}
	if(tool==1 && TheMouse.x>130 && TheMouse.y<H-20){
		if (button==GLUT_LEFT_BUTTON){
			if (state==GLUT_UP){
				line_on==0;
			}
		}
	}
	if(tool==5){
		if(TheMouse.x>130 && TheMouse.y<H-20){
			if (button==GLUT_LEFT_BUTTON){
				if (state==GLUT_DOWN){
					//printf("x:%d\ny:%d\n",x,y);
       				mc_x = x;
       				mc_y = y;
       				point_no=1;
				}else if (state==GLUT_UP){
						if(point_no==1){
							glPointSize(brushSize);
							if ((x-mc_x)!=0){
								m=(float)(mc_y-y)/(float)(x-mc_x);
							}
							circle(mc_x, x, mc_y, y);
							point_no=0;
							glFlush();
						}
					}
			}
		}else{
			point_no=0;
		}
	//glutPostRedisplay();
	}
	if(tool==6 && TheMouse.x>130 && TheMouse.y<H-20){
		if (button==GLUT_LEFT_BUTTON){
			if (state==GLUT_DOWN){
				if(point_no!=0)
					point_no=0;
				ml_x = x;
				ml_y = y;
				point_no=1;
			}else if(state==GLUT_UP && point_no==1){
				glPointSize(brushSize);
				draw_rect(ml_x, x, ml_y, y);
				point_no=0;
				glFlush();
			}
		}
	}
}

void MouseMotion(int x, int y){
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;
	if (tool==1 && x >130 && y<H-20){
		glColor3ub(r,g,b);
		if (line_on==0){
    	glRecti(x,y,x+brushSize,y+brushSize);
		}else{
			glLineWidth(brushSize);
			glBegin(GL_LINES);
				glVertex2i(TheMouse.x, TheMouse.y);
				glVertex2i(x,y);
			glEnd();
		}
		glutSwapBuffers();
    glFlush();
	}else if (tool==4 && TheMouse.lmb && x>140 && y<H-30){
		glColor3f(1.0,1.0,1.0);
		glRecti(x-10,y-10,x+10,y+10);
		glutSwapBuffers();
		glFlush();
		//glutPostRedisplay();
	}else{
		ButtonPassive(&MyButton0,x,y);
		ButtonPassive(&MyButton1,x,y);
		ButtonPassive(&MyButton2,x,y);
		ButtonPassive(&MyButton3,x,y);
		ButtonPassive(&MyButton4,x,y);
		ButtonPassive(&MyButton5,x,y);
		ButtonPassive(&MyButton6,x,y);
		ButtonPassive(&MyButton7,x,y);
		ButtonPassive(&MyButton8,x,y);
		//glutPostRedisplay();
	}
	TheMouse.x = x;
	TheMouse.y = y;
	line_on=1;
}

void MousePassiveMotion(int x, int y){
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;
	TheMouse.x = x;
	TheMouse.y = y;
	ButtonPassive(&MyButton0,x,y);
	ButtonPassive(&MyButton1,x,y);
	ButtonPassive(&MyButton2,x,y);
	ButtonPassive(&MyButton3,x,y);
	ButtonPassive(&MyButton4,x,y);
	ButtonPassive(&MyButton5,x,y);
	ButtonPassive(&MyButton6,x,y);
	ButtonPassive(&MyButton7,x,y);
	ButtonPassive(&MyButton8,x,y);
}

void myKeboard(unsigned char theKey, int mouseX, int mouseY){
	if(tool==2 && textx>130 && texty<H-20 ){
		int i=0;
		int j=0;
		int len,fchar;

		if (X_coord != textx){
		        memset(input, 0, sizeof(input));
        		X_coord = mouseX;
    		}
    		//glClearColor(0,0,0,1);
    		glColor3ub(r,g,b);
		glRasterPos2f(textx,texty);
		//printf("THE KEY:%c", theKey);
		if(theKey==127 || theKey==8){
        		int delwid = 0;
			len = strlen(input);
    			for (i = 0; i <len-1; i++)
				delwid+=(int)glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, input[i]);
			fchar=(int)glutBitmapWidth(GLUT_BITMAP_TIMES_ROMAN_24, input[len-1]);
			glScissor(textx+delwid,700-texty-5,fchar,25);
			glEnable(GL_SCISSOR_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glDisable(GL_SCISSOR_TEST);
			if(strlen(input)>0){
				for(int i=0;i<strlen(input)-1;i++)
					temp[j++]=input[i];
				strcpy(input, temp);
        			memset(temp, 0, sizeof(temp));
			}
    		}else{
        		keystroke = (char)theKey;
        		len = strlen(input);
        		input[len]=keystroke;
    		}
    		len = strlen(input);
    		for (i = 0; i <len; i++)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, input[i]);
    		glutSwapBuffers();
		glFlush();
	}
}

int main(int argc, char **argv){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	glutInitWindowSize(1150,700);
	glutInitWindowPosition(200,50);
	glutCreateWindow("PAINT RELOADED");
	glutReshapeFunc(reshape);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(MousePassiveMotion);
	glutKeyboardFunc(myKeboard);
	glutMainLoop();
}

