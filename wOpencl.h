#ifndef wOpengl_H
#define wOpengl_H
///////////////////////////////////////////////////////////////////////
//opencl header
#include "wDebug.h"
#if defined(__APPLE__) || defined(__MACOSX)
#include<OpenCL/cl.hpp>
#else
#include <CL/cl.h>
#endif
#define CLKERNEL(...)#__VA_ARGS__
//////////////////////////////////////////////////////////////////////////
	size_t deviceListSize;
	cl_uint numPlatforms;
	cl_platform_id platform=NULL;
	cl_device_id* devices;
	cl_context context;
cl_platform_id getPlatform(char* vendor)
{
	cl_uint numPlatforms;
	cl_platform_id platform=NULL;
	cl_int status=0;
	sysout("\nGetting number of Platforms\n");
	status=clGetPlatformIDs(0,NULL,&numPlatforms);
	valAssert(status,CL_SUCCESS,"ERROR: Getting Platforms (clGetPlatformIDs)");
	if(numPlatforms >0)
	{
		sysout("Detected platforms: %d\n",numPlatforms);
		cl_platform_id* platforms=(cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
		status=clGetPlatformIDs(numPlatforms,platforms,NULL);
		valAssert(status,CL_SUCCESS,"ERROR: Getting Platforms (clGetPlatformIDs)");
		for(unsigned int i=0;i< numPlatforms;i++)
		{
			char pbuff[100];
			status=clGetPlatformInfo(
				platforms[i],
				CL_PLATFORM_VENDOR,
				sizeof(pbuff),
				pbuff,
				NULL
				);
			platform=platforms[i];
			if(!strcmp(pbuff,vendor)){
				sysout("\nAMD detected nyan!");
				break;
			}
		}
		delete platforms;
		return platform;
	}
	else return 0;
}
cl_context createContext(cl_platform_id platform, int DeviceType){
	int status=0;
	cl_context_properties cps[3]=
	{
		CL_CONTEXT_PLATFORM,
		(cl_context_properties)platform,
		0
	};
	cl_context_properties* cprops=(NULL==platform)?NULL:cps;
	cl_context result=
		clCreateContextFromType
		(
		cprops,
		DeviceType,
		NULL,
		NULL,
		&status
		);
	
	valAssert(status,CL_SUCCESS,"ERROR: Creating Context (clCreateContextFromtType)\n");
	return  result;
}
cl_device_id* getDevices(cl_context context){
	cl_int status;
		/////////////////////////////////////////////////////////////////
	//get opencl devices
	//////////////////////////////////////////////////////////////////
	status=clGetContextInfo(
		context,
		CL_CONTEXT_DEVICES,
		0,
		NULL,
		&deviceListSize
		);
	valAssert(status,CL_SUCCESS,"ERROR: Getting device list size\n");
	sysout("devices detected: %d \n",deviceListSize);
	cl_device_id *devices =(cl_device_id *)malloc(deviceListSize*sizeof(cl_device_id));
	valAssertNot(devices,NULL,"ERROR: No devices found\n");
	//getting device list
	status=clGetContextInfo(
		context,
		CL_CONTEXT_DEVICES,
		deviceListSize,
		devices,
		NULL
		);
	valAssert(status,CL_SUCCESS,"ERROR: Getting device List\n");
	return devices;
}
void cl_init(){
	cl_int status;
	sysout("hello Opencl\n");
	platform=getPlatform("Advanced Micro Devices, Inc.");
	context=createContext(platform,CL_DEVICE_TYPE_GPU);
	devices=getDevices(context);
	///////////////////////////////////////////////////////////

	

}
void cl_release(){
	
	clReleaseContext(context);
	free(devices);
}
void inline cl_createKernel(cl_kernel *a,char* b,cl_program c){
	int err;
	*a = clCreateKernel(c, b, &err);
	if (!*a || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel %s!\n",b);
		// exit(1);
	}
}
#endif