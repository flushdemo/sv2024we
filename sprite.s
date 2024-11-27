	section	"CODE",code
	public	_update_sprite

_update_sprite:
        movem.l d0-a6,-(sp)             ; Save every register
	move.w	(reg_cnt+14,a7),d1      ; clk (word)
        move.l  (reg_cnt+8,a7),a2       ; background_ptr (address)
	move.l	(reg_cnt+4,a7),a1       ; video_ptr (address)

	add.l	#14416,a1
	moveq	#0,d0
	bra	l4
l3
	move.w	d1,(a1)
	add.l	#160,a1
	addq.w	#1,d0
l4
	cmp.w	#20,d0
	bcs	l3
        movem.l (sp)+,d0-a6
reg_cnt	equ	4*15
	rts
