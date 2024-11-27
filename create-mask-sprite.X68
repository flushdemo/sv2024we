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
    ; collect the palette data
    movem.l     spr_pic+pi1_pal,d0-d7
    movem.l     d0-d7,pal    

    ; clean pic_final variable
    lea         pic_final,a0
	move.l      #32000/4-1,d0
	moveq.l     #0,d1
.loop:
    move.l      d1,(a0)+
    dbf         d0,.loop

	; show gnome frame1 mask
	lea         spr_pic+pi1_start,a0
	lea         pic_final,a1
	move.l      #200-1,d1

.shw_gnome_frame_0:
	move.l      #20-1,d7

.loopinner:
    moveq.l     #0,d0
    moveq.l     #0,d2
    moveq.l     #0,d3
    moveq.l     #0,d4
    

; 1 block of 16 pixels
	move.w      0(a0),d0
	move.w      2(a0),d2	
    move.w      4(a0),d3
    move.w      6(a0),d4
	
    ; plane 1
	or.w        d0,0(a1)
	or.w        d2,0(a1)
	or.w        d3,0(a1)
	or.w        d4,0(a1)
	not.w       0(a1)

    ; plane 2
	or.w        d0,2(a1)
	or.w        d2,2(a1)
	or.w        d3,2(a1)
	or.w        d4,2(a1)
	not.w       2(a1)
	
	; plane 3
	or.w        d0,4(a1)
	or.w        d2,4(a1)
	or.w        d3,4(a1)
	or.w        d4,4(a1)
	not.w       4(a1)
	
	; plane 4
	or.w        d0,6(a1)
	or.w        d2,6(a1)
	or.w        d3,6(a1)
	or.w        d4,6(a1)
	not.w       6(a1)

    lea         8(a0),a0
    lea         8(a1),a1
    
    dbf         d7,.loopinner
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

    
    SIMHALT             ; halt simulator

* Put variables and constants here
spr_pic             INCBIN      '.\Assets\g7.pi1',0

pi1_file            dc.b        '.\g7msk.pi1',0 ; picture with character masked and bg 

pic_final           ds.l        (screen_size_in_bytes)/4 ; 2 screen, the frontbuffer and backbuffer 

pal                 ds.l        8

pic_final_buffer    ds.l        (screen_size_in_bytes+66)/4
 

    END    START        ; last line of source









*~Font name~Fixedsys~
*~Font size~9~
*~Tab type~1~
*~Tab size~4~
