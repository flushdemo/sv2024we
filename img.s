

gemdos						EQU	 	1
bios						EQU		13
xbios						EQU		14

pterm0						EQU		$4c
supexec						EQU		38
setscreen					EQU		5
ccoin						EQU		1

nb_color					EQU		16
pi1_res						EQU		0
pi1_pal						EQU		2
pi1_start					EQU		(2+(2*nb_color))

screen_width_in_pixels				=		320
screen_height					=		200
screen_nb_planes				=		4
screen_nb_planes_in_bytes			=		(4*2) ;4 planes of 2 bytes each = 8 
screen_line_size_in_bytes			=		((screen_width_in_pixels*screen_nb_planes)/screen_nb_planes_in_bytes) 
screen_size_in_bytes				=		(((screen_width_in_pixels*screen_nb_planes)/screen_nb_planes_in_bytes)*screen_height)

_v_bas_ad					EQU 		$44e		; screen base address (idem logbase)

	section text

start:
	lea	screen,a0
	move.l	#(((screen_size_in_bytes*2)+256)/4)-1,d0
	moveq.l #0,d1
loop_clr_scr:
	move.l d1,(a0)+
	dbf d1,loop_clr_scr


	pea.l	show
	move.w	#supexec,-(a7)
	trap	#xbios
	lea	6(a7),a7

	move.w	#0,-(a7)
	move.w	#pterm0,-(a7)
	trap	#gemdos
	lea	4(a7),a7
	
show:
backup_pal:
	; clean bss palette variable
	moveq.l	#((2*nb_color)/4)-1,d0	; 16 colors (a color requiring 2 bytes (a word))
	lea	oldpal,a1
	moveq.l	#0,d1
.cleanpal:
	move.l	d1,(a1)+
	dbf	d0,.cleanpal

	; backup palette
	movem.l	$ffff8240.w,d0-d7
	movem.l	d0-d7,oldpal

set_resolution:
	; set low resolution
	clr.w	-(a7)
  	pea	-1.w
 	move.l	(a7),-(a7)
  	move.w	#setscreen,-(a7)
   	trap	#xbios
   	lea	12(a7),a7

set_bg_pal:
	; set image palette
	movem.l	myBgnd+pi1_pal(pc),d0-d7
	movem.l	d0-d7,$ffff8240.w

set_bg:
	; transfer image to screen
	lea	myBgnd+pi1_start(pc),a0
	move.l	_v_bas_ad.w,a1
	
	move.l	#screen_size_in_bytes/4-1,d0
.shw_bg:
	move.l	(a0)+,(a1)+
	dbf	d0,.shw_bg

	moveq.l	#8-1,d7
	lea	garray,a2
loop_characters:
	; show gnome frame1 mask
	move.l	(a2)+,a0
	move.l	_v_bas_ad.w,a1
	lea	16*screen_line_size_in_bytes(a1),a1
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
	add.l	#screen_line_size_in_bytes-56,a1
	dbf d1,.shw_gnome_msk_frame_0
	; show gnome frame 0

	;wait a key press
	move.w	#ccoin,-(sp)
	trap	#gemdos
	lea	2(a7),a7
	dbf	d7,loop_characters
; ; backup_bg_to_pi1:
; ; 	lea	buffer_outfile,a0
; ; 	move.w	#0,(a0)+

; ; 	movem.l $ffff8240.w,d0-d7
; ; 	movem.l	d0-d7,(a0)
	
; ; 	move.l	_v_bas_ad.w,a1
; ; 	lea		32(a0),a0

; ; 	move.l	#screen_size_in_bytes/4-1,d0
; ; .fill_file:
; ; 	move.l	(a1)+,(a0)+
; ; 	dbf	d0,.fill_file

; 	move.w #32/2-1,d0
; .fill_endoffile:
; 	move.w #0,(a0)+
; 	dbf d0,.fill_endoffile

	; ; move.w	#0,-(a7)
	; ; pea		outfile_fname
	; ; move.w	#$3c,-(a7)		; fcreate
	; ; trap	#gemdos
	; ; addq.l	#8,a7

	; ; move.w	d0,handle_outfile

	; ; pea.l buffer_outfile
	; ; move.l	#32066,-(a7)
	; ; move.w	d0,-(a7)
	; ; move.w	#$40,-(a7)
	; ; trap	#gemdos
	; ; lea		$c(a7),a7

	; ; move.w	handle_outfile,-(a7)
	; ; move.w	#$3e,-(a7)
	; ; trap	#gemdos
	; ; addq.l	#4,a7



end_program:	
restore_screen:
	; set mid resolution
	move.w	#1,-(a7)
	pea	-1.w
	move.l	(a7),-(a7)
	move.w	#setscreen,-(a7)
	trap	#xbios
	lea	12(a7),a7

restore_pal:
	; restore pal
	movem.l	oldpal,d0-d7
	movem.l	d0-d7,$ffff8240.w

EOF:	
	rts


	section data

	even
myBgnd incbin .\Assets\fond.pi1
myMskFrame0	incbin .\Assets\g0msk.pi1
gnomeFrame0 incbin .\Assets\g0.pi1
buffer_outfile	dcb 32066,0
handle_outfile	dc.w 0
outfile_fname	dc.b "..\toto\g0f.pi1",0

garray	dc.l g0f, g1f, g2f, g3f
		dc.l g4f, g5f, g6f, g7f

g0f	incbin '.\g0f.bin' ; the data of gnome and background for frame 1
g1f	incbin '.\g1f.bin' ; the data of gnome and background for frame 1
g2f	incbin '.\g2f.bin' ; the data of gnome and background for frame 1
g3f	incbin '.\g3f.bin' ; the data of gnome and background for frame 1
g4f	incbin '.\g4f.bin' ; the data of gnome and background for frame 1
g5f	incbin '.\g5f.bin' ; the data of gnome and background for frame 1
g6f	incbin '.\g6f.bin' ; the data of gnome and background for frame 1
g7f	incbin '.\g7f.bin' ; the data of gnome and background for frame 1
	section bss
oldpal	ds.l 8
screen	ds.l	((screen_size_in_bytes*2)+256)/4 ; 2 screen, the frontbuffer and backbuffer