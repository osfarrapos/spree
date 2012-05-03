// v0    - Vertex Position
// v3    - Vertex Normal
// v7    - Vertex Texture Data u,v 
// v8    - Vertex Tangent (v direction)
// v5    - Wave Height Scale
//
// c0    - { 0.0,  0.5, 1.0, 2.0}
// c1    - { 4.0, .5pi, pi, 2pi}  
// c2    - {1, -1/3!, 1/5!, -1/7!  }  //for sin
// c3    - {1/2!, -1/4!, 1/6!, -1/8!  }  //for cos
// c4-7 - Composite World-View-Projection Matrix
// c8     - ModelSpace Camera Position
// c9     - ModelSpace Light Position
// c10   - {fixup factor for taylor series imprecision, }(1.02, 0.1, 0, 0)
// c11   - {waveHeight0, waveHeight1, waveHeight2, waveHeight3}  (80.0, 100.0, 5.0, 5.0) 
// c12   - {waveOffset0, waveOffset1, waveOffset2, waveOffset3}  (0.0, 0.2, 0.0, 0.0) 
// c13   - {waveSpeed0, waveSpeed1, waveSpeed2, waveSpeed3}   (0.2, 0.15, 0.4, 0.4)
// c14   - {waveDirX0, waveDirX1, waveDirX2, waveDirX3}   (0.25, 0.0, -0.7, -0.8)
// c15   - {waveDirY0, waveDirY1, waveDirY2, waveDirY3}   (0.0, 0.15, -0.7, 0.1)
// c16   - { time, sin(time)}
// c17   - {basetexcoord distortion x0, y0, x1, y1} (0.031, 0.04, -0.03, 0.02)
// c18   - World Martix
vs.1.1

def c0, 0.0,  0.5, 1.0, 2.0
//def c1, 4.0f, 0.5f*D3DX_PI, D3DX_PI, 2.0f*D3DX_PI);
//def c2, 1.0f,      -1.0f/6.0f,  1.0f/120.0f, -1.0f/5040.0f);
//def c3, 1.0f/2.0f, -1.0f/24.0f, 1.0f/720.0f, -1.0f/40320.0f );
	
def c10, 1.02, 0.1, 0.0, 0.0	
def c11, 0.4, 0.5, 0.025, 0.025
def c12, 0.0, 0.02, 0.0f, 0.0f	
	
def c13, 0.0, 0.0, 0.0, 0.0
def c14, 0.0, 0.0, 0.0, 0.0
def c15, 0.0, 0.0, 0.0, 0.0

dcl_position	v0
dcl_normal		v3
dcl_texcoord0   v7
dcl_texcoord1   v5
dcl_texcoord2   v8

mul r0, c14, v7.x       // use tex coords as inputs to sinusoidal warp 
mad r0, c15, v7.y, r0   // use tex coords as inputs to sinusoidal warp 

mov r1, c16.x           //time...
mad r0, r1, c13, r0     // add scaled time to move bumps according to frequency
add r0, r0, c12         // starting time offset
frc r0.xy, r0           // take frac of all 4 components
frc r1.xy, r0.zwzw      //
mov r0.zw, r1.xyxy      //

mul r0, r0, c10.x       // multiply by fixup factor (due to inaccuracy)
sub r0, r0, c0.y        // subtract .5
mul r0, r0, c1.w        // mult tex coords by 2pi  coords range from(-pi to pi)

mul r5, r0, r0          // (wave vec)^2
mul r1, r5, r0          // (wave vec)^3 
mul r6, r1, r0          // (wave vec)^4 
mul r2, r6, r0          // (wave vec)^5
mul r7, r2, r0          // (wave vec)^6
mul r3, r7, r0          // (wave vec)^7 
mul r8, r3, r0          // (wave vec)^8

mad r4, r1, c2.y, r0    //(wave vec) - ((wave vec)^3)/3! 
mad r4, r2, c2.z, r4    //  + ((wave vec)^5)/5! 
mad r4, r3, c2.w, r4    //  - ((wave vec)^7)/7! 

mov r0, c0.z            //1
mad r5, r5, c3.x ,r0    //-(wave vec)^2/2!
mad r5, r6, c3.y, r5    //+(wave vec)^4/4!
mad r5, r7, c3.z, r5    //-(wave vec)^6/6!
mad r5, r8, c3.w, r5    //+(wave vec)^8/8!

sub r0, c0.z, v5.x      //... 1-wave scale
mul r4, r4, r0          // scale sin 
mul r5, r5, r0          // scale cos
dp4 r0, r4, c11         //multiply wave heights by waves

mul r0.xyz, v3, r0      //multiply wave magnitude at this vertex by normal 
add r0.xyz, r0, v0      //add to position
mov r0.w, c0.z          //homogenous component

m4x4    oPos, r0, c4    // OutPos = ObjSpacePos * World-View-Projection Matrix

mul     r1, r5, c11               //cos* waveheight
dp4     r9.x, -r1, c14            //normal x offset
dp4     r9.yzw, -r1, c15          //normal y offset and tangent offset

mov     r5, v3                    //starting normal
mad     r5.xy, r9, c10.y, r5      //warped normal move nx, ny according to 
                                  //cos*wavedir*waveeheight

mov     r4, v8                    //tangent
mad     r4.z, -r9.x, c10.y, r4.z  //warped tangent vector  
 
mov     r10, r5
m3x3    r5, r10, c18       // transform normal
dp3     r10.x, r5, r5
rsq     r10.y, r10.x
mul     r5, r5, r10.y             //normalize normal

mov     r10, r4
m3x3    r4, r10, c18       // transform tangent
dp3     r10.x, r4, r4
rsq     r10.y, r10.x
mul     r4, r4, r10.y             //normalize tangent

mul     r3, r4.yzxw, r5.zxyw
mad     r3, r4.zxyw, -r5.yzxw, r3 //xprod to find binormal

mov     r10, r0
m4x4    r0, r10, c18       // transform vertex position

sub     r2, c8,  r0                //view vector
dp3     r10.x, r2, r2
rsq     r10.y, r10.x
mul     r2, r2, r10.y             //normalized view vector          

mov     r0, c16.x
mul     r0, r0, c17.xyxy
frc     r0.xy, r0             //frc of incoming time
add     r0, v7, r0            //add time to tex coords
mov     oT0, r0               //distorted tex coord 0

mov     r0, c16.x
mul     r0, r0, c17.zwzw
frc     r0.xy, r0             //frc of incoming time         
add     r0, v7, r0            //add time to tex coords
mov     oT1, r0.yxzw          //distorted tex coord 1

mov     oT2, r2               //pass in view vector (worldspace)
mov     oT3, r3               //binormal
mov     oT4, r4               //tangent
mov     oT5, r5               //normal


