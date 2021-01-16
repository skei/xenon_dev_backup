;-----------------------------------------------------------------------------
; FadeTo - Fade palette effect.
; In:
;  FromPalette - Source palette.
;  ToPalette   - Destination palette.
;-----------------------------------------------------------------------------

FadeTo         proc near

FadeLoop:       lea     si,[FromPalette]
                call    SetPalette

                mov     cx,768
                xor     dx,dx
                xor     bx,bx
FadeColor:      mov     al,[FromPalette+bx]
                mov     ah,[ToPalette+bx]
                cmp     al,ah
                je      SkipFade
                ja      FadeOut
FadeIn:         add     al,FADESPEED
                cmp     al,ah
                jl      SetColor
                mov     al,ah
                jmp     SetColor
FadeOut:        sub     al,FADESPEED
                cmp     al,ah
                jg      SetColor
                mov     al,ah
                jmp     SetColor
SetColor:       mov     [FromPalette+bx],al
                inc     dx
SkipFade:       inc     bx
                loop    FadeColor
                test    dx,dx
                jne     FadeLoop
                ret

FadeTo          endp
