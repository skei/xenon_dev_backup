
 /*
    For ›yeblikket er det mest "brute force" koding her... Bortsett fra at
    jeg har fiksa s† vertices bare blir kalkulert en gang... Shared vertices

    ToDo:
    1) Surface crawling.
       start i kube med blob-senter... G† i en eller annen retning inntil
       vi finner en kube som ikke har verdi 255 (dva. en eller annen vertex
       er utenfor blob'en)... Start med denne kuben.
       Deretter g†r vi gjennom rekursivt alle naboene som deler en eller
       flere vertex'er som deles mellom de to kubene...
       Return‚r hvis vi m›ter en kube som allerede er "behandlet"...
       Gj›r for alle... Rutina vil terminere hvis to eller flere kuler
       "smeltes", fordi da vil "surface crawling" ha v‘rt innom alle, og
       kubene blitt merket som "behandlet".. N†r vi da gpr gjennom neste
       kule, vil vi m›re en behandlet kube, og, finito for denne kuben...
       Dette kan gj›res i CalcField ogs†, s† vi slipper † kalkulere Iso-
       verdiene for alle kubene (den mest tids-krevende prosessen her...)
    2) Fiks opplegget med BackFace culling...
    3) Sett opp blob'en som er komplet 3D objekt... Shared vertices, poly
       liste... Sort‚r denne som valing 3d objekt, etc... S† kan vi ha
       dette objektet i en vanlig 3d engine...
    4) En bedte m†te † kalkulere ISO verdiene p†... N† brukes 1/dý hvor d er
       avstand fra grid til kube-senter.... Alts† m† alle grid-elementene
       kalkuleres...
       Med MetaBalls, har kulene en "strength", og man kalkulerer bare
       punktet hvis avstanden er mindre enn "strength"...
       Flere metoder = "Soft Objects", og, hmmm, torus, kube og slikt???P
    5) Legg inn muligheten for Negative blobs...
    6) Fiks Cube oppsettet...

    * Grid'en er en slags distanse fra de forskjellige blob elementene

 */

 #include <stdio.h>
 #include <conio.h>
 #include <math.h>

 #include "meta.h"


// ---

 // format:
 // char numedges, then chars for each edge (clockwise)
 // repeat until -1, eller max 16...
