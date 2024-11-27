        xdef    _set_music_player_vbl
        xdef    _restore_vbl

        xref    _soundtrack_vbl

_set_music_player_vbl:
        move    sr,-(sp)
        move    #$2700,sr       ; Disable interrupts (assumption to check)
        move.l  $70.w,old_vbl   ; Save VBL
        move.l  #new_vbl,$70.w      ; Set new VBL with player
        move    (sp)+,sr        ; Enable interrupts
        rts

new_vbl:
        movem.l d0-a6,-(sp)
        move.l  _soundtrack_vbl,a0
        jsr     (a0)            ; play that funky music
        movem.l (sp)+,d0-a6
old_vbl=*+2
        jmp     'Fixx'

_restore_vbl:
        move    sr,-(sp)
        move    #$2700,sr
        move.l  old_vbl,$70.w           ;restore vbl
        move    (sp)+,sr                ;enable interrupts - tune will stop playing
        rts
