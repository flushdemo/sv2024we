ASM_SNOW_FLAKE_HEIGHT equ 7 ; To synchronize with snowflake.h value

	section	"CODE",code

	public	_display_flake_sprite_opt
	cnop	0,4
_display_flake_sprite_opt:
	movem.l	regsf_lst, -(a7)
	move.l	(32 + regsf_cnt, a7), d6 ; foreground_flake_flag
	move.l	(28 + regsf_cnt, a7), a6 ; foreground_mask
	move.l	(24 + regsf_cnt, a7), a5 ; background_mask
	move.l	(20 + regsf_cnt, a7), a4 ; flake_mask
	move.l	(16 + regsf_cnt, a7), a3 ; flake_pic
	move.l	(12 + regsf_cnt, a7), a2 ; background_ptr
	move.l	( 8 + regsf_cnt, a7), a1 ; backsnow_ptr
	move.l	( 4 + regsf_cnt, a7), a0 ; video_ptr

        MACRO DUPLICATE_WORD_UPPER
        move.w  \1, d0
        swap    \1
        move.w  d0, \1
        ENDM

;;; Inner loop
        MACRO APPLY_FLAKE_MASK
        move.l  (\1, a3), d2    ; sp
        move.l  (\1, a2), d1    ; bg

        move.l  d5, d0          ; sm
        and.l   d1, d0          ; bg & sm
        or.l    d2, d0          ; sp | (bg & sm)
        move.l  d0, (\1, a1)    ; -> backsnow_ptr[j]

        ;; Drawing foreground or not ?
        tst.w   d6
        beq     \@_end_inner_loop

        ;; reusing d1
        and.l   d4, d1          ; bg & bm
        or.l    d2, d1          ; sp | (bg & bm)
        move.l  (\1, a0), d0    ; video_ptr[j]
        and.l   d3, d0          ; video_ptr[j] & fm
        or.l    d1, d0          ; (sp | (bg & bm)) | (video_ptr[j] & fm)
        move.l  d0, (\1, a0)    ; -> video_ptr[j]
\@_end_inner_loop:
        ENDM
;;; Inner loop

;;; Outer loop
        REPT    ASM_SNOW_FLAKE_HEIGHT
        move.w  (2*REPTN, a4), d5  ; sm
        DUPLICATE_WORD_UPPER d5
        move.w  (2*REPTN, a5), d4  ; bm
        DUPLICATE_WORD_UPPER d4
        move.w  (2*REPTN, a6), d3  ; fm
        DUPLICATE_WORD_UPPER d3

        APPLY_FLAKE_MASK 0
        APPLY_FLAKE_MASK 4

        ;; Increase pointers
	add.l	  #8, a3
	add.l	#160, a0
	add.l	#160, a1
	add.l	#160, a2
        ENDR
;;; Outer loop

regsf_lst:      reg	a2/a3/a4/a5/a6/d2/d3/d4/d5/d6/d7
	        movem.l	(a7)+,a2/a3/a4/a5/a6/d2/d3/d4/d5/d6/d7
regsf_cnt       equ	44
	        rts
