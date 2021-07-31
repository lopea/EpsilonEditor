//Cube Spin by Javier Sandoval (lopea)
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
layout(std140, binding = 0) uniform UniformData
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

mat2 rot(float sc)
{
	float d = time * sc, s = sin(d), c = cos(d);
	return mat2(c, -s, s, c);
}
float scene(vec3 v)
{
	
	v.xy *= rot(1.);
	v.yz *= rot(0.5);
	
	vec3 q = abs(v + vec3(sin(time), sin(cos(time)),cos(time))) - vec3(1.);
	return length(max(q, 0.)) + min(max(q.x, max(q.y,q.z)),0.0);
}

vec3 norm(vec3 val)
{
	vec3 n = vec3(0.0);
	for(int i = 0; i < 4; i++)
	{
		vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
		n += e*scene(val + 0.0005 *e);
	} 
	return normalize(n);
}
vec4 rm(vec3 ro, vec3 rd)
{
	float len = 0.;
	for(int i = 0; i < 255; i++)
	{
		float dist = scene( ro + rd *len);
		
		if(dist < 0.001)
			return vec4(ro + rd * len, len);
			
		len += dist;
		
		if(len > 100.)
			break;
	}
	return vec4(0, 0, 0, 100);
}

vec4 color(vec3 pos)
{
	vec3 n = norm(pos);
	
	vec3 lightdir = vec3( 2, 3, -5); 
	
	return dot(n,lightdir) * vec4(0, .3, .6, 1) *0.5; 
}
void main()
{
	vec2 u = ((uv * resolution)* 2 - resolution) / resolution.y;
	vec2 r = cos(2./(u.x) * atan(u.x, u.y) + time) - sin( u+ 1./length((u )));
	
	vec3 ro = vec3(0,0,-10);
	vec3 rd = normalize(vec3(u, 2.0));
	
	vec4 res = rm(ro,rd);
	
	fragColor = vec4(0);
	
	if(res.w < 100)
		fragColor = color(res.xyz );
}

