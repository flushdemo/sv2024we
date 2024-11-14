

gemdos		EQU 1
bios		EQU 13
xbios		EQU 14

pterm0		EQU	$4c
supexec		EQU	38
setscreen	EQU	5
ccoin		EQU	1

nb_color	EQU	16
pi1_res		EQU	0
pi1_pal		EQU	2
pi1_start	EQU	(2+(2*nb_color))


_v_bas_ad	EQU 	$44e		; screen base address

	section text

start:
	dc.w $a00a
	pea.l	show
	move.w	#supexec,-(a7)
	trap	#xbios
	lea	6(a7),a7

	move.w	#0,-(a7)
	move.w	#pterm0,-(a7)
	trap	#gemdos
	lea	4(a7),a7
	
show:
	; clean bss palette variable
	moveq.l	#8-1,d0
	lea	oldpal,a1
	moveq.l	#0,d1
.cleanpal:
	move.l	d1,(a1)+
	dbf	d0,.cleanpal

	; backup palette
	movem.l	$ffff8240.w,d0-d7
	movem.l	d0-d7,oldpal

	; set low resolution
	clr.w	-(a7)
  	pea	-1.w
 	move.l	(a7),-(a7)
  	move.w	#setscreen,-(a7)
   	trap	#xbios
   	lea	12(a7),a7

	; set image palette
	movem.l	myBgnd+pi1_pal(pc),d0-d7
	movem.l	d0-d7,$ffff8240.w

	; transfer image to screen
	lea	myBgnd+pi1_start(pc),a0
	move.l	_v_bas_ad.w,a1
	
	move.l	#32000/4-1,d0
.shw_bg:
	move.l	(a0)+,(a1)+
	dbf	d0,.shw_bg

	; show gnome frame1 mask
	lea	myMskFrame0+pi1_start,a0
	move.l	_v_bas_ad.w,a1
	lea	16*160(a0),a0
	lea	16*160(a1),a1
	move.l #175,d1
.shw_gnome_msk_frame_0:
	; 1 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 2 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 3 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 4 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 5 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 6 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 7 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	and.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	add.l	#160-112,a1
	add.l	#160-112,a0
	dbf d1,.shw_gnome_msk_frame_0
	; show gnome frame 0

	; show gnome frame1 mask
	lea	gnomeFrame0+pi1_start,a0
	move.l	_v_bas_ad.w,a1
	lea	16*160(a0),a0
	lea	16*160(a1),a1
	move.l #175,d1

.shw_gnome_frame_0:
	; 1 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 2 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 3 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 4 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 5 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 6 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	; 7 block of 16 pixels
	REPT 4
	move.l	(a0)+,d0
	or.l	(a1),d0
	move.l	d0,(a1)+
	ENDR
	add.l	#160-112,a1
	add.l	#160-112,a0
	dbf d1,.shw_gnome_frame_0


	; wait a key press
	move.w	#ccoin,-(sp)
  	trap	#gemdos
  	lea	2(a7),a7

	; set mid resolution
	move.w	#1,-(a7)
	pea	-1.w
	move.l	(a7),-(a7)
	move.w	#setscreen,-(a7)
	trap	#xbios
	lea	12(a7),a7

	; restore pal
	movem.l	oldpal,d0-d7
	movem.l	d0-d7,$ffff8240.w



	
	rts


	section data

	even
myBgnd incbin .\Assets\fond.pi1
myMskFrame0	incbin .\Assets\g7msk.pi1
gnomeFrame0 incbin .\Assets\g7.pi1


	section bss
oldpal	ds.l 8
