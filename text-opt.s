        xref    _font_mask
        xdef    _display_character_opt

	dsource	"text.c"
	debug	1
	symdebug
	opt o+,ol+,op+,oc+,ot+,oj+,ob+,om+
	public	_display_character_opt
	cnop	0,4
_display_character_opt: 
	movem.l	l23,-(a7)
	move.w	(18 + l25, a7), d6  ; chr
	move.l	(12 + l25, a7), a6  ; font_base
	move.l	(8  + l25, a7), a4  ; background_ptr
	move.l	(4  + l25, a7), a3  ; video_ptr
	dsource	"text.c"
	debug	45
	moveq	#0,d0
	move.w	d6,d0
	move.l	d0,-(a7)
	jsr	_font_position
	debug	45
	move.w	d0,d5           ; font_pos
	debug	46
	moveq	#0,d0
	move.w	d5,d0
	lsl.l	#1,d0           ; ??? why
	lea	(a6,d0.l),a0
	move.l	a0,a2           ; font_ptr
	debug	47
	move.w	d5,d0
	lsl.w	#1,d0
	and.l	#65535,d0
	lea	_font_mask,a0           ; font_mask
	add.l	d0,a0
	move.l	a0,a5           ; mask_ptr
	debug	49
	moveq	#15,d4
	addq.w	#4,a7
	bra	l16
l15
	debug	50
	move.w	(a5),d3
	debug	51
	moveq	#3,d2
	bra	l20
l19
	debug	52
	move.w	d2,d0
	ext.l	d0
	lsl.l	#1,d0
	lea	(a4,d0.l),a0
	moveq	#0,d0
	move.w	d3,d0
	not.l	d0
	and.w	(a0),d0
	move.w	d2,d1
	ext.l	d1
	lsl.l	#1,d1
	lea	(a2,d1.l),a0
	move.w	(a0),d1
	and.w	d3,d1
	or.w	d1,d0
	move.w	d2,d1
	ext.l	d1
	lsl.l	#1,d1
	move.w	d0,(0,a3,d1.l)
	debug	51
l22
	debug	51
	subq.w	#1,d2
l20
	debug	51
	tst.w	d2
	bge	l19
	debug	54
l21
	debug	54
	add.l	#160,a4
	debug	55
	add.l	#160,a3
	debug	56
	add.l	#160,a2
	debug	57
	add.l	#160,a5
	debug	49
l18
	debug	49
	subq.w	#1,d4
l16
	debug	49
	tst.w	d4
	bge	l15
	debug	59
l17
	debug	61
l13
l23	reg	a2/a3/a4/a5/a6/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/a3/a4/a5/a6/d2/d3/d4/d5/d6
l25	equ	40
	rts
; stacksize=52