int          triTable[256][16] =
 {
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 0, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 0, 3, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 2, 3, 9, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 2,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 8,10, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 2,10, 1, 3, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 3, 9, 8,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 8, 2, 1,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 1,11, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1,11, 9, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 2,10,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 8,10,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 2,10,11, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 8,10,11, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 4, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 3, 9, 4, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 3, 9, 4, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 2, 8, 4, 6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 4, 6,10, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 3, 9, 4, 6,10, 3, 0, 2, 8,-1,-1,-1,-1,-1 },
  { 6, 1, 3, 9, 4, 6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 6, 8, 3, 1,11, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 4, 6, 2, 1,11, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 1,11, 9, 4, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,11, 9, 4, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 2, 8, 4, 6,10,11, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 4, 6,10,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 4, 6,10,11, 9, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1 },
  { 5, 4, 6,10,11, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 9, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 9, 7, 4, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 3, 7, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 3, 7, 4, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 9, 7, 3, 1, 2,10,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 9, 7, 4, 8,10, 1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 2,10, 1, 3, 7, 4,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 3, 7, 4, 8,10,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,11, 7, 4, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,11, 7, 4, 8, 2, 3, 0, 9, 3,-1,-1,-1,-1,-1 },
  { 5, 0, 1,11, 7, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,11, 7, 4, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 2,10,11, 7, 4, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 4, 8,10,11, 7, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 2,10,11, 7, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 4, 8,10,11, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6, 8, 9, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 9, 7, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 3, 7, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 2, 3, 7, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 1, 2, 8, 9, 7, 6,10,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 9, 7, 6,10, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 3, 7, 6,10, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 3, 7, 6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 1,11, 7, 6, 8, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1,11, 7, 6, 2, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 1,11, 7, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1,11, 7, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6,10,11, 7, 4, 2, 8, 9, 3,-1,-1,-1,-1,-1,-1 },
  { 4, 6,10,11, 7, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6,10,11, 7, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6,10,11, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,10, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 8, 6, 5,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,10, 6, 3, 0, 3, 9,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 2, 3, 9, 8, 6, 5,10,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 2, 6, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 8, 6, 5, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 2, 6, 5, 1, 3, 9,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 3, 9, 8, 6, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,10, 6, 5,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,10, 2, 6, 0, 8, 6, 5,11, 3,-1,-1,-1,-1,-1 },
  { 7, 0, 1,10, 6, 5,11, 9,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 5,11, 9, 8, 6, 3, 1,10, 2,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 6, 5,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 8, 6, 5,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 2, 6, 5,11, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 5,11, 9, 8, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 5,10, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 4, 5,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 3, 9, 4, 5,10, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 2, 3, 9, 4, 5,10,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 2, 8, 4, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 4, 5, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 3, 9, 4, 5, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 3, 9, 4, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 1,10, 8, 4, 5,11, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,10, 2, 5, 0, 4, 5,11, 3,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 5,11, 9, 4, 0, 1,10, 8,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 5,11, 9, 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 2, 8, 4, 5,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 4, 5,11, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 5,11, 9, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 5,11, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 4, 9, 7, 5,10, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 6, 0, 9, 7, 5,10, 2,-1,-1,-1,-1,-1 },
  { 7, 0, 3, 7, 5,10, 6, 4,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 5, 2, 3, 7, 5,10,-1,-1,-1,-1,-1,-1 },
  { 7, 1, 2, 6, 4, 9, 7, 5,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 5, 0, 9, 7, 5, 1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 3, 7, 5, 4, 0, 2, 6, 4,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 4, 1, 3, 7, 5,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 6, 1,10, 6, 4, 9, 3,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 4, 8, 6, 3, 1,10, 2, 3, 0, 9, 3 },
  { 3, 5,11, 7, 5, 0, 1,10, 6, 4,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 4, 8, 6, 3, 1,10, 2,-1,-1,-1,-1 },
  { 3, 5,11, 7, 5, 2, 6, 4, 9, 3,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 4, 8, 6, 3, 0, 9, 3,-1,-1,-1,-1 },
  { 3, 5,11, 7, 4, 0, 2, 6, 4,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 4, 8, 6,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 5,10, 8, 9, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 9, 7, 5,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 3, 7, 5,10, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 3, 7, 5,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 2, 8, 9, 7, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 9, 7, 5, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 3, 7, 5, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 3, 7, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 5, 1,10, 8, 9, 3,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 1,10, 2, 3, 0, 9, 3,-1,-1,-1,-1 },
  { 3, 5,11, 7, 4, 0, 1,10, 8,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 4, 2, 8, 9, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5,11, 7,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 7,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 7,11, 3, 0, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 3,11, 5, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 2, 3,11, 5, 7, 9, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 2,10, 5, 7,11,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 8,10, 5, 7,11, 1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 3,11, 6, 0, 2,10, 5, 7, 9,-1,-1,-1,-1,-1 },
  { 5, 5, 7, 9, 8,10, 3, 1, 3,11,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 5, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 8, 2, 1, 5, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 1, 5, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 5, 7, 9, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2,10, 5, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 8,10, 5, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 2,10, 5, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 5, 7, 9, 8,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 4, 7,11, 5, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 4, 7,11, 5, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 6, 0, 3,11, 5, 6, 8,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 5, 2, 3,11, 5, 6,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 6, 1, 2, 8, 4, 7,11,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 5, 0, 4, 7,11, 1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 4, 7, 9, 3, 1, 3,11, 3, 0, 2, 8 },
  { 3, 5, 6,10, 3, 4, 7, 9, 3, 1, 3,11,-1,-1,-1,-1 },
  { 7, 1, 5, 6, 8, 4, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 5, 6, 2, 4, 0, 4, 7, 3,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 5, 0, 1, 5, 6, 8,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 4, 1, 5, 6, 2,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 5, 2, 8, 4, 7, 3,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 4, 0, 4, 7, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 4, 7, 9, 3, 0, 2, 8,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 4, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 9,11, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 9,11, 5, 4, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 3,11, 5, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 2, 3,11, 5, 4, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 1, 2,10, 5, 4, 9,11,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 8,10, 5, 4, 0, 9,11, 1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 3,11, 5, 0, 2,10, 5, 4,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 8,10, 5, 3, 1, 3,11,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 5, 4, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 5, 4, 8, 2, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 1, 5, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 5, 4, 8, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 2,10, 5, 4, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 8,10, 5, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 2,10, 5, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 4, 8,10, 5,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 5, 6, 8, 9,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 9,11, 5, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 3,11, 5, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 3,11, 5, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 5, 1, 2, 8, 9,11,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 4, 0, 9,11, 1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 1, 3,11, 3, 0, 2, 8,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 1, 3,11,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 5, 6, 8, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 5, 6, 2, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 1, 5, 6, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 1, 5, 6, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 4, 2, 8, 9, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 5, 6,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6, 7,11,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 8, 6, 7,11,10, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 7, 0, 3,11,10, 6, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6, 7, 9, 8, 4, 2, 3,11,10,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 2, 6, 7,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 8, 6, 7,11, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 3,11, 5, 0, 2, 6, 7, 9,-1,-1,-1,-1,-1,-1 },
  { 4, 6, 7, 9, 8, 3, 1, 3,11,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1,10, 6, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,10, 2, 5, 0, 8, 6, 7, 3,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 1,10, 6, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6, 7, 9, 8, 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 2, 6, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 8, 6, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 2, 6, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 6, 7, 9, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 4, 7,11,10, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 4, 7,11,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 5, 0, 3,11,10, 8,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 4, 2, 3,11,10,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 2, 8, 4, 7,11,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 4, 7,11, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 3, 1, 3,11, 3, 0, 2, 8,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 3, 1, 3,11,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,10, 8, 4, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,10, 2, 4, 0, 4, 7, 3,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 4, 0, 1,10, 8,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 8, 4, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 4, 7, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 7, 9,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 4, 9,11,10, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 5, 0, 9,11,10, 2,-1,-1,-1,-1,-1,-1 },
  { 6, 0, 3,11,10, 6, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 4, 2, 3,11,10,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1, 2, 6, 4, 9,11,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 4, 0, 9,11, 1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 3,11, 4, 0, 2, 6, 4,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 3, 1, 3,11,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 6, 1,10, 6, 4, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 3, 1,10, 2, 3, 0, 9, 3,-1,-1,-1,-1 },
  { 5, 0, 1,10, 6, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 2, 6, 4, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 2, 6, 4,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 4, 8, 6,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 8, 9,11,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 9,11,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 0, 3,11,10, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 2, 3,11,10,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1, 2, 8, 9,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 9,11, 1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 3,11, 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1, 3,11,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 5, 1,10, 8, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,10, 2, 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 0, 1,10, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 1,10, 2,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 4, 2, 8, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 0, 9, 3,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  { 3, 0, 2, 8,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 },
  {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1 }
 };

unsigned int edgeTable[256] =
 {
  0x000,0x105,0x209,0x30c,0x406,0x503,0x60f,0x70a,
  0x80a,0x90f,0xa03,0xb06,0xc0c,0xd09,0xe05,0xf00,
  0x150,0x055,0x359,0x25c,0x556,0x453,0x75f,0x65a,
  0x95a,0x85f,0xb53,0xa56,0xd5c,0xc59,0xf55,0xe50,
  0x290,0x395,0x099,0x19c,0x696,0x793,0x49f,0x59a,
  0xa9a,0xb9f,0x893,0x996,0xe9c,0xf99,0xc95,0xd90,
  0x3c0,0x2c5,0x1c9,0x0cc,0x7c6,0x6c3,0x5cf,0x4ca,
  0xbca,0xacf,0x9c3,0x8c6,0xfcc,0xec9,0xdc5,0xcc0,
  0x460,0x565,0x669,0x76c,0x066,0x163,0x26f,0x36a,
  0xc6a,0xd6f,0xe63,0xf66,0x86c,0x969,0xa65,0xb60,
  0x530,0x435,0x739,0x63c,0x136,0x033,0x33f,0x23a,
  0xd3a,0xc3f,0xf33,0xe36,0x93c,0x839,0xb35,0xa30,
  0x6f0,0x7f5,0x4f9,0x5fc,0x2f6,0x3f3,0x0ff,0x1fa,
  0xefa,0xfff,0xcf3,0xdf6,0xafc,0xbf9,0x8f5,0x9f0,
  0x7a0,0x6a5,0x5a9,0x4ac,0x3a6,0x2a3,0x1af,0x0aa,
  0xfaa,0xeaf,0xda3,0xca6,0xbac,0xaa9,0x9a5,0x8a0,
  0x8a0,0x9a5,0xaa9,0xbac,0xca6,0xda3,0xeaf,0xfaa,
  0x0aa,0x1af,0x2a3,0x3a6,0x4ac,0x5a9,0x6a5,0x7a0,
  0x9f0,0x8f5,0xbf9,0xafc,0xdf6,0xcf3,0xfff,0xefa,
  0x1fa,0x0ff,0x3f3,0x2f6,0x5fc,0x4f9,0x7f5,0x6f0,
  0xa30,0xb35,0x839,0x93c,0xe36,0xf33,0xc3f,0xd3a,
  0x23a,0x33f,0x033,0x136,0x63c,0x739,0x435,0x530,
  0xb60,0xa65,0x969,0x86c,0xf66,0xe63,0xd6f,0xc6a,
  0x36a,0x26f,0x163,0x066,0x76c,0x669,0x565,0x460,
  0xcc0,0xdc5,0xec9,0xfcc,0x8c6,0x9c3,0xacf,0xbca,
  0x4ca,0x5cf,0x6c3,0x7c6,0x0cc,0x1c9,0x2c5,0x3c0,
  0xd90,0xc95,0xf99,0xe9c,0x996,0x893,0xb9f,0xa9a,
  0x59a,0x49f,0x793,0x696,0x19c,0x099,0x395,0x290,
  0xe50,0xf55,0xc59,0xd5c,0xa56,0xb53,0x85f,0x95a,
  0x65a,0x75f,0x453,0x556,0x25c,0x359,0x055,0x150,
  0xf00,0xe05,0xd09,0xc0c,0xb06,0xa03,0x90f,0x80a,
  0x70a,0x60f,0x503,0x406,0x30c,0x209,0x105,0x000
 };

