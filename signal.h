#ifndef WSIGNAL_H
#define WSIGNAL_H
#include "Window.h"
#include "kiss_fftr.h"
//#define _DEBUGSIGNAL

typedef kiss_fft_cpx complex;
#ifdef SELF_DEFINED_COMPLEXTYPE
typedef struct {
float r;
float i;
}complex;
#endif
namespace SignalProcessing{

	 static kiss_fftr_cfg fwd=NULL;
	 static kiss_fftr_cfg inv=NULL;
	static float outputScaleFactor;
	static int nfft;
	static void applyWindow(float* sample,float*window,int windowSize){
		for(int i=0;i<windowSize;i++){
			sample[i]*=window[i];
		}
	}
	static void normalize(float* sample,int n){
		float max=0;
		for(int i=0;i<n;i++){
			if(abs(sample[i])>max)max=abs(sample[i]);
		}
		for(int i=0;i<n;i++){
			sample[i]/=max;
		}
	}
	static void initFFT(int fftSize){
		if(fwd!=NULL) free(fwd);

		 fwd=kiss_fftr_alloc(fftSize,0,NULL,NULL);
		 if(inv!=NULL) free(inv);
		 
		 inv=kiss_fftr_alloc(fftSize,1,NULL,NULL);
		 outputScaleFactor=1.0f/fftSize;
		 nfft=fftSize;
	}
	static void applyFFT(float* sample,complex* output){
		if(fwd!=NULL){
			
		kiss_fftr(fwd,sample,output);

		}
		
	}
	static void applyIFFT(complex*  freq,float* output){
		if(inv!=NULL){
		kiss_fftri(inv,freq,output);
		for(int i=0;i<nfft;i++) output[i]*=outputScaleFactor;}
	}
	static float note2Frequency(float m){
		return 440*pow(2,(m-69)/12.0f);
	}
	static float getNorm(complex aComplex){
		return aComplex.r*aComplex.r+aComplex.i*aComplex.i;
	}
	static float getAbs(complex aComplex){
		return sqrt(getNorm(aComplex));
	}
	static float getPhase(complex aComplex){
		return atan2(aComplex.i,aComplex.r);
	}
	static complex getComplex(float rho,float phase){
		complex result;
		result.r=rho*cos(phase);
		result.i=rho*sin(phase);
		return result;
	}
#define frames(x,y) frames[x*windowSize+y]
#define spectrum(x,y) spectrum[(windowSize/2+1)*x+y]
#define phase(x,y) phase[(windowSize/2+1)*x+y]
#define magnitude(x,y)magnitude[(windowSize/2+1)+y]
	static float* getFramePtr(float* frames,int windowSize,int i){
		return &(frames[i*windowSize]);
	}

	static float* enFrame(float* sample,float* analysisWindow,int sampleSize,int windowSize,int hopSize){
		int frameCount=(sampleSize+windowSize)/hopSize;
		float* frames;
		frames=new float[frameCount*windowSize];
		for(int i=0;i<frameCount;i++){
			float* curFrame=getFramePtr(frames,windowSize,i);
			for(int j=0;j<windowSize;j++){
				if(hopSize*i+j-windowSize<sampleSize&&hopSize*i+j-windowSize>=0)
				curFrame[j]=sample[hopSize*i+j-windowSize];
				else
					curFrame[j]=0;
			}
			applyWindow(curFrame,analysisWindow,windowSize);
		}
		
		//for(int i=0;i<frameCount*windowSize;i++){
			//std::cout<<frames[i]<<" ";
			//if((i+1)%windowSize==0)std::cout<<std::endl;
		//}
		return frames;
	}
	static float* deFrame(float* frames,float* synthesizeWindow,int sampleSize,int windowSize,int hopSize){
		int frameCount=(sampleSize+windowSize)/hopSize;
		//do overlap add
		float* result=new float[sampleSize];
		for(int i=0;i<sampleSize;i++)
			result[i]=0;
		for(int i=0;i<frameCount;i++){
			float* curFrame=getFramePtr(frames,windowSize,i);
			
			applyWindow(curFrame,synthesizeWindow,windowSize);
			for(int j=0;j<windowSize;j++){
				if(hopSize*i+j-windowSize<sampleSize&&hopSize*i+j-windowSize>=0)
				result[hopSize*i+j-windowSize]+=curFrame[j];
			}
		}
		
		//for(int i=0;i<sampleSize;i++){
			//std::cout<<result[i]<<" ";
		//}
		return result;
	}
	static int calculateFrameCount(int sampleSize,int windowSize,int hopSize){
		return (sampleSize+windowSize)/hopSize;
	}
	static int calculateSampleSize(int frameCount,int windowSize,int hopSize){
		return frameCount*hopSize-windowSize;
	}
	static int calculateFreqBinSize(int windowSize){
		return windowSize/2+1;
	}
	static complex* getSpectrumPtr(complex* spectrum,int windowSize,int i){
		return &(spectrum(i,0));
	}
	static complex* mergeSpectrum(float* magnitude,float* phase,int frameCount,int windowSize){
		int count=((windowSize/2)+1)*frameCount;
		complex* spectrum=new complex[count];
		for(int i=0;i<count;i++){
			spectrum[i]=getComplex(magnitude[i],phase[i]);
		}
		return spectrum;
	}

	
	static void splitSpectrum(complex* spectrum,int frameCount,int windowSize,float** outMagnitude,float** outPhase){
		int count=((windowSize/2)+1)*frameCount;
		*outMagnitude=new float[count];
		*outPhase=new float[count];
		for(int i=0;i<count;i++){
			(*outMagnitude)[i]=getAbs(spectrum[i]);
			(*outPhase)[i]=getPhase(spectrum[i]);
			//(*outMagnitude)[i]=1;
			//(*outPhase)[i]=1;
		}
	}
	static float* Spectrum2Frames(complex* spectrum,int frameCount,int windowSize){
		float* frames=new float[windowSize*frameCount];
		
		initFFT(windowSize);
		for(int i=0;i<frameCount;i++){
			complex* curSpectrum=&(spectrum(i,0));
			float* frame=&(frames(i,0));
			applyIFFT(curSpectrum,frame);
		}
		return frames;
	}
	static complex*  frames2Spectrum(float* frames,int frameCount,int windowSize){
		complex* spectrum=new complex[(windowSize/2+1)*frameCount];
		initFFT(windowSize);
		for(int i=0;i<frameCount;i++){
			complex* curSpectrum=&(spectrum(i,0));
			float* frame=&(frames(i,0));
			applyFFT(frame,curSpectrum);
		}
		 
		return spectrum;
	}
	static float* spectrum2audio_float(float* mag,float* phase,int frameCount,int hopSize,int windowSize,float* window){
	complex* frameFft2=SignalProcessing::mergeSpectrum(mag,phase,frameCount,windowSize);
	float* frames2=SignalProcessing::Spectrum2Frames(frameFft2,frameCount,windowSize);
	float* result=SignalProcessing::deFrame(frames2,window,SignalProcessing::calculateSampleSize(frameCount,windowSize,hopSize),windowSize,hopSize);
	delete [] frames2;
	delete[] frameFft2;
	return result;
	
	}
}

#undef frames
#undef spectrum
#undef phase
#undef magnitude
#ifdef _DEBUGSIGNAL

#include <complex>
#include "kiss_fft.h"


int main(){
#define testSize 1024
#define WINDOWSIZE 128
#define HOPSIZE WINDOWSIZE/2
	float* a=Window::rectangular(testSize);
 
	
	std::cout<<"\n---en-\n";
	int nFrame=SignalProcessing::calculateFrameCount(testSize,WINDOWSIZE,HOPSIZE);
	float* tmp=SignalProcessing::enFrame(a,Window::HammingSqrt(WINDOWSIZE),testSize,WINDOWSIZE,HOPSIZE);
	std::cout<<"\n----\n";
	complex* spectrum=SignalProcessing::frames2Spectrum(tmp,nFrame,WINDOWSIZE);
	float* mag,*phase;
	SignalProcessing::splitSpectrum(spectrum,nFrame,WINDOWSIZE,&mag,&phase);
	complex* spectrum2=SignalProcessing::mergeSpectrum(mag,phase,nFrame,WINDOWSIZE);
	float* tmp2=SignalProcessing::Spectrum2Frames(spectrum2,nFrame,WINDOWSIZE);
	SignalProcessing::deFrame(tmp2,Window::HammingSqrt(WINDOWSIZE),testSize,WINDOWSIZE,HOPSIZE);
	
	system("pause");
	return 0;
}
#endif
#endif