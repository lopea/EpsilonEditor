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

//node size
#define ns 0.007

//max horizontal height for a font(in node size)
#define my 16.

//max vertical height for a font (in node size)
#define mx 8.

//starting position for a font glyph (accomodate for letters like "j" or "g" that go below starting point)
#define sy 5.

//macros to place squares for a font glyph
#define place(pos, size) sq(pos - size/2., size/2.)
#define g_pl(off, size) place(p - (off + vec2(0, sy)) *ns, size * ns)

float sq(vec2 uv, vec2 s)
{
	vec2 q = abs(uv) - s;
	return min( max(q.x, q.y),0.) + length(max(q,0.));
}


//glyphs are (loosely) based on this font
//https://www.zap.org.au/projects/console-fonts-distributed/psftx-ubuntu-20.04/CyrAsia-TerminusBoldVGA16.psf.large.pdf



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// lower case
///////////////////////////////////////////////////////////////////////////////////////////////////////////
float G_a(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	
	//modeling left to right
	
	//outer "Curved" area 
	float oc1 = place(p - vec2(0,sy * ns + ns), vec2(ns, ns*3.));
	//inner "Curved" area
	float oc2 = place(p - vec2(ns, sy * ns), vec2(ns, ns*5.));
	//bottom 
	float b = place(p - vec2 (ns * 2., sy*ns), vec2(ns*3.,ns));
	//top
	float t = place(p - vec2 (ns * 2., sy*ns + 4.*ns), vec2(ns*3., ns));
	//tail
	float ta = place(p - vec2 (ns, sy*ns + 7.*ns) , vec2(ns*5., ns));
	float r =  place(p - vec2 (ns * 5., sy*ns), vec2(ns*2., ns*7.));
	float res = min(min(min(oc1,oc2), min(b, t)),min(ta, r) );
	return res;
}

float G_c(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(0, 1), vec2(2, 5)),
		  b = g_pl(vec2(1,0), vec2(5,1)),
		  r1 = g_pl(vec2(5,1), vec2 (2,1)),
		  r2 = g_pl(vec2(5,5), vec2 (2,1)),
		  t = g_pl(vec2(1, 6), vec2 (5, 1));
	return min(l, min(b , min(r1, min(r2, t))));
}

float G_o(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,ns * sy)) - pos;
	p.x -= 4.*ns;
	p.y -= (sy - 1.)*ns;
	p = abs(p);
	
	return min(g_pl(vec2(2,-sy),vec2(2,3)), g_pl(vec2(0,-sy + 3.), vec2(3,1)));
}

float G_d(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,ns * sy)) - pos;
	return min(G_o(uv,pos), g_pl(vec2(6, -5), vec2(2, 10)));
}
float G_e(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(0,1), vec2(2,6)),
		  b = g_pl(vec2(1,0), vec2(5,1)),
		  m = g_pl(vec2(2,4), vec2(3,1)),
		  t = g_pl(vec2(1,7), vec2(5,1)),
		  r = g_pl(vec2(5,4), vec2(2,3));
	
	return min(min(min(l,t),min(b, m)), r);
}

float G_i(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	
	float b = g_pl(vec2(2,0), vec2(4, 1)),
		  i = g_pl(vec2(3,0), vec2(2., 7.)),
		  t = g_pl(vec2(2, 6), vec2(3, 1)),
		  s = g_pl(vec2(3, 8), vec2(2, 2));
	return min(min(b,s), min(i,t));
}

float G_t(vec2 uv , vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	
	float b = g_pl(vec2(3, 0), vec2(4, 1)),
		  i = g_pl(vec2(2, 1), vec2(2, 9)),
		  t = g_pl(vec2(0, 6), vec2(6,1));
	return min(b, min(i,t));
}

float G_h(vec2 uv,vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(0), vec2(2,10)),
		  i = g_pl(vec2(2,6), vec2(4, 1)),
		  e = g_pl(vec2(5,0), vec2(2,6));
	return min(l,min(i, e));
}






float G_r(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float b = place(p - vec2(0., sy * ns),vec2(ns*2., ns*8.));
	float t1 =place(p - vec2(ns *2. , sy*ns + ns* 5.), vec2(ns));
	float t2 =place(p - vec2(ns *2., sy*ns + ns* 6.), vec2(ns * 2., ns));
	float t3 =place(p - vec2(ns *3., sy*ns + ns* 7.), vec2(ns * 4., ns));
	return min(min(b,t3),min(t1,t2)); 
}

