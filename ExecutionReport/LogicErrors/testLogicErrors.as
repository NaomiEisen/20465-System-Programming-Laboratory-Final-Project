LA_BEL: add    #1 ,   LIST
LOOP: prn #48
;-------------------
; duplicate label
LA_BEL: prn #48
;-------------------
; invalid label name
    .laebl:  prn #48
;-------------------
; invalid label name
    7something:  prn #48
;-------------------
; invalid param number
inc r6, r10
;-------------------
inc r4
mov *r6, K
sub r1, r4

;-------------------
; invalid command name
mor r1, r5
;-------------------
; invalid address mode
lea LA_BEL, #5
;-------------------
; invalid address mode
dec #59
;-------------------
; integer out of range
prn #50000000000
;-------------------
; integer out of range
prn #16384
;-------------------
; integer out of range
prn #-16385
;-------------------
; invalid address mode
jmp r4
;-------------------
; invalid register name
mov *r9
;-------------------
; extern on existin label
.extern LA_BEL

; WARNINGS
;-------------------
; label before extern
INVALID: .extern somethinggg
;-------------------
; label before entry
INVALID2: .entry somethinggg2
;-------------------
;extern twice
.extern one
.extern one
