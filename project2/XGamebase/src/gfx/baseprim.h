void FBPP(put_pixel)(_XBitmap* bmp, int x, int y, int color)
{
 if(((x >= bmp->cl) && (x < bmp->cr) && (y >= bmp->ct) && (y < bmp->cb)))
 {
  PIXEL_PTR d = PIXEL_PTR_ADDR(bmp->line[y], x);
  WRITE(d, color);
 }
}

int FBPP(get_pixel)(_XBitmap* bmp, int x, int y)
{
 PIXEL_PTR d;

 if((x < 0) || (x >= bmp->w) || (y < 0) || (y >= bmp->h))
  return -1;

 d = PIXEL_PTR_ADDR(bmp->line[y], x);

 return READ(d);
}


void FBPP(hline)(_XBitmap* bmp, int x1, int y, int x2, int color)
{
 PIXEL_PTR d;

 if(x1 > x2)
 {
  _XSwap(x1, x2);
 }

 if(x1 < bmp->cl)
  x1 = bmp->cl;
 if(x2 >= bmp->cr)
  x2 = bmp->cr - 1;
 if((x1 > x2) || (y < bmp->ct) || (y >= bmp->cb))
  return;

 d = PIXEL_PTR_ADDR(bmp->line[y], x1);

 while(x1 <= x2)
 { 	
  WRITE(d, color);   
  INC_PIXEL_PTR(d);
  x1++;
 }
}

void FBPP(vline)(_XBitmap* bmp, int x, int y1, int y2, int color)
{
 if(y1 > y2)
 {
  _XSwap(y1, y2);
 }

 if(y1 < bmp->ct)
  y1 = bmp->ct;
 if(y2 >= bmp->cb)
  y2 = bmp->cb - 1;
 if((x < bmp->cl) || (x >= bmp->cr) || (y1 > y2))
  return;

 while(y1 <= y2)
 { 	
  PIXEL_PTR d = PIXEL_PTR_ADDR(bmp->line[y1], x);
  WRITE(d, color);
  INC_PIXEL_PTR(d);
  y1++;
 }  
}