int          vertTable[12][6] =
 {
  { 0, 0, 0,    0, 0,+1},
  { 0,-1, 0,    0,-1,+1},
  { 0, 0, 0,    0,-1, 0},
  { 0, 0,+1,    0,-1,+1},
  {+1, 0, 0,   +1, 0,+1},
  {+1,-1, 0,   +1,-1,+1},
  {+1, 0, 0,   +1,-1, 0},
  {+1, 0,+1,   +1,-1,+1},
  { 0, 0, 0,   +1, 0, 0},
  { 0, 0,+1,   +1, 0,+1},
  { 0,-1, 0,   +1,-1, 0},
  { 0,-1,+1,   +1,-1,+1}
 };

// ---


float         IsoGrid[GRIDX][GRIDY][GRIDZ];
//unsigned char CubeCalced[CUBEX][CUBEY][CUBEZ];
cube_t        Cube[CUBEX][CUBEY][CUBEZ];
sVertex      *Vertices;      // NumCubes*3 vertices...
sPolygon     *Polygons;
char         *texture;
char         *buffer;
char          pal[768];
int           NumPolys;
int           NumCalced;
int           NumReused;
int           DoCubeCounter;
int           CubesWithData;
sPolygon    **RenderList;
sPolygon    **Sorted;
BALL          balls[NUMBALLS];

// ---

 // float/real -> int konvertering
 // har h›rt snakk om at denne kanskje ikke er kjappere enn FIST(P) likevel?
 // m† sjekke litt...
 #define MAGIC ((((65536.0*65536.0*16)+(65536.0*0.5))*65536))
int   real2int(float val)
 {
  double temp=MAGIC+val;
  return ((*(int *)&temp)-0x80000000);
 }

 // 1/sqrt(n) approximation
 // opp til 3.5% avvik.... Brukes i kalkulering av EnvMap U/V's
float InvSqrt(const float number)
 {
  int i;

  i = 0x5f3759df - ((*((int *)&number))>>1);
  return *(float *)&i;
 }

 // ---

 /*

 // En test-rutine jeg brukte i starten.... Kan fjernes n†...
void  Draw3DPoint(char *buffer,sVertex *v)
 {
  int x,y;

  if (v->Pos.z < 1) return;
  x = real2int(v->pPos.x);
  y = real2int(v->pPos.y);

  if (x > 319) return;
  if (x < 0)   return;
  if (y > 199) return;
  if (y < 0)   return;
  buffer[(y*320)+x] = 15;
 }

 */

 // ---

void  InitCube(void)
 {
  int x,y,z;
  int NumVertex;

 //       /--11---
 //      /|      /|
 //     / |     / |
 //    1  3    5  |
 //   /   |   /   |
 //  /    |  /    |
 // /--10---|     7
 // |     | |     |
 // |     | |     |
 // 2    / -|-9--/
 // |   /   |   /
 // |  0    6  4
 // | /     | /
 // |/      |/
 //  ---8---
  printf("þ Allocating space for %i cubes\n",NUMCUBES*3);
  Vertices = (sVertex *)malloc(sizeof(sVertex)*(NUMCUBES*3));
  NumVertex = 0;
  for (z=0;z<CUBEZ;z++)
  {
   for (y=0;y<CUBEY;y++)
   {
    for (x=0;x<CUBEX;x++)
    {
     Cube[x][y][z].V[ 1]   = &(Vertices[NumVertex]); NumVertex++;
     Cube[x][y][z].V[ 2]   = &(Vertices[NumVertex]); NumVertex++;
     Cube[x][y][z].V[10]   = &(Vertices[NumVertex]); NumVertex++;
     if (x!=0)
     {
      Cube[x-1][y  ][z  ].V[ 5] = Cube[x][y][z].V[ 1];
      Cube[x-1][y  ][z  ].V[ 6] = Cube[x][y][z].V[ 2];
     }
     if (y!=0)
     {
      Cube[x  ][y-1][z  ].V[ 8] = Cube[x][y][z].V[10];
      Cube[x  ][y-1][z  ].V[ 0] = Cube[x][y][z].V[ 1];
     }
     if (z!=0)
     {
      Cube[x  ][y  ][z-1].V[11] = Cube[x][y][z].V[10];
      Cube[x  ][y  ][z-1].V[ 3] = Cube[x][y][z].V[ 2];
     }
     if ((x!=0)&&(y!=0)) Cube[x-1][y-1][z  ].V[4] = Cube[x][y][z].V[1];
     if ((x!=0)&&(z!=0)) Cube[x-1][y  ][z-1].V[7] = Cube[x][y][z].V[2];
     if ((y!=0)&&(z!=0)) Cube[x  ][y-1][z-1].V[9] = Cube[x][y][z].V[10];

     // set right edge of cube
     // Det jeg pr›vde b›gga som faen her... M† sette opp slik at n†r vi
     // har n†dd "right edge" av kubene, settes nye vertex'er opp, som ikke
     // er avhengig av neste kube.... Avsluttes p† en m†te...
    }
   }
  }
  printf("Number of vertices used in InitCube: %i\n",NumVertex);
 }

 // ---

 // Kalkulerer ISO verdiene for hele grid'en.... Denne m† vi gj›re noe med!
 // den er for treg!!! Og s† blir ikke blob'en smooth nok....