float G_u(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = place(p - vec2(0, ns * sy  + ns), vec2(ns * 2., 7.*ns)), 
	r = place(p - vec2(ns * 5., ns*sy), vec2(ns * 2., ns* 8.)) , 
	b = place(p - vec2(ns, ns*sy), vec2(ns *5., ns));
	return min(l,min(b,r));
}

float G_m(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l1 = place(p - vec2(0, sy * ns), vec2(2. * ns, 7.* ns)),
		  l2 = place(p - vec2(3.* ns, sy * ns), vec2(2. * ns, 7.* ns)) ,
		  l3 = place(p - vec2(6.* ns, sy * ns), vec2(2. * ns, 7.* ns)),
		  t = place(p - vec2(0, sy*ns + ns * 7.), vec2(ns*7., ns));
	return min(min(l1, t), min(l2, l3));
}

float G_s(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float b = g_pl(vec2(0), vec2(5, 1)),
		  l1 = g_pl(vec2(5, 0), vec2(1, 4)),
		  l2 = g_pl(vec2(6, 1), vec2(1, 2)),
		  b2 = g_pl(vec2(1, 3), vec2(4, 1)),
		  r1 = g_pl(vec2(1, 4), vec2(1, 3)),
		  r2 = g_pl(vec2(0, 4), vec2(1, 3)),
		  b3 = g_pl(vec2(1, 7), vec2(6, 1));
	return min(min(min(min(l2, l1), min(b,b2)), min(r1, r2) ), b3);
}

float G_n(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(0),  vec2 (2,7)),
		  r= g_pl(vec2(5,0),  vec2 (2,7)),
		  t = g_pl(vec2(0,7), vec2(5,1));
	return min(l, min(t,r));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// UPPER CASE
///////////////////////////////////////////////////////////////////////////////////////////////////////////


float G_F(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float b = place(p - vec2 (0,sy*ns), vec2(ns * 2.,ns* 10.)); 
	float f1 = place(p - vec2(ns*2., sy*ns + ns*6.), vec2(ns*3., ns));
	float f2 = place(p - vec2(ns*2., sy*ns + 9.*ns), vec2(ns*5., ns)); 
	return min(b,min(f1, f2)); 
}




float G_C(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(0,1), vec2(2, 8)),
		  t = g_pl(vec2(1, 9), vec2(5,1)),
		  b = g_pl(vec2(1, 0), vec2(5, 1)),
		  c1 = g_pl(vec2(5, 7),vec2(2,2)),
		  c2 = g_pl(vec2(5, 1), vec2(2,2));
	
	return min(min(l, min(t, b)), min (c1, c2));
}
float G_O(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(5,1), vec2(2, 8));
	return min(G_C(uv, pos), l);
}


float G_T(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(3,0), vec2(2, 9)),
	t = g_pl(vec2(0,9),vec2(8, 1));
	return min(l, t);
}
float G_H(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(0,0), vec2(2, 10)),
	s= g_pl(vec2(2,5), vec2(3,1)),
	r = g_pl(vec2(5,0), vec2(2, 10));
	return min(s, min(l, r));
}

float G_E(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	
	float l = g_pl(vec2(0,0), vec2(2, 10)),
	t =  g_pl(vec2(2,9), vec2(5, 1)),
	m =   g_pl(vec2(2,5), vec2(3, 1)),
	b =  g_pl(vec2(2,0), vec2(5, 1));
	return min(min(t, l), min(m,b));
}

float G_B(vec2 uv, vec2 pos )
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float tc = g_pl(vec2(6,6), vec2(2, 3)),
	bc = g_pl(vec2(6,1), vec2(2, 4)),
	//ALL CAPS WHEN YOU SPELL THE MAN NAME
	MF = g_pl(vec2(5,5), vec2(2, 1));

	return min(min(MF,tc), min(bc, G_E(uv, pos)));
	
}

float G_M(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	p.x -=  4.*ns;
	
	p.x= abs(p.x);
	float l1 = g_pl(vec2(3,0), vec2(1, 10)),
	l2 = g_pl(vec2(2,0), vec2(1, 9)), 
	l3 = g_pl(vec2(1,6), vec2(1, 2)), 
	m  = g_pl(vec2(0,5), vec2(1, 2));
	return min(l1, min(l2, min(l3 ,m)));
}


