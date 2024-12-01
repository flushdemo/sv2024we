        xdef _hide_mouse
        xdef _show_mouse
        xdef _disable_conterm
        xdef _enable_conterm

;;; CODE section ;;;
        section code
_hide_mouse:
        movem.l d2/a2,-(sp)
        dc.w    $A00A
        movem.l (sp)+,d2/a2
        rts

_show_mouse:
        movem.l d2/a2,-(sp)
        dc.w    $A009
        movem.l (sp)+,d2/a2
        rts

_disable_conterm:
        movem.l a0,-(sp)
        lea     old_conterm,a0
        move.b  $484.w,(a0)
        bclr.b  #0,$484.w
        movem.l (sp)+,a0
        rts

_enable_conterm:
        move.b  old_conterm,$484.w
        rts

        section bss
old_conterm     ds.w    1
