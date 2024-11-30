	section	"CODE",code
	public	_init_sprite
	public	_update_sprite

_init_sprite:
     ;movem.l     d0-d7/a0-a6,-(a7)
    ; collect the palette data

    lea         garray,a6
    lea         gmskarray,a5
    moveq.l     #8-1,d7

loop_index_spr:
    move.l      (a6)+,a4     ; a4 pointer to sprite
    move.l      (a5)+,a3     ; a3 pointer to computed mask
    move.l      #9800/8-1,d6

loop_msk_compute:
    moveq.l     #0,d1
    moveq.l     #0,d2
    moveq.l     #0,d3
    moveq.l     #0,d4


; 1 block of 16 pixels
	move.w      0(a4),d1
	move.w      2(a4),d2
    move.w      4(a4),d3
    move.w      6(a4),d4

	or.w        d1,d2
	or.w        d2,d3
	or.w        d3,d4
	not.w       d4
	move.w      d4,(a3)+

    lea         8(a4),a4

    dbf         d6,loop_msk_compute
    dbf         d7,loop_index_spr


    movem.l     (a7)+,d0-d7/a0-a6
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
	lea		gmskarray,a3
	move.l  (a2,d1.l),a0 ; a0 spr
	move.l	(a3,d1.l),a4 ; a4 msk

loop_characters:
	; show gnome frame1 mask

	movea.l	a5,a1		; a1 video ptr
	lea	16*160(a1),a1
	move.l #175-1,d2
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
	REPT 7
    move.w      (a4),d1
    move.l      (a0),d4
    move.l      4(a0),d3
    and.w       d1,0(a1)
    and.w       d1,2(a1)
    and.w       d1,4(a1)
    and.w       d1,6(a1)
    or.l        d4,0(a1)
    or.l        d3,4(a1)
    lea         8(a0),a0
    lea         2(a4),a4
    lea         8(a1),a1
	ENDR
	
	add.l	#160-(7*8),a1        ; 7 blocks of 16pixels
	dbf d2,.shw_gnome_msk_frame_0

	movem.l	(a7)+,d0-d7/a0-a6
	rts

	section "DATA",data


garray		dc.l g0f, g1f, g2f, g3f
			dc.l g4f, g5f, g6f, g7f

gmskarray	dc.l g0msk, g1msk, g2msk, g3msk
			dc.l g4msk, g5msk, g6msk, g7msk

g0f			incbin '.\g0spr.bin' ; the data of gnome and background for frame 1
g1f			incbin '.\g1spr.bin' ; the data of gnome and background for frame 1
g2f			incbin '.\g2spr.bin' ; the data of gnome and background for frame 1
g3f			incbin '.\g3spr.bin' ; the data of gnome and background for frame 1
g4f			incbin '.\g4spr.bin' ; the data of gnome and background for frame 1
g5f			incbin '.\g5spr.bin' ; the data of gnome and background for frame 1
g6f			incbin '.\g6spr.bin' ; the data of gnome and background for frame 1
g7f			incbin '.\g7spr.bin' ; the data of gnome and background for frame 1


        section bss

g0msk		dcb.b 2450
g1msk		dcb.b 2450
g2msk		dcb.b 2450
g3msk		dcb.b 2450
g4msk		dcb.b 2450
g5msk		dcb.b 2450
g6msk		dcb.b 2450
g7msk		dcb.b 2450
