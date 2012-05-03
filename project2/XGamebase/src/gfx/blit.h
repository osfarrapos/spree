#define USE_MEMCPY

/*

  Remplit un bitmap linaire avec la couleur specifiee

*/
void FBPP(clear_to_color)(_XBitmap* bmp, int color)
{
 int x, y;
 int w = bmp->cr - bmp->cl;

 for(y = bmp->ct; y < bmp->cb; y++)
 {
  PIXEL_PTR d = PIXEL_PTR_ADDR(bmp->line[y], bmp->cl);

  for(x = w; x > 0; INC_PIXEL_PTR(d), x--)
  {
   WRITE(d, color);
  }
 }
}

/*

  Blitting normal

*/
void FBPP(blit)(_XBitmap* src, _XBitmap* dst, int sx, int sy, 
				int dx, int dy, int w, int h)
{
#ifdef USE_MEMCPY
 int y;

 w *= BYTES_PER_PIX;
 sx *= BYTES_PER_PIX;
 dx *= BYTES_PER_PIX;

 for(y = 0; y < h; y++)
  memcpy((U8*)dst->line[y+dy]+dx, (U8*)src->line[y+sy]+sx, w);
#else
 int x, y;

 for(y = 0; y < h; y++)
 {
  PIXEL_PTR s = PIXEL_PTR_ADDR(src->line[sy+y], sx);
  PIXEL_PTR d = PIXEL_PTR_ADDR(dst->line[dy+y], dx);

  for(x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--)
  {
   WRITE(d, READ(s));
  }
 }
#endif
}

/*

  Blitting masquer

*/
void FBPP(masked_blit)(_XBitmap *src, _XBitmap *dst, int s_x, int s_y,
					   int d_x, int d_y, int w, int h)
{
 int x, y;

 for(y = 0; y < h; y++)
 {
  PIXEL_PTR s = PIXEL_PTR_ADDR(src->line[s_y+y], s_x);
  PIXEL_PTR d = PIXEL_PTR_ADDR(dst->line[d_y+y], d_x);

  for(x = w - 1; x >= 0; INC_PIXEL_PTR(s), INC_PIXEL_PTR(d), x--)
  {
   unsigned long c;
   c = READ(s);

   if(!ISMASK(c))
   {
	WRITE(d, c);
   }
  }
 }
}
