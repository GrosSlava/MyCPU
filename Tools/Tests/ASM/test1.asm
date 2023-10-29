
.Code

ENTRY:
    sub x0, x0         ; Clear register
    LOOP:
        add x0, #1     ; x0 = x0 + 1
        led x0   
        j @LOOP        ; loop
    sys hlt

.Data
