 #include <stdio.h>
 #include <io.h>

FILE *fp;
int filesize,numread;
char *buffer,*ptr;
int iptr;
int size_3ds;
char *ptr_3ds;
char name_3ds[13];
int number_3ds;
char c;

void main(int argc, char *argv[])
 {
  fp = fopen(argv[1],"rb");
  filesize = filelength(fileno(fp));
  printf("þ FileSize = %i\n",filesize);
  buffer = (char *)malloc(filesize);
  numread = fread(buffer,1,filesize,fp);
  fclose(fp);
  printf("þ Bytes read from %s = %i\n",argv[1],numread);
  if (numread != filesize) printf("þ Entire file NOT loaded into memory!\n");
  number_3ds = 1;
  iptr = 0;
  ptr = buffer;
  while (iptr < (numread-5))
  {
   if ( (*(int *)ptr==0x00000003) && (*(short int *)(ptr+4)==0x3d3d) )
   {
    printf("þ 3DS probably found at %i\n",iptr);
    if ( *(short int *)(ptr-12) == 0x4d4d )
    {
     size_3ds = *(unsigned int *)(ptr-10);
     // printf("þ 3DS size: %i\n",size_3ds);
     ptr_3ds = ptr-12;
     memset(name_3ds,0,12);
     sprintf(&name_3ds,"%i.3ds\0",number_3ds);
     printf("þ Saving %s (size %i) from offset: %i\n",name_3ds,size_3ds,iptr-12);
     printf("þ Press [ESC] to cancel, or any other key to save...\n");
     c = getch();
     if (c != 27)
     {
      fp = fopen(name_3ds,"wb");
      fwrite(ptr_3ds,1,size_3ds,fp);
      fclose(fp);
      number_3ds++;
     }
    }
    else printf("þ False alarm.. Didn't find 0x4D4D at start of 3DS\n");
   }
   ptr++;
   iptr++;
  }
  free(buffer);
 }

