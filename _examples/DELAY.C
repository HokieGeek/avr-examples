void delay(int pause)  //time in mS
{
  for (;pause;pause--)
  {
#asm
  push    2
  mov     R2,#0D7h    // fine tune to crystal speed
  djnz    R2,$
  mov     R2,#0D7h
  djnz    R2,$
  pop     2
#endasm
  }
}

