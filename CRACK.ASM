.model tiny
.code
org 100h

Start:
    jmp real_start

prompt      db 'Enter password: $'
correct     db 0Dh, 0Ah, 'Access granted!!!$'
incorrect   db 0Dh, 0Ah, 'Access denied$'
buffer      db 0FFh, 0        
            db 7Fh dup(0)    
password    db 26h, 30h, 36h, 27h, 30h, 21h
xor_key     equ 55h          

real_start:
    mov ah, 09h
    mov dx, offset prompt
    int 21h

    mov ah, 0Ah
    mov dx, offset buffer
    int 21h
	
	mov al, [buffer + 1]
	cmp al, 0
	je access_granted

    mov si, offset buffer + 2 
    mov di, offset password
    mov cx, 6
check_loop:
    lodsb                   
    xor al, xor_key         
    cmp al, [di]           
    jne access_denied
    inc di
    loop check_loop

access_granted:
    mov dx, offset correct
    jmp print_result

access_denied:
    mov dx, offset incorrect

print_result:
    mov ah, 09h
    int 21h

    mov ax, 4C00h
    int 21h

end Start