
#ifndef SIGNALUTILITIES_H
#define SIGNALUTILITIES_H
#include<SFML/OpenGL.hpp>
namespace SignalUtilities{
	static void drawSpectrum(float* raw,int n,float width,float height,float scale){
		glPushMatrix();
		glLineWidth(1.0f);
		//glColor3f(0.0f,0.0f,1.0f);
	   glScalef(width,height,0);
		glTranslatef(0,1,0);
		glBegin(GL_LINES);
		for(int i=0;i<n;i++){
			glVertex2f(i/(float)n,-raw[i]*scale);
			glVertex2f(i/(float)n,0);
		}
		glEnd(); 
				glColor4f(0.0f,0.0f,0.0f,.5f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(1.0f,0.0f);
		glVertex2f(0.0f,0.0f);
		glEnd();
	
		for(int i=0;i<10;i++){
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.0f,-1.0f);
		glVertex2f(0.0f,0.0f);
		glEnd();
		glTranslatef(0.1f,0,0);
		}
		glPopMatrix();
	}
	static void drawSignal(float* raw,int n,float width,float height){
		glPushMatrix();
		
		//glColor3f(0.0f,0.0f,1.0f);
		
	   glScalef(width,height,0);
		glTranslatef(0,0.5,0);
		glBegin(GL_LINE_STRIP);
		for(int i=0;i<n;i++)
			glVertex2f(i/(float)n,-raw[i]/2);
		glEnd();
		
		glColor4f(0.0f,0.0f,0.0f,.5f);
		glBegin(GL_LINE_STRIP);
		glVertex2f(1.0f,0.0f);
		glVertex2f(0.0f,0.0f);
		glEnd();
		glPushMatrix();
		for(int i=0;i<10;i++){
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.0f,-.5f);
		glVertex2f(0.0f,.5f);
		glEnd();
		glTranslatef(0.1f,0,0);
		}
		glPopMatrix();
		for(int i=0;i<10;i++){
		glBegin(GL_LINE_STRIP);
		glVertex2f(0.0f,-0.5f);
		glVertex2f(1.0f,-0.5f);
		glEnd();
		glTranslatef(0.0f,0.1f,0.0f);
		}
		glPopMatrix();
	}
}
#endif