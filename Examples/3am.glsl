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


float ss(vec3 p)
{
	vec2 xz = vec2(p.x, p.y + p.z * .05);

	p.xz = mod(p.xz + 1,2) - 1; 
	return length(p + sin(time + xz.y *2 - xz.x *0.2)* vec3(0,1,0)) - 1.;
}

vec3 rt(vec3 ro, vec3 rd)
{
	float dist = 0.0;
	for(int i = 0; i < 255; i++)
	{
		float ll = ss(ro+rd*dist);
		
		if(ll < 0.01)
			return (ro+rd*dist);
			
		dist += ll;
		if(dist > 130)
			break;
		
		
	}
	
	return vec3(200);
}


vec3 nn(vec3 p)
{
	vec2 xx = vec2(0.001, 0);
	return normalize(vec3(
		ss(p + xx.xyy) - ss(p -  xx.xyy),
		ss(p + xx.yxy) - ss(p - xx.yxy),
		ss(p + xx.yyx) - ss(p - xx.yyx)));
}

vec4 cc(vec3 p, vec2 u)
{
	
	vec4 amb =  mix(vec4(0.1,0,0,1), vec4(1,.2,.5,1), (u.y  + 1) );
	if(p.z == 200)
		return amb;
	vec3 n = nn(p);
	vec4 col =  dot(n, normalize(vec3(0,1,-1)))* vec4(1,0.4,.5,1) * p.z /100 + abs(p.y) * vec4(1, 0,.4,1)  + amb * 0.1;
	return col;
}


void main()
{
	
	vec2 r = resolution.xy, u = (2.* (uv*r) - r)/r.y;
	
	vec3 rd = normalize(vec3(u, 2.0)),
	ro = vec3(0,4 + cos(time * 0.05) * 0.5,-10);	
 	
	fragColor = cc(rt(ro,rd), u);
}
