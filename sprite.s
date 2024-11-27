	section	"CODE",code
	public	_update_sprite

_update_sprite:
    movem.l d0-a6,-(sp)             ; Save every register
	move.w	(reg_cnt+14,a7),d1      ; clk (word)
    move.l  (reg_cnt+8,a7),a2       ; background_ptr (address)
	move.l	(reg_cnt+4,a7),a1       ; video_ptr (address)

	move.l	a1,a6



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

	bsr gnome_show

    
	movem.l (sp)+,d0-a6
reg_cnt	equ	4*15
	rts

gnome_show:
	movem.l	d0-d7/a0-a6,-(a7)

	lsr.l	#2,d1
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
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	; 2 block of 16 pixels
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	; 3 block of 16 pixels
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	; 4 block of 16 pixels
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	; 5 block of 16 pixels
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	; 6 block of 16 pixels
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	; 7 block of 16 pixels
	REPT 2
	move.l	(a0)+,(a1)+
	ENDR
	add.l	#160-(7*8),a1
	dbf d1,.shw_gnome_msk_frame_0

	movem.l	(a7)+,d0-d7/a0-a6
	rts

	section "DATA",data
	
garray		dc.l g0f, g1f, g2f, g3f
			dc.l g4f, g5f, g6f, g7f

g0f			incbin '.\g0f.bin' ; the data of gnome and background for frame 1
g1f			incbin '.\g1f.bin' ; the data of gnome and background for frame 1
g2f			incbin '.\g2f.bin' ; the data of gnome and background for frame 1
g3f			incbin '.\g3f.bin' ; the data of gnome and background for frame 1
g4f			incbin '.\g4f.bin' ; the data of gnome and background for frame 1
g5f			incbin '.\g5f.bin' ; the data of gnome and background for frame 1
g6f			incbin '.\g6f.bin' ; the data of gnome and background for frame 1
g7f			incbin '.\g7f.bin' ; the data of gnome and background for frame 1