void  CalcField(void)
 {
  int ix,iy,iz,k;
  float dx,dy,dz,dst;
  float x,y,z;

  x = -GRIDX*(GRIDSIZE/2);
  for(ix=0;ix<GRIDX;ix++)
  {
   y = -GRIDY*(GRIDSIZE/2);
   for(iy=0;iy<GRIDY;iy++)
   {
    z = -GRIDZ*(GRIDSIZE/2);
    for(iz=0;iz<GRIDZ;iz++)
    {
     for(k=0;k<NUMBALLS;k++)
     {
      dx = x - balls[k].x;
      dy = y - balls[k].y;
      dz = z - balls[k].z;
      dst = (dx*dx)+(dy*dy)+(dz*dz);
      //if (dst <= balls[k].strength)
      {
       if (dst > 0.00001) IsoGrid[ix][iy][iz] += (1000000/dst)*balls[k].strength;
      }
     }
     z += GRIDSIZE;
    }
    y += GRIDSIZE;
   }
   x += GRIDSIZE;
  }
 }

 // ---


 // interpolerer mellom 2 vertices, kalkulerer U/V for envmap, og projekterer
 // koordinatene 3D -> 2D
void  Interp(sVertex *res, int x1, int y1, int z1,
                          int x2, int y2, int z2)
 {
  float nx1,ny1,nz1,nx2,ny2,nz2,c1,c2,pc,int_x,int_y,int_z,inv_modulo;
  float qwe;
  float inv_z;

  c1 = IsoGrid[x1][y1][z1];
  c2 = IsoGrid[x2][y2][z2];
  pc = (THRESHOLD-c1)/(c2-c1);

  res->Pos.x =  (float)((x1+(x2-x1)*pc)-(GRIDX/2))*GRIDSIZE;
  res->Pos.y =  (float)((y1+(y2-y1)*pc)-(GRIDY/2))*GRIDSIZE;
  res->Pos.z = ((float)((z1+(z2-z1)*pc)-(GRIDZ/2))*GRIDSIZE) + ZADD;

  // Calc U/V values for EnvMapping (256x256 map)
  nx1 = (IsoGrid[x1+1][y1][z1] - IsoGrid[x1+0][y1][z1]);
  ny1 = (IsoGrid[x1][y1+1][z1] - IsoGrid[x1][y1+0][z1]);
  nz1 = (IsoGrid[x1][y1][z1+1] - IsoGrid[x1][y1][z1+0]);
  nx2 = (IsoGrid[x2+1][y2][z2] - IsoGrid[x2+0][y2][z2]);
  ny2 = (IsoGrid[x2][y2+1][z2] - IsoGrid[x2][y2+0][z2]);
  nz2 = (IsoGrid[x2][y2][z2+1] - IsoGrid[x2][y2][z2+0]);
  int_x = nx1+(nx2-nx1)*pc;
  int_y = ny1+(ny2-ny1)*pc;
  int_z = nz1+(nz2-nz1)*pc;
  qwe = int_x*int_x + int_y*int_y + int_z*int_z;
  if (qwe > 1 )
  {
   inv_modulo = InvSqrt(qwe);
   //inv_modulo = 1/sqrt(qwe);
   res->u = 128 + real2int(127*int_x*inv_modulo);
   res->v = 128 + real2int(127*int_y*inv_modulo);
  }
  else
  {
   //inv_modulo = 0;
   res->u = 128;
   res->v = 128;
  }

  inv_z = 1/(res->Pos.z/* + ZADD*/);
  res->pPos.x = 160 + ( (res->Pos.x * XMUL) * inv_z);
  res->pPos.y = 100 + ( (res->Pos.y * YMUL) * inv_z);
 }

 // ---

