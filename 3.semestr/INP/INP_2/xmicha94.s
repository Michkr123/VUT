; Autor reseni: Kryštof Michálek xmicha94
; Pocet cyklu k serazeni puvodniho retezce: 2178
; Pocet cyklu razeni sestupne serazeneho retezce: 2993
; Pocet cyklu razeni vzestupne serazeneho retezce: 453
; Pocet cyklu razeni retezce s vasim loginem: 647
; Implementovany radici algoritmus: insertion sort
; ------------------------------------------------

; DATA SEGMENT
                .data
; login:          .asciiz "vitejte-v-inp-2023"    ; puvodni uvitaci retezec
; login:          .asciiz "vvttpnjiiee3220---"  ; sestupne serazeny retezec
; login:          .asciiz "---0223eeiijnpttvv"  ; vzestupne serazeny retezec
 login:          .asciiz "xmicha94"            ; SEM DOPLNTE VLASTNI LOGIN
; A POUZE S TIMTO ODEVZDEJTE

params_sys5:    .space  8   ; misto pro ulozeni adresy pocatku
                            ; retezce pro vypis pomoci syscall 5
                            ; (viz nize - "funkce" print_string)

; CODE SEGMENT
                .text
main:
        ; SEM DOPLNTE VASE RESENI
        xor $t1, $t1, $t1
        xor $t6, $t6, $t6
        addi $t6, $t6, -1
for:    
        addi $t1, $t1, 1
        lb $t3, login($t1)
        beqz $t3, end
        xor $t0, $t0, $t0 
        add $t0, $t0, $t1
        b while
while:
        addi $t0, $t0, -1
        beq $t0, $t6, for
        lb $t2, login($t0)
        slt $t4, $t3, $t2
        beqz $t4, for
        sb $t3, login($t0)
        addi $t5, $t0, 1  
        sb $t2, login($t5)
        b while
end:
        daddi   r4, r0, login   ; vozrovy vypis: adresa login: do r4
        jal     print_string    ; vypis pomoci print_string - viz nize

        syscall 0   ; halt


print_string:   ; adresa retezce se ocekava v r4
                sw      r4, params_sys5(r0)
                daddi   r14, r0, params_sys5    ; adr pro syscall 5 musi do r14
                syscall 5   ; systemova procedura - vypis retezce na terminal
                jr      r31 ; return - r31 je urcen na return address
