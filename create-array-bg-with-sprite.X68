*-----------------------------------------------------------
* Title      : create-array-bg-with-sprite
* Written by : mara
* Date       : 24/11/2024
* Description: create the picture with the characters masked
*              then create a binary file of these data only
*              to make a incbin on img.s file.
*-----------------------------------------------------------
    ORG    $1000

nb_color                            EQU		16
pi1_pal						        EQU		2
pi1_start					        EQU		(2+(2*nb_color))


screen_width_in_pixels				EQU		320
screen_height					    EQU		200
screen_nb_planes				    EQU		4
screen_nb_planes_in_bytes			EQU		(4*2) ;4 planes of 2 bytes each = 8 
screen_line_size_in_bytes			EQU		((screen_width_in_pixels*screen_nb_planes)/screen_nb_planes_in_bytes) 
screen_size_in_bytes				EQU		(((screen_width_in_pixels*screen_nb_planes)/screen_nb_planes_in_bytes)*screen_height)
    
    
START:                  ; first instruction of program
backup_pal:
	; clean bss palette variable
	move.l      #((2*nb_color)/4)-1,d0	; 16 colors (a color requiring 2 bytes (a word))
	lea         pal,a0
	move.l      #$deadcafe,d1
.cleanpal:
	move.l      d1,(a0)+
	dbf         d0,.cleanpal

; fill the variable with pic_bg palette
    lea         pic_bg+pi1_pal,a0
	movem.l     (a0),d0-d7
	movem.l     d0-d7,pal
	
; fill the pic_bg to pic_final
    lea         pic_bg+pi1_start,a0
    lea         pic_final,a1
	move.l      #screen_size_in_bytes/4-1,d0
.shw_bg:
	move.l      (a0)+,(a1)+
	dbf         d0,.shw_bg
    
	; show gnome frame1 mask
	lea         spr_msk+pi1_start,a0
	lea         pic_final,a1
	lea         16*screen_line_size_in_bytes(a0),a0
	lea         16*screen_line_size_in_bytes(a1),a1
	move.l      #175-1,d1
	; d1 oqp
	; d2-d7 ok
	; d0 ok
.shw_gnome_msk_frame_0:
	; 1 block of 16 pixels
	; get and apply the mask for the first block of 16 pixels
	; (4 planes) : a long word get 2 planes.
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+

	; 2 block of 16 pixels
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+

	; 3 block of 16 pixels
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	
	; 4 block of 16 pixels
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	
	; 5 block of 16 pixels
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	
	; 6 block of 16 pixels
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	
	; 7 block of 16 pixels
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	and.l       (a1),d0
	move.l      d0,(a1)+
	
	add.l       #screen_line_size_in_bytes-56,a1
	add.l       #screen_line_size_in_bytes-56,a0
	dbf         d1,.shw_gnome_msk_frame_0
	; show gnome frame 0

	; show gnome frame1 mask
	lea         spr_pic+pi1_start,a0
	lea         pic_final,a1
	lea         16*screen_line_size_in_bytes(a0),a0
	lea         16*screen_line_size_in_bytes(a1),a1
	move.l      #175-1,d1

.shw_gnome_frame_0:
	; 1 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	; 2 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	; 3 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	; 4 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	; 5 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	; 6 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	; 7 block of 16 pixels
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+
	move.l      (a0)+,d0
	or.l        (a1),d0
	move.l      d0,(a1)+

	add.l       #screen_line_size_in_bytes-56,a1
	add.l       #screen_line_size_in_bytes-56,a0
	dbf         d1,.shw_gnome_frame_0

construct_pic_pi1:
    lea         pic_final_buffer,a1
    move.w      #$0,(a1)+
    
    movem.l     pal,d0-d7
    movem.l     d0-d7,(a1)
    lea         32(a1),a1
    
    lea         pic_final,a0
    move.l      #((screen_size_in_bytes)/4)-1,d0
.loop:
    move.l      (a0)+,(a1)+
    dbf         d0,.loop
    
 	move.l      #32/2-1,d0
.fill_endoffile:
 	move.w      #0,(a1)+
	dbf         d0,.fill_endoffile

* Put program code here

put_pal_in_file:    
    lea         pi1_file,a1
    move.b      #52,d0
    trap        #15
    
    lea         pic_final_buffer,a1
    move.l      #32066,d2
    move.b      #54,d0
    trap        #15
    
    
    move        #50,d0
    trap        #15


    lea         pic_spr_final,a1
    move.l      #$deaddead,d1
    move.l      #((screen_size_in_bytes)/4)-1,d0
clean:
    move.l      d1,(a1)+
    dbf         d0,clean
    
    ; copy each long word value of character masked with background
copytobuf:
    lea         pic_final,a0
    lea         pic_spr_final,a1
	lea         16*screen_line_size_in_bytes(a0),a0
    move.l      #175-1,d0
.loop:
    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    move.l      (a0)+,(a1)+
	move.l      (a0)+,(a1)+

    add.l       #screen_line_size_in_bytes-56,a0
    dbf         d0,.loop

put_spr_in_file:    
    lea         pic_spr_bin_file,a1
    move.b      #52,d0
    trap        #15
    
    lea         pic_spr_final,a1
    move.l      #9800,d2
    move.b      #54,d0
    trap        #15
    
    
    move        #50,d0
    trap        #15

    SIMHALT             ; halt simulator

* Put variables and constants here
spr_pic             INCBIN      '.\Assets\g1.pi1',0
spr_msk             INCBIN      '.\Assets\g1msk.pi1',0

pic_bg              INCBIN      '.\Assets\fond.pi1',0

pi1_file            dc.b        '.\e68kpi1.pi1',0 ; picture with character masked and bg 

pic_spr_bin_file    dc.b        '.\e68kspr.bin',0 ; picture with only the character and bg in a bytes array


pal                 ds.l        8

pic_final           ds.l        (screen_size_in_bytes)/4 ; 2 screen, the frontbuffer and backbuffer 

pic_spr_final       ds.l        (((8*7)*175)/4) : 9800 bytes

pic_final_buffer    ds.l        (screen_size_in_bytes+66)/4
 

    END    START        ; last line of source





*~Font name~Fixedsys~
*~Font size~9~
*~Tab type~1~
*~Tab size~4~
