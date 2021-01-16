Unit VESA;
{$A+}           {Enable Word Alignment}
{$G+}           {Enable 80286/80287 Instructions}
{$F+}           {FAR CALLS are ON}
{$S-}           {No Stack Checking}
{$R-}           {No Range Checking}
{$Q-}           {No Overflow Checking}
{$Y-}           {No Symbol Generation}


(**************VESA Unit Written by Romesh Prakashpalan, 1994**************)
(********General Purpose Library for HIGH PERFORMANCE VESA graphics********)
(***********As of version 1.22 only 640x480x256 FULLY supported************)
(*                                                                        *)
(*========================================================================*)
(*                           Introduction                                 *)
(*========================================================================*)
(*       This Unit is a clean break from my GraphHi2 unit, which was VERY *)
(*       messy, and failed at times. The parts that failed were due to the*)
(*       fact that my routines did not adjust for bank granularities that *)
(*       were less than 64K long, plus the bank switching functions for   *)
(*       VESA did not work reliably at times. All of these problems plus  *)
(*       more are fixed in this version, plus support for cards other than*)
(*       VESA were dropped (all of these cards cards can be tricked into  *)
(*       thinking they are a VESA card with a very small TSR). However,   *)
(*       the biggest difference is in that now many KEY routines are      *)
(*       written in ASSEMBLY. This allows for blazing speed (relative) to *)
(*       TP instructions. If you have the source code (only my friends    *)
(*       do), then you will see that there is an enourmous amount of      *)
(*       assembly, where the previous versions didn't have much.          *)
(*                                                                        *)
(* Some brand new procedures (not in GraphHi2) are listed below:          *)
(*========================================================================*)
(*    Procedure Circle (CenterX, CenterY, Radius: Integer; Color: Byte);  *)
(*    Procedure WaitRetrace;                                              *)
(*    Procedure FadeCurs;                                                 *)
(*    Procedure SetDisplay (X, Y: Word);                                  *)
(*========================================================================*)
(* Circle draws a circle centered at CenterX, CenterY, with a Radius of   *)
(* Radius, and a Color of Color. WaitRetrace waits for the vertical       *)
(* retrace of the (S)VGA board to complete, thereby allowing your code to *)
(* set the palette flicker free. FadeCurs fades out a text screen (First  *)
(* time I saw this, I said WOW!, this looks professional (compared to     *)
(* blanking out the text screen immediately). SetDisplay will pan         *)
(* to location X, Y (in any 256 color modes) utilizing hardware panning,  *)
(* it is incredibly smooth, and if one can properly utilize it, it        *)
(* can create breathtaking results (horizontal panning especially)        *)
(*                                                                        *)
(*+----------------------------------------------------------------------+*)
(*|NOTE: When drawing circles, make sure that they do not go off of the  |*)
(*|      screen!, my code does not compensate for such an occurence, so  |*)
(*|      beware!!                                                        |*)
(*+----------------------------------------------------------------------+*)
(* For those of you who have been programming with the Borland BGI        *)
(* routines, in order to convert programs that use the Graph unit, change *)
(* the Uses Graph --> Uses VESA, and change all of the routines in your   *)
(* old code to their equivalents in the VESA library, you should notice a *)
(* tremendous speed increase (unless you are using 256 color fills, which *)
(* take a LOT longer than their 16-color counterparts).                   *)
(* What I personally didn't (and still don't) like about Borland's BGI    *)
(* routines, is that they had a LOT of overhead, and they called the BIOS *)
(* for a few routines. They were also very unoptimized (most people could *)
(* probably write faster code in their sleep...), 256 color support was   *)
(* virtually nil, and VESA modes were a laugh (16-colors at best!). It    *)
(* reportedly takes a much LONGER time to create a BGI file for a certain *)
(* graphics adapter than it would be to program it into a .TPU or .TPP!   *)
(*                                                                        *)
(* Well, that's about it, enjoy...                                        *)
(*                             Romesh Prakashpalan, 1994                  *)
(*                                                                        *)
(* References:                                                            *)
(*   Super VGA Graphics Programming Secrets (By Steve Rimmer):            *)
(*     This is a decent book on how to program SVGA cards in their native *)
(*     and VESA modes. However, he programs in a mixture of C and external*)
(*     assembly, and he doesn't expain too much of what he is doing. Plus,*)
(*     his code is pretty much unoptimized (not too bad though).          *)
(*   VESASP12.TXT:                                                        *)
(*     PCGPE article on programming in VESA modes (I personally didn't use*)
(*     it, but it has the COMPLETE VESA standard in it).                  *)
(*========================================================================*)
(*   Revision History:                                                    *)
(*     Version 0.5à: GraphHi2 (VERY BUGGY and MESSY). Only supported cards*)
(*                   with a 64K bank granularity (BIG mistake!), and also *)
(*                   the bank switching function was faulty! (stoopid     *)
(*                   mistake), it is a piece of SH*T! (04/94). Plus, there*)
(*                   is very little assembly code (only where I NEED it), *)
(*                   and I hope to have this corrected in the next version*)
(*     Version 0.9á: First Release (fixed above bugs), scrapped v0.1à, and*)
(*                   started from scratch (v0.1à had NO comments).        *)
(*                   Unfortunately, I am using Turbo Pascal's MOVE and    *)
(*                   FILLCHAR instructions instead of MOVSB and STOSB.    *)
(*                   This will definitely be fixed in the next release!   *)
(*                   *Note*: I was using an i386DX-25 to test the routines*)
(*                   in Version 0.1à, so I was able to tell the difference*)
(*                   between a QUICK algorithm and a sloooow one very     *)
(*                   easily, but now I upgraded to a PENTIUM, and really, *)
(*                   the only way I can tell if my routines are any faster*)
(*                   is by going over THEORY, not by practice (all the    *)
(*                   damn routines seem to execute at the same speed!)    *)
(*     Version 1.0á: Added Assembly portions to PutPixel routine, and as  *)
(*                   promised, added REAL assembly language routines to   *)
(*                   fix lack of MOVSB and STOSB (got rid of a lot of     *)
(*                   MOVE and FILLCHAR instructions). Also added GetImage *)
(*                   and PutImage, but these still use the TP MOVE command*)
(*                   More and Better palette support routines were added  *)
(*                   as well!                                             *)
(*     Version 1.1a: Added support for Panning, Vertical Retrace, and now *)
(*                   FillRect and ClearScreen moves WORDS instead of BYTES*)
(*     Version 1.2 : Dropped support for 16-bit CPUs (sorry :-( ), and    *)
(*                   PutPixel now supports the 32-bit math for 80386+ CPUs*)
(*                   so it now can scream with the best of 'em, and I also*)
(*                   got SetBank to call the WINFUNPTR instead of doing an*)
(*                   INT 10h (Now, it is much faster)                     *)
(*                   (At this point, I'd be hard pressed to get much more *)
(*                    SIGNIFICANT speed out of this product, so from now  *)
(*                    on, I shall concentrate on adding more features...) *)
(*                   As it stands, it is much faster than the code in     *)
(*                   Steve Rimmer's book: Super VGA programming secrets,  *)
(*                   (But, then again he used a mixture of C and EXTERNAL *)
(*                   16-bit assembly (yuck!)). In fact, much of the code  *)
(*                   was derived from his book, then I put in a few       *)
(*                   optimizations and special features which weren't     *)
(*                   present in his.                                      *)
(*     Version 1.21: A minor upgrade from Version 1.2, a few relatively   *)
(*                   minor optimizations were made, and I added support   *)
(*                   for 2 assembly language procedures which should      *)
(*                   replace the need for using TP's MOVE and FILLCHAR    *)
(*                   instructions (for writing to the screen at least).   *)
(*                   These procedures, while not as fast assembly dropped *)
(*                   right in the middle of your code, is MUCH faster than*)
(*                   the TP Move and FillChar instructions, since they    *)
(*                   move WORDS, and not BYTES. Also, I have created a    *)
(*                   SEPERATE Unit that allows one to view PCX and iRP.   *)
(*                   For those of you who are saying iRP?, what file      *)
(*                   format is that?, well it is a file format created by *)
(*                   ME. Since PCX files take too long to unpack off of   *)
(*                   disk, and have negative compression when dealing with*)
(*                   scanned images, I just created the iRP file format to*)
(*                   get around that (I even created an utility to convert*)
(*                   from PCX->iRP). As of yet, it doesn't support any    *)
(*                   kind of compression algorithm, but there is a lot of *)
(*                   flexibility in the format if I wanted to do so.      *)
(*                   Oh, by the way, GetPixel has FINALLY been rewritten  *)
(*                   in assembly language, so it and PutPixel are finally *)
(*                   at an equal footing. Plus, I got ClearScreen to xfer *)
(*                   double words instead of words (now, it really cooks, *)
(*                   and starts to move my product into the 32-bit        *)
(*                   direction). The next procedure that needs to be      *)
(*                   implemented in 32-bit assembly (IMHO) is FillRect.   *)
(*                   If I get FillRect to write Double Words instead of   *)
(*                   Words, the product will be much faster. If I get this*)
(*                   done, there isn't any other place (that I can see)   *)
(*                   that can benefit from 32-bit assembly (except for    *)
(*                   GetImage and PutImage).                              *)
(*                   I have charted the speed increase of my code (using  *)
(*                   Turbo Profiler, and my code is now approx 45% faster *)
(*                   than version 0.5à!(and 25% faster than Steve Rimmers)*)
(*     Version 1.22: Now, PutImage has been rewritten in assembly (key    *)
(*                   parts only), and GetImage has also been rewritten in *)
(*                   assembly (again, only the key parts).                *)
(*  Future Releases: Now, finally ALL of my routines have assembly where  *)
(*                   they absolutely needed them the most. GetImage and   *)
(*                   PutImage have been rewritten in Version 1.22, so I   *)
(*                   believe that only a few optimizations shall be needed*)
(*                   from now on. Come on, you have to admit that for VESA*)
(*                   modes (hi-res), my routines are pretty fast! (esp.   *)
(*                   since I don't have MULTIPLE PAGES to work with, and  *)
(*                   that I have to switch banks in order to address      *)
(*                   pixels outside of a bank!                            *)
(*                   For many applications however, hi-res VESA Modes are *)
(*                   VERY nice to have (pretty much picture resolutions), *)
(*                   so if you can get away with creating games in such an*)
(*                   environment, do it, (games like Gateway, and The 7th *)
(*                   guest pop up in my mind)                             *)
(*============================Stuff to think about========================*)
(*------------------------------------------------------------------------*)
(*                        One might think that using a 32-bit environment *)
(*                  to access the video card (like 32-bit extenders) might*)
(*                  speed up the code a bit (after all, a 4GB flat address*)
(*                  space is pretty nice), however we can still only SEE  *)
(*                  64K on the card, because the CARD was designed that   *)
(*                  way! So, while other portions of your code might run  *)
(*                  faster (XMS memory access), don't count on the video  *)
(*                  card go any faster, in fact it will go a lot SLOWER,  *)
(*                  because you have to switch from 32-bit mode to 16-bit *)
(*                  mode everytime you want to access the video RAM!      *)
(*------------------------------------------------------------------------*)
(*                  God is REAL unless defined as an INTEGER...           *)
(*                    -The above statement doesn't work in C/C++ (we all  *)
(*                     know that people who program in C are Godless      *)
(*                     heathens! ;-) ).                                   *)
(*------------------------------------------------------------------------*)
(*                  C is more decipherable disassembled than in its source*)
(*                  code form...                                          *)
(*                    -Oh, how true it is!                                *)
(*------------------------------------------------------------------------*)
(*                  Some newbies to computer graphics, see VESA modes as a*)
(*                  better environment for game programming. Sadly, this  *)
(*                  is not always the case. If you are going for that     *)
(*                  Mortal Kombat/Street Fighter/Action Arcade Game look  *)
(*                  and feel, then hi-res VESA modes are not for you, and *)
(*                  you should be looking at Mode-X graphics programming  *)
(*                  instead (320x200x256). Hi-Res VESA modes do not have  *)
(*                  some of the "neat" features that the lower-res modes  *)
(*                  do, such as multiple pages (with 1 visible page), for *)
(*                  fast graphics "flipping", and the ability to write off*)
(*                  of the screen (for scrolling). Instead, the hi-res    *)
(*                  VESA programmer is better off doing programming for   *)
(*                  games more like those found in the Ultima series (up  *)
(*                  to VII part 2), and Sierra type games. Other types of *)
(*                  games require much more speed than VESA modes provide *)
(*                  However, my routines are MUCH faster than ANY code    *)
(*                  written for the BGI routines, so if you had a program *)
(*                  running just fine under them, you will notice an      *)
(*                  ENORMOUS SPEED INCREASE under mine (in some cases, as *)
(*                  high as 1000%! (1 thousand, not a typo)).             *)
(*========================================================================*)
(*                  If you have any suggestions, for future releases,     *)
(*                  e-mail me at hacscb93@hp.csun.edu (until 01/15/95)    *)
(*                  Registration fee is $20 for non-commercial version,   *)
(*                  $50 for the commercial version. *Note: Version 1.22 is*)
(*                  the first to be sent to the public, so whoever reports*)
(*                  a new bug in the code will get their registration free*)
(*                  of charge! Contact me first before paying the         *)
(*                  registration fee (I might have changed my Internet    *)
(*                  address!). Also note that you have to contact me first*)
(*                  telling me what type of bug was detected (I might have*)
(*                  already received a bug report of the same type!)      *)
(*========================================================================*)

(* (By Microsoft standards, perhaps I shall rename this Unit to VESA94 ;-)*)
Unit VESA;
{$A+}           {Enable Word Alignment}
{$G+}           {Enable 80286/80287 Instructions}
{$F+}           {FAR CALLS are ON}
{$S-}           {No Stack Checking}
{$R-}           {No Range Checking}
{$Q-}           {No Overflow Checking}
{$Y-}           {No Symbol Generation}


(**************VESA Unit Written by Romesh Prakashpalan, 1994**************)
(********General Purpose Library for HIGH PERFORMANCE VESA graphics********)
(***********As of version 1.22 only 640x480x256 FULLY supported************)
(*                                                                        *)
(*========================================================================*)
(*                           Introduction                                 *)
(*========================================================================*)
(*       This Unit is a clean break from my GraphHi2 unit, which was VERY *)
(*       messy, and failed at times. The parts that failed were due to the*)
(*       fact that my routines did not adjust for bank granularities that *)
(*       were less than 64K long, plus the bank switching functions for   *)
(*       VESA did not work reliably at times. All of these problems plus  *)
(*       more are fixed in this version, plus support for cards other than*)
(*       VESA were dropped (all of these cards cards can be tricked into  *)
(*       thinking they are a VESA card with a very small TSR). However,   *)
(*       the biggest difference is in that now many KEY routines are      *)
(*       written in ASSEMBLY. This allows for blazing speed (relative) to *)
(*       TP instructions. If you have the source code (only my friends    *)
(*       do), then you will see that there is an enourmous amount of      *)
(*       assembly, where the previous versions didn't have much.          *)
(*                                                                        *)
(* Some brand new procedures (not in GraphHi2) are listed below:          *)
(*========================================================================*)
(*    Procedure Circle (CenterX, CenterY, Radius: Integer; Color: Byte);  *)
(*    Procedure WaitRetrace;                                              *)
(*    Procedure FadeCurs;                                                 *)
(*    Procedure SetDisplay (X, Y: Word);                                  *)
(*========================================================================*)
(* Circle draws a circle centered at CenterX, CenterY, with a Radius of   *)
(* Radius, and a Color of Color. WaitRetrace waits for the vertical       *)
(* retrace of the (S)VGA board to complete, thereby allowing your code to *)
(* set the palette flicker free. FadeCurs fades out a text screen (First  *)
(* time I saw this, I said WOW!, this looks professional (compared to     *)
(* blanking out the text screen immediately). SetDisplay will pan         *)
(* to location X, Y (in any 256 color modes) utilizing hardware panning,  *)
(* it is incredibly smooth, and if one can properly utilize it, it        *)
(* can create breathtaking results (horizontal panning especially)        *)
(*                                                                        *)
(*+----------------------------------------------------------------------+*)
(*|NOTE: When drawing circles, make sure that they do not go off of the  |*)
(*|      screen!, my code does not compensate for such an occurence, so  |*)
(*|      beware!!                                                        |*)
(*+----------------------------------------------------------------------+*)
(* For those of you who have been programming with the Borland BGI        *)
(* routines, in order to convert programs that use the Graph unit, change *)
(* the Uses Graph --> Uses VESA, and change all of the routines in your   *)
(* old code to their equivalents in the VESA library, you should notice a *)
(* tremendous speed increase (unless you are using 256 color fills, which *)
(* take a LOT longer than their 16-color counterparts).                   *)
(* What I personally didn't (and still don't) like about Borland's BGI    *)
(* routines, is that they had a LOT of overhead, and they called the BIOS *)
(* for a few routines. They were also very unoptimized (most people could *)
(* probably write faster code in their sleep...), 256 color support was   *)
(* virtually nil, and VESA modes were a laugh (16-colors at best!). It    *)
(* reportedly takes a much LONGER time to create a BGI file for a certain *)
(* graphics adapter than it would be to program it into a .TPU or .TPP!   *)
(*                                                                        *)
(* Well, that's about it, enjoy...                                        *)
(*                             Romesh Prakashpalan, 1994                  *)
(*                                                                        *)
(* References:                                                            *)
(*   Super VGA Graphics Programming Secrets (By Steve Rimmer):            *)
(*     This is a decent book on how to program SVGA cards in their native *)
(*     and VESA modes. However, he programs in a mixture of C and external*)
(*     assembly, and he doesn't expain too much of what he is doing. Plus,*)
(*     his code is pretty much unoptimized (not too bad though).          *)
(*   VESASP12.TXT:                                                        *)
(*     PCGPE article on programming in VESA modes (I personally didn't use*)
(*     it, but it has the COMPLETE VESA standard in it).                  *)
(*========================================================================*)
(*   Revision History:                                                    *)
(*     Version 0.5à: GraphHi2 (VERY BUGGY and MESSY). Only supported cards*)
(*                   with a 64K bank granularity (BIG mistake!), and also *)
(*                   the bank switching function was faulty! (stoopid     *)
(*                   mistake), it is a piece of SH*T! (04/94). Plus, there*)
(*                   is very little assembly code (only where I NEED it), *)
(*                   and I hope to have this corrected in the next version*)
(*     Version 0.9á: First Release (fixed above bugs), scrapped v0.1à, and*)
(*                   started from scratch (v0.1à had NO comments).        *)
(*                   Unfortunately, I am using Turbo Pascal's MOVE and    *)
(*                   FILLCHAR instructions instead of MOVSB and STOSB.    *)
(*                   This will definitely be fixed in the next release!   *)
(*                   *Note*: I was using an i386DX-25 to test the routines*)
(*                   in Version 0.1à, so I was able to tell the difference*)
(*                   between a QUICK algorithm and a sloooow one very     *)
(*                   easily, but now I upgraded to a PENTIUM, and really, *)
(*                   the only way I can tell if my routines are any faster*)
(*                   is by going over THEORY, not by practice (all the    *)
(*                   damn routines seem to execute at the same speed!)    *)
(*     Version 1.0á: Added Assembly portions to PutPixel routine, and as  *)
(*                   promised, added REAL assembly language routines to   *)
(*                   fix lack of MOVSB and STOSB (got rid of a lot of     *)
(*                   MOVE and FILLCHAR instructions). Also added GetImage *)
(*                   and PutImage, but these still use the TP MOVE command*)
(*                   More and Better palette support routines were added  *)
(*                   as well!                                             *)
(*     Version 1.1a: Added support for Panning, Vertical Retrace, and now *)
(*                   FillRect and ClearScreen moves WORDS instead of BYTES*)
(*     Version 1.2 : Dropped support for 16-bit CPUs (sorry :-( ), and    *)
(*                   PutPixel now supports the 32-bit math for 80386+ CPUs*)
(*                   so it now can scream with the best of 'em, and I also*)
(*                   got SetBank to call the WINFUNPTR instead of doing an*)
(*                   INT 10h (Now, it is much faster)                     *)
(*                   (At this point, I'd be hard pressed to get much more *)
(*                    SIGNIFICANT speed out of this product, so from now  *)
(*                    on, I shall concentrate on adding more features...) *)
(*                   As it stands, it is much faster than the code in     *)
(*                   Steve Rimmer's book: Super VGA programming secrets,  *)
(*                   (But, then again he used a mixture of C and EXTERNAL *)
(*                   16-bit assembly (yuck!)). In fact, much of the code  *)
(*                   was derived from his book, then I put in a few       *)
(*                   optimizations and special features which weren't     *)
(*                   present in his.                                      *)
(*     Version 1.21: A minor upgrade from Version 1.2, a few relatively   *)
(*                   minor optimizations were made, and I added support   *)
(*                   for 2 assembly language procedures which should      *)
(*                   replace the need for using TP's MOVE and FILLCHAR    *)
(*                   instructions (for writing to the screen at least).   *)
(*                   These procedures, while not as fast assembly dropped *)
(*                   right in the middle of your code, is MUCH faster than*)
(*                   the TP Move and FillChar instructions, since they    *)
(*                   move WORDS, and not BYTES. Also, I have created a    *)
(*                   SEPERATE Unit that allows one to view PCX and iRP.   *)
(*                   For those of you who are saying iRP?, what file      *)
(*                   format is that?, well it is a file format created by *)
(*                   ME. Since PCX files take too long to unpack off of   *)
(*                   disk, and have negative compression when dealing with*)
(*                   scanned images, I just created the iRP file format to*)
(*                   get around that (I even created an utility to convert*)
(*                   from PCX->iRP). As of yet, it doesn't support any    *)
(*                   kind of compression algorithm, but there is a lot of *)
(*                   flexibility in the format if I wanted to do so.      *)
(*                   Oh, by the way, GetPixel has FINALLY been rewritten  *)
(*                   in assembly language, so it and PutPixel are finally *)
(*                   at an equal footing. Plus, I got ClearScreen to xfer *)
(*                   double words instead of words (now, it really cooks, *)
(*                   and starts to move my product into the 32-bit        *)
(*                   direction). The next procedure that needs to be      *)
(*                   implemented in 32-bit assembly (IMHO) is FillRect.   *)
(*                   If I get FillRect to write Double Words instead of   *)
(*                   Words, the product will be much faster. If I get this*)
(*                   done, there isn't any other place (that I can see)   *)
(*                   that can benefit from 32-bit assembly (except for    *)
(*                   GetImage and PutImage).                              *)
(*                   I have charted the speed increase of my code (using  *)
(*                   Turbo Profiler, and my code is now approx 45% faster *)
(*                   than version 0.5à!(and 25% faster than Steve Rimmers)*)
(*     Version 1.22: Now, PutImage has been rewritten in assembly (key    *)
(*                   parts only), and GetImage has also been rewritten in *)
(*                   assembly (again, only the key parts).                *)
(*  Future Releases: Now, finally ALL of my routines have assembly where  *)
(*                   they absolutely needed them the most. GetImage and   *)
(*                   PutImage have been rewritten in Version 1.22, so I   *)
(*                   believe that only a few optimizations shall be needed*)
(*                   from now on. Come on, you have to admit that for VESA*)
(*                   modes (hi-res), my routines are pretty fast! (esp.   *)
(*                   since I don't have MULTIPLE PAGES to work with, and  *)
(*                   that I have to switch banks in order to address      *)
(*                   pixels outside of a bank!                            *)
(*                   For many applications however, hi-res VESA Modes are *)
(*                   VERY nice to have (pretty much picture resolutions), *)
(*                   so if you can get away with creating games in such an*)
(*                   environment, do it, (games like Gateway, and The 7th *)
(*                   guest pop up in my mind)                             *)
(*============================Stuff to think about========================*)
(*------------------------------------------------------------------------*)
(*                        One might think that using a 32-bit environment *)
(*                  to access the video card (like 32-bit extenders) might*)
(*                  speed up the code a bit (after all, a 4GB flat address*)
(*                  space is pretty nice), however we can still only SEE  *)
(*                  64K on the card, because the CARD was designed that   *)
(*                  way! So, while other portions of your code might run  *)
(*                  faster (XMS memory access), don't count on the video  *)
(*                  card go any faster, in fact it will go a lot SLOWER,  *)
(*                  because you have to switch from 32-bit mode to 16-bit *)
(*                  mode everytime you want to access the video RAM!      *)
(*------------------------------------------------------------------------*)
(*                  God is REAL unless defined as an INTEGER...           *)
(*                    -The above statement doesn't work in C/C++ (we all  *)
(*                     know that people who program in C are Godless      *)
(*                     heathens! ;-) ).                                   *)
(*------------------------------------------------------------------------*)
(*                  C is more decipherable disassembled than in its source*)
(*                  code form...                                          *)
(*                    -Oh, how true it is!                                *)
(*------------------------------------------------------------------------*)
(*                  Some newbies to computer graphics, see VESA modes as a*)
(*                  better environment for game programming. Sadly, this  *)
(*                  is not always the case. If you are going for that     *)
(*                  Mortal Kombat/Street Fighter/Action Arcade Game look  *)
(*                  and feel, then hi-res VESA modes are not for you, and *)
(*                  you should be looking at Mode-X graphics programming  *)
(*                  instead (320x200x256). Hi-Res VESA modes do not have  *)
(*                  some of the "neat" features that the lower-res modes  *)
(*                  do, such as multiple pages (with 1 visible page), for *)
(*                  fast graphics "flipping", and the ability to write off*)
(*                  of the screen (for scrolling). Instead, the hi-res    *)
(*                  VESA programmer is better off doing programming for   *)
(*                  games more like those found in the Ultima series (up  *)
(*                  to VII part 2), and Sierra type games. Other types of *)
(*                  games require much more speed than VESA modes provide *)
(*                  However, my routines are MUCH faster than ANY code    *)
(*                  written for the BGI routines, so if you had a program *)
(*                  running just fine under them, you will notice an      *)
(*                  ENORMOUS SPEED INCREASE under mine (in some cases, as *)
(*                  high as 1000%! (1 thousand, not a typo)).             *)
(*========================================================================*)
(*                  If you have any suggestions, for future releases,     *)
(*                  e-mail me at hacscb93@hp.csun.edu (until 01/15/95)    *)
(*                  Registration fee is $20 for non-commercial version,   *)
(*                  $50 for the commercial version. *Note: Version 1.22 is*)
(*                  the first to be sent to the public, so whoever reports*)
(*                  a new bug in the code will get their registration free*)
(*                  of charge! Contact me first before paying the         *)
(*                  registration fee (I might have changed my Internet    *)
(*                  address!). Also note that you have to contact me first*)
(*                  telling me what type of bug was detected (I might have*)
(*                  already received a bug report of the same type!).     *)
(*                  Note: Information IS FREE! Just not the whole         *)
(*                  source code (so contact me if you need any details on *)
(*                  VESA programming!)                                    *)
(*========================================================================*)

(* (By Microsoft standards, perhaps I shall rename this Unit to VESA94 ;-)*)

interface
const
  MaskTable: Array [1..8] of Byte = (128, 64, 32, 16, 8, 4, 2, 1);
 (* The above mask is used for interfacing to the char set in the VGA BIOS *)
type
  VESAInfoPtr = ^VesaInfo;
  VESAInfo =  Record
                ModeAttributes: Word;
                WindowAttr_A: Byte;
                WindowAttr_B: Byte;
                Granularity: Word;
                WindowSize: Word;
                WindowSegment_A: Word;
                WindowSegment_B: Word;
                WindowPtr: Procedure;
                BytesPerLine: Word;
                XRes: Word;
                YRes: Word;
                XCharSize: Byte;
                YCharSize: Byte;
                NumberofPlanes: Byte;
                Bitsperpixel: Byte;
                Banks: Byte;
                MemoryModel: Byte;
                BankSize: Byte;
                ImagePages: Byte;
                Reserved1: Byte;
                RedMask: Byte;
                RedField: Byte;
                GreenMask: Byte;
                GreenField: Byte;
                BlueMask: Byte;
                BlueField: Byte;
                ResMask: Byte;
                ResField: Byte;
                DirectColor: Byte;
                Reserved: Array [1..216] of Byte;
              end;

  VESABlock = Record
                VesaSig: Array [1..4] of Char;
                Version: Word;
                OemString: PChar;
                Capabilities: Array [1..4] of Char;
                VideoMode: ^Word;
                Memory: Word;
                Reserved: Array [1..242] of Char;
              end;

 ColorType = Record
                R, G, B : Byte;
             end;

 PaletteType = Array[0..255] of ColorType;

 BytePtr     = ^Byte;

 ImageType   = Record
                 Width: Word;
                 Depth: Word;
                 Buffer: BytePtr;
               end;

var
  TempVI: VesaInfo;
  VB: VesaBlock;
  (* Assumes Screen is 480 pixels deep...                       *)
  ScreenTable: Array [0..479] of Record
                                   Offset: Word;
                                   PageNumber: Byte;
                                   Flag: Byte;
                                 end;

(* Variables used primarily for ASM procedures                  *)
  WINSIZE: Word;
  WINGRAN: Word;
  BANKSHIFT: WORD;
  BYTESPERLINE: WORD;
  BANKSIZE: WORD;
  XRES, YRES: WORD;
  WINFUNPTR: PROCEDURE;
  FONTSEG: WORD;
  FONTOFS: WORD;
  PALETTE: PALETTETYPE;
  const
    CODEPAGE: WORD = 0;


(* VESA Mode functions (and system calls)                               *)
Procedure SetText;
Procedure ChangeMode (Mode: Word);
Procedure WaitRetrace;
Procedure SetBank (BANKNUMBER: WORD);
Procedure SetDisplay (X, Y: Integer);

(* Pixel related functions:                                             *)
Procedure Putpixel (X,Y : Integer; Color : Byte);
Function  Getpixel (X,Y : Integer): Byte;

(* Line Drawing, Circle and Box functions:                              *)
Procedure Line (Left, Top, Right, Bottom: Integer; Color: Byte);
Procedure Circle (CenterX, CenterY, Radius: Integer; Color: Byte);
Procedure FillRect (X1, Y1, X2, Y2: Integer; Color: Byte);
Procedure FrameRect (X1, Y1, X2, Y2: Integer; Color: Byte);

(* Palette related procedures:                                          *)
Procedure SetPalette(var Palette : PaletteType);
Procedure GetPalEntry (Color : Byte; var R, G, B : Byte);
Procedure SetPalEntry (Color, R, G, B : Byte);
Procedure FadeDown;
Procedure Fadeup (Pall: PaletteType);
Procedure BlackOut;
Procedure LoadDefaultPalette;
Procedure LoadPalette1;
Procedure LoadPalette2;
Procedure FadeCurs;

(* Graphics Text functions:                                             *)
Procedure PutChar (X, Y: Integer; Ch: Char; Color: Byte);
Procedure WriteString (X, Y: Integer; S: String; Color: Byte);

(* Image Buffer techniques:                                             *)
Function  ImageSize (X1, Y1, X2, Y2: Integer): LongInt;
Procedure GetImage (var Image: ImageType; X1, Y1, X2, Y2: Integer);
Procedure PutImage (Image: ImageType; X1, Y1: Integer);

(* Image filling techniques:                                            *)
Procedure MoveVideoFast (var Buffer; VOffset, Length: Word);
Procedure FillVideoFast (VOffset, Length: Word; Value: Byte);

(* Screen Clearing functions:                                           *)
Procedure ClearScreen (Color: Byte);

implementation
