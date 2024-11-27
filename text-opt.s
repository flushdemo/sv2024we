        xref    _font_mask
        xdef    _display_character_opt

_display_character_opt:
	movem.l	reg_lst, -(a7)
	move.w	(18 + reg_cnt, a7), d6  ; chr
	move.l	(12 + reg_cnt, a7), a6  ; font_base
	move.l	(8  + reg_cnt, a7), a4  ; background_ptr
	move.l	(4  + reg_cnt, a7), a3  ; video_ptr

	move.l	d6, -(a7)       ; Only word will be used, but needs long alignment
        ;; d6 is not used anymore at this point

	jsr	_font_position
	addq.w	#4, a7          ; Fix stack
	move.w	d0, d5
        ;; multiply by 2 cause items are words
        lsl.w   #1, d5          ; font_pos * 2

	lea	(a6, d5.w), a2  ; font_ptr
	lea	_font_mask, a5
	add.w	d5, a5          ; mask_ptr
        ;; d5 is not used anymore

	moveq	#15, d4         ; j
outer_loop:
	move.w	(a5), d3        ; mask
	moveq	#3, d2          ; i
inner_loop:
	move.w	d2, d5
	lsl.w	#1, d5          ; 2*i for word items
        move.w  (a2, d5.w), d6  ; font_ptr[i]
        and.w   d3, d6          ; font_ptr[i] & mask
        move.w  d3, d1          ; mask
        not.w   d1              ; ~mask
	and.w	(a4, d5.w), d1  ; background_ptr[i] & ~mask
        or.w    d1, d6          ; (background_ptr[i] & ~mask) | (font_ptr[i] & mask)
        move.w  d6, (a3, d5.w)  ; store into video_ptr[i]

	subq.w	#1,d2
	bge	inner_loop

	add.l	#160,a4
	add.l	#160,a3
	add.l	#160,a2
	add.l	#160,a5

	subq.w	#1,d4
	bge	outer_loop

reg_lst	reg	a2/a3/a4/a5/a6/d2/d3/d4/d5/d6
	movem.l	(a7)+,a2/a3/a4/a5/a6/d2/d3/d4/d5/d6
reg_cnt	equ	40
	rts
