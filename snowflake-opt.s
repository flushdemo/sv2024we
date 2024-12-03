ASM_SNOW_FLAKE_HEIGHT equ 10 ; To synchronize with common.h SNOW_FLAKE_HEIGHT value

	section	"CODE",code

	public	_display_flake_sprite_opt
	cnop	0,4
_display_flake_sprite_opt:
	movem.l	regsf_lst, -(a7)
	move.l	(24 + regsf_cnt, a7), d6 ; foreground_flake_flag
	move.l	(20 + regsf_cnt, a7), a5 ; background_mask
	move.l	(16 + regsf_cnt, a7), a3 ; flake_pic
	move.l	(12 + regsf_cnt, a7), a2 ; background_ptr
	move.l	( 8 + regsf_cnt, a7), a1 ; backsnow_ptr
	move.l	( 4 + regsf_cnt, a7), a0 ; video_ptr

;;; Inner loop
        MACRO APPLY_FLAKE_MASK
        move.l  (\1, a2), d1    ; bg
        and.l   d4, d1          ; bg & bm
        or.l    (\1, a3), d1    ; sp | (bg & bm)
        move.l  d1, (\1, a1)    ; -> backsnow_ptr[j]

        ;; Drawing foreground or not ?
        tst.w   d6
        beq     \@_end_inner_loop
        move.l  d1, (\1, a0)    ; -> video_ptr[j]
\@_end_inner_loop:
        ENDM
;;; Inner loop

;;; Outer loop
        REPT    ASM_SNOW_FLAKE_HEIGHT
        move.l  (4*REPTN, a5), d4  ; bm

        APPLY_FLAKE_MASK 0
        APPLY_FLAKE_MASK 4

        ;; Increase pointers
	add.l	#160, a0
	add.l	#160, a1
	add.l	#160, a2
	add.l	  #8, a3
        ENDR
;;; Outer loop

regsf_lst:      reg	a2/a3/a5/d2/d4/d6
	        movem.l	(a7)+,a2/a3/a5/d2/d4/d6
regsf_cnt       equ	(4*6)
	        rts
