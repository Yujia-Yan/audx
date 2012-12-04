
 const char* NMF::sourceOption=CLKERNEL(
	-D V(x,y)=V[x*m+y]
-D W(x,y)=W[x*r+y]
-D H(x,y)=H[x*m+y]
-D WH(x,y)=WH[x*m+y]
-D LAMDAD=1e-8
	);
 const char* NMF::source = CLKERNEL(
	 	kernel void getPart
	(
	__global float* W,
	__global float* H,
	__global float* WH,
	int n,int m,int  r,int k
	)
{
	int i=get_global_id(0);
	int j=get_global_id(1);
	float WH_tmp=0;
	WH_tmp+=W(i,k)*H(k,j);
	WH(i,j)=WH_tmp;
}
	kernel void updateWH
	(
	__global float* W,
	__global float* H,
	__global float* WH,
	int n,int m,int r
	)
{
	int i=get_global_id(0);
	int j=get_global_id(1);
	float WH_tmp=0;
	for(int k=0;k<r;k++)
	{
		WH_tmp+=W(i,k)*H(k,j);
	}
	WH(i,j)=WH_tmp;
}
kernel void updateW
	(
	__global float* V,
	__global float* W,
	__global float* H,
	__global float* WH,
	int n,int m,int r
	)
{
	int i=get_global_id(0);
	int a=get_global_id(1);
	float factor1=0;
	float factor2=LAMDAD;
	for(int mu=0;mu<m;mu++)
	{
		factor1+=H(a,mu)*V(i,mu)/(WH(i,mu)+LAMDAD);
		factor2+=H(a,mu);
	}
	W(i,a)*=factor1/factor2;
}
kernel void updateH
	(
	__global float* V,
	__global float* W,
	__global float* H,
	__global float* WH,
	int n,int m,int r
	)
{
	int a=get_global_id(0);
	int mu=get_global_id(1);
	float factor1=0;
	float factor2=LAMDAD;
	for(int i=0;i<n;i++)
	{
		factor1+=W(i,a)*V(i,mu)/(WH(i,mu)+LAMDAD);
		factor2+=W(i,a);
	}
	H(a,mu)*=factor1/factor2;
}
);