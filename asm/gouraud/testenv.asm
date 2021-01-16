;DOSSEG
.MODEL MEDIUM
.386
.STACK
;JUMPS
LOCALS

GLOBAL EnvMap:FAR
GLOBAL SetPalette:FAR 
GLOBAL WaitVR:FAR     
;GLOBAL ClearScreen:FAR
GLOBAL TreDSeg:word             ; Segment of object data
GLOBAL AddToXOff:word           ; Added to X-offset each frame
GLOBAL AddToYOff:word           ;          Y
GLOBAL AddToZOff:word           ;          Z
GLOBAL NumCounter:Word          ; Number of frames before returning
GLOBAL UseBlur:word             ; 1 for "Blur" copy
GLOBAL Texture:word             ; Segment of texture
GLOBAL Buffer:word              ; Segment of Background-buffer
GLOBAL Xoff:Dword               ; X-offset - 160
GLOBAL Yoff:Dword               ; Y-offset - 100
GLOBAL zoff:Dword               ; Z-offset - 500?
GLOBAL NumCoords:word           ; Number of coordinates for current object
GLOBAL NumPolys:word            ;           polygons
GLOBAL Coords:word              ; Offset of coordinates        
GLOBAL VertexNormals:word       ;           Vertexnormals    
GLOBAL Polygon:word             ;           Face-data    
GLOBAL VX:word                  ; X-rotation angle
GLOBAL VY:word                  ; Y
GLOBAL VZ:word                  ; Z   
GLOBAL VxAdd:word               ; Added to X-angle each frame
GLOBAL VyAdd:word               ;          Y
GLOBAL VzAdd:word               ;          Z
             
Segment treDsegment
 nc1 dw 420             ; Antall koordinater
 np1 dw 840             ; antall polygons
 include e:\axon\asc\4toruses.inc
ENDS              
                  
Segment BufferSeg 
bufr label byte
 db 64000 dup(?)
ends              
                  
Segment TextureSeg
textr label byte           
 db 65536 dup(?)
ENDS           
               
;° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
SEGMENT DATA PARA PUBLIC USE16
               
Buffer dw ?    
Label palette byte       
i=0            
rept 64
 db i,0,0
 i=i+1
endm         
rept 256
 db 0,63,0
endm           

ENDS DATA         
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
SEGMENT CODE PARA PUBLIC USE16
ASSUME CS:CODE,DS:DATA

; DS:SI = Palette  
PROC SetPalette FAR        
 mov dx,3c8h         
 xor al,al              
 out dx,al       
 inc dx          
 mov cx,768      
 rep outsb     
 ret                
ENDP                   
             
PROC WaitVR FAR
 mov dx,3DAh   
@@1:               
 in al,dx          
 test al,8         
 jnz @@1             
@@2:                
 in al,dx      
 test al,8     
 jz @@2        
 ret             
ENDP           
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC DoObject1
 mov ax,treDsegment    
 mov gs,ax     
 mov [treDseg],ax
 mov ax,OFFSET Coords1 
 mov [Coords],ax       
 mov ax,OFFSET VertexNormals1
 mov [VerTexNormals],ax
 mov ax,OFFSET Polygon1
 mov [Polygon],ax    
 mov ax,[gs:nc1]     
 mov [NumCoords],ax  
 mov ax,[gs:np1]                                           
 mov [NumPolys],ax   
 mov [vx],256                
 mov [vy],1
 mov [vz],2
 mov [vxadd],5
 mov [vyadd],6
 mov [vzadd],7
 mov [Xoff],160              
 mov [yoff],100              
 mov [zoff],300
 mov [AddToXoff],0 
 mov [AddToYOff],0 
 mov [AddToZoff],0 
 mov [NumCounter],1500
 mov [UseBlur],0     
 Call EnvMap         
 ret                 
ENDP                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC CalcTex                       
 cld                 
 mov ax,[Texture]    
 mov es,ax     
 xor di,di     
@@fory_loop:   
 xor si,si     
@@forx_loop:   
 mov ax,si     
 sub ax,64  
 imul ax,ax 
 mov bx,di  
 sub bx,64  
 imul bx,bx 
 add ax,bx            
 shr ax,7     
 mov cx,31
 sub cx,ax  
 cmp cx,32  
 jb @@1     
 mov cx,0
@@1:
 mov bx,di  
 shl bx,8   
 add bx,si  
 mov [es:bx],cl
 inc si     
 cmp si,127 
 jb @@forx_loop
 mov si,0   
 inc di     
 cmp di,127 
 jb @@fory_loop
 ret        
ENDP                  
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

;° EntryPoint from DOS °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
EntryPoint: 
 mov ax,data
 mov ds,ax   
 mov ax,13h 
 int 10h    
 mov ax,SEG bufr
 mov [Buffer],ax
 mov ax,SEG textr
 mov [Texture],ax
 Call CalcTex           ; Denne funker
 mov si,OFFSET Palette
 Call SetPalette
 Call DoObject1
 mov ax,3
 int 10h
 mov ax,4c00h
 int 21h
ENDS CODE
End EntryPoint
