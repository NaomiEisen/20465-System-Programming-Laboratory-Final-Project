LABLE: inc r6 ; I'm allowing comments within the line :)
LLL: mov r6 ,LIST
sub r1, r4
mov r1   , muna
A: .string "ab "  cd"
LIST: .data 7898,0,-1
    .data -16384

macr        m_macr
cmp r3, #-6
; comment inside macro
bne END ; and here
endmacr

m_macr

clr r2
clr *r2
dec r3
clr LLL
rts
stop

lea LIST, *r5


LOOP: prn #48
    .entry LABLE
    .entry LIST
    .extern END
    .extern muna
