#ifndef NMF_H
#include "wOpencl.h"
#define NMF_H
class NMF{
private:
cl_program nmf_program;
cl_kernel kernel_updateWH;
cl_kernel kernel_getPart;
cl_kernel kernel_updateW;
cl_kernel kernel_updateH;

cl_command_queue commandQueue;
cl_mem W;
cl_mem H;
cl_mem WH;
cl_mem V;
static const char* sourceOption;
static const char* source;
public:
void init()
{
	int err;
	nmf_program = clCreateProgramWithSource(context, 1,  & source, NULL, &err);
	if (!nmf_program)
	{
		printf("Error: Failed to create compute program!\n");
		pause();
		// return EXIT_FAILURE;
	}

	// Build the program executable
	//
	err = clBuildProgram(nmf_program, 0, NULL, sourceOption, NULL, NULL);
	if (err != CL_SUCCESS)
	{
		size_t len;
		char buffer[2048];

		printf("Error: Failed to build program executable!\n");
		clGetProgramBuildInfo(nmf_program, devices[0], CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
		printf("%s\n", buffer);
		pause();
		// exit(1);
	}

	// Create the compute kernel in the program we wish to run
	//
	kernel_updateWH = clCreateKernel(nmf_program, "updateWH", &err);
	if (!kernel_updateWH || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		// exit(1);
	}
	kernel_updateW = clCreateKernel(nmf_program, "updateW", &err);
	if (!kernel_updateW || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		// exit(1);
	}
	kernel_updateH = clCreateKernel(nmf_program, "updateH", &err);
	if (!kernel_updateH || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		// exit(1);
	}
		kernel_getPart = clCreateKernel(nmf_program, "getPart", &err);
	if (!kernel_getPart || err != CL_SUCCESS)
	{
		printf("Error: Failed to create compute kernel!\n");
		// exit(1);
	}
	cl_int status;
		commandQueue=clCreateCommandQueue(
		context,
		devices[0],
		0,
		&status
		);
		valAssert(status,CL_SUCCESS,"ERROR: Creating Command QUEUE\n");

}

int n;
int m;
int r;
void initParameter(float* _V,float* _W,float* _H,int _n,int _m,int _r)
{
	int err=0;
	n=_n;
	m=_m;
	r=_r;
	V=clCreateBuffer(context,CL_MEM_COPY_HOST_PTR,_n*_m*sizeof(float),_V,&err);
	valAssert(err,0,"\nERROR:Setting argument\n");
	W=clCreateBuffer(context,CL_MEM_COPY_HOST_PTR,_n*r*sizeof(float),_W,&err);
	
		valAssert(err,0,"\nERROR:Setting argument\n");
	H=clCreateBuffer(context,CL_MEM_COPY_HOST_PTR,r*_m*sizeof(float),_H,&err);
	
		valAssert(err,0,"\nERROR:Setting argument\n");
	WH=clCreateBuffer(context,CL_MEM_READ_WRITE,_n*_m*sizeof(float),NULL,&err);
	
		valAssert(err,0,"\nERROR:Setting argument\n");
	err|=clSetKernelArg(kernel_updateWH,0,sizeof(cl_mem),&W);

	err|=clSetKernelArg(kernel_updateWH,1,sizeof(cl_mem),&H);
	err|=clSetKernelArg(kernel_updateWH,2,sizeof(cl_mem),&WH);
	err|=clSetKernelArg(kernel_updateWH,3,sizeof(int),&n);
	err|=clSetKernelArg(kernel_updateWH,4,sizeof(int),&m);
	err|=clSetKernelArg(kernel_updateWH,5,sizeof(int),&r);
		err|=clSetKernelArg(kernel_getPart,0,sizeof(cl_mem),&W);

	err|=clSetKernelArg(kernel_getPart,1,sizeof(cl_mem),&H);
	err|=clSetKernelArg(kernel_getPart,2,sizeof(cl_mem),&WH);
	err|=clSetKernelArg(kernel_getPart,3,sizeof(int),&n);
	err|=clSetKernelArg(kernel_getPart,4,sizeof(int),&m);
	
	err|=clSetKernelArg(kernel_getPart,5,sizeof(int),&r);

	err|=clSetKernelArg(kernel_updateW,0,sizeof(cl_mem),&V);
	err|=clSetKernelArg(kernel_updateW,1,sizeof(cl_mem),&W);
	err|=clSetKernelArg(kernel_updateW,2,sizeof(cl_mem),&H);
	err|=clSetKernelArg(kernel_updateW,3,sizeof(cl_mem),&WH);
	err|=clSetKernelArg(kernel_updateW,4,sizeof(int),&n);
	err|=clSetKernelArg(kernel_updateW,5,sizeof(int),&m);
	err|=clSetKernelArg(kernel_updateW,6,sizeof(int),&r);

	err|=clSetKernelArg(kernel_updateH,0,sizeof(cl_mem),&V);
	err|=clSetKernelArg(kernel_updateH,1,sizeof(cl_mem),&W);
	err|=clSetKernelArg(kernel_updateH,2,sizeof(cl_mem),&H);
	err|=clSetKernelArg(kernel_updateH,3,sizeof(cl_mem),&WH);
	err|=clSetKernelArg(kernel_updateH,4,sizeof(int),&n);
	err|=clSetKernelArg(kernel_updateH,5,sizeof(int),&m);
	err|=clSetKernelArg(kernel_updateH,6,sizeof(int),&r);
		valAssert(err,0,"\nERROR:Setting argument\n");

}
void updateWH()
{
	size_t global[]={n,m};
	clEnqueueNDRangeKernel(commandQueue,kernel_updateWH,2,NULL,global,NULL,NULL,NULL,NULL);
	///clFinish(commandQueue);
}
void getPart(int i){
	int err=clSetKernelArg(kernel_getPart,6,sizeof(int),&i);
	valAssert(err,0,"\nERROR:Setting argument\n");
	size_t global[]={n,m};
	clEnqueueNDRangeKernel(commandQueue,kernel_getPart,2,NULL,global,NULL,NULL,NULL,NULL);
}
void updateW()
{
	size_t global[]={n,r};
	clEnqueueNDRangeKernel(commandQueue,kernel_updateW,2,NULL,global,NULL,NULL,NULL,NULL);
}
void updateH()
{
	size_t global[]={r,m};
	clEnqueueNDRangeKernel(commandQueue,kernel_updateH,2,NULL,global,NULL,NULL,NULL,NULL);
}
float* mapH(){
	float * result=(float*)clEnqueueMapBuffer(commandQueue,H,CL_TRUE,CL_MAP_READ|CL_MAP_WRITE,NULL,m*r*sizeof(float),NULL,NULL,NULL,NULL);
	clFinish(commandQueue);
	#ifdef __DEBUG
	
	std::cout<<"H:\n";
	for(int i=0;i<r*m;i++){
		std::cout<<result[i]<<" ";
	}
	std::cout<<std::endl;
	pause();
	#endif
	return result;
}
void unmapH(float* result)
{
	clEnqueueUnmapMemObject(commandQueue,H,result,NULL,NULL,NULL);
}
float* mapWH(){
	float * result=(float*)clEnqueueMapBuffer(commandQueue,WH,CL_TRUE,CL_MAP_READ,NULL,m*n*sizeof(float),NULL,NULL,NULL,NULL);
	clFinish(commandQueue);
	#ifdef __VERBOSE
	
	std::cout<<"WH:\n";
	pause();
	for(int i=0;i<n*m;i++){
		std::cout<<result[i]<<" ";
		if(n*m>1000)i+=m;
	}
	std::cout<<std::endl;
	pause();
	#endif
	return result;
}
void unmapWH(float* result)
{
	
	clEnqueueUnmapMemObject(commandQueue,WH,result,NULL,NULL,NULL);
}
float* mapV(){
	float * result=(float*)clEnqueueMapBuffer(commandQueue,V,CL_TRUE,CL_MAP_READ,NULL,m*n*sizeof(float),NULL,NULL,NULL,NULL);
	clFinish(commandQueue);
	return result;
}
void unmapV(float* result)
{
	
	clEnqueueUnmapMemObject(commandQueue,V,result,NULL,NULL,NULL);
}
float* mapW()
{
	float * result=(float*)clEnqueueMapBuffer(commandQueue,W,CL_TRUE,CL_MAP_READ|CL_MAP_WRITE,NULL,n*r*sizeof(float),NULL,NULL,NULL,NULL);
	clFinish(commandQueue);
	#ifdef __VERBOSE
	std::cout<<"W:\n";
	for(int i=0;i<r*n;i++){
		std::cout<<result[i]<<" ";
	}
	std::cout<<std::endl;
	pause();
	#endif
	return result;
}
void unmapW(float* result)
{
	
	clEnqueueUnmapMemObject(commandQueue,W,result,NULL,NULL,NULL);
}

void releaseObject(){
	clReleaseMemObject(V);
	clReleaseMemObject(W);
	clReleaseMemObject(H);
	clReleaseMemObject(WH);
}
void finish(){
	clFinish(commandQueue);
}

};
void cl_nmf_test()
{
#define SIZE 100
	//float v[]={1,2,3,4};
	//float w[]={6,2};
	//float h[]={6,2 };
	//cl_nmf_initParameter(v,w,h,2,2,1);
	NMF a;
	a.init();
	float* v=new float[SIZE*SIZE];
	float* w=new float[SIZE*6];
	float* h=new float[6*SIZE];
	for(int i=0;i<SIZE*SIZE;i++){
		v[i]=(i+1)/(float)SIZE*SIZE;
	}
	for(int i=0;i<SIZE*2;i++)
	{
		w[i]=(i+1)/(float)SIZE*SIZE;
		h[i]=(i+1)/(float)SIZE*SIZE;
	}
	a.initParameter(v,w,h,SIZE,SIZE,6);
	//cl_nmf_initParameter(v,w,h,2,2,1);
	a.updateWH();
	for(int i=0;i<500;i++)
	{

		a.updateWH();
		a.updateH();

		a.updateWH();
		a.updateW();
		
		a.finish();
		if(i%100==0)std::cout<<"NO:"<<i<<std::endl;
	}
	a.getPart(0);
	a.updateWH();
	a.finish();
	a.mapWH();
	//a.updateWH();
	//mapWH();
}
#include "NMF_kernel.h"
#endif