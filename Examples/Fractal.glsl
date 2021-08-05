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

float c  =0;

mat2 rot(float f)
{
	float s = sin(f), c = cos(f);
	return mat2(c, -s , s, c);
}
float scene(vec3 p)
{
	
    for( int i = 0; i<10; ++i){
        float t = time*.2;
        p = abs(p - 0.2);
        p.xy *= rot(t + c);
		
        c -= 0.1;
	}
	
	float thing = max(dot(p , (vec3(45, 0,-524))), length(p) - 2);
	
	return max(thing, p.y);
}


vec3 calcnorm(vec3 value)
{
    vec3 n = vec3(0);
    if(length(n - value) == 0.)
    	return vec3(0);
    for(int i = 0; i < 4; i++)
    {
        vec3 eps = 0.001 * (vec3(9 >> i&1, i >> 1 & 1, i & 1) * 2. - 1.);
        n += eps * scene(value + eps);
    }
    return normalize(n);
}


vec4 rc(vec3 ro, vec3 rd)
{
	float len = 0;
	float closest = 100;
	for(int i = 0; i < 255; i ++)
	{
		float dist = scene(ro + rd * len);
		
		closest = min(closest, dist/.01);
		if(dist < 0.001)
			return vec4(ro + rd * len,closest);
		
		len += dist;
		
		if(len  > 100 ) 
			break;
	}
	
	return vec4(ro + rd * len,closest);
}

mat3 cam(vec3 pos, vec3 la, vec3 up)
{
	vec3 ww = normalize(la - pos);
	vec3 vv = normalize(up);
	vec3 uu = normalize(cross(ww, vv));
	return mat3(ww, uu ,vv);
} 


vec3 pallete( vec3 pos)
{
	float len = length(pos) / 15;
	
	return mix(1- vec3 (0, 0.5, 0.7), 1-vec3(1, 0,10), len)/100; 
}
vec3 color(vec4 pos)
{
	vec3 lightcolor = vec3( 1, 1, 1), lightpos = vec3(0, 4, -4);
	vec3 norm = calcnorm(pos.xyz);
	
	return pallete(pos.xyz)- (1/(1 + (pos.w * 0.3))) * vec3(0,1,1);
}

void main()
{
	vec2 r = resolution, u = ((2.*uv * r) - r)/r.y;
	
	vec3 ro = vec3(0., 0., -5.);

	vec3 rd = normalize(vec3(u, 2.0));
	

	
	fragColor = vec4(color(rc(ro,rd) ), 1.);
}





