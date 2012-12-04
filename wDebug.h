#ifndef wDebug_H
#define wDebug_H 
////////////////////////////////////////////////////////////////////////
#if defined(__DEBUG) || defined(__VERBOSE)
	#if defined(__cplusplus)
	#include<iostream>
	#else
	#include<stdio.h>
	#endif
#endif
#if defined(__DEBUG)
#define sysout(...)  printf(__VA_ARGS__)
#define pause() system("pause")
#else 
#define pause() ;
#define sysout(...) ;
#endif
#if defined(__VERBOSE)
	#if defined(__DEBUG)
	#define valAssert(x,y,meaning) if(x!=y) printf("\nParameter \""#x"\" not equal to "#y" :"meaning"\n");
	#define valAssertNot(x,y,meaning) if(x==y) printf("\nParameter \""#x"\" not equal to "#y" :"meaning"\n");
	#else
	#define valAssert(x,y,meaning) if(x!=y) printf(meaning"\n");
	#define valAssertNot(x,y,meaning) if(x==y) printf(meaning"\n");
	#endif
#else
#define valAssert(...);
#define valAssertNot(...);
#endif
////////////////////////////////////////////////////////////////////////
#endif