#ifndef WAUDIOSTREAM_H
#define WAUDIOSTREAM_H
#include <iostream>
#include<cstdint>
#include<climits>
//#define _AUDIOSTREAM_DEBUG
namespace AudioStream{
	//now  works only for stereo&mono sound
	static void splitStereoChannel(int16_t * InterleavedRaw,int channelCount,int sampleCount,float** OUTleft,float** OUTright)
	{
		float factor=1.0f/INT16_MAX;
		bool flag1=(OUTleft!=NULL);
		bool flag2=(OUTright!=NULL);
		int nSample=sampleCount/channelCount;
		if(channelCount==0) 
		{
			if(flag1)
			*OUTleft=NULL;
			if(flag2)
			*OUTright=NULL;
			return;
		}
		else if(channelCount==1){
			if(flag1)
			*OUTleft=new float[nSample];
			if(flag2)
			*OUTright=NULL;
		}
		else if(channelCount==2){
			if(flag1)
			*OUTleft=new float[nSample];
			if(flag2)
			*OUTright=new float[nSample];
		}
		//ignore excess channels
		for(int i=0;i<nSample;i++)
		{
			//if(i%100==99)
			//std::cout<<i<<std::endl;
			for(int j=0;j<channelCount;j++)
			{
				switch(j)
				{
				case 0:if(flag1)(*OUTleft)[i]=InterleavedRaw[i*2]*factor;break;
				case 1:if(flag2)(*OUTleft)[i]=InterleavedRaw[i*2+1]*factor;break;
				default:break;
				}
			}
		}
		

	}
	static float* getDefaultChannel( int16_t * InterleavedRaw,int channelCount,int sampleCount)
	{
		float* result=NULL;
		splitStereoChannel(InterleavedRaw,channelCount,sampleCount,&result,NULL);
		return result;
	}
	static int16_t* float_to_int16_t(float* sample,int n){
		int16_t * result=new int16_t[n];
		for(int i=0;i<n;i++){
			result[i]=(int16_t)(sample[i]*INT16_MAX);
		}
		return result;
	}

}
#ifdef _AUDIOSTREAM_DEBUG
int main(){
	std::cout<<"hello"<<std::endl;
	sf::SoundBuffer a;
	a.loadFromFile("sample1.wav");
	sf::Sound b(a); 
	std::cout<<a.getSampleCount()<<std::endl;
	//b.play();
	float* channel1=AudioStream::getDefaultChannel((int16_t *)a.getSamples(),a.getChannelCount(),a.getSampleCount());
	sf::SoundBuffer ff;
	int16_t*  tmp=AudioStream::float_to_int16_t(channel1,a.getSampleCount()/a.getChannelCount());
	//for(int i=0;i<a.getSampleCount()/a.getChannelCount();i++)std::cout<<tmp[i];
	ff.loadFromSamples(tmp,a.getSampleCount()/a.getChannelCount(),1,44100);
	sf::Sound dd(ff);
	dd.play();
	system("pause");
	return 0;
	
}
#endif
#endif