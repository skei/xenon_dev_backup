.386p
.model flat,c
.code

x  dd 0
y  dd 0
tg dd 0

extrn	maindegree	: dword
extrn	twistsource	: dword
extrn	twisttarget	: dword
extrn cosd        : word
extrn sind        : word
extrn sqrtd       : word

public   twist
twist:
 mov eax,twisttarget
 mov tg,eax
 pushad
 mov y,0
yloop:
 mov x,0
xloop:
 mov eax,x
 mov ebx,eax
 shl eax,6
 lea eax,[eax+2*eax]
 lea eax,[eax+8*ebx]
 add eax,y
; xor ebx,ebx
 mov bx,sqrtd[eax*2]
 mov eax,maindegree
; mov eax,0
 cdq
 idiv ebx
 sar eax,1
 and eax,0ffh
 mov ebx,eax
 mov esi,160
 sub esi,x
 mov edi,100
 sub edi,y
 movsx eax,sind[ebx*2]
 push eax
 imul edi
; imul esi
 mov ecx,eax
 movsx eax,cosd[ebx*2]
 push eax
 imul esi
 add ecx,eax
 sar ecx,16
 add ecx,160
 pop eax
 imul edi
 mov ebp,eax
 pop eax
 imul esi
 lea ebp,[ebp-eax]
 sar ebp,16
 mov eax,ebp
 add eax,100
 shl eax,6
 lea esi,[eax+4*eax]
 add esi,ecx
 add esi,twistsource
 mov edi,tg
 add tg,2
 mov eax,[esi]
 mov [edi],eax
 inc x
 inc x
 cmp x,320
 jne xloop
 mov x,0
 inc y
 cmp y,200
 jb yloop
 popad
 ret
end
