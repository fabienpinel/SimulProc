//-----------------
// Instructions
//-----------------
        TEXT 30

        // Programme principal
main    EQU *
        LOAD R00, @op1
        LOAD R01, @op2
        SUB R01, #1
loop    BRANCH LE, @fin
        ADD R00, @op1
        SUB R01, #1
        BRANCH NC, @loop
fin     ADD R00, 0[R00]
		STORE R00, @result
        HALT 
        ILLOP 
        ILLOP 
        ILLOP 
       
        END
        
//-----------------
// Données et pile
//-----------------
        DATA 4
        
        WORD 0
result  WORD 0
op1     WORD 25
op2     WORD 2
        
        END
