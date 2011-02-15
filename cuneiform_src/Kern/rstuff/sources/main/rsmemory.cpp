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

/**********  Заголовок  ****************************************************************************/
/*  Автор,                                                                                         */
/*  комментарии                                                                                    */
/*  правка     : Алексей Коноплёв                                                                  */
/*  Редакция   :  20.10.00                                                                         */
/*  Файл       :  'RSMemory.cpp'                                                                   */
/*  Содержание :                                                                                   */
/*  Назначение :                                                                                   */
/*  Комментарий:                                                                                   */
/*                                                                                                 */
/*-------------------------------------------------------------------------------------------------*/
// Для использования без CFIO.DLL
// см файл RSDefines.h
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
//#define  RSTUFF_USE_GLOBAL_MEM
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifdef RSTUFF_USE_GLOBAL_MEM
#include <windows.h>
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#include "resource.h"
#include "rsdefines.h"
#include "rsmemory.h"
#include "rsfunc.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#ifndef _NO_CFIO
 #include "cfio.h"
#else
 #define CFIO_MAX_COMMENT       48
#endif
// extern functions
//void SetReturnCode_rstuff(uint16_t rc);
//uint16_t GetReturnCode_rstuff();
/////////////////////////////////////////////////////////////////////////////////////////////////////
//CFIO Entries
static void* (*pDAlloc)(uint32_t, uint32_t, puchar, puchar) = NULL;
static void* (*pAlloc)(uint32_t, uint32_t)                  = NULL;
static void  (*pFree)(void *)                           = NULL;
static void* (*pLock)(void *)                           = NULL;
static void  (*pUnlock)(void *)                         = NULL;

uchar* Buffer=NULL;
uchar* WorkMem=NULL;
int BufferSize=0;
int WorkMemSize=0;
/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define TAKE_ENTRIE(Name,Pointer,Out)     	if ( !CFIO_GetExportData(Name, (void*)(&Pointer)) ) Out= FALSE;
Bool32 InitCFIOInterface(Bool32 Status)
{
	Bool32 bRet = TRUE;

#ifndef _NO_CFIO

	if ( Status == TRUE )
	{
		CFIO_Init(NULL, NULL);

		TAKE_ENTRIE(CFIO_FNDAllocMemory, pDAlloc, bRet)
		TAKE_ENTRIE(CFIO_FNAllocMemory, pAlloc, bRet)
		TAKE_ENTRIE(CFIO_FNFreeMemory, pFree, bRet)
		TAKE_ENTRIE(CFIO_FNLockMemory, pLock, bRet)
		TAKE_ENTRIE(CFIO_FNUnlockMemory, pUnlock, bRet)
	}
	else
	{
		bRet = CFIO_Done();
	}

#endif //_NO_CFIO

	return bRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//
#define RSTUFF
char cCommentBuffer[CFIO_MAX_COMMENT];

void    RSTUFFComment(const char *Comment)
{
	uint32_t Len = strlen(Comment);
	strncpy(cCommentBuffer, Comment, (Len < CFIO_MAX_COMMENT ? Len : CFIO_MAX_COMMENT - 1 ) );
}

void *	RSTUFFDAlloc(uint32_t stAllocateBlock, const char *Comment)
{
	RSTUFFComment(Comment);
	return RSTUFFAlloc(stAllocateBlock);
}

void *	RSTUFFAlloc(uint32_t stAllocateBlock)
{
	char * mem = NULL;

#ifdef _NO_CFIO

	#ifdef  RSTUFF_USE_GLOBAL_MEM

		mem = (char *)GlobalAlloc(GPTR, stAllocateBlock);

	#else

		mem = ::new char[stAllocateBlock];
		memset(mem, 0, stAllocateBlock );

	#endif

	if(!mem)
		SetReturnCode_rstuff((uint16_t)IDS_RSTUFF_ERR_NO_MEMORY);
#else

	mem = (char *)CFIO_DAllocMemory(stAllocateBlock,MAF_GALL_GPTR,(char*)"RSTUFF", (char*)cCommentBuffer);

	if(!mem)
		SetReturnCode_rstuff((uint16_t)IDS_RSTUFF_ERR_NO_MEMORY);

#endif

	return mem;
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
void	RSTUFFFree(void * mem)
{
#ifdef _NO_CFIO

	#ifdef  RSTUFF_USE_GLOBAL_MEM

		GlobalFree(mem);

	#else

		::delete []	mem;

	#endif
#else

	CFIO_FreeMemory(mem);

#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
void *    RSTUFFLock( void * mem )
{
#ifdef _NO_CFIO

	#ifdef  RSTUFF_USE_GLOBAL_MEM

		return GlobalLock(mem);

	#else

		return mem;

	#endif

#else

	void * pMem;

	pMem = CFIO_LockMemory(mem);

	if ( pMem == NULL && mem != NULL )
		return mem;

	return pMem;

#endif
}
//////////////////////////////////////////////////////////////////////////////////////////////////
//
void     RSTUFFUnlock( void * mem )
{
#ifdef _NO_CFIO

	#ifdef  RSTUFF_USE_GLOBAL_MEM

		GlobalUnlock(mem);

	#else

		return;

	#endif

#else

	CFIO_UnlockMemory(mem);
	return;

#endif
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//
Handle  RSTUFFOpenSave(char * lpName)
{
	Handle rc = NULL;
#ifdef _NO_CFIO
	rc = (Handle)fopen(lpName,"wb");
#else
	rc = (Handle)fopen(lpName,"wb");
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
Handle  RSTUFFOpenRestore(char * lpName)
{
	Handle rc = NULL;
#ifdef _NO_CFIO
	rc = (Handle)fopen(lpName,"rb");
#else
	rc = (Handle)fopen(lpName,"rb");
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int  RSTUFFWrite(Handle h,void * lpdata,unsigned int size)
{
	uint32_t rc = 0;
#ifdef _NO_CFIO
	rc = fwrite(lpdata,1,size,(FILE*)h);
#else
	rc = fwrite(lpdata,1,size,(FILE*)h);
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
unsigned int  RSTUFFRead(Handle h,void * lpdata,unsigned int size)
{
	uint32_t rc = 0;
#ifdef _NO_CFIO
	rc = fread(lpdata,1,size,(FILE *)h);
#else
	rc = fread(lpdata,1,size,(FILE *)h);
#endif
	return rc;
}
/////////////////////////////////////////////////////////////////////////////////////////
//
void    RSTUFFClose(Handle h)
{
#ifdef _NO_CFIO
	fclose((FILE*)h);
#else
	fclose((FILE*)h);
#endif
}

void GiveMainBuff (void **vvBuff, int *Size)
{
	*vvBuff = Buffer;
	*Size = BufferSize;
}
void GiveWorkBuff (char **ccBuff, int *Size)
{
	*ccBuff = (char*)WorkMem;
	*Size = WorkMemSize;
}

void SetMainBuff(void *vBuff, int Size)
{
	Buffer=(uchar*)vBuff;
	BufferSize=Size;
}

void SetWorkBuff(void *vBuff, int Size)
{
	WorkMem=(uchar*)vBuff;
	WorkMemSize=Size;
}

void ReSetMem()
{
	Buffer=NULL;
	BufferSize=0;
	WorkMem=NULL;
	WorkMemSize=0;
}

/////////////////////////////////////////////////////////////////////////////////////////
// end of file
