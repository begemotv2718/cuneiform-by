/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



/****************************************************************************
 *                                                                          *
 *                   P A G E   L A Y O U T                                  *
 *                                                                          *
 *              Written in 1991 by Yuri Panchul                             *
 *                                                                          *
 *              LTREMOVE.C - Blocks removing                                *
 *                                                                          *
 ***************************************************************************/

# include <stdlib.h>

# include "layout.h"
# include "my_mem.h"
# include "lang.h"

# include "dpuma.h"
#include "minmax.h"


extern Handle hRemoveEmptyBlocks;

// new page
/****************************************************************************
 *                                                                          *
 *                   P A R T   O N E                                        *
 *                                                                          *
 *              Blocks removing with Pictures data                          *
 *                                                                          *
 *       Heuristic: If block is small and is surrounded by picture          *
 *       at least from three directions, this block should be removed.      *
 *                                                                          *
 ***************************************************************************/

# define MIN_DISTANCE    0
# define MAX_DISTANCE    3
# define N_LAYERS        (MAX_DISTANCE - MIN_DISTANCE + 1)

# define N_DIRECTIONS    4

# define LEFT   0
# define TOP    1
# define RIGHT  2
# define BOTTOM 3

# ifdef LT_DEBUG
char *apDirName [N_DIRECTIONS] = { "Left  ", "Top   ", "Right ", "Bottom" };
# endif

typedef struct _Dir
        {
            int nDir;
            int nWidth;
            int nPictureSquare;
        } DIRECTION;

static DIRECTION Dirs [N_DIRECTIONS];
// new page
static int PassHorzInterval (int y, int x1, int x2)
{
    int nCounter;
    uchar *p;

    if (y < 0 || y >= PAGE_MATRIX_HEIGHT)
        return (0);

    if (x1 > x2)
        EXCHANGE_INTS (x1, x2);

    if (x1 >= PAGE_MATRIX_WIDTH)
        return (0);

    if (x1 < 0)
        x1 = 0;

    if (x2 >= PAGE_MATRIX_WIDTH)
        x2 = PAGE_MATRIX_WIDTH - 1;

    nCounter = 0;

    for (p = PageMatrix + (y << PAGE_MATRIX_WIDTH_SHIFT) + x1;
             p <= PageMatrix + (y << PAGE_MATRIX_WIDTH_SHIFT) + x2;
                 p++)
    {
        if (*p & PMC_PICTURE)
            nCounter++;
    }

    return (nCounter);
}
// new page
static int PassVertInterval (int x, int y1, int y2)
{
    int nCounter;
    uchar *p;

    if (x < 0 || x >= PAGE_MATRIX_WIDTH)
        return (0);

    if (y1 > y2)
        EXCHANGE_INTS (y1, y2);

    if (y1 >= PAGE_MATRIX_HEIGHT)
        return (0);

    if (y1 < 0)
        y1 = 0;

    if (y2 >= PAGE_MATRIX_HEIGHT)
        y2 = PAGE_MATRIX_HEIGHT - 1;

    nCounter = 0;

    for (p = PageMatrix + (y1 << PAGE_MATRIX_WIDTH_SHIFT) + x;
             p <= PageMatrix + (y2 << PAGE_MATRIX_WIDTH_SHIFT) + x;
                 p += PAGE_MATRIX_WIDTH)
    {
        if (*p & PMC_PICTURE)
            nCounter++;
    }

    return (nCounter);
}
// new page
static int CompDirsPictureDensity (const void *pi,
								   const void *qi)
{
    const DIRECTION *p = (const DIRECTION*) pi;
    const DIRECTION *q = (const DIRECTION*) qi;
    if (p -> nWidth == 0 || q -> nWidth == 0)
        ErrorInternal ("p -> nWidth == 0 || q -> nWidth == 0 !");

    return (q -> nPictureSquare * p -> nWidth
            - p -> nPictureSquare * q -> nWidth);
}
// new page
static RECTANGLE rMatrix;

# ifdef LT_DEBUG
static RECTANGLE rBoundary;
static uchar      *pFirstLine, *pLastLine, *pLine, *p;