unsigned char SetCorners(int x, int y, int z)
 {
  unsigned char value;

  value = 0;
  // 1 bit for hvert hj›rne som er inne i blob'en
  if (IsoGrid[x  ][y  ][z  ] > THRESHOLD) value |= 1;
  if (IsoGrid[x  ][y  ][z+1] > THRESHOLD) value |= 2;
  if (IsoGrid[x  ][y-1][z  ] > THRESHOLD) value |= 4;
  if (IsoGrid[x  ][y-1][z+1] > THRESHOLD) value |= 8;
  if (IsoGrid[x+1][y  ][z  ] > THRESHOLD) value |= 16;
  if (IsoGrid[x+1][y  ][z+1] > THRESHOLD) value |= 32;
  if (IsoGrid[x+1][y-1][z  ] > THRESHOLD) value |= 64;
  if (IsoGrid[x+1][y-1][z+1] > THRESHOLD) value |= 128;
  return value;
 }

 // ---

 // Polygonize en enkelt kube...
 // in : x,y,z of lower left front of cube
unsigned char Polygonize( int x, int y, int z )
 {
  int i,n,num;
  int a,b,c;
  unsigned char value;
  unsigned int  edge,bit;
  // sVertex vert[12];
  sVertex *vert[12];
  sPolygon *P;

  value = SetCorners(x,y,z);

  if ((value==0)||(value==255)) return 0;   // no polys (all vertices inside or outside)
  // kalkul‚r edge verdier for de edges som beh›ves (finnes i edgeTable)
  CubesWithData++;
  edge = edgeTable[value];
  for (i=0;i<12;i++)
  {
   bit = (1<<i);
   if ( (edge & bit ) != 0)
   {
    if ( Cube[x][y][z].V[i]->CalcFlag == 1 ) NumReused++;
    else
    {
     NumCalced++;
     Interp( /*&(vert[i]),*/ Cube[x][y][z].V[i],
             x+vertTable[i][0],y+vertTable[i][1],z+vertTable[i][2],
             x+vertTable[i][3],y+vertTable[i][4],z+vertTable[i][5]);
     Cube[x][y][z].V[i]->CalcFlag = 1;
    }
   }
  }
  // --- Draw polygons in cube ---
  //P->texture = texture;

  n=0;    // "ptr" in table
  while ( (triTable[value][n]!=-1) && (n<16) )
  {
   num = triTable[value][n  ];    // num = antall polyer n†
   a   = triTable[value][n+1];    // vertex 1
   b   = triTable[value][n+2];    // vertex 2
   c   = triTable[value][n+3];    // vertex 3
   n += 4;               // point to after the number and 3 first vertices
   P = &Polygons[NumPolys];
   P->v[0] = Cube[x][y][z].V[a];
   P->v[1] = Cube[x][y][z].V[b];
   P->v[2] = Cube[x][y][z].V[c];
   // backface culling
   if (  ((P->v[1]->pPos.x - P->v[0]->pPos.x) * (P->v[2]->pPos.y - P->v[0]->pPos.y))
       - ((P->v[2]->pPos.x - P->v[0]->pPos.x) * (P->v[1]->pPos.y - P->v[0]->pPos.y)) > 0)
   {
    P->Z = (P->v[0]->Pos.z + P->v[1]->Pos.z + P->v[2]->Pos.z);
    P->texture = texture;
    RenderList[ NumPolys ] = P;
    NumPolys++;
    // Poly_Tex(&P, buffer);
   }
   num -= 3;             // finished with 3 vertices...
   while (num > 0)      // if there's slitt vertices left
   {
    a = a;
    b = c;
    c = triTable[value][n];
    n++;
    P = &Polygons[NumPolys];
    P->v[0] = Cube[x][y][z].V[a];
    P->v[1] = Cube[x][y][z].V[b];
    P->v[2] = Cube[x][y][z].V[c];
    // backface culling
    if (  ((P->v[1]->pPos.x - P->v[0]->pPos.x) * (P->v[2]->pPos.y - P->v[0]->pPos.y))
        - ((P->v[2]->pPos.x - P->v[0]->pPos.x) * (P->v[1]->pPos.y - P->v[0]->pPos.y)) > 0)
    {
     P->Z = (P->v[0]->Pos.z + P->v[1]->Pos.z + P->v[2]->Pos.z);
     P->texture = texture;
     RenderList[ NumPolys ] = P;
     NumPolys++;
     // Poly_Tex(&P, buffer);
    }
    num--;
   }
  }
  return value;
 }

 // ---

 /*

void  CalcCube(float x, float y, float z)
 {
  int ix,iy,iz,k;
  float dx,dy,dz,dst;
  // float x,y,z;

  for(k=0;k<NUMBALLS;k++)
  {
   dx = x - balls[k].x;
   dy = y - balls[k].y;
   dz = z - balls[k].z;
   dst = (dx*dx)+(dy*dy)+(dz*dz);
   //if (dst <= balls[k].strength)
   {
    if (dst > 0.00001) IsoGrid[ix][iy][iz] += (1000000/dst)*balls[k].strength;
   }
  }
 }

 */

 // ---

 /*

 // This one will crash if it reaches outside of grid!!!!!!!!!!!!!
void DoFace(int x, int y, int z)
 {
  unsigned char value;

  DoCubeCounter++;
  value = Polygonize(x,y,z);
  // Set "processed"-flag
  CubeCalced[x][y][z] = 1;
  // ... and check neighbours...

  //   3---7
  //  /|  /|
  // 2---6 |
  // | | | |
  // | 1-|-5
  // |/  |/
  // 0---4

  // left
  if (    ((value & 0x0f) != 0)
       &&  (CubeCalced[x-1][y][z] == 0)) DoFace(x-1,y,z);

  // right
  if (    ((value & 0xf0) != 0)
       &&  (CubeCalced[x+1][y][z] == 0)) DoFace(x+1,y,z);

  // top
  if (    ((value & 0xcc) != 0)
       &&  (CubeCalced[x][y-1][z] == 0)) DoFace(x,y-1,z);

  // bottom
  if (    ((value & 0x33) != 0)
       &&  (CubeCalced[x][y+1][z] == 0)) DoFace(x,y+1,z);

  // front
  if (    ((value & 0x55) != 0)
       &&  (CubeCalced[x][y][z-1] == 0)) DoFace(x,y,z-1);

  // back
  if (    ((value & 0x99) != 0)
       &&  (CubeCalced[x][y][z+1] == 0)) DoFace(x,y,z+1);
 }

 */

 // ---

 /*

void SurfaceCrawl( void )
 {
  int k,gx,gy,gz;
  unsigned char value;
  float x,y,z;

  for (k=0;k<NUMBALLS;k++)
  {
   x = balls[k].x;
   y = balls[k].y;
   z = balls[k].z;

   //x + ((gridx/2)*gridsize)
   //------------------------
   //gridsize
   gx = (x+((GRIDX/2)*GRIDSIZE)) / GRIDSIZE;
   gy = (y+((GRIDY/2)*GRIDSIZE)) / GRIDSIZE;
   gz = (z+((GRIDZ/2)*GRIDSIZE)) / GRIDSIZE;
   value = 0;

   // go towards screen until cube is intersected by surface
   // This one will crash if it reaches outside of grid!!!!!!!!!!!!!
   while ((value == 0) || (value == 255))
   {
    gz--;

    if (CubeCalced[gx][gy][gz] == 1) goto SkipThisBall;
    value = SetCorners(gx,gy,gz);
   }
   // gx,gy,gz = initial cube

   DoFace(gx,gy,gz);

 SkipThisBall: ;
  }
 }

 */

 // ByteSort
