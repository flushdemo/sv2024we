	section	"CODE",code
	public	_init_sprite
	public	_update_sprite

_init_sprite:
    movem.l     d0-d7/a0-a6,-(a7)
    ; collect the palette data

    lea         garray,a6
    lea         gmskarray,a5
    moveq.l     #8-1,d7

loop_index_spr:
    move.l      (a6)+,a4     ; a4 pointer to sprite
    move.l      (a5)+,a3     ; a3 pointer to computed mask
    move.l      #9800/8-1,d6 ; 175 lines of 56 bytes (7 * 8 bytes (16 pixels * 4 planes))

loop_msk_compute:

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

    addq.l      #8,a4

    dbf         d6,loop_msk_compute
    dbf         d7,loop_index_spr



    movem.l     (a7)+,d0-d7/a0-a6
    rts



_update_sprite:
reg_cnt	equ	4*15
	movem.l	d0-d7/a0-a6,-(a7)
	move.w	(reg_cnt+14,a7),d1      ; clk (word)
	move.l  (reg_cnt+8,a7),a5       ; background_ptr (address)
	move.l	(reg_cnt+4,a7),a6       ; video_ptr (address)

gnome_show:
	and.w		#7,d1
	lsl.w	  	#2,d1
	lea			garray,a2      ; tableau des sprites
	lea			gmskarray,a3   ; tableau des masques
	move.l 	 	(a2,d1.w),a0 ; a0 spr
	move.l		(a3,d1.w),a4 ; a4 msk


	add.l		#16*160,a5
	add.l 		#16*160,a6
	move.l		#175-1,d2

	.shw_gnome_msk_frame_0:


	REPT 7
    move.w      (a4),d1		; msk nb
	swap		d1
	move.w		(a4),d1
	move.l		d1,d3
	and.l       0(a5),d1    ; background bit plane 1 and 2 and mask
	or.l        0(a0),d1    ; or sprite bit plane 1 and 2
	move.l		d1,0(a6)    ; a6 screen a5 background a0 sprite

	and.l		4(a5),d3   ; background bit plane 3 and 4 and mask
	or.l        4(a0),d3   ; or sprite bit plane 3 and 4
    move.l		d3,4(a6)
	
	addq.l		#8,a0
    addq.l      #8,a6
    addq.l      #8,a5
    addq.l      #2,a4
	ENDR
	
	add.l		#160-(7*8),a5        ; 7 blocks of 16pixels
	add.l		#160-(7*8),a6        ; 7 blocks of 16pixels
	dbf 		d2,.shw_gnome_msk_frame_0

	movem.l		(a7)+,d0-d7/a0-a6
	rts

	section "DATA",data


garray		dc.l g0f, g1f, g2f, g3f
			dc.l g4f, g5f, g6f, g7f

gmskarray	dc.l g0msk, g1msk, g2msk, g3msk
			dc.l g4msk, g5msk, g6msk, g7msk

g0f			incbin '.\g0spr.bin' ; the data of gnome and background for frame 1
g1f			incbin '.\g1spr.bin' ; the data of gnome and background for frame 2
g2f			incbin '.\g2spr.bin' ; the data of gnome and background for frame 3
g3f			incbin '.\g3spr.bin' ; the data of gnome and background for frame 4
g4f			incbin '.\g4spr.bin' ; the data of gnome and background for frame 5
g5f			incbin '.\g5spr.bin' ; the data of gnome and background for frame 6
g6f			incbin '.\g6spr.bin' ; the data of gnome and background for frame 7
g7f			incbin '.\g7spr.bin' ; the data of gnome and background for frame 8


        section bss

g0msk		ds.b 2450
g1msk		ds.b 2450
g2msk		ds.b 2450
g3msk		ds.b 2450
g4msk		ds.b 2450
g5msk		ds.b 2450
g6msk		ds.b 2450
g7msk		ds.b 2450