float G_S(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2. * 2, 0)) - pos;
	float b = g_pl(vec2(1,0), vec2(5, 1)),
	      b1 = g_pl(vec2(0,1), vec2(2, 2)),
	      r1 = g_pl(vec2(5,1), vec2(2, 4)),
	      m = g_pl(vec2(1,5), vec2(5, 1)),
	      l1 = g_pl(vec2(0,6), vec2(2, 3)),
	      r2 = g_pl(vec2(5,8), vec2(2, 1)),
	      t = g_pl(vec2(1,9), vec2(5, 1));
	
	return min(min(b,t), min(b1,min(r1, min(m, min(l1,r2)))));
}

float G_I(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float b = g_pl(vec2(1,0), vec2(6, 1)),
	i = g_pl(vec2(3,1), vec2(2, 8)), 
	t = g_pl(vec2(1,9), vec2(6, 1));
	return min(b, min(i, t));
}
float G_U(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float l = g_pl(vec2(-2, 8), vec2(10, 4));
	return min(min(g_pl(vec2(0,8), vec2(2, 2)), g_pl(vec2(5,8), vec2(2, 2))), max(-l, G_O(uv, pos)));
}

float G_L(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float b = g_pl(vec2(0,0), vec2(2, 10)),
	i = g_pl(vec2(2,0), vec2(5, 1));
	return min(b, i);
}

float G_G(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float b = g_pl(vec2(5,3), vec2(2, 1)),
	i = g_pl(vec2(4,4), vec2(3, 1));
	return min(min(b, i), G_C(uv, pos));
}
float G_A(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	return min(g_pl(vec2(2,9),vec2(3,1)), max(-g_pl(vec2(-1,9),vec2(10,3)), G_H(uv, pos)));
}
float G_P(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float l = g_pl(vec2(0,0), vec2(2, 10)),
	t = g_pl(vec2(2,9), vec2(3, 1)),
	s = g_pl(vec2(5,5), vec2(2, 4)),
	t2 = g_pl(vec2(1,4), vec2(4, 1));
	return min(min(l,t2),min(s, t));
}
float G_R(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float e1 = g_pl(vec2(2,3), vec2(2, 1)),
	e2 = g_pl(vec2(3,2), vec2(2, 1)),
	e3 = g_pl(vec2(4,1), vec2(2, 1)),
	e4 = g_pl(vec2(5,0), vec2(2, 1));
	return min(G_P(uv,pos), min(e1, min(e2, min(e3, e4))));
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Symbols/Numbers
///////////////////////////////////////////////////////////////////////////////////////////////////////////
float G_UP(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2. * 2, 0)) - pos;
	p.x -=  4.*ns;
	
	p.x= abs(p.x);
	float l1 = g_pl(vec2(0,0), vec2(1, 10)),
	l2 = g_pl(vec2(1,0), vec2(1, 9)), 
	l3 = g_pl(vec2(2,6), vec2(1, 2)), 
	m  = g_pl(vec2(3,5), vec2(1, 2));
	return min(l1, min(l2, min(l3 ,m)));
}

float G_DOWN(vec2 uv, vec2 pos)
{
	vec2 ouv = uv;
	float rad = radians(180);
	vec2 p = (uv - vec2(ns/2. * 2, 0)) - pos;
	p.x -=  4.*ns;
	
	p.x= abs(p.x);
	float l1 = g_pl(vec2(0,0), vec2(1, 10)),
	l2 = g_pl(vec2(1,1), vec2(1, 9)), 
	l3 = g_pl(vec2(2,2), vec2(1, 2)), 
	m  = g_pl(vec2(3,3), vec2(1, 2));
	return min(l1, min(l2, min(l3 ,m)));
}

float G_COLON(vec2 uv,vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	return min(g_pl(vec2(0,1), vec2(2,2)),g_pl(vec2(0,7), vec2(2,2)));
	
}
float G_0(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2.,0)) - pos;
	float l = g_pl(vec2(5,1), vec2(2, 8)),
	s1 =  g_pl(vec2(2,3), vec2(1, 2)),
	s2 =  g_pl(vec2(3,4), vec2(1, 2)),
	s3 =  g_pl(vec2(4,5), vec2(1, 2));
	return min(min(min(G_C(uv,pos), l), min(s2, s1)), s3);
}

