PROC CopyBack1                       
 mov esi,[Background]                
 mov edi,[buffer]                                               
 add edi,(960*200)+320                                          
 mov ecx,200                                                    
@@1:                                 
 rept 80                             
  movsd                              
 endm                
 add edi,640         
 dec ecx             
 jnz @@1               
 ret                 
ENDP                 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
PROC CalcTex                       
 cld                   
; F›rst blank buffer, s† vi er sikker
 mov edi,[Gouraud]
 xor eax,eax                 
 mov ecx,16384    
 rep stosd                       
 mov di,0                        
@@fory_loop:                     
 mov si,0                        
@@forx_loop:                     
 mov ax,si                       
 sub ax,64;128                   
 imul ax,ax        
 mov bx,di          
 sub bx,64;128       
 imul bx,bx        
 add ax,bx     
 shr ax,6;9 
 mov cx,63              
 sub cx,ax              
 cmp cx,5               
 cmp cx,64    
 jb @@1                 
 mov cx,0               
@@1:                    
 mov bx,di                       
 shl ebx,8              
 add bx,si              
 movzx ebx,bx
 add ebx,[Gouraud]
 mov [ebx],cl        
 inc si       
 cmp si,127;255
 jb @@forx_loop           
 mov si,0      
 inc di        
 cmp di,127;255     
 jb @@fory_loop     
 ret                
ENDP                    
