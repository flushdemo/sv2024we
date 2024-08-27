

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
	lea	oldpal(pc),a1
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
	movem.l	myPicture+pi1_pal,d0-d7
	movem.l	d0-d7,$ffff8240.w

	; transfer image to screen
	lea	myPicture+pi1_start,a0
	move.l	_v_bas_ad.w,a1
	
	move.l	#32000/4-1,d0
.shw_img:
	move.l	(a0)+,(a1)+
	dbf	d0,.shw_img

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
myPicture incbin theimage.pi1


	section bss
oldpal	ds.l 8