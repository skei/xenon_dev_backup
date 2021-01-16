.486p           
.MODEL FLAT     
.CODE           
                
include c:\excess\timeout\gfx\gfx.inc   
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
                                      
 db 320*32 dup(?)                     
label theythought byte                
 include c:\excess\timeout\gfx\they-th.db            
 db 320*32 dup(?)                     
label wewere byte
 include c:\excess\timeout\gfx\we-were.db
 db 320*32 dup(?)                     
label textpal byte                    
 include c:\excess\timeout\gfx\textpal.db
label textpal2 byte            
 include c:\excess\timeout\gfx\wepal.db
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�

End                                   
