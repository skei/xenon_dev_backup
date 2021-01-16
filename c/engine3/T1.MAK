CFLAGS  = /zq /c /5r /fp5 /j /oneatxm /fpi87
LFLAGS  = /fe=$(APP)

APP = t1.exe
OBJS = engine.obj   &
       load3ds.obj  &
       poly2.obj    &
       gfx.obj      &
       t1.obj

CC = wcl386
LL = wcl386

.c.obj : .AUTODEPEND
        $(CC) $(CFLAGS) $[@

$(APP) : $(OBJS)
         $(LL) $(LFLAGS) $(OBJS) $(LIBS)

