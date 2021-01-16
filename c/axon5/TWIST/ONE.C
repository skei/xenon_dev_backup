/*

  This is the main source of Shock!'s Flag96 party contribution demo entitled:

                             ú úúùùùOùNùEùùùúú ú

  Remember, if you modify something in the sources, and you compile it with
  the ass.bat, it's gonna hang, because the music loader routines  need the
  correct length of the exe file, so you gotta modify  those pointers which
  hold the music_offsets (got the point?)...

  have phun, but don't rip too much ;))

  GENESIS & TSC

*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <conio.h>
#include <io.h>
#include <dos.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "\wc\myh\macro.h"
#include "c\loader3.c"
#include "c\motionbl.c"
#include "c\tunnel.c"
#include "c\sive.c"
#include "c\pal.c"
#include "c\twist.c"

#pragma aux tunnel1 "_*" parm caller [];
extern void tunnel1(void);
#pragma aux tunnel2 "_*" parm caller [];
extern void tunnel2(void);
#pragma aux scroller "_*" parm caller [];
extern void scroller(void);
#pragma aux julia "_*" parm caller [];
extern void julia(byte *lofasz);
#pragma aux julia2 "_*" parm caller [];
extern void julia2(byte *lofasz);
#pragma aux Effect1Init "_*" parm caller [];
extern void Effect1Init(void);
#pragma aux Effect1 "_*" parm caller [];
extern void Effect1(void);
#pragma aux Effect2Init "_*" parm caller [];
extern void Effect2Init(void);
#pragma aux Effect2 "_*" parm caller [];
extern void Effect2(void);
#pragma aux Effect3Init "_*" parm caller [];
extern void Effect3Init(void);
#pragma aux Effect3 "_*" parm caller [];
extern void Effect3(void);
#pragma aux Effect4Init "_*" parm caller [];
extern void Effect4Init(void);
#pragma aux Effect4 "_*" parm caller [];
extern void Effect4(void);
#pragma aux SP_SETCALLBACK "_*" parm caller [];
extern void SP_SETCALLBACK(dword whattocall());
#pragma aux Plazmabug "_*" parm caller [];
extern void Plazmabug(byte *where);
#pragma aux Plazmabug2 "_*" parm caller [];
extern void Plazmabug2(byte *where);
#pragma aux Plazmabug3 "_*" parm caller [];
extern void Plazmabug3(byte *where);
#pragma aux Plazmabug4 "_*" parm caller [];
extern void Plazmabug4(byte *where);
#pragma aux dixymainloop "_*" parm caller [];
extern void dixymainloop(void);
#pragma aux rangatas "_*" parm caller [];
extern void rangatas(void);
#pragma aux szovegkirako "_*" parm caller [];
extern void szovegkirako(byte *fromwhere);
#pragma aux szovegkirako2 "_*" parm caller [];
extern void szovegkirako2(dword desvar);
#pragma aux szovegkirako3 "_*" parm caller [];
extern void szovegkirako3(byte *fromwhere);
#pragma aux szovegkirako4 "_*" parm caller [];
extern void szovegkirako4(dword desvar);
#pragma aux puttimer "_*" parm caller [];
extern void puttimer(byte *towhere,dword color);
#pragma aux dectimer "_*" parm caller [];
extern void dectimer(void);
#pragma aux designeffect6 "_*" parm caller [];
extern void designeffect6(void);
extern word ory1;
extern word orx1;
extern word rzoom;
extern dword CReal;
extern dword CImag;
extern byte tunincer;

	byte	*pic1, *pic2, *pic3, *pic4, *pic5, *pic6, *pic7, *pic8;
        byte    *pic9, *pic10, *pic11, *pic12, *pic13;
	byte	*memtestptr, *d61, *d62;
	byte	*pal1, *pal2, *pal3, *pal4, *pal5, *pal6;
	byte	*modplace, *screenplace;
	byte	*fontbuffer,  *scrollbuffer, *fontpal;
	byte	*texturebuff, *texturepal,   *screenbuff;
	dword	desvar, n,u,m, oldpos, scrollend, actrow;
	sdword	sintx[320],z;
	float	flag,k;

#include "c\sp32.h"
#include "c\getenv.c"


void AllocateMem() {
	if ((modplace  = (byte *)malloc(788000)) == NULL) ExitProcess2();
	if ((pic1      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic2      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic3      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic4      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic5      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic6      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic7      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic8      = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic9      = (byte *)malloc(100000)) == NULL) ExitProcess2();
	if ((pic10     = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic11     = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pic12     = (byte *)malloc(65536)) == NULL) ExitProcess2();
        if ((pic13     = (byte *)malloc(65536)) == NULL) ExitProcess2();
	if ((pal1      = (byte *)malloc(768)) == NULL) ExitProcess2();
	if ((pal2      = (byte *)malloc(768)) == NULL) ExitProcess2();
	if ((pal3      = (byte *)malloc(768)) == NULL) ExitProcess2();
	if ((pal4      = (byte *)malloc(768)) == NULL) ExitProcess2();
	if ((pal5      = (byte *)malloc(768)) == NULL) ExitProcess2();
	if ((pal6      = (byte *)malloc(768)) == NULL) ExitProcess2();

	if ((memtestptr= (byte *)malloc(300000)) == NULL) ExitProcess2();
	free (memtestptr);

	for (n = 0; n < 788000; n++)   {*(modplace + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic2 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic3 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic4 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic5 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic6 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic7 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic8 + n) = 0x00;}
	for (n = 0; n < 100000; n++)   {*(pic9 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic10 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic11 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic12 + n) = 0x00;}
        for (n = 0; n < 65536; n++)   {*(pic13 + n) = 0x00;}
	for (n = 0; n <   768; n++)   {*(pal1 + n) = 0x00;}
	for (n = 0; n <   768; n++)   {*(pal2 + n) = 0x00;}
	for (n = 0; n <   768; n++)   {*(pal3 + n) = 0x00;}
	for (n = 0; n <   768; n++)   {*(pal4 + n) = 0x00;}
	for (n = 0; n <   768; n++)   {*(pal5 + n) = 0x00;}
	for (n = 0; n <   768; n++)   {*(pal6 + n) = 0x00;}
	}

void DeAllocateMem() {
	free(modplace);
	free(pic1);
	free(pic2);
	free(pic3);
	free(pic4);
	free(pic5);
	free(pic6);
	free(pic7);
	free(pic8);
	free(pic9);
	free(pic10);
	free(pic11);
	free(pic12);
        free(pic13);
	free(pal1);
	free(pal2);
	free(pal3);
	free(pal4);
	free(pal5);
	free(pal6);
	}

#include "c\vectsys.c"

void ExitProcess() {
	if (getch()==27)
		{
		set80x25x16();
		SP_QUIET=0;
                stopmusic();
		DeAllocateMem();
		printf("DEMO_DEINIT: RELEASING MEMORY\n");
		exit(0);
		}
	}

void gensintables(void)
{
	dword d;
	float k;
	float f;
	dword l;
	dword m;

	f=flag/14;
	for(d=0; d<320; d++)
	{
		k=d;
		sintx[d]=sin(k/((10*sin(cos(f*10)/15))-f))*11;
	}
}

void ExitProcess2() {
	printf ("DEMO_INIT: NOT ENOUGH MEMORY!\n");
	SP_QUIET=0;
        stopmusic();
	DeAllocateMem();
	printf("DEMO_DEINIT: RELEASING MEMORY\n");
	exit(0);
	}

/****** System initialization ************************************************/

void main(void) {

        headermessy();
	printf("\nDEMO_INIT: ALLOCATING MEMORY (8 MBYTES REQUIRED!)\n");
	AllocateMem();
	printf("DEMO_INIT: INITIALIZING VIDEO SUBSYSTEM\n");
	siveinit();

/****** Effect: kezdodik a moka **********************************************/

	printf("DEMO_INIT: LOADING GRAPHICS\n");
        loader(0,angrad);
	loader(20,angrad2);
        loader(1,pic8);
	loader(17,pic4);		// Loading the credit pics
	loader(18,pic5);
	loader(19,pic6);
	loader(8,pic7); 		// Loading the ONE pic
        loader(9,pal6);
	loader(14,pic9+18000);		// Loading the REDGREEN.LBM
	loader(6,blurcolors);		// For Kewl Forgo Genya1
	loader(10,blurcolors3); 	// For Kewl Forgo Genya2
	loader(12,blurcolors4); 	// For Kewl Forgo Genya3
	loader(23,pic1);		// For the first SHOCK! logo
	loader(24,pal1);		// For the first SHOCK! logo
        loader(25,pic11);               // Loading the MURDER.DAT
        loader(26,pal5);                // Loading the MURDER.PAL
        loader(27,pic12);               // Message for repulo
        loader(22,vct);                 // Loading vector part 1
        loader(28,pic13);               // Loading the JOKER.DAT
        loader(31,blurcolors5);         // Loading the varos.mot
        getgusport();
        startmusic();
	SP_SETCALLBACK(&dectimer);

/****** Effect: Bejon az elso SHOCK! logo ************************************/

	set320x200x256();
	setpal8(pal1);
	oldpos=position;
	screenplace=0x0a0000;
	d62=pic3;
	d61=pic2;
	m=0;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 designeffect6();
			 m++;
			 disp320x200x256(d62);
			} while (m!=199);
	m=0;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 rasterwait();
			 disp320x200x256(d62);
			} while (position!=oldpos+4);

/****** Effect: Felvillan az ONE felirat *************************************/

	set320x200x256();
	loader(7,pal1);
	setpal8(pal1);
	texture=pic8;
	vmem=pic1;
	oldpos=position;
        do              {
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 tunnel2();
			 szovegkirako3(pic7);
			 puttimer(pic1,50);
			 disp320x200x256(pic1);
			} while (position!=oldpos+1);

/****** Effect: Kewl forgo genya 1 *******************************************/

	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic2 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic3 + n) = 0x00;}
	loader(7,pal1);
	setpal8(pal1);
	Effect2Init();;
	InitBlur();
	oldpos=position;

	do		{
                         if (SP_ACTROW == 2)  { ory1=16384; orx1=0;}
			 if (SP_ACTROW == 8)  { ory1=0; orx1=16384;}
			 if (SP_ACTROW == 16) { ory1=32768; orx1=0;}
			 if (SP_ACTROW == 24) { ory1=16384; orx1=32768;}
			 if (SP_ACTROW == 32) { ory1=0; orx1=0;}
			 if (SP_ACTROW == 40) { ory1=32768; orx1=16384;}
			 if (SP_ACTROW == 48) { ory1=16384; orx1=40000;}
			 if (SP_ACTROW == 56) { ory1=40000; orx1=0;}
			 if (SP_ACTROW > 62)  { ory1=16384; orx1=20000;}

			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 Effect2();
			 julia2(pic2);
			 Plazmabug(pic1);
			 motionblur(pic2);
			 puttimer(toscreen,50);
			 disp320x200x256(toscreen);
			} while (position!=oldpos+3);

/****** Effect: Kewl forgo genya 2 *******************************************/

	set320x200x256();
	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic2 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic3 + n) = 0x00;}
	loader(11,pal1);
	loader(16,pic3);
	setpal8(pal1);
	Effect3Init();;
	InitBlur();
	oldpos=position;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 Effect3();
                         Plazmabug4(pic1);
			 motionblur3(pic2);
			 puttimer(toscreen,83);
			 disp320x200x256(toscreen);
			} while (position!=oldpos+1);
	oldpos=position;
	desvar=1;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
                         if (SP_ACTROW == 2) desvar=320*77;
			 if (SP_ACTROW == 16) desvar=320*77;
			 if (SP_ACTROW == 24) desvar=320*77;
			 if (SP_ACTROW == 36) desvar=320*77;
			 if (SP_ACTROW == 48) desvar=320*77;
			 if (SP_ACTROW == 56) desvar=320*77;
			 Effect3();
			 Plazmabug(pic1);
			 motionblur3(pic2);
			 szovegkirako2(desvar);
			 desvar=320*1;
			 puttimer(toscreen,83);
			 disp320x200x256(toscreen);
			} while (position!=oldpos+2);

/****** Effect: Megint Felvillan az ONE felirat ******************************/

	set320x200x256();
	loader(7,pal1);
	setpal8(pal1);
	texture=pic8;
	vmem=pic1;
	oldpos=position;

	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 tunnel2();
			 szovegkirako3(pic7);
			 puttimer(pic1,50);
			 disp320x200x256(pic1);
			} while (position!=oldpos+1);


/****** Effect: Kewl forgo genya 3 *******************************************/

	set320x200x256();
	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic2 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic3 + n) = 0x00;}
	loader(13,pal1);
	setpal8(pal1);
	Effect4Init();;
	InitBlur();
	oldpos=position;
	do		{
			 if (kbhit()) ExitProcess();
                         if (SP_ACTROW == 2)  szovegkirako(pic4);
			 if (SP_ACTROW == 16) szovegkirako(pic5);
			 if (SP_ACTROW == 24) szovegkirako(pic6);
			 if (SP_ACTROW == 36) szovegkirako(pic4);
			 if (SP_ACTROW == 48) szovegkirako(pic5);
			 if (SP_ACTROW == 56) szovegkirako(pic6);
			 UpdateInfo();
			 Effect4();
			 Plazmabug2(pic1);
			 motionblur4(pic2);
			 puttimer(toscreen,210);
			 disp320x200x256(toscreen);
			} while (position!=oldpos+4);

/****** Effect: Bekocsonyazodik a RedGreen.lbm *******************************/

	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic2 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic3 + n) = 0x00;}
	set320x200x256();
	texturebuff=pic9+18000;
	texturepal=pal1;
	screenbuff=pic2;
	loader(15,texturepal);
	setpal8(texturepal);
	flag=0;
	gensintables();
	oldpos=position;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 dixymainloop();
			 puttimer(screenbuff,50);
			 rasterwait();
			 disp320x200x256(screenbuff);
			 flag++;
			 gensintables();
			} while (position!=oldpos+2);

/****** Effect: Megjon a Redgreen.lbm normalisan *****************************/

	for (n = 0; n < 768; n++)   {*(pal2 + n) = 63;}
	setpal8(pal2);
	disp320x200x256(pic9+18000);
	oldpos=position;
	do		{
			 if (kbhit()) ExitProcess();
			 palmorph(pal2,pal1);
			 setpal8(pal2);
			 rasterwait();
			 disp320x200x256(pic9+18000);
			 puttimer(screenplace,50);
			 UpdateInfo();
			} while (position!=oldpos+2);
	n=0;
	m=1;
	u=1;
	oldpos=position;
	rzoom=0;
	do		{
			 if (kbhit()) ExitProcess();
			 n=SP_ACTROW;
			 if (SP_ACTROW == 4) {m=m+2;n++;}
			 if (SP_ACTROW ==16) {m=m+2;n++;}
			 if (SP_ACTROW ==32) {m=m+2;n++;}
			 if (SP_ACTROW ==48) {m=m+2;n++;}
			 u=-u;
			 rzoom=320*((rand()/(32768/m)));
			 rasterwait();
			 disp320x200x256(pic9+18000+rzoom);
			 puttimer(screenplace,50);
			 UpdateInfo();
			} while (position!=oldpos+2);

/****** Effect: M r Megint Felvillan az ONE felirat **************************/

	set320x200x256();
	loader(7,pal1);
	setpal8(pal1);
	texture=pic8;
	vmem=pic1;
	oldpos=position;

	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 tunnel2();
			 szovegkirako3(pic7);
			 puttimer(pic1,50);
			 disp320x200x256(pic1);
			} while (position!=oldpos+1);

/****** Effect: erdekes twister **********************************************/


        loader(21,pal1);
	set320x200x256();
	setpal8(pal1);
	twistinit();
	twistsource=pic1;
	twisttarget=pic2;
	maindegree=0;
	desvar=1;
	u=1;
	n=0;
	oldpos=position;
	InitBlur();
	maindegree=u*sind[n];
	maindegree=u*cosd[n]+maindegree;

	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 m=0;
                         if (SP_ACTROW == 0)  {m=10000;}
			 if (SP_ACTROW == 8)  {m=-10000;}
			 if (SP_ACTROW == 16) {m=10000;}
			 if (SP_ACTROW == 24) {m=-10000;}
			 if (SP_ACTROW == 32) {m=10000;}
			 if (SP_ACTROW == 40) {m=-10000;}
			 if (SP_ACTROW == 48) {m=10000;}
			 if (SP_ACTROW == 56) {m=-10000;}
                         if (SP_ACTROW == 1)  {m=10000;}
                         if (SP_ACTROW == 9)  {m=-10000;}
                         if (SP_ACTROW == 17) {m=10000;}
                         if (SP_ACTROW == 25) {m=-10000;}
                         if (SP_ACTROW == 33) {m=10000;}
                         if (SP_ACTROW == 41) {m=-10000;}
                         if (SP_ACTROW == 49) {m=10000;}
                         if (SP_ACTROW == 57) {m=-10000;}
			 maindegree=maindegree+m;
			 twist();
			 Plazmabug3(pic1);
                         puttimer(pic2,160);
			 rasterwait();
                         disp320x200x256(pic2);
			} while (position!=oldpos+2);
	oldpos=position;
	n=0;
	m=0;
	u=2;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 m=0;
                         if (SP_ACTROW == 0)  {u=-u;m=5000;}
			 if (SP_ACTROW == 8)  {u=-u;m=5000;}
			 if (SP_ACTROW == 16) {u=-u;m=5000;}
			 if (SP_ACTROW == 24) {u=-u;m=5000;}
			 if (SP_ACTROW == 32) {u=-u;m=5000;}
			 if (SP_ACTROW == 40) {u=-u;m=5000;}
			 if (SP_ACTROW == 48) {u=-u;m=5000;}
			 if (SP_ACTROW == 56) {u=-u;m=5000;}
                         if (SP_ACTROW == 1)  {u=-u;m=5000;}
                         if (SP_ACTROW == 9)  {u=-u;m=5000;}
                         if (SP_ACTROW == 17) {u=-u;m=5000;}
                         if (SP_ACTROW == 25) {u=-u;m=5000;}
                         if (SP_ACTROW == 33) {u=-u;m=5000;}
                         if (SP_ACTROW == 41) {u=-u;m=5000;}
                         if (SP_ACTROW == 49) {u=-u;m=5000;}
                         if (SP_ACTROW == 57) {u=-u;m=5000;}
			 maindegree=u*sind[n];
			 maindegree=u*cosd[n]+maindegree+m;
                         n=n+2;
			 if (n==128) n=0;
			 twist();
                         puttimer(pic2,160);
			 rasterwait();
                         disp320x200x256(pic2);
			} while (position!=oldpos+2);


/****** Effect: julia morpher + tunnel ***************************************/

	loader(2,pal1);
	set320x200x256();
	setpal8brighten(pal1);
	texture=pic8;
	vmem=pic2;
	oldpos=position;
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
                         if (SP_ACTROW == 0)  {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
			 if (SP_ACTROW == 8)  {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
			 if (SP_ACTROW == 16) {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
			 if (SP_ACTROW == 24) {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
			 if (SP_ACTROW == 32) {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
			 if (SP_ACTROW == 40) {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
			 if (SP_ACTROW == 48) {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
			 if (SP_ACTROW == 56) {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
                         if (SP_ACTROW == 1)  {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
                         if (SP_ACTROW == 9)  {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
                         if (SP_ACTROW == 17) {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
                         if (SP_ACTROW == 25) {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
                         if (SP_ACTROW == 33) {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
                         if (SP_ACTROW == 41) {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
                         if (SP_ACTROW == 49) {CReal=CReal+400;CImag=CImag+400;tunincer=-tunincer;}
                         if (SP_ACTROW == 57) {CReal=CReal-400;CImag=CImag-400;tunincer=-tunincer;}
			 tunnel1();
			 julia(vmem);
			 puttimer(vmem,30);
			 rasterwait();
			 disp320x200x256(vmem);
			} while (position!=oldpos+4);

/****** Effect: Bejon a MURDER picture ***************************************/

	set320x200x256();
	oldpos=position;
	screenplace=0x0a0000;
	for (n = 0; n < 768; n++)   {*(pal2 + n) = 63;}
	setpal8(pal2);
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 rasterwait();
			 palmorph(pal2,pal5);
			 setpal8(pal2);
			 disp320x200x256(pic11);
			 puttimer(screenplace,10);
			} while (position!=oldpos+1);

/****** Effect: nyomorult repulo *********************************************/

        vectorpart1();

/****** Effect: M r Megint Felvillan az ONE felirat **************************/

	set320x200x256();
	loader(7,pal1);
	setpal8(pal1);
	texture=pic8;
	vmem=pic1;
	oldpos=position;

	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 tunnel2();
			 szovegkirako3(pic7);
			 puttimer(pic1,50);
			 disp320x200x256(pic1);
			} while (position!=oldpos+1);

/****** Effect: forgo varosmicsoda *******************************************/

        vectorpart2();

/****** Effect: Bejon a JOKER picture ****************************************/

	set320x200x256();
        loader(29,pal1);
        oldpos=position;
	screenplace=0x0a0000;
	for (n = 0; n < 768; n++)   {*(pal2 + n) = 63;}
	setpal8(pal2);
	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 rasterwait();
                         palmorph(pal2,pal1);
			 setpal8(pal2);
                         disp320x200x256(pic13);
			} while (position!=oldpos+1);
        for (n = 0; n < 768; n++)   {*(pal2 + n) = 0;}
        for (n = 0; n < 63; n++)
			{
			 if (kbhit()) ExitProcess();
			 rasterwait();
                         palmorph(pal1,pal2);
                         setpal8(pal1);
                         disp320x200x256(pic13);
                        }

/****** Effect: endscroller **************************************************/

	SP_QUIET=1;
	stopmusic();
	startmusic2();
	SP_MASTERVOLUME=50;

	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic2 + n) = 0x00;}
	for (n = 0; n < 65536; n++)   {*(pic3 + n) = 0x00;}
	fontbuffer=pic3;
	fontpal=pal1;
	loader(3,fontbuffer);
	loader(4,blurcolors);
	loader(5,fontpal);
	scrollbuffer=pic1;
	scrollend=0;
	set320x200x256();
	setpal8(pal1);
	Effect1Init();
	Initblur();
	oldpos=position;

	do		{
			 if (kbhit()) ExitProcess();
			 UpdateInfo();
			 Effect1();
			 scroller();
			 motionblur(pic2);
			 rasterwait();
			 disp320x200x256(toscreen);
			} while (scrollend!=1);

	for (n = 0; n < 65536; n++)   {*(pic1 + n) = 0x00;}
	for (n = 0; n < 50; n++)
			{
			 SP_MASTERVOLUME=SP_MASTERVOLUME-1;
			 rasterwait();
			 disp320x200x256(pic1);
			 rasterwait();
			 disp320x200x256(pic1);
			 rasterwait();
			 disp320x200x256(pic1);
			 rasterwait();
			 disp320x200x256(pic1);
			 if (kbhit()) ExitProcess();
			}

/****** System deinitialization **********************************************/

	set80x25x16();
	SP_QUIET=0;
	stopmusic();
	printf("DEMO_DEINIT: RELEASING MEMORY\n");
	DeAllocateMem();
}
