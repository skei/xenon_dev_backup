CFLAGS  = /zq /c /5r /fp5 /j /oneatxm /fpi87
LFLAGS  = /fe=$(APP)

APP = info.exe
OBJS = engine.obj   &
       load3ds.obj  &
       math3d.obj   &
       info.obj

CC = wcl386
LL = wcl386

.c.obj : .AUTODEPEND
        $(CC) $(CFLAGS) $[@

$(APP) : $(OBJS)
         $(LL) $(LFLAGS) $(OBJS) $(LIBS)

