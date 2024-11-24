*-----------------------------------------------------------
* Title      : extract-pal-pi1
* Written by : mara
* Date       : 22/11/2024
* Description: extract the palette of a pi1 to bin file.
*              same idea of picpal.py
*-----------------------------------------------------------
    ORG    $1000
START:                  ; first instruction of program
    bra main
    
    
not_supported:
    movem.l d0/a0/a1,-(a7)

    move.l  #not_supported_message,a1
    move.b  #14,d0
    trap    #15
    
    movem.l (a7)+,d0/a0/a1
    bra end_program

init_value_pal_bss:
    movem.l d1-d2/a1,-(a7)
    
    move.l  #$0BADCAFE,d1
    lea     palette,a1
    move.l  #(16*2)/4-1,d2
 
fill:
    move.l  d1,(a1)+
    dbf     d2,fill

    movem.l (a7)+,d1-d2/a1
    rts
    
fill_value_pal_bss:
    movem.l d0-d7,-(a7)
    
    movem.l (a0),d0-d7
    movem.l  d0-d7,palette
    
    movem.l (a7)+,d0-d7
    rts

put_pal_in_file:
    movem.l d0-d7/a0-a6,-(a7)
    
    lea     pal_bin_file,a1
    move.b  #52,d0
    trap    #15
    
    lea     palette,a1
    move.l  #(16*2),d2
    move.b  #54,d0
    trap    #15
    
    
    move    #50,d0
    trap    #15
    
    movem.l (a7)+,d0-d7/a0-a6
    rts
    
main:
    lea     picture,a0
    
    move.w  (a0)+,d0
    
    tst.l   d0
    bne     not_supported
    
    bsr     init_value_pal_bss
    
    bsr     fill_value_pal_bss

    bsr     put_pal_in_file
    
* Put program code here
end_program:
    SIMHALT             ; halt simulator

picture                 INCBIN  adn-font3.pi1

palette                 ds.l    8

pal_bin_file            dc.b    'easy68k-adn-font3-pal.bin',0
not_supported_message   dc.b    'this picture is not a degas elite or it''s compressed !',13,10,0
* Put variables and constants here



    END    START        ; last line of source



*~Font name~Fixedsys~
*~Font size~9~
*~Tab type~1~
*~Tab size~4~
