.486p           
.MODEL FLAT     
.CODE           
                
include c:\excess\timeout\gfx\gfx.inc   
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
; 128x128 phong SHADE (0..63) map     
label phongshade byte                 
 include c:\excess\timeout\gfx\tex128.db
                                      
label pmap byte                       
 include c:\excess\timeout\gfx\phong.db
                               
; blob texture til Credits part
Label BlobTex byte             
 include c:\excess\timeout\gfx\blobtex.db

; Palette #1                               
label palette1 byte
 include c:\excess\timeout\gfx\palette.db
label palette2 byte
 include c:\excess\timeout\gfx\pal2.db
 
label cred_gfx byte
 include c:\excess\timeout\gfx\credits.db
                                          
 db 320*17 dup(?)                     
label timeoutgfx byte                     
 include c:\excess\timeout\gfx\timeout.db
 db 320*17 dup(?)                     
                                      
label timeoutpal byte                     
 include c:\excess\timeout\gfx\timepal.db
                                      
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
End                                   
                                      
