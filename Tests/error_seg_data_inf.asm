//-----------------
// Instructions
//-----------------
        TEXT 30

        // Programme principal
main    EQU *
        LOAD R00, @result
		ADD R00, -1[R00]
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
