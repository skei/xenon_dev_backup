Det som er i EXE'n n†:

þ Extender
þ Std
þ Mxm player
þ data (module)
þ VBE 2.0 routines
þ MPG single frame decoder
þ Data file packing (LZARI w/modifications)
þ routines
  - polyfiller
  - random
  - Texture creator
    - plasma/noise/turbulence
    - hexagon
  - sphere mapping
  - rotozoom
  - skurr/static
  - voxel

EXE pakket med PKLITE.


-----------------------------------------------------------------------------

Some words about the code:

Not all of this is optimized. A lot of the stuff is the result of various
experiments and testings... When the ime came to put together the intro, I
just used the latest working version of the routines and code...

DOS extender
  A very small, DPMI only extender, for Watcom. Based on the XIDS package
  by Maxwell Sayles, but modified a LOT - so much that I now can call it
  entirely my own. Most of the code size is replacements for the Watcom
  std libs.

Compression
  Two types of compression have been used. LZARI for lossless compression,
  and an MPEG decoder (single frame MPG's) for lossy compression

SoftSynth
  To further reduce the intro size, I coded a small soft-synth thing, to
  synthesize the samples instead of including them with the module.

Texture Synthesis
  Just like the SoftSynth but for textures and other bitmaps. A lot of
  different textures can be made, and processed with stuff like noise,
  turbulence, marble, etc, etc

Player
  For the 64k version, the MXM player has been used, and modified to work
  with the DOS extender

In addition to this, there are the effects/routines themselves:

MetaBalls
  An implicit surface routine, with surface crawling

Raytracing
  A few tricks have been used to speed up the tracing.

4D Quaternion Julia fractal
  Major math stuff here!!

True Mandelbrot zoomer
  No cheating like calculating the 0th frame, and zooming the images
  between the calculated ones here... This is real stuff! The main problem
  is the precision of the FPU. Even when I tried using doubles instead of
  floats, etc, the calculated bitmap shaked like hell when zooming in a
  LOT.

Particle/string systems
  Various particle systems - some physics - some springs, mass, wind,
  gravity, etc... And polygonizing final result...

And the usual demo-effects...

If anyone is interested in anything of the above, or want to discuss
vaious code-related stuff, or is interested in the source (I won't give it
all out freely!!!), email me...
