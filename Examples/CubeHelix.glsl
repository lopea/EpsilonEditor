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


mat2 rot(float a)
{
	float s = sin(a), c = cos(a);
	return mat2(c, -s ,s, c);
}

//https://www.shadertoy.com/view/Xds3zN
float cube (vec3 p, vec3 s)
{
	vec3 d = abs(p) - s;
	return min(max(d.x, max(d.y, d.z)), 0.) + length(max(d, 0.));
}

float scene(vec3 p)
{
	
	float z = p.z;
	
	//p.z += time;
	
	p.z = mod(p.z - 2., 4.) -2.;
	p.x = mod(p.x - 3., 6.) -3.;
	p.xy /= 12.;
	p.xy *= rot(time + p.x * .3 * sin(time + z));
	p.xy *= 12.;
	p.y = abs(p.y);
	//return cube( p, vec3(1));
	return cube(p , vec3(abs(sin(time - p.z * 0.44 + z)), 0., 1.)) - .5;
}

mat3 cam(vec3 E, vec3 l)
{
	vec3 ww = normalize(l - E);
	vec3 uu = normalize(cross(ww, vec3(0,1,0)));
	vec3 vv = cross(uu,ww);
	
	return mat3(uu,vv, ww);
}


//https://www.shadertoy.com/view/Xds3zN
vec3 norm(vec3 value)
{
    vec3 n = vec3(0.0);
    for( int i=0; i<4; i++ )
    {
        vec3 e = 0.5773*(2.0*vec3((((i+3)>>1)&1),((i>>1)&1),(i&1))-1.0);
        n += e * scene(value+0.0005*e);
    }
    return normalize(n);
}


vec4 rc(vec3 ro, vec3 rd)
{
	float len = 0.;
	float closest = 50.;
	for(int i = 0; i < 255; i ++)
	{
		vec3 pos = ro + rd * len;
		float dist = scene(pos);
				closest = min(closest, dist);
		if(dist < .001)
			return vec4(ro + rd * len, closest);
			

		len += dist;
		
		if(len > 50)
		return vec4(0,0,0, closest);
		
	}
	return vec4(0,0,0, closest);
}

vec3 color(vec4 pos, vec3 cam, vec2 uv)
{
	vec3 n = norm(pos.xyz);
	vec3 amb = vec3(1.-uv.y) * 0.2 * vec3( .4, .4, .7);
	vec3 dis = cam - pos.xyz;
	if(dot (pos,pos) == 0.)
		return amb + vec3(1.,1.,1.) ;
	
	return vec3(dot(n , normalize(dis))) * vec3(0.1, .6, .7) * ( pos.x) + amb + 1./(1.+clamp(pos.w, 0.,1.) * 10.) * 0.6* vec3(.4,.4,.7);
}
void main()
{
	vec2 r = resolution, u = ((uv*r)* 2. - r)/r.y;
	
	vec3 ro = vec3(0.,10.,0.);

	//ro.xz *= rot(time);
	
	vec3 rd = cam(ro, vec3(1)) * normalize(vec3(u, 2.));
	
	vec4 pos = rc(ro,rd);
	
	fragColor = vec4(color(pos, ro, u), 1.) ;
}


