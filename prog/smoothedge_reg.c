
/*
 * smoothedge_reg.c
 *
 *   Analyzes edges of a 1 bpp (connected component) image for smoothness.
 */

#include <stdio.h>
#include <stdlib.h>
#include "allheaders.h"

static l_int32 MIN_JUMP = 2;
static l_int32 MIN_REVERSAL = 3;

void PixAddEdgeData(PIXA *pixa, PIX *pixs, l_int32 side, l_int32 minjump,
                    l_int32 minreversal);


main(int    argc,
     char **argv)
{
l_int32      w;
PIX         *pixs, *pixt, *pixd;
PIXA        *pixa;
static char  mainName[] = "smoothedge_reg";

    pixs = pixRead("raggededge.png");
    w = pixGetWidth(pixs);
    pixa = pixaCreate(0);
    PixAddEdgeData(pixa, pixs, L_FROM_RIGHT, MIN_JUMP, MIN_REVERSAL);
    PixAddEdgeData(pixa, pixs, L_FROM_LEFT, MIN_JUMP, MIN_REVERSAL);
    pixt = pixRotateOrth(pixs, 1);
    PixAddEdgeData(pixa, pixt, L_FROM_BOTTOM, MIN_JUMP, MIN_REVERSAL);
    PixAddEdgeData(pixa, pixt, L_FROM_TOP, MIN_JUMP, MIN_REVERSAL);
    pixDestroy(&pixt);
    pixt = pixRotateOrth(pixs, 2);
    PixAddEdgeData(pixa, pixt, L_FROM_LEFT, MIN_JUMP, MIN_REVERSAL);
    PixAddEdgeData(pixa, pixt, L_FROM_RIGHT, MIN_JUMP, MIN_REVERSAL);
    pixDestroy(&pixt);
    pixt = pixRotateOrth(pixs, 3);
    PixAddEdgeData(pixa, pixt, L_FROM_TOP, MIN_JUMP, MIN_REVERSAL);
    PixAddEdgeData(pixa, pixt, L_FROM_BOTTOM, MIN_JUMP, MIN_REVERSAL);
    pixDestroy(&pixt);
    pixDestroy(&pixs);

        /* Display at 2x scaling */
    pixd = pixaDisplayTiledAndScaled(pixa, 32, 2 * (w + 10), 2, 0, 25, 2);
    pixWrite("/tmp/junkpixd.png", pixd, IFF_PNG);
    pixDestroy(&pixd);
    pixaDestroy(&pixa);
    return 0;
}


void PixAddEdgeData(PIXA    *pixa,
                    PIX     *pixs,
                    l_int32  side,
                    l_int32  minjump,
                    l_int32  minreversal)
{
l_float32  jpl, jspl, rpl;
PIX       *pixt1, *pixt2;

    pixMeasureEdgeSmoothness(pixs, side, minjump, minreversal, &jpl,
                             &jspl, &rpl, "/tmp/junkedge.png");
    fprintf(stderr, "side = %d: jpl = %6.3f, jspl = %6.3f, rpl = %6.3f\n",
            side, jpl, jspl, rpl);
    pixt1 = pixRead("/tmp/junkedge.png");
    pixt2 = pixAddBorder(pixt1, 10, 0);  /* 10 pixel white border */
    pixaAddPix(pixa, pixt2, L_INSERT);
    pixDestroy(&pixt1);
    return;
}
    
