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

/*------------------------------------------------------------------------------------------------*/
/**
    \file   UN_Buff.CPP
    \brief  Реализация функций работы со складом информации
    \author Александр Михайлов
    \date   22.04.2005
*/
/*------------------------------------------------------------------------------------------------*/
/*  N-я Редакция 22.04.2005:
    - приведение к стандарту алгоритмов, реализованных в модуле "UN_Buff.CPP" к 02.03.2001  */
/*------------------------------------------------------------------------------------------------*/
/**********  Заголовок  **********/
/*  Автор      :  Александр Михайлов                                          */
/*  Редакция   :  03.02.01                                                    */
/*  Файл       :  'UN_Buff.CPP'                                               */
/*  Содержание :  Функции работы с памятью.                                   */
/*  Назначение :  Оптимизация работы с памятью.                               */
/*----------------------------------------------------------------------------*/
#include <stdio.h>
/*  межпроектные мои  */
#include "un_buff.h"
/*----------    Переходные функции    ------------------------------------------------------------*/
void CleanLastDataPart (void *p_vB)
{
    UN_BUFF *pB = static_cast<UN_BUFF *>(p_vB);
    pB->SizeCurr += pB->SizePartTotal[pB->nPart - 1];
    pB->vCurr = pB->vPart[pB->nPart - 1];
    pB->vPart[pB->nPart - 1] = NULL;
    pB->TypePart[pB->nPart - 1] = 0;//==UN_DT_Unknown;
    pB->AimPart[pB->nPart] = 0;//==UN_DA_Unknown;
    pB->SizePartUnits[pB->nPart - 1] = 0;
    pB->nPartUnits[pB->nPart-1] = 0;
    pB->SizePartTotal[pB->nPart - 1] = 0;
    (pB->nPart)--;
}
/*------------------------------------------------------------------------------------------------*/
void EndLastDataPart (void *p_vB, const char Aim, const char Type, const int SizeU, const int nU)
{
    UN_BUFF *pB = static_cast<UN_BUFF *>(p_vB);
    pB->vPart[pB->nPart] = pB->vCurr;
    pB->TypePart[pB->nPart] = Type;
    pB->AimPart[pB->nPart] = Aim;
    pB->SizePartUnits[pB->nPart] = SizeU;
    pB->nPartUnits[pB->nPart] = nU;
    pB->SizePartTotal[pB->nPart] = SizeU * nU;
    (pB->nPart)++;
    pB->SizeCurr -= (SizeU * nU);
    char *p_cCurr = (static_cast<char *>(pB->vCurr)) + SizeU * nU;
    pB->vCurr = static_cast<void *>(p_cCurr);
}
/*------------------------------------------------------------------------------------------------*/
int FindSuchData (const void *p_vB, const int WhatData)
{
    const UN_BUFF *pB = static_cast<const UN_BUFF *>(p_vB);
    int Ind = -1;
    /*  Перебираем все записи  */
    for (int i = 0;  i < pB->nPart;  i++)
    {
        if (pB->TypePart[i] != WhatData)
            continue;
        Ind = i;
        break;
    }
    return Ind;
}
/*------------------------------------------------------------------------------------------------*/
int FindSuchAimedData (const void *p_vB, const int WhatData, const int WhatAim)
{
    const UN_BUFF *pB = static_cast<const UN_BUFF *>(p_vB);
    int Ind = -1;
    /*  Перебираем все записи  */
    for (int i = 0;  i < pB->nPart;  i++)
    {
        if (pB->TypePart[i] != WhatData)
            continue;
        if (pB->AimPart[i] != WhatAim)
            continue;
        Ind = i;
        break;
    }
    return Ind;
}
/*------------------------------------------------------------------------------------------------*/
int FindNextSuchData (const void *p_vB, const int WhatData, const int IndPrev)
{
    const UN_BUFF *pB = static_cast<const UN_BUFF *>(p_vB);
    int Ind = IndPrev;
    /*  Перебираем все записи после заданной  */
    for (int i = IndPrev + 1;  i < pB->nPart;  i++)
    {
        if (pB->TypePart[i] != WhatData)
            continue;
        Ind = i;
        break;
    }
    return Ind;
}
/*------------------------------------------------------------------------------------------------*/