void PrepareDebugPictureOutput (void)
{
    pFirstLine = PageMatrix + (rMatrix.yTop    << PAGE_MATRIX_WIDTH_SHIFT);
    pLastLine  = PageMatrix + (rMatrix.yBottom << PAGE_MATRIX_WIDTH_SHIFT);

    for (pLine = pFirstLine; pLine <= pLastLine; pLine += PAGE_MATRIX_WIDTH)
    {
        for (p = pLine + rMatrix.xLeft; p <= pLine + rMatrix.xRight; p++)
        {
            *p |= PMC_DEBUG1;
        }
    }

    rBoundary.xLeft   =
        MAX (0, rMatrix.xLeft - MAX_DISTANCE);
    rBoundary.xRight  =
        MIN (PAGE_MATRIX_WIDTH  - 1, rMatrix.xRight  + MAX_DISTANCE);
    rBoundary.yTop    =
        MAX (0, rMatrix.yTop  - MAX_DISTANCE);
    rBoundary.yBottom =
        MIN (PAGE_MATRIX_HEIGHT - 1, rMatrix.yBottom + MAX_DISTANCE);

    pFirstLine = PageMatrix + (rBoundary.yTop    << PAGE_MATRIX_WIDTH_SHIFT);
    pLastLine  = PageMatrix + (rBoundary.yBottom << PAGE_MATRIX_WIDTH_SHIFT);

    for (pLine = pFirstLine; pLine <= pLastLine; pLine += PAGE_MATRIX_WIDTH)
    {
        for (p = pLine + rBoundary.xLeft;
                    p <= pLine + rBoundary.xRight;
                        p++)
        {
            *p |= PMC_DEBUG2;
        }
    }
}
// new page
void UnPrepareDebugPictureOutput (void)
{
    for (pLine = pFirstLine; pLine <= pLastLine; pLine += PAGE_MATRIX_WIDTH)
    {
        for (p = pLine + rBoundary.xLeft;
                    p <= pLine + rBoundary.xRight;
                        p++)
        {
            *p &= ~(PMC_DEBUG1 | PMC_DEBUG2);
        }
    }
}
# endif
// new page
static void CalculateDirectionsValues (BLOCK *pBlock)
{
    LPOINT    TopLeft, TopRight, BottomLeft, BottomRight;
    RECTANGLE rReal;
    int       i;
    int       nDistance;

    // Get rectangle of block in real coordinates

    TopLeft.x     = pBlock -> Rect.xLeft;
    TopLeft.y     = pBlock -> Rect.yTop;
    TopRight.x    = pBlock -> Rect.xRight;
    TopRight.y    = pBlock -> Rect.yTop;
    BottomLeft.x  = pBlock -> Rect.xLeft;
    BottomLeft.y  = pBlock -> Rect.yBottom;
    BottomRight.x = pBlock -> Rect.xRight;
    BottomRight.y = pBlock -> Rect.yBottom;

    REAL_XY (TopLeft.x    , TopLeft.y);
    REAL_XY (TopRight.x   , TopRight.y);
    REAL_XY (BottomLeft.x , BottomLeft.y);
    REAL_XY (BottomRight.x, BottomRight.y);

    rReal.xLeft   = MIN (TopLeft.x, BottomLeft.x);
    rReal.yTop    = MIN (TopLeft.y, TopRight.y);
    rReal.xRight  = MAX (TopRight.x, BottomRight.x);
    rReal.yBottom = MAX (BottomLeft.y, BottomRight.y);

    // Normalize rectangle coordinates

    rReal.xLeft   = MIN (MAX (rReal.xLeft,   0), PAGE_MATRIX_REAL_WIDTH  - 1);
    rReal.yTop    = MIN (MAX (rReal.yTop,    0), PAGE_MATRIX_REAL_HEIGHT - 1);
    rReal.xRight  = MIN (MAX (rReal.xRight,  0), PAGE_MATRIX_REAL_WIDTH  - 1);
    rReal.yBottom = MIN (MAX (rReal.yBottom, 0), PAGE_MATRIX_REAL_HEIGHT - 1);

    // Calculate matrix coordinates

    rMatrix.xLeft   = XY_COMPRESS (rReal.xLeft);
    rMatrix.xRight  = XY_COMPRESS (rReal.xRight);
    rMatrix.yTop    = XY_COMPRESS (rReal.yTop);
    rMatrix.yBottom = XY_COMPRESS (rReal.yBottom);

    // Calculate directions values

    for (i = 0; i < N_DIRECTIONS; i++)
        Dirs [i].nDir = i;

    Dirs [LEFT]  .nWidth = rMatrix.yBottom - rMatrix.yTop  + 1;
    Dirs [TOP]   .nWidth = rMatrix.xRight  - rMatrix.xLeft + 1;
    Dirs [RIGHT] .nWidth = rMatrix.yBottom - rMatrix.yTop  + 1;
    Dirs [BOTTOM].nWidth = rMatrix.xRight  - rMatrix.xLeft + 1;

    for (i = 0; i < N_DIRECTIONS; i++)
        Dirs [i].nPictureSquare = 0;

    for (nDistance = MIN_DISTANCE; nDistance <= MAX_DISTANCE; nDistance++)
    {
        Dirs [LEFT].nPictureSquare   +=
            PassVertInterval (rMatrix.xLeft   - nDistance,
                              rMatrix.yTop,  rMatrix.yBottom);

        Dirs [TOP].nPictureSquare    +=
            PassHorzInterval (rMatrix.yTop    - nDistance,
                              rMatrix.xLeft, rMatrix.xRight);

        Dirs [RIGHT].nPictureSquare  +=
            PassVertInterval (rMatrix.xRight  + nDistance,
                              rMatrix.yTop,  rMatrix.yBottom);

        Dirs [BOTTOM].nPictureSquare +=
            PassHorzInterval (rMatrix.yBottom + nDistance,
                              rMatrix.xLeft, rMatrix.xRight);
    }

    qsort ((char *) Dirs, N_DIRECTIONS, sizeof (DIRECTION), CompDirsPictureDensity); //AK 04.03.97
}
// new page
/****************************************************************************
 *                                                                          *
 *                   P A R T   T W O                                        *
 *                                                                          *
 *              Blocks removing with dust distribution                      *
 *                                                                          *
 *       Heuristic: If block contains many dust and this dust is            *
 *       proportional distributed on block rectangle, this block            *
 *       should be removed.                                                 *
 *                                                                          *
 ***************************************************************************/

