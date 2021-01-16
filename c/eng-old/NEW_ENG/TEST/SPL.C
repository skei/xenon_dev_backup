/*

This is not a tutorial on interpolation.  It shows a very common algorithm
used for interpolation which I made general purpose to compare several
interpolation types.  I added comments to explain some subtle aspects.

I included some common types: B굗ier, B-spline and Catmul-Rom.
Some others I ran across are: parabolic, simple cubic, Hermite and beta with
tension & bias.  I completely derived the linear basis matrix myself (big
deal).

I also derived the Kochanek-Bartels from their 84 SIGGRAPH paper to provide
global control of tension, bias, continuity  (it's been 25 years since I've
done matrix math so it took a few passes).   NO NURBS, I haven't gotten in
that deep.

I am not a math major nor an expert on splines and interpolation.  For the
last 2-1/2 years I read comp.graphics.algorithms, did some searches and
grabbed related things.  In Jan. '96, 4 months ago,  I decided to finally
code something and found that it was rather simple, thanks mostly to Leon
DeBoer's posting.

This routine does interpolation on a 2D list in the array: IT(c,n) where c=1
for the X coordinate, c=2 for Y and "n" the vector number.
The same type of interpolation is done on both X and Y coordinates though
this is not required in general and some interesting effects may be possible
by using different interpolation types for the dimensions - room for a little
art.
There are comments for going to more dimensions later.  I set up this routine
to study the interpolation types and debug the code with the goal of moving
to key frame animation.

There is a problem with terminology which I will now define.  Since Laser
(light show) Graphics is "vector" graphics, the coordinate sets (X,Y here)
are called vectors.  As this code is in my image editor, I use that term.  I
think I use both vector and control point in my comments.  They are the same.
This algorithm considers all input image vectors as control points, even for
Hermite (once you understand the Hermite, you'll see why this is incorrect
for it).  For key frame animation, the input control points are called key
frames or "keys".

This is parametric interpolation, which means that the coordinate values are
interpolated vs. a parameter, in this case J.  Y is NOT interpolated as a
function of X as we might think to do.

Because some types need data points before and after an interval in order to
calculate a curve, the first and last vectors need extra data.  A common
solution is simply to repeat the end vectors to provide the data.  This is
done here.  This forces the curve to always hit the 1st and last vectors.
This along with the way each type works can confuse what is going on.  The
extra values could be made changeable to gain more control of the curve at
the end vectors.

This is a general purpose routine, a single interpolation type won't need all
the terms to be calculated.  Any basis matrix entry of zero means that the
corresponding term in the coef. equation can be removed.  An entire zero row
means that the corresponding power of T calculation can be dropped.

This routine calculates the polynomial equation in matrix form:

                                       | n-1 |
     1                                 | n   |              Steve Noskowicz
P = --- * [T^3  T^2  T  1] * [basis] * | n+1 |              Q10706@email.mot.com
     D                                 | n+2 |              May-13:Nov-24 1996

Entry conditions (if I remember everything):

Arrays subscripts start at 1, not zero.
The first vector is in IT(c,2).  IT(c,1) is the extra n-1 data.
The last vector is in IT(c,P).

InterPts is the number of NEW points added "between" key points as
T goes 0 --> 1
NOTE that for types which hit the control points (linear, cubic, Catmul-Rom),
when T=0 control vectors are being "re-calculated".  For these types, time
can be saved by eliminating those calculations and passing the original
vectors through.  Also, if T goes to 1, you are double calculating.

*/

// This section adds the extra data at the ends by repeating the end vectors.
// Making these accessible would allow customizing the end behavior.

IT(1,1) = IT(1,2)  // Make the n-1 data for first X
IT(2,1) = IT(2,2)  // for Y

// Look where your type starts & stops to see if you need to do this.

IT(1,P+1) = IT(1,P)   // Make the n+1 for
IT(2,P+1) = IT(2,P)   // the last vector
IT(1,P+2) = IT(1,P)   // Make the n+2 for last vector
IT(2,P+2) = IT(2,P)   // This is only necessary for some conditions.

// You may want different start/stop points (NStart, NEnd) from what I have here..
// In this long section we select the desired basis matrix.

// !! Only Kochanek-Bartels spline kept.... Axon/Inf !!!

// 컴컴� Kochanek-Bartels 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

// Basis matrix for global Tension, Continuity  & Bias
// THIS IS IT! I figured it out from the SIGGRAPH paper !
// As T goes from 0 to 1 you go from n to n+1

//       n-1      n        n+1       n+2
//  T^3  -A    4+A-B-C   -4+B+C-D     D     /
//  T^2 +2A  -6-2A+2B+C  6-2B-C+D    -D    /
//  T^1  -A     A-B         B         0   /  2
//  T^0   0      2          0         0  /

FT =  0   // Tension       T=+1-->Tight             T=-1--> Round
FB =  0   // Bias          B=+1-->Post Shoot        B=-1--> Pre shoot
FC = -1   // Continuity    C=+1-->Inverted corners  C=-1--> Box corners

// When T=B=C=0 this is the Catmul-Rom.
// When T=1 & B=C=0 this is the Simple Cubic.
// When T=B=0 & C=-1 this is the linear interp.

// Here, the three parameters are folded into the basis matrix.  If you want
// independent control of the segment start and end, make two T, C & Bs.
// One for A & B (beginning) and one for C & D (end).  For local control of
// each point, you'll need an array of T, C & Bs for each individual point.
// If you want the local control as shown on the video or in the paper, you
// use the "A" & "B" for the current segment and the "C" & "D" from the
// previous segment.

FFA=(1-FT)*(1+FC)*(1+FB)
FFB=(1-FT)*(1-FC)*(1-FB)
FFC=(1-FT)*(1-FC)*(1+FB)
FFD=(1-FT)*(1+FC)*(1-FB)

// Here, the basis matrix coefficients are defined

DD = 2  //divisor for K-B

Ft1 = -FFA/DD
Ft2 = (+4+FFA-FFB-FFC)/DD
Ft3 = (-4+FFB+FFC-FFD)/DD
Ft4 = FFD/DD

Fu1 = +2*FFA/DD
Fu2 = (-6-2*FFA+2*FFB+FFC)/DD
Fu3 = (+6-2*FFB-FFC+FFD)/DD
Fu4 = -FFD/DD

Fv1 = -FFA/DD
Fv2 = (FFA-FFB)/DD
Fv3 = FFB/DD
Fv4 = 0

Fw1 = 0
Fw2 = +2/DD
Fw3 = 0
Fw4 = 0

NStart = 2    // Start at the first vector
NEnd   = P-1  // End at next to last
NStep  = 1    // Step one at a time
JS     = 0    // Interpolate from zero

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

SplineDo:

// Finally, this is where the rubber meets the road.
// Derived from source code posted by Leon de Boer.

delta2!=delta! * delta!     // Pre-compute delta squared and cubed
delta3!=delta2! * delta!    // These two lines for forward differences only.

// Step through the vectors

FOR n = NStart TO NEnd STEP NStep

// These are the coefficients a, b, c, d, in  aT^3 + bT2 + cT + d
// NOTE: All terms are here, so all types use same code.
// To conserve calculations, terms with a zero in the basis matrix can be
// removed

FAX = Ft1*IT(1,n-1) + Ft2*IT(1,n) + Ft3*IT(1,n+1) + Ft4*IT(1,n+2)
FBX = Fu1*IT(1,n-1) + Fu2*IT(1,n) + Fu3*IT(1,n+1) + Fu4*IT(1,n+2)
FCX = Fv1*IT(1,n-1) + Fv2*IT(1,n) + Fv3*IT(1,n+1) + Fv4*IT(1,n+2)
FDX = Fw1*IT(1,n-1) + Fw2*IT(1,n) + Fw3*IT(1,n+1) + Fw4*IT(1,n+2)

FAY = Ft1*IT(2,n-1) + Ft2*IT(2,n) + Ft3*IT(2,n+1) + Ft4*IT(2,n+2)
FBY = Fu1*IT(2,n-1) + Fu2*IT(2,n) + Fu3*IT(2,n+1) + Fu4*IT(2,n+2)
FCY = Fv1*IT(2,n-1) + Fv2*IT(2,n) + Fv3*IT(2,n+1) + Fv4*IT(2,n+2)
FDY = Fw1*IT(2,n-1) + Fw2*IT(2,n) + Fw3*IT(2,n+1) + Fw4*IT(2,n+2)

// For 3D you will have four more equations here for Z

// Calculate one segment of the interpolation here.
// NOTE: For types which pass through the keys, J can stop at InterPts
// Then just pass the key through (and the first one).

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

FX=FDX    // The first point in the segment
FY=FDY

// Start drawing at the very first point

IF  n = NStart THEN PSET(FX,255-FY)

// Accent the beginning of each segment (knot)
LINE-STEP(2,2) : LINE-STEP(-4,0) : LINE-STEP(2,-2)

// Note, My display is 0-255 high & wide & the origin is in the lower left.

FOR J = JS+1 TO InterPts+1  // Interpolate one segment
T=J/(InterPts+1)            // The interp. parameter steps from delta to 1.0

// The simple polynomial evaluation first pre-computes T^2 & T^3
//   T2 = T * T  :  T3 = T2 * T
// Then the polynomials
//   FX = FAX*T3 + FBX*T2 + FCX*T + FDX
//   FY = FAY*T3 + FBY*T2 + FCY*T + FDY
// However, using Horner's Rule saves calculations & time.

FX = ((FAX*T + FBX)*T + FCX)*T +FDX  // interpolated x value (using Horner)
FY = ((FAY*T + FBY)*T + FCY)*T +FDY  // interpolated y value

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

IF ILines=1 THEN LINE-(FX,255-FY) ELSE PSET(FX,255-FY)  // Draw lines/dots
NEXT    // J   sub-step "between" vectors
NEXT    // n   Move to the next vector set.
                                       Accent the very last point
LINE-STEP(2,2):LINE-STEP(-4,0):LINE-STEP(2,-2)

SplineEnd:
RETURN        //Interpolation complete.  Back to Kansas

