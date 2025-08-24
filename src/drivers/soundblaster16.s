; code sourced from osdev.org
; please omit this from the makefile, it's no use for it.

  %macro outb 2 ; huh, some macros.
    mov dx, %1
    mov al, %2
    out dx, al
  %endmacro

  %macro inb 1
    mov dx, %1
    in al, dx
  %endmacro



  ; reset

  outb 0x226, 1
  mov ah, 86h
  mov cx, 0x0000
  mov dx, 0xFFFF
  int 15h
  outb 0x226, 0



  outb 0x22C, 0xD1

  ; DMA 1

  outb 0x0A, 5
  outb 0x0C, 1
  outb 0x0B, 0x49
  outb 0x83, 0x01
  outb 0x02, 0x04
  outb 0x02, 0x0F
  outb 0x03, 0xFF
  outb 0x04, 0x0F
  outb 0x0A, 1


  outb 0x22C, 0x40
  outb 0x22C, 165
  outb 0x22C, 0xC0
  outb 0x22C, 0x00
  outb 0x22C, 0xFE
  outb 0x22C, 0x0F

  ; also need to handle irq