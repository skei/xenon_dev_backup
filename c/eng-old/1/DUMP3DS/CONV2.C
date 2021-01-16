#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

#define TRUE 1
#define FALSE 0
#define QUOTE '\"'

typedef unsigned int bool;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

char *dummy;
char _dummy_[256];
char buf[256];        // temp buffer for current line read from dmp file

FILE *INfile, OUTfile;

int NumVertices;

// 컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴

// --------------------------------------------------------------------------
// returns TRUE if the string s contains the substring v
// --------------------------------------------------------------------------
bool instr( char *s, char *v )
{
 bool ret;
 ret = FALSE;
 if ( strstr( s, v ) != NULL )
 {
  ret = TRUE;
 }
 return ret;
}


// --------------------------------------------------------------------------
// Returning the value following V, converted to double
// --------------------------------------------------------------------------
double GetFloat ( char *s, char *v )
{
 char *pos;
 double x;
 pos = strstr( s, v );        // pos -> 1st occurence of V in S
 pos+=strlen(v);              // Skip V
 x = strtod( pos, &dummy);   // x = convert following string to double
 return x;
}

// --------------------------------------------------------------------------
// Returning the value following V, converted to int
// --------------------------------------------------------------------------
int GetInt ( char *s, char *v )
{
 char *pos;
 int x;

 pos = strstr( s, v );        // pos -> 1st occurence of V in S
 pos+=strlen(v);              // Skip V
 x = strtol( pos, &dummy,10);   // x = convert following string to int
 return x;
}

// --------------------------------------------------------------------------
// Returning the string (in ""'s) following V (in string S) returning in ret
// --------------------------------------------------------------------------
void GetString( char *s, char *v, char *ret )
{
 char *pos;
 pos = strstr( s, v );        // pos -> 1st occurence of V in S
 pos+=strlen(v);              // Skip V
 while ( *pos != QUOTE ) pos++;
 while ( *pos == QUOTE ) pos++;
 while ( *pos != QUOTE )
 {
  *ret++ = *pos++;
 }
 *ret = '\0';
}

// --------------------------------------------------------------------------

void ReadLine(void)
{
 if (!feof(INfile)) fgets(buf,255,INfile);
// printf("%s\n",buf);
}

// --------------------------------------------------------------------------
// Reading lines from INfile until v found in read line.
// Starts by reading new line from INfile
// on return: buf is the line containing v
// --------------------------------------------------------------------------
void SearchFor( char *v)
{
 do
 {
  ReadLine();
 } while((!instr(buf,v)) && (!feof(INfile)));
}

// 께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께께

void main(int argc, char *argv[])
{
 printf("\n");
 if (argc != 2)
 {
  printf("USAGE: CONV2 '*.dmp'\n");
  printf("*.DMP = dumped 3DS file... Create with '3DSRDR *.3DS'\n");
  exit(1);
 }

 INfile = fopen(argv[1],"rt");
 if (ferror(INfile))
 {
  printf("Error opening file '%s'\n",argv[1]);
  exit(2);
 }
 dummy = &_dummy_;
 memset(buf,0,sizeof(buf));

 while (!feof(INfile))
 {
  SearchFor("Object name \"");
  GetString( buf,"Object name",dummy);
  printf("Object found: %s\n",dummy);
  SearchFor("Vertices:");
  NumVertices = GetInt(buf,"Vertices:");
  printf("Tri-Mesh, %i vertices\n",NumVertices);
 }
 close(INfile);
}
