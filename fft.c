///////////////////////////////////////////////////////////////////////  
//     function   name   :   fft()  
//     purpose               :   fourier   transfer  
//     input   parameter:  
//       xr   ---pointer   to   input   and   output   data   real   part;  
//     xi   ---pointer   to   input   and   output   data   imaginary   part  
//     l     ---data   length;  
//     inv---inv=1   means   IFFT,otherwise   FFT.  
//     This   is   a   standard   programm.  
///////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void   fft(float   *xr,float   *xi,int   l,int   inv)  
{  
	int   i,j,k,a,b;  
	float   ha,dr,di,pi,x;  
	int   cn,ar,p0,p1,po;  
	float   *co,*si;  
	co=(float   *)calloc(l,sizeof(float));  
	if(co==NULL)  
	{  
		printf("\n   Memory   Error   !   ");  
		exit(1);  
	}  
	si=(float   *)calloc(l,sizeof(float));  
	if(si==NULL)  
	{  
		printf("\n   Memory   Error   !   ");  
		free(co);  
		exit(1);  
	}  
	if(inv!=0)  
	{  
		for(i=0;i<l;i++)  
		{  
			xr[i]=xr[i]/l;  
			xi[i]=xi[i]/l;  
		}  
	}  
	po=0.0;  
	i=l;  
	while(i!=1){po=po+1;i=i/2;}  
	pi=3.141592635897932;  
	j=0;  
	for(i=0;i<l-1;i++)  
	{  
		if(i<j)  
		{  
			x=xr[i];  
			xr[i]=xr[j];  
			xr[j]=x;  
			x=xi[i];  
			xi[i]=xi[j];  
			xi[j]=x;  
		}  
		k=l/2;  
		while(k<=j)  
		{  
			j=j-k;  
			k=k/2;  
		}  
		j=j+k;  
	}  
	ha=2.0*pi/l;  
	for(i=0;i<l;i++)  
	{  
		si[i]=sin(ha*i);  
		co[i]=cos(ha*i);  
	}  
	if(inv==0)  
	{  
		for(i=0;i<l;i++)si[i]=-si[i];  
	}  
	a=2;  
	b=1;  
	for(cn=1;cn<=po;cn++)  
	{  
		p0=l/a;  
		p1=0;  
		for(k=0;k<b;k++)  
		{  
			i=k;  
			while(i<l)  
			{  
				ar=i+b;  
				if(k==0)  
				{  
					dr=xr[ar];  
					di=xi[ar];  
				}  
				else  
				{  
					dr=xr[ar]*co[p1]-xi[ar]*si[p1];  
					di=xi[ar]*co[p1]+xr[ar]*si[p1];  
				}  
				xr[ar]=xr[i]-dr;  
				xi[ar]=xi[i]-di;  
				xr[i]=xr[i]+dr;  
				xi[i]=xi[i]+di;  
				i=i+a;  
			}  
			p1=p1+p0;  
		}  
		a=2*a;  
		b=b*2;  
	}  
	free(co);  
	free(si);  
	return;  
}  

