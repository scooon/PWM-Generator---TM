;Oznaczenie pamieci
;BIT, CODE, DATA, IDATA, XDATA, NUMBER

;Wejscie
;Arg Number               char, 1-byte ptr       int, 2-byte ptr                long, float                         generic ptr
;    1                          R7                   R6 & R7                       R4由7                              R1由3
;                                             (MSB in R6,LSB in R7)           (MSB in R4,LSB in R7)      (Mem type in R3, MSB in R2, LSB in R1)
;    2                          R5                   R4 & R5                       R4由7                              R1由3
;                                             (MSB in R4,LSB in R5)           (MSB in R4,LSB in R7)      (Mem type in R3, MSB in R2, LSB in R1)
;    3                          R3                   R2 & R3                     ----------                           R1由3
;                                             (MSB in R2,LSB in R3)                                      (Mem type in R3, MSB in R2, LSB in R1)

;Wyjscie
;bit                                 - Carry bit
;char, unsigned char, 1-byte pointer - R7 
;int, unsigned int, 2-byte ptr       - R6 & R7 MSB in R6, LSB in R7. 
;long,  unsigned long                - R4-R7 MSB in R4, LSB in R7. 
;float R4-R7                         - 32-Bit IEEE format. 
;generic pointer                     - R1-R3 Memory type in R3, MSB R2, LSB R1. 

PTWY	EQU 8006h

PRGSEG      	SEGMENT CODE

		PUBLIC  POTASM
		
		RSEG    PRGSEG
			
; Niby zapala odpowiednie diody ale i bez tego chyba pewnie bedzie dzialac XD
POTASM:		    mov DPTR, #PTWY			;Przenosimy to co jest w 8008h to dptr
				movx A, @DPTR			;przenosimy to co w dptr do akumulatora.
				RET						;Wracamy do jezyka C.

		END
