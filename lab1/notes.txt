; Donaven Bruce
; I draw an 8x8 capital D with an 8 byte mask
; I map a top left screen address and write row by row
; I shift a mask and step a pointer

define screen_ptr_lo $00
define screen_ptr_hi $01
define row_mask      $02

define glyph_row0 $10
define glyph_row1 $11
define glyph_row2 $12
define glyph_row3 $13
define glyph_row4 $14
define glyph_row5 $15
define glyph_row6 $16
define glyph_row7 $17

; glyph bytes for D msb is left
LDA #$F0
STA glyph_row0
LDA #$88
STA glyph_row1
LDA #$84
STA glyph_row2
LDA #$84
STA glyph_row3
LDA #$84
STA glyph_row4
LDA #$84
STA glyph_row5
LDA #$88
STA glyph_row6
LDA #$F0
STA glyph_row7

; clear screen 0200 to 05FF
LDA #$00
STA screen_ptr_lo
LDA #$02
STA screen_ptr_hi
clear_page:
LDY #$00
clear_byte_loop:
LDA #$00
STA (screen_ptr_lo),Y
INY
BNE clear_byte_loop
INC screen_ptr_hi
LDA screen_ptr_hi
CMP #$06
BNE clear_page

; reset pointer to 0200
LDA #$00
STA screen_ptr_lo
LDA #$02
STA screen_ptr_hi

; position top left y is 8 rows x is 8 cols
LDX #$08
advance_rows:
CLC
LDA screen_ptr_lo
ADC #$20
STA screen_ptr_lo
BCC advance_rows_no_carry
INC screen_ptr_hi
advance_rows_no_carry:
DEX
BNE advance_rows

CLC
LDA screen_ptr_lo
ADC #$08
STA screen_ptr_lo
BCC advance_cols_no_carry
INC screen_ptr_hi
advance_cols_no_carry:

; draw 8 rows
LDX #$00
glyph_row_loop:
LDA glyph_row0,X
STA row_mask
LDY #$00
glyph_col_loop:
ASL row_mask
BCC skip_pixel
LDA #$01
STA (screen_ptr_lo),Y
skip_pixel:
INY
CPY #$08
BNE glyph_col_loop
CLC
LDA screen_ptr_lo
ADC #$20
STA screen_ptr_lo
BCC advance_row_no_carry
INC screen_ptr_hi
advance_row_no_carry:
INX
CPX #$08
BNE glyph_row_loop

BRK