# define DD_MATRIX_WIDTH   2
# define DD_MATRIX_HEIGHT  2
# define DD_MATRIX_SIZE    (DD_MATRIX_WIDTH * DD_MATRIX_HEIGHT)

int DQD_Matrix [DD_MATRIX_SIZE]; /* Dust Quantity Distribution */
int DSD_Matrix [DD_MATRIX_SIZE]; /* Dust Square   Distribution */

static int DD_CompProc (const void *pi, const void *qi)
{
    const int *p = (const int*) pi;
    const int *q = (const int*) qi;
    return (*q - *p);
}
// new page
int CalculateSquareOfLetters (BLOCK *p)
{
    ROOT * pRoot;
    int    nSquare = 0;

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        if (! IS_LAYOUT_DUST (*pRoot) &&
            (pRoot -> bType & ROOT_LETTER) != 0)
        {
            nSquare += pRoot -> nWidth * pRoot -> nHeight;
        }
    }

    return (nSquare);
}
// new page
void CalculateDustDistribution (BLOCK *p)
{
    ROOT *pRoot;
    int  nBlockWidth  = p -> Rect.xRight  - p -> Rect.xLeft + 1;
    int  nBlockHeight = p -> Rect.yBottom - p -> Rect.yTop  + 1;
    int  x, y;
    int  i;
    int  nDQD_Sum;
    int  nDSD_Sum;

    memset (DQD_Matrix, 0, DD_MATRIX_SIZE * sizeof (int));
    memset (DSD_Matrix, 0, DD_MATRIX_SIZE * sizeof (int));

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {

        if (! IS_LAYOUT_DUST (*pRoot))
        {
            continue;
        }
        x = (pRoot -> xColumn - p -> Rect.xLeft)
            * DD_MATRIX_WIDTH  / nBlockWidth;

        y = (pRoot -> yRow    - p -> Rect.yTop)
            * DD_MATRIX_HEIGHT / nBlockHeight;

        DQD_Matrix [x + y * DD_MATRIX_WIDTH]++;

        DSD_Matrix [x + y * DD_MATRIX_WIDTH] +=
            pRoot -> nWidth * pRoot -> nHeight;
    }

    nDQD_Sum = 0;
    nDSD_Sum = 0;

    for (i = 0; i < DD_MATRIX_SIZE; i++)
    {
        nDQD_Sum += DQD_Matrix [i];
        nDSD_Sum += DSD_Matrix [i];
    }

    if (nDQD_Sum == 0 || nDSD_Sum == 0)
        return;

    for (i = 0; i < DD_MATRIX_SIZE; i++)
    {
        DQD_Matrix [i] = DQD_Matrix [i] * 100 / nDQD_Sum;
        DSD_Matrix [i] = DSD_Matrix [i] * 100 / nDSD_Sum;
    }

    qsort ((char*)DQD_Matrix, DD_MATRIX_SIZE, sizeof (int), DD_CompProc); //AK 04.03.97
    qsort ((char*)DSD_Matrix, DD_MATRIX_SIZE, sizeof (int), DD_CompProc); //AK 04.03.97
}
// new page
# ifdef LT_DEBUG
static Bool bAtLeastOneOutput;
# endif

