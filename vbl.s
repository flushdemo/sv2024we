	xdef    _set_sprite_timer
        xdef    _set_custom_vbl
        xdef    _restore_vbl
        xdef    _interrupt_handler
	
        xref    _soundtrack_vbl
        xref    _interrupt_draw_gnome

TIMER_A_CONTROL = $FFFA19        ; byte
TIMER_A_DATA    = $FFFA1F        ; byte
	
INTERRUPT_ENABLE_A  = $FFFA07   ; byte
INTERRUPT_SERVICE_A = $FFFA0F   ; byte
INTERRUPT_MASK_A    = $FFFA13   ; byte
INTERRUPT_VECTOR_A  = $000134   ; address
IEA_TIMER_A         = $20	
	
        section code

_set_custom_vbl:
        move    sr,-(sp)
        move    #$2700,sr       ; Disable interrupts
        move.l  $70.w,old_vbl   ; Save VBL
        move.l  #new_vbl,$70.w  ; Set new VBL with player
        move    (sp)+,sr        ; Enable interrupts
        rts

new_vbl:
        movem.l d0-a6,-(sp)

        ;; trigger timer A for sprite drawing
        move.b  #150, TIMER_A_DATA
        move.b  #6, TIMER_A_CONTROL

        ;; Play the music
        move.l  _soundtrack_vbl,a0
        jsr     (a0)
        movem.l (sp)+,d0-a6
old_vbl=*+2
        jmp     'Fixx'

_restore_vbl:
        move    sr,-(sp)
        move    #$2700,sr       ; disable interrupts
        move.l  old_vbl,$70.w   ;restore vbl
        move    (sp)+,sr        ;enable interrupts - tune will stop playing
        rts

_interrupt_handler:
        movem.l d0-a6, -(a7)
	;; Acknowledge interruption
        move.b  INTERRUPT_SERVICE_A, d0
        and.b   #(~IEA_TIMER_A), d0
        move.b  d0, INTERRUPT_SERVICE_A

	;; Re-enable VBL interrupt
        move    sr, d0
        and.l   #$f8ff, d0
        or.l    #$03, d0
        move    d0, sr

        ;; Turn off Timer A
        move.b  #0, TIMER_A_CONTROL

        jsr     _interrupt_draw_gnome
        movem.l (a7)+, d0-a6
        rte
        
_set_sprite_timer:
        move.l  #_interrupt_handler, INTERRUPT_VECTOR_A
	
        ;; Ensure Timer A interruption is not masked
        move.b  INTERRUPT_MASK_A, d0
        or.b    #IEA_TIMER_A, d0
        move.b  d0, INTERRUPT_MASK_A
	
	;; Enable Timer A interruption
        move.b  INTERRUPT_ENABLE_A, d0
        or.b    #IEA_TIMER_A, d0
        move.b  d0, INTERRUPT_ENABLE_A
        rts