void  SortPolys( void )
 {
  static int Numbers[256];

  int i;
  short int a,b,d;

  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<NumPolys;i++ )
  {
   Numbers[ RenderList[i]->Z & 255 ]++;
  }
 // Convert from numbers to offset in list to sort
  a = 0;
  for ( i=0;i<256;i++ )
  {
   b = Numbers[i];
   Numbers[i] = a;
   a+=b;
  }
 // Sort according to LOW byte
  for ( i=0;i<NumPolys;i++ )
  {
   a = RenderList[i]->Z & 255; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   Sorted[d] = RenderList[i];
   Numbers[a]++;
  }
 // -
 // Now, do the same, but with high byte...
 // -
  // clear numbers array
  memset( Numbers,0,sizeof (Numbers));
 // Count number of each byte-value
  for ( i=0;i<NumPolys;i++ )
  {
   Numbers[ (Sorted[i]->Z & 0xff00) >> 8 ]++;
  }
 // Convert from numbers to offset in list to sort
  a = 0;
  for ( i=0;i<256;i++ )
  {
   b = Numbers[i];
   Numbers[i] = a;
   a+=b;
  }
 // Sort according to HIGH byte
  for ( i=0;i<NumPolys;i++ )
  {
   a = (Sorted[i]->Z & 0xff00) >> 8; //sorttable[i].sortvalue & 255;
   d = Numbers[a];
   RenderList[d] = Sorted[i];
   Numbers[a]++;
  }
 }

 // ---