void BlockRemove (BLOCK *p)
{
    ROOT *pRoot;

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel == 2)
	if(!LDPUMA_Skip(hRemoveEmptyBlocks))
    {
        pDebugBlock = p;
        LT_GraphicsBlockOutput2 ("Removed or converted blocks");
        bAtLeastOneOutput = TRUE;
    }

# endif
// for pictures
	//DDD
    //DDD if(p -> Type == BLOCK_PICTURE){
    //DDD int ret = del_picture( p -> hPicture);
	//DDD ret=ret;
    //DDD goto OUT;
    //DDD }
// for any
    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        pRoot -> nBlock =
            IS_LAYOUT_DUST (*pRoot) ? DUST_BLOCK_NUMBER : REMOVED_BLOCK_NUMBER;
    }
//OUT:
    BlocksRemoveDescriptor (p);
}
// new page
// Pit 02-11-94
void BlockEnglish(BLOCK *p)
{
  p -> language = LANG_ENGLISH;
}
// new page
void BlockConvertToDust (BLOCK *p)
{
    ROOT *pRoot;

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel == 2)
	if(!LDPUMA_Skip(hRemoveEmptyBlocks))
    {
        pDebugBlock = p;
        LT_GraphicsBlockOutput2 ("Removed or converted blocks");
        bAtLeastOneOutput = TRUE;
    }
# endif

    for (pRoot = p -> pRoots; pRoot != NULL; pRoot = pRoot -> u1.pNext)
    {
        pRoot -> nBlock = DUST_BLOCK_NUMBER;
    }

    BlocksRemoveDescriptor (p);
}
// new page
void BlocksRemoveEmptyBlocks (void)
{
    BLOCK *p, *pNext;
    int nBlockWidth, nBlockHeight;

# ifdef LT_DEBUG_CALIBRATE
    if (bDebugOptionCalibratePictureRemovingCriteria ||
        bDebugOptionCalibrateDD_RemovingCriteria     ||
        bDebugOptionCalibrateLinearRemovingCriteria)
    {
        LT_GraphicsOpen ();
    }
# endif

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel == 2)
	if(!LDPUMA_Skip(hRemoveEmptyBlocks))
    {
        LT_GraphicsClearScreen ();
        bAtLeastOneOutput = FALSE;
    }