float G_1(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float b = g_pl(vec2(1, 0), vec2(6, 1)),
	      t = g_pl(vec2(3,1), vec2(2, 9)),
	      l1 = g_pl(vec2(2,7), vec2(1,2)),
	      l2 = g_pl(vec2(1,7), vec2(1,1));
	return min(b, min(t, min(l1, l2)));
}
float G_7(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float b = g_pl(vec2(2,0), vec2(2,3)),
	d1 = g_pl(vec2(3,3), vec2(2,2)),
	d2 = g_pl(vec2(4,5), vec2(2,2)),
	d3 = g_pl(vec2(5,7), vec2(2,2)),
	t = g_pl(vec2(0,9), vec2(7,1));
	return min(min(b,t) ,min(min(d1,d2), d3));
}
float G_9(vec2 uv, vec2 pos)
{
	vec2 p = (uv - vec2(ns/2., 0)) - pos;
	float b = g_pl(vec2(1,0), vec2(4,1)),
		  b2 = g_pl(vec2(4,1), vec2(2,1)),
		  ta = g_pl(vec2(5,2), vec2(2,7)),
		  to = g_pl(vec2(1,9), vec2(5,1)),
		  l = g_pl(vec2(0, 5), vec2(2,4)),
		  m = g_pl(vec2(1,4), vec2(5,1));
	return min(min(b,m), min(min(ta,l), min(to,b2)));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Words
///////////////////////////////////////////////////////////////////////////////////////////////////////////

float W_Frums(vec2 uv, vec2 pos)
{
	
	return min(min( min( G_F(uv,pos),        //F
	G_r(uv, pos + vec2( mx*ns,0))),          //r
	 min(G_u(uv, pos + vec2(2.*mx*ns, 0 )),  //u
	 G_m(uv, pos + vec2(3.*mx *ns ,0)))),    //m
	 G_s(uv, pos + vec2(4.*ns * mx, 0)));     //s
}

float W_Credits(vec2 uv, vec2 pos)
{
	return min(min(min(min(min(min(
	           G_C(uv, pos), 
	           G_r(uv, pos + vec2(mx*ns,0))),
	           G_e(uv, pos + vec2(mx*ns * 2., 0))),
	           G_d(uv, pos + vec2(mx *ns * 3. , 0))),
	           G_i(uv, pos + vec2(mx *ns * 4. , 0))),
	           G_t(uv, pos + vec2(mx *ns * 5. , 0))),
	           G_s(uv, pos + vec2(mx *ns * 6. , 0)));
}


float W_BMS(vec2 uv, vec2 pos)
{
	vec2 offset = vec2(mx * ns, 0.);
	
	return min(min(min(min(min(min(min(min(min(min(
	min(min(min(min(min(min(min(min(min(min(min(min(min(
			   G_T(uv, pos), 
			   G_H(uv,pos + offset)),
			   G_E(uv,pos + offset*2.)),
			   G_B(uv, pos + offset *4.)),
			   
			   G_M(uv, pos + offset * 5.)),
			   G_S(uv, pos + offset * 6.)),
			   
			   G_O(uv, pos + offset * 8.)),
			   G_F(uv, pos + offset * 9.)),
			   
			   G_F(uv, pos + offset * 11.)),
			   G_I(uv, pos + offset * 12.)),
			   G_G(uv, pos + offset * 13.)),
			   G_H(uv, pos + offset * 14.)),
			   G_T(uv, pos + offset * 15.)),
			   G_E(uv, pos + offset * 16.)),
			   G_R(uv, pos + offset * 17.)),
			   G_S(uv, pos + offset * 18.)),
			   
			   G_U(uv, pos + offset * 20.)),
			   G_L(uv, pos + offset * 21.)),
			   G_T(uv, pos + offset * 22.)),
			   G_I(uv, pos + offset * 23.)),
			   G_M(uv, pos + offset * 24.)),
			   G_A(uv, pos + offset * 25.)),
			   G_T(uv, pos + offset * 26.)),
			   G_E(uv, pos + offset * 27.));
}

float W_Smith_au_lait(vec2 uv, vec2 pos)
{
	vec2 offset = vec2(mx *ns, 0.);
	return min(min(min(min(min(
	       min(min(min(min(min(
	           G_S(uv, pos), 
			   G_m(uv, pos + offset)),
			   G_i(uv, pos + offset * 2.)),
			   G_t(uv, pos + offset * 3.)),
			   G_h(uv, pos + offset * 4.)),
			   G_a(uv, pos + offset * 6.)),
			   G_u(uv, pos + offset * 7.)),
			   G_L(uv, pos + offset * 9.)),
			   G_a(uv, pos + offset * 10.)),
			   G_i(uv, pos + offset * 11.)),
			   G_t(uv, pos + offset * 12.));
}

float W_Music(vec2 uv, vec2 pos)
{
	vec2 offset = vec2(mx * ns, 0.);
	
	return min(min(min(min(
		   G_M(uv, pos),
		   G_u(uv, pos + offset)),
		   G_s(uv, pos + offset * 2.)),
		   G_i(uv, pos + offset * 3.)),
		   G_c(uv, pos + offset * 4.));
}

float W_BGA(vec2 uv, vec2 pos)
{
	vec2 offset = vec2(mx * ns,0);
	
	return min(min(G_B(uv, pos), 
			       G_G(uv, pos + offset)),
			       G_A(uv, pos + offset* 2.));
	
}

float W_Genre(vec2 uv, vec2 pos)
{
	vec2 offset = vec2 (mx * ns, 0.);
	return min(min(min(min(min(min(
	min(min(min(min(min(min(min(min(
		   G_G(uv, pos),
		   G_e(uv, pos + offset)),
		   G_n(uv, pos + offset * 2.)),
		   G_r(uv, pos + offset * 3.)), 
		   G_e(uv, pos + offset * 4.)),
		   G_COLON(uv, pos + offset*5.)),
		   
		   G_O(uv, pos + offset * 7.)),
		   G_T(uv, pos + offset * 8.)),
		   G_H(uv, pos + offset * 9.)),
		   G_E(uv, pos + offset * 10.)),
		   G_R(uv, pos + offset * 11.)),
		   
		   G_T(uv, pos + offset * 13.)),
		   G_I(uv, pos + offset * 14.)),
		   G_M(uv, pos + offset * 15.)),
		   G_E(uv, pos + offset * 16.));
}

float W_BPM(vec2 uv, vec2 pos)
{
	vec2 offset = vec2(mx * ns,0);
	
	return min(min(min(min(min(min(G_B(uv, pos), 
			       G_P(uv, pos + offset)),
			       G_M(uv, pos + offset* 2.)),
			       G_COLON(uv, pos + offset * 4)),
			       G_1(uv, pos + offset * 5)),
			       G_7(uv, pos + offset * 6)),
			       G_9(uv, pos + offset * 7));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Helper Functions/Logic 
///////////////////////////////////////////////////////////////////////////////////////////////////////////

vec2 posToUV(vec2 uv)
{
	vec2 r = resolution.xy , u = (2.* (uv * r) - r) / r.y;
	return u;
}

float CreditsBG(vec2 uv)
{
	vec2 fpos =posToUV(vec2(1. - mx *ns * 2.,0)), 
	cpos = posToUV(vec2(ns*3., 1. - my * ns ));
	return  min(W_Frums(uv, fpos), W_Credits(uv, cpos));
}

float rep(vec2 uv)
{
   
   vec2 size = vec2(mx * ns, my * ns);
   uv = mod( uv + size/2., size ) - size/2.;
   uv += size/2.;
   return G_0(uv, posToUV(vec2(.5,.5)));
}

float cb(vec2 uv)
{
	uv -= vec2(mx * ns, my *ns) / 2.;
	return mod(floor(uv.x * 1./(ns*mx)) + floor(uv.y * 1./(ns*my)),2.0);
}

float alt0(vec2 uv)
{
	float s = cb(uv);
	float t = mod(time * 10.,2.);
	
	//i fucking hate this line but it works
	if(t > 1.)
		s = 1.-s;
	
	return max(s, rep(uv));
}


float scene(vec2 uv)
{
	return W_BPM(uv, vec2(0));//CreditsBG(uv);
}


float introScene(vec2 uv, float t)
{
	float result = 0;
	
	float rt = mod(t, 3) * 2 - 2;
	
	if(rt < 0)
		return alt0(uv);
	else
	{
		float res = 1;
		float c = W_BMS(uv, posToUV(vec2(0,1)) - vec2(0,my * ns * 2));
		float su = W_Smith_au_lait(uv, posToUV(vec2(1,0.75) - vec2(mx* ns * 4., 0)));
		float mu = min(W_Music(uv, posToUV(vec2(0,.65))), G_COLON(uv, posToUV(vec2(0,.65)) + vec2(mx* ns * 5,0)));
		float f1 = W_Frums(uv, posToUV(vec2(0,.65) +  vec2(mx * ns *2,0)));
		float bga = W_BGA(uv, posToUV(vec2(0, .5)));
		return min(min(c,f1), min(mu, su));
	}
	return rt;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Main Code
///////////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
	vec2 r = resolution , u = posToUV(uv);
	
	fragColor = vec4(step(introScene(u, time), .001)) * 0.89;
}
































