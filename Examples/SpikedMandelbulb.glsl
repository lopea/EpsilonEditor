//Spiked Mandelbulb by Javier Sandoval (lopea)
//
//Copyright © 2021 Javier Sandoval
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”),
//to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#version 450

layout(std140,binding=0) uniform UniformData
{
	vec4 mouse;
	vec2 resolution;
	float time;
};

in InData
{
	vec2 uv;
	vec2 outpos;
};

out vec4 fragColor;
 
vec2 scene(vec3 p)
{
	float po=5.+4.0*(sin(time/30.)+1.);
	vec3 z=p;
	float dr=1.0, r=0.;
	//p.xy*=mat2(cos(time),sin(time),-sin(time),cos(time))*p.xz;
	for(int i=0;i<64;i++)
	{
		p.xz*=mat2(cos(time),sin(time),-sin(time),cos(time)) * p.xz;
		r=length(z);
		if(r>1.4)break;
		float t=atan(z.z/r), ph = atan(z.y,z.x);
		dr=pow(r,po-1)*po*dr+1.;
		float zr=pow(r,po);
		t*=po;
		ph*=po;
		z=zr*vec3(sin(t)*cos(ph),sin(ph)*cos(t),cos(t));
		z+=p;
	}
	return vec2(0.5*log(r)*r/dr,r * 25);
}

vec4 rc(vec3 ro,vec3 rd) 
{
	float len=0;
	for(int i=0;i<255;i++)
	{
		vec3 pos=ro+rd*len;
		vec2 dist=scene(pos);
		if(dist.x<0.001) return vec4(pos,dist.y);
		len+=dist.x;
		if(dist.x>100.) break;
	}
	return vec4(100);
}
void main()
{
	vec2 u=((uv*resolution)*2.-resolution)/resolution.y;
	vec3 ro=vec3(0,0,-3);
	vec3 rd=normalize(vec3(u,2.));
	vec4 bulb=rc(ro,rd);
	float lol=1.-bulb.w+10;
	float len=length(bulb.xyz-ro);
	fragColor=vec4(bulb.w*0.0052, sin(len + bulb.w + time), cos(time + len), 1);
	fragColor*=1./(len-1);
}










