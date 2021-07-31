//Intro by Javier Sandoval (lopea)
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
void main()
{
	vec2 u = -1. + 2. * uv;
	vec2 r = cos(2./(u.x) * atan(u.x, u.y) + time) - sin( u+ 1./length((u )));
	fragColor = vec4(r.yxxy);
}
