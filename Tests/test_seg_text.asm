        TEXT 30

main    EQU *
        ADD R05, #2048
        CALL NC, 10[R05]
        NOP
        NOP

        END

        DATA 30

        WORD 0

        END
