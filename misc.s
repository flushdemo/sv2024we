        xdef _hide_mouse
        xdef _show_mouse

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