void  SetRGB (char,char,char,char);
 #pragma aux SetRGB=\
  "mov dx,3c8h",\
  "out dx,al",\
  "inc dx",\
  "mov al,ah",\
  "out dx,al",\
  "mov al,bl",\
  "out dx,al",\
  "mov al,bh",\
  "out dx,al",\
  parm [al] [ah] [bl] [bh] modify [edx];

// ---

void  main(void)
 {
  FILE *fp;
  int x,y,z,k,i;

  RenderList = 0;
  Polygons   = 0;
  Vertices   = 0;
  Sorted     = 0;
  texture    = 0;
  buffer     = 0;

  RenderList = (sPolygon **)malloc(MAXPOLYS*sizeof(sPolygon **));
  Polygons   = (sPolygon *)malloc(MAXPOLYS*sizeof(sPolygon));
  Sorted     = (sPolygon **)malloc(MAXPOLYS*sizeof(sPolygon **));

  printf("þ Initializing Cube\n");
  InitCube();
  printf("þ Allocating memory for texture & buffer\n");
  texture = (char *)malloc(256*256);
  buffer  = (char *)malloc(320*200);
  if (texture == 0) printf("! Error allocating memory for texture\n");
  if (buffer == 0) printf("! Error allocating memory for buffer\n");
  printf("þ Loading texture & palette\n");
  fp = fopen("texture.pix","rb");
  fread(texture,1,65536,fp);
  fclose(fp);
  fp = fopen("texture.pal","rb");
  fread(&pal,1,768,fp);
  fclose(fp);
  printf("þ Initialization\n");

  for (i=0;i<NUMBALLS;i++)
  {
   balls[i].sx = 0;
   balls[i].sy = 0;
   balls[i].sz = 0;

   balls[i].sx_add = (float)rand()/300000;
   balls[i].sy_add = (float)rand()/300000;
   balls[i].sz_add = (float)rand()/300000;

   balls[i].strength = 1;
  }

  printf("þ OK... Press any key\n");
  getch();
  _asm
  {
   mov ax,13h
   int 10h
  }
  for (x=0;x<256;x++) SetRGB(x,pal[x*3],pal[x*3+1],pal[x*3+2]);
  while (!kbhit())
  {
   for (x=0;x<(NUMCUBES*3);x++) Vertices[x].CalcFlag = 0;
   memset(&IsoGrid,0,sizeof(IsoGrid));
   //memset(&CubeCalced,0,sizeof(CubeCalced));

   for (i=0;i<NUMBALLS;i++)
   {
    balls[i].x = sin(balls[i].sx)*100;
    balls[i].y = sin(balls[i].sy)*100;
    balls[i].z = sin(balls[i].sz)*100;
   }
   CalcField();
   memset(buffer,0,64000);
   NumPolys = 0;
   NumCalced = 0;
   NumReused = 0;
   CubesWithData = 0;
   DoCubeCounter = 0;
   // -1 i alle fordi right, bottom og back av grid'ens vertices er b›ggete

   // Brute force: Calculate iso values for ALL cubes in grid

   for (z=1;z<(CUBEZ-1);z++)
   {
    for (y=1;y<(CUBEY-1);y++)
    {
     for (x=1;x<(CUBEX-1);x++)
     {
      Polygonize(x,y,z);
     }
    }
   }

   // SurfaceCrawl();

   SortPolys();
   for (i=NumPolys-1 ; i>=0 ; i-- ) Poly_Tex(RenderList[i],buffer);

   //balls[0].x += 5;
   for (i=0;i<NUMBALLS;i++)
   {
    balls[i].sx += balls[i].sx_add;
    balls[i].sy += balls[i].sy_add;
    balls[i].sz += balls[i].sz_add;
   }
   memcpy((char *)0xA0000,buffer,64000);
  }
  getch();
  _asm
  {
   mov ax,3h
   int 10h
  }
  printf("þ Clearing allocated memory\n");
  if (texture    != 0) free (texture);
  if (buffer     != 0) free (buffer);
  if (Vertices   != 0) free (Vertices);
  if (Polygons   != 0) free (Polygons);
  if (RenderList != 0) free (RenderList);
  if (Sorted     != 0) free (Sorted);
  printf("þ Finito...\n");
  printf("Number of polys rendered last frame: %i\n",NumPolys);
  printf("Number of Calculated vertices last frame: %i\n",NumCalced);
  printf("Number of Reused vertices last frame: %i\n",NumReused);
  printf("Number of Cubes with intersections: %i\n",CubesWithData);
  printf("Number of Cubes tested in DoFace: %i\n",DoCubeCounter);
  //for (x=0;x<GRIDX;x++)
  //{
  // for (y=0;y<GRIDY;y++)
  // {
  //  for (z=0;z<GRIDZ;z++)
  //  {
  //   printf("%f\n",IsoGrid[x][y][z]);
  //  }
  // }
  //}
 }