# endif

    pNext = pBlocksList;

    while (pNext != NULL)
    {
        p = pNext;
        pNext = pNext -> pNext;

        if (p -> Type != BLOCK_TEXT)
            continue;

# ifdef LT_DEBUG
            //if (LT_DebugGraphicsLevel >= 3)
		if(!LDPUMA_Skip(hRemoveEmptyBlocks))
            {
                pDebugBlock = p;
                LT_ShowBlock("This block...");
                LT_Getch ();
            }
# endif

        nBlockWidth  = p -> Rect.xRight  - p -> Rect.xLeft + 1;
        nBlockHeight = p -> Rect.yBottom - p -> Rect.yTop  + 1;

        if (p -> nRoots == p -> nDust)
        {
            BlockRemove (p);
            continue;
        }

        if (! bOptionBlocksRemovingByPageMatrix)
            goto AFTER_REMOVING_BY_PAGE_MATRIX;

        if (p -> nLetters > 5 ||
            p -> nRoots - p -> nDust > 50)
        {
            goto AFTER_REMOVING_BY_PAGE_MATRIX;
        }

        CalculateDirectionsValues (p);

        if (Dirs [0].nPictureSquare >= Dirs [0].nWidth     &&
            Dirs [1].nPictureSquare >= Dirs [1].nWidth     &&
            Dirs [2].nPictureSquare >= Dirs [2].nWidth / 2
                ||
            Dirs [0].nPictureSquare >= Dirs [0].nWidth * 2 &&
            Dirs [1].nPictureSquare >= Dirs [1].nWidth * 2 &&
            Dirs [2].nPictureSquare >= Dirs [2].nWidth / 3)
        {
# ifdef LT_DEBUG
            //if (LT_DebugGraphicsLevel >= 3)
			if(!LDPUMA_Skip(hRemoveEmptyBlocks))
            {
                CalculateDirectionsValues (p);

                PrepareDebugPictureOutput ();
                pDebugBlock = p;
                rDebugRect  = rBoundary;

                LT_GraphicsPictureRemovingConditionsOutput2
                    ("Remove by PageMatrix");

                UnPrepareDebugPictureOutput ();
                LT_Getch ();
            }
# endif
            BlockRemove (p);
            continue;
        }

      AFTER_REMOVING_BY_PAGE_MATRIX:
        ;

# ifdef LT_DEBUG_CALIBRATE
        if (bDebugOptionCalibratePictureRemovingCriteria &&
            p -> nLetters < 10)
        {
            int i;
            char cAnswer;

            CalculateDirectionsValues (p);

            PrepareDebugPictureOutput ();
            pDebugBlock = p;
            rDebugRect  = rBoundary;

            LT_GraphicsPictureRemovingConditionsOutput2
                ("Picture removing conditions");

            UnPrepareDebugPictureOutput ();

            cAnswer = LT_Getch ();

            if (cAnswer == '\r')
            {
                printf ("Roots: %4d Letters: %4d NoDust: %4d Dust: %4d ",
                        p -> nRoots,
                        p -> nLetters,
                        p -> nRoots - p -> nDust,
                        p -> nDust);

                printf ("Width %4d Height: %4d [ ", nBlockWidth, nBlockHeight);

                for (i = 0; i < N_DIRECTIONS; i++)
                {
                    printf ("%5.2f ",
                            (double) Dirs [i].nPictureSquare / Dirs [i].nWidth);
                }

                printf ("]\n");
            }

            continue;
        }
# endif

        if (! bOptionBlocksRemovingByDustDistribution)
            goto AFTER_REMOVING_BY_DUST_DISTRIBUTION;

        if (p -> nDust < 3 * (p -> nRoots - p -> nDust) ||
            CalculateSquareOfLetters (p)
                > (p -> Rect.xRight  - p -> Rect.xLeft + 1) *
                  (p -> Rect.yBottom - p -> Rect.yTop  + 1) / 3)
        {
            goto AFTER_REMOVING_BY_DUST_DISTRIBUTION;
        }

        CalculateDustDistribution (p);

        if (DSD_Matrix [0] <= 75 && DQD_Matrix [0] <= 60 &&
           // DSD_Matrix [2] >=  7 && DQD_Matrix [2] >= 11
            DSD_Matrix [3] >=  7 && DQD_Matrix [3] >= 11 // Pit 7-7-94
            &&  CalculateSquareOfLetters (p) == 0       // Pit
            )
        {
# ifdef LT_DEBUG
            //if (LT_DebugGraphicsLevel >= 3)
			if(!LDPUMA_Skip(hRemoveEmptyBlocks))
            {
                CalculateDustDistribution (p);

                pDebugBlock = p;
                rDebugRect.xLeft   = 0;
                rDebugRect.xRight  = DD_MATRIX_WIDTH - 1;
                rDebugRect.yTop    = 0;
                rDebugRect.yBottom = DD_MATRIX_HEIGHT - 1;

                LT_GraphicsDD_RemovingConditionsOutput2
                    ("Remove by dust distribution");

                LT_Getch ();
            }
# endif
            BlockRemove (p);
            continue;
        }

      AFTER_REMOVING_BY_DUST_DISTRIBUTION:
        ;

# ifdef LT_DEBUG_CALIBRATE
        if (bDebugOptionCalibrateDD_RemovingCriteria)
        {
            int i;
            char cAnswer;

            CalculateDustDistribution (p);

            pDebugBlock = p;
            rDebugRect.xLeft   = 0;
            rDebugRect.xRight  = DD_MATRIX_WIDTH - 1;
            rDebugRect.yTop    = 0;
            rDebugRect.yBottom = DD_MATRIX_HEIGHT - 1;

            LT_GraphicsDD_RemovingConditionsOutput2 ("Dust distribution");

            cAnswer = LT_Getch ();

            if (cAnswer == '\r')
            {
                printf ("Roots: %4d Letters: %4d NoDust: %4d Dust: %4d ",
                        p -> nRoots,
                        p -> nLetters,
                        p -> nRoots - p -> nDust,
                        p -> nDust);

                printf ("Width %4d Height: %4d [ ", nBlockWidth, nBlockHeight);

                for (i = 0; i < DD_MATRIX_SIZE; i++)
                {
                    printf ("%5.2f ", (double) DSD_Matrix [i] / DQD_Matrix [i]);
                }

                printf ("]\n");
            }

            continue;
        }
# endif

        if ((
             p -> nLetters == 0                     &&
             p -> nRoots - p -> nDust <= 10      &&  /* CRSH5 */
             nBlockWidth >  0                       &&
             nBlockWidth <= MAX_DUST_WIDTH / 2      &&
             nBlockHeight / nBlockWidth >= 7
                 ||
             p -> nLetters == 0                     &&
             p -> nRoots - p -> nDust <= 2          &&
             nBlockHeight > 0                       &&
             nBlockHeight <= MAX_DUST_HEIGHT        &&
             nBlockWidth / nBlockHeight >= 10
                 ||
             p -> nLetters == 0                     &&
             p -> nRoots - p -> nDust == 1          &&
             nBlockHeight < MAX_DUST_HEIGHT * 3 / 2
                 ||                                       /* OK */
             p -> nLetters == 0                     &&
             p -> nRoots - p -> nDust < 5           &&
             nBlockHeight < MAX_DUST_HEIGHT * 4     &&
             nBlockWidth  < MAX_DUST_WIDTH  * 4)

             &&

            ! (                                 /* Page number */
             p -> nRoots < 5 &&
             p -> Rect.xLeft  > rRootSpace.xLeft + nRootSpaceWidth  * 3 / 10 &&
             p -> Rect.xRight < rRootSpace.xLeft + nRootSpaceWidth  * 7 / 10 &&
             p -> Rect.yTop   > rRootSpace.yTop  + nRootSpaceHeight * 7 / 10 &&
             nBlockHeight > MAX_DUST_HEIGHT                                  &&
             nBlockWidth  > MAX_DUST_WIDTH
            ))
        {
# ifdef LT_DEBUG
            //if (LT_DebugGraphicsLevel >= 3)
			if(!LDPUMA_Skip(hRemoveEmptyBlocks))
            {
                pDebugBlock = p;

                LT_GraphicsLinearRemovingConditionsOutput2
                    ("Converted to dust by block parameter heuristic");

                LT_Getch ();
            }
# endif
            BlockConvertToDust (p);
            continue;
        }

# ifdef LT_DEBUG_CALIBRATE
        if (bDebugOptionCalibrateLinearRemovingCriteria &&
            p -> nRoots < 5)
        {
            char cAnswer;

            pDebugBlock = p;
            LT_GraphicsLinearRemovingConditionsOutput2 ("Remove");
            cAnswer = LT_Getch ();

            if (cAnswer == '\r')
            {
                printf ("Roots: %4d Letters: %4d NoDust: %4d Dust: %4d ",
                        p -> nRoots,
                        p -> nLetters,
                        p -> nRoots - p -> nDust,
                        p -> nDust);

                printf ("Width %4d Height: %4d H/W: %5.2f\n",
                        nBlockWidth,
                        nBlockHeight,
                        p -> Rect.xRight  - p -> Rect.xLeft + 1,
                        p -> Rect.yBottom - p -> Rect.yTop  + 1,
                        (double) (p -> Rect.yBottom - p -> Rect.yTop  + 1) /
                                 (p -> Rect.xRight  - p -> Rect.xLeft + 1)
                        );
            }

            continue;
        }
# endif

/*
        if (p -> nLetters <= (p -> nRoots - p -> nDust) / 10)
        {
            BlockRemove (p);
            continue;
        }
*/
    }

# ifdef LT_DEBUG
    //if (LT_DebugGraphicsLevel == 2 && bAtLeastOneOutput)
	if (!LDPUMA_Skip(hRemoveEmptyBlocks) && bAtLeastOneOutput)
        LT_Getch ();
# endif

# ifdef LT_DEBUG_CALIBRATE
    if (bDebugOptionCalibratePictureRemovingCriteria ||
        bDebugOptionCalibrateDD_RemovingCriteria     ||
        bDebugOptionCalibrateLinearRemovingCriteria)
    {
        LT_GraphicsClose ();
        ErrorInternal ("Calibrating complete");
    }
# endif
}
