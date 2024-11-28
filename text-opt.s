        xref    _font_mask
        xdef    _display_character_opt

        section code

_display_character_opt:
	movem.l	reg_lst, -(a7)
        ;; Retrieving parameters from stack
	move.w	(18 + reg_cnt, a7), d0  ; chr
	move.l	(12 + reg_cnt, a7), a6  ; font_base
	move.l	(8  + reg_cnt, a7), a4  ; background_ptr
	move.l	(4  + reg_cnt, a7), a3  ; video_ptr

	move.l	d0, -(a7)       ; Only word will be used, but needs long alignment
	jsr	_font_position
	addq.w	#4, a7          ; Fix stack
        lsl.w   #1, d0          ; font_pos * 2 - multiply by 2 cause items are words

	lea	(a6, d0.w), a6  ; font_ptr
	lea	_font_mask, a5
	add.w	d0, a5          ; mask_ptr
        ;; d0 can be reused

	moveq	#15, d7         ; j
outer_loop:
	move.w	(a5), d5        ; mask
        move.w  d5, d4
        not.w   d4              ; ~mask

        macro MASK_FONT
        move.w  (\1, a6), d3    ; font_ptr[i]
        and.w   d5, d3          ; font_ptr[i] & mask
	move.w	(\1, a4), d2    ; background_ptr[i]
        and.w   d4, d2          ; background_ptr[i] & ~mask
        or.w    d3, d2          ; (background_ptr[i] & ~mask) | (font_ptr[i] & mask)
        move.w  d2, (\1, a3)    ; store into video_ptr[i]
        endm

        MASK_FONT 6
        MASK_FONT 4
        MASK_FONT 2
        MASK_FONT 0

	add.l	#160,a4
	add.l	#160,a3
	add.l	#160,a6
	add.l	#160,a5

	subq.w	#1,d7
	bge	outer_loop

reg_lst	reg	a3/a4/a5/a6/d2/d3/d4/d5/d7
	movem.l	(a7)+,a3/a4/a5/a6/d2/d3/d4/d5/d7
reg_cnt	equ	4*9
	rts
