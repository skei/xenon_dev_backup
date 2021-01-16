; some data labels for math.inc
; some may be missing, you'll know which ones are missing when you compile it.

xad                dd ?                     ; 3d offset of object
yad                dd ?
zad                dd ?
vxcos              dd ?
vxsin              dd ?
vycos              dd ?
vysin              dd ?
vzcos              dd ?
vzsin              dd ?

_ecosx             dd ?                     ; multipliers of eyeax and eyeaz
_esinx             dd ?                     ; reset at begining of each new frame
_ecosy             dd ?
_esiny             dd ?
_ecosz             dd ?
_esinz             dd ?

vcosx              dd ?                     ; temp storage for object matrix calculation
vsinx              dd ?                     ; can be used if needed during draw
vcosy              dd ?
vsiny              dd ?
vcosz              dd ?
vsinz              dd ?

_vmatrix           dd 9 dup (?)             ; 3x3 rotation matrix for object (includes eye)
_tmatrix           dd 9 dup (?)             ; 3x3 temp rotation matrix for hierarchys
_ematrix           dd 9 dup (?)             ; 3x3 rotation matrix for eye (32 bit)
_lmatrix           dd 3 dup (?)             ; 1x3 z resultant matrix for _lambert shading

_y_angle_of_sun    dd 1500h                 ; where is the sun? for shading

