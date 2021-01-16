include c:\excess\timeout\music\dos32.inc
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
;		       Global Definitions for the Player
;²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²
;** Procedures **
Global AMS_Init:Near,AMS_LoadModule:Near,AMS_ErrorHandler:Near,AMS_ShutDown:Near
Global AMS_ReadCommandLine:Near,AMS_StartModule:Near,AMS_StopModule:Near,AMS_MemAlloc:Near
Global AMS_DetectSoundCard:Near,AMS_PauseModule:Near,AMS_UnPauseModule:Near,AMS_MasterVolume:Near
Global FXEngine:Near,SpeechEngine:Near,StopSpeech:Near,CheckSpeech:Near
;** Init variables **
Global SoundCard_IRQ:Byte,SoundCard_MIDIIRQ:Byte,SoundCard_DMA1:Byte,SoundCard_DMA2:Byte
Global TimerSelect:Byte,SoundCard_Baseport:Word,SoundCard:Dword,PlayFreq:Dword
Global LoadFXEngine:Byte,SB_StereoAllowed:Byte,SB_16BitsAllowed:Byte,Amplification:Byte
;** Music Sync variables **
Global Sync_Byte:Byte,CallRoutine:Dword
;** Loading variables **
Global LoadOffset:Dword
;** Playing Info **
Global CurrRow:Word,CurrPosition:Word,CurrPatt:Word,Vol:Byte:32
;** Song Info **
Global Composer:Byte:30,SongName:Byte:30,MasterVolume:Byte
;** Other variables **
Global PositionJump:Word,GotoRow:Word,MemoryFree:Dword,Positions:Word
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
 
ModName db 'timeout.xm',0
                                   
;° CODE °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                                   
Proc Music_Init             
 call AMS_DetectSoundCard       ; Read the Environment instead
 mov [LoadFXEngine],0           ; Don't use any effects
; mov [Timerselect],0           ; Use GUS timer
 call AMS_Init                  ; Init Music System
 ret              
ENDP                            
                                
Proc Music_Start                
 lea edx,ModName                ; Ptr to Song Name
 mov [LoadOffset],0             ; Ptr from EOF
 call AMS_LoadModule            ; Load the song
 call AMS_StartModule           ; Start the playing.
 ret                            
ENDP                            
                                  
PROC Music_Stop                 
 call AMS_StopModule            ; Stops the playing.
 call AMS_ShutDown              ; Shut down the MusicSystem
 ret                            
ENDP                            
                                               
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
AMS_Errorhandler:    
 mov edx,esi         
 mov ah,9            
 int 21h             
 call AMS_ShutDown   
 mov ax,4c00h        
 int 21h             
                     
