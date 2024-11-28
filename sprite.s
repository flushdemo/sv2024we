	section	"CODE",code
	public	_update_sprite

_init_sprite:

	rts

_update_sprite:
reg_cnt	equ	4*15
	movem.l	d0-d7/a0-a6,-(a7)
	move.w	(reg_cnt+14,a7),d1      ; clk (word)
        ;move.l  (reg_cnt+8,a7),a2       ; background_ptr (address)
	move.l	(reg_cnt+4,a7),a6       ; video_ptr (address)

gnome_show:
	and.l	#7,d1
	lsl.l   #2,d1
	lea		garray,a2
	move.l  (a2,d1.l),a0

loop_characters:
	; show gnome frame1 mask

	movea.l	a6,a1
	lea	16*160(a1),a1
	move.l #175-1,d1
	; d1 oqp
	; d2-d7 ok
	; d0 ok
.shw_gnome_msk_frame_0:
	; 1 block of 16 pixels
	;movem.l	(a0)+,d2-d4

	; get and apply the mask for the first block of 16 pixels
	; (4 planes) : a long word get 2 planes.
	; ; movem.l	(a0)+,d0/d2-d7/a3-a5
	; ; movem.l	d0/d2-d7/a3-a5,(a1)
	; ; lea  40(a1),a1
	; ; movem.l	(a0)+,d2-d5
	; ; movem.l	d2-d5,(a1)
	; ; lea  16(a1),a1
	REPT 2*7
	move.l	(a0)+,(a1)+
	ENDR
	add.l	#160-(7*8),a1        ; 7 blocks of 16pixels
	dbf d1,.shw_gnome_msk_frame_0

	movem.l	(a7)+,d0-d7/a0-a6
	rts

	section "DATA",data

garray		dc.l g0f, g1f, g2f, g3f
			dc.l g4f, g5f, g6f, g7f

g0f			incbin '.\g0spr.bin' ; the data of gnome and background for frame 1
g1f			incbin '.\g1spr.bin' ; the data of gnome and background for frame 1
g2f			incbin '.\g2spr.bin' ; the data of gnome and background for frame 1
g3f			incbin '.\g3spr.bin' ; the data of gnome and background for frame 1
g4f			incbin '.\g4spr.bin' ; the data of gnome and background for frame 1
g5f			incbin '.\g5spr.bin' ; the data of gnome and background for frame 1
g6f			incbin '.\g6spr.bin' ; the data of gnome and background for frame 1
g7f			incbin '.\g7spr.bin' ; the data of gnome and background for frame 1
