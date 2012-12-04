#ifndef WINDOW_H
#define WINDOW_H
#include<cmath>
#define PI 3.14159265358979
#include<iostream>
namespace Window{
	   static float* rectangular(int n){
		 float* result=new float[n];
		 for(int i=0;i<n;i++){
			 result[i]=1;
		 }
		 return result;
	 }
	   static float* Hamming(int n){
		 float* result=new float[n];
		 for(int i=0;i<n;i++){
			 result[i]=0.5f*(1.0f-cos(2.0f*(float)PI*i/(n-1)));
		 }
		 return result;
	 }
	  static  float* HammingSqrt(int n){
		  float* result=Hamming(n);
		  for(int i=0;i<n;i++){
			  result[i]=sqrt(result[i]);
		 }
		 return result;
	  }
	  	static  	   float* Blackman(int n){
		 float* result=new float[n];
		 float a0=7938.0f/18608.0f;
		 float a1=9240.0f/18608.0f;
		 float a2=1430.0f/18608.0f;
		 for(int i=0;i<n;i++){
			 result[i]=a0-a1*cos(2.0f*(float)PI*i/(n-1))+a2*cos(4.0f*(float)PI*i/(n-1));
		 }
		 return result;
	 }
	  static  float* BlackmanSqrt(int n){
		  float* result=Blackman(n);
		  for(int i=0;i<n;i++){
			  result[i]=sqrt(result[i]);
		 }
		 return result;
	  }

}

#endif