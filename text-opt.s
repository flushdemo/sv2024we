        xdef    _display_character_opt

        section code

_display_character_opt:
	movem.l	reg_lst, -(a7)
        ;; Retrieving parameters from stack
	move.l	(16 + reg_cnt, a7), a5  ; mask_ptr
	move.l	(12 + reg_cnt, a7), a6  ; font_ptr
	move.l	(8  + reg_cnt, a7), a4  ; background_ptr
	move.l	(4  + reg_cnt, a7), a3  ; video_ptr

	moveq	#15, d7         ; j
outer_loop:
	move.l	(a5), d4        ; ~mask

        macro MASK_FONT
        move.l  (\1, a6), d3    ; font_ptr[i]
	move.l	(\1, a4), d2    ; background_ptr[i]
        and.l   d4, d2          ; background_ptr[i] & ~mask
        or.l    d3, d2          ; (background_ptr[i] & ~mask) | (font_ptr[i] & mask)
        move.l  d2, (\1, a3)    ; store into video_ptr[i]
        endm

        MASK_FONT 4
        MASK_FONT 0

	add.l	#160,a4
	add.l	#160,a3
	add.l	#160,a6
	add.l	#160,a5

	subq.w	#1,d7
	bge	outer_loop

reg_lst	reg	a3/a4/a5/a6/d2/d3/d4/d7
	movem.l	(a7)+,a3/a4/a5/a6/d2/d3/d4/d7
reg_cnt	equ	4*8
	rts
