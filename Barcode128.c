#include "Barcode128.h"

Barcode128::Barcode128()
{
    //ctor
}

Barcode128::~Barcode128()
{
    //dtor
}

void Barcode128::Draw128(wxDC& hDC, int iX, int iY0, int iY1, const wxColour clrBar, const wxColour clrSpace, const int iPenW)
{
    DrawBarcode(hDC, iX, iY0, iY1, iY1, clrBar, clrSpace, iPenW);
}

bool Barcode128::P_Encode128(char*pszCode, const int iSetIn)
{
    Clear();
    wxByte*pFst = ia_Buf;
    wxByte*pb = pFst;


    if (iSetIn == SUB::SETA)	pb = P_GetBarSpace128(pb, 103);
    else
        if (iSetIn == SUB::SETB)	pb = P_GetBarSpace128(pb, 104);
        else					pb = P_GetBarSpace128(pb, 105);
        if (pb == 0)	return 0;

        const int iCheckDigit = GetCheckDigit(iSetIn, pszCode);
        const int iNum = strlen(pszCode);

        int iChar, iCharNext;
        int iPosition = 0;
        int iSet = iSetIn;

        while (iPosition < iNum) {
            if (iSet == SUB::SETC) {
                if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 101) {
                    pb = P_GetBarSpace128(pb, 101);
                    iPosition++;
                    iSet = SUB::SETA;
                }
                else
                    if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 100) {
                        pb = P_GetBarSpace128(pb, 100);
                        iPosition++;
                        iSet = SUB::SETB;
                    }
                    else if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 102) {
                        pb = P_GetBarSpace128(pb, 100);
                        iPosition++;
                    }
                    else {
                        char chT = pszCode[iPosition + 2];
                        pszCode[iPosition + 2] = 0;
                        iChar = atoi(&pszCode[iPosition]);
                        pszCode[iPosition + 2] = chT;

                        pb = P_GetBarSpace128(pb, iChar);
                        if (pb == 0)	return 0;
                        iPosition += 2;
                    }
            }
            else {
                int iTemp2 = pszCode[iPosition];
                if (iTemp2 < -1)	iTemp2 = iTemp2 & 255;

                iChar = ga2_Code128[iSet][iTemp2];

                pb = P_GetBarSpace128(pb, iChar);
                if (pb == 0)	return 0;
                iPosition++;
                if (iSet == SUB::SETA) {
                    if (iChar == 100)	iSet = SUB::SETB;
                    else
                        if (iChar == 99)	iSet = SUB::SETC;
                }
                else if (iSet == SUB::SETB) {
                    if (iChar == 101)	iSet = SUB::SETA;
                    else
                        if (iChar == 99)	iSet = SUB::SETC;
                }
                else
                    if (iChar == 98) {
                        if (iSet == SUB::SETA)
                            iCharNext = ga2_Code128[SUB::SETB][pszCode[iPosition]];
                        else
                            iCharNext = ga2_Code128[SUB::SETA][pszCode[iPosition]];

                        pb = P_GetBarSpace128(pb, iChar);
                        if (pb == 0)	return 0;
                        iPosition++;
                    }
            }
        }

        pb = P_GetBarSpace128(pb, iCheckDigit);
        if (pb == 0)	return 0;
        pb = P_GetBarSpace128(pb, 106);
        i_LenBuf = pb - pFst;

        return 1;
}

wxByte* Barcode128::P_GetBarSpace128(wxByte*pb, int iV)
{

    if (iV < 0)	return 0;
    if (iV > 106)	return 0;
    IntString infs[] =
    {
        {0,		"bbsbbssbbss"},
        {1,		"bbssbbsbbss"},
        {2,		"bbssbbssbbs"},
        {3,		"bssbssbbsss"},
        {4,		"bssbsssbbss"},
        {5,		"bsssbssbbss"},
        {6,		"bssbbssbsss"},
        {7,		"bssbbsssbss"},
        {8,		"bsssbbssbss"},
        {9,		"bbssbssbsss"},
        {10,	"bbssbsssbss"},
        {11,	"bbsssbssbss"},
        {12,	"bsbbssbbbss"},
        {13,	"bssbbsbbbss"},
        {14,	"bssbbssbbbs"},
        {15,	"bsbbbssbbss"},
        {16,	"bssbbbsbbss"},
        {17,	"bssbbbssbbs"},
        {18,	"bbssbbbssbs"},
        {19,	"bbssbsbbbss"},
        {20,	"bbssbssbbbs"},
        {21,	"bbsbbbssbss"},
        {22,	"bbssbbbsbss"},
        {23,	"bbbsbbsbbbs"},
        {24,	"bbbsbssbbss"},
        {25,	"bbbssbsbbss"},
        {26,	"bbbssbssbbs"},
        {27,	"bbbsbbssbss"},
        {28,	"bbbssbbsbss"},
        {29,	"bbbssbbssbs"},
        {30,	"bbsbbsbbsss"},
        {31,	"bbsbbsssbbs"},
        {32,	"bbsssbbsbbs"},
        {33,	"bsbsssbbsss"},
        {34,	"bsssbsbbsss"},
        {35,	"bsssbsssbbs"},
        {36,	"bsbbsssbsss"},
        {37,	"bsssbbsbsss"},
        {38,	"bsssbbsssbs"},
        {39,	"bbsbsssbsss"},
        {40,	"bbsssbsbsss"},
        {41,	"bbsssbsssbs"},
        {42,	"bsbbsbbbsss"},
        {43,	"bsbbsssbbbs"},
        {44,	"bsssbbsbbbs"},
        {45,	"bsbbbsbbsss"},
        {46,	"bsbbbsssbbs"},
        {47,	"bsssbbbsbbs"},
        {48,	"bbbsbbbsbbs"},
        {49,	"bbsbsssbbbs"},
        {50,	"bbsssbsbbbs"},
        {51,	"bbsbbbsbsss"},
        {52,	"bbsbbbsssbs"},
        {53,	"bbsbbbsbbbs"},
        {54,	"bbbsbsbbsss"},
        {55,	"bbbsbsssbbs"},
        {56,	"bbbsssbsbbs"},
        {57,	"bbbsbbsbsss"},
        {58,	"bbbsbbsssbs"},
        {59,	"bbbsssbbsbs"},
        {60,	"bbbsbbbbsbs"},
        {61,	"bbssbssssbs"},
        {62,	"bbbbsssbsbs"},
        {63,	"bsbssbbssss"},
        {64,	"bsbssssbbss"},
        {65,	"bssbsbbssss"},
        {66,	"bssbssssbbs"},
        {67,	"bssssbsbbss"},
        {68,	"bssssbssbbs"},
        {69,	"bsbbssbssss"},
        {70,	"bsbbssssbss"},
        {71,	"bssbbsbssss"},
        {72,	"bssbbssssbs"},
        {73,	"bssssbbsbss"},
        {74,	"bssssbbssbs"},
        {75,	"bbssssbssbs"},
        {76,	"bbssbsbssss"},
        {77,	"bbbbsbbbsbs"},
        {78,	"bbssssbsbss"},
        {79,	"bsssbbbbsbs"},
        {80,	"bsbssbbbbss"},
        {81,	"bssbsbbbbss"},
        {82,	"bssbssbbbbs"},
        {83,	"bsbbbbssbss"},
        {84,	"bssbbbbsbss"},
        {85,	"bssbbbbssbs"},
        {86,	"bbbbsbssbss"},
        {87,	"bbbbssbsbss"},
        {88,	"bbbbssbssbs"},
        {89,	"bbsbbsbbbbs"},
        {90,	"bbsbbbbsbbs"},
        {91,	"bbbbsbbsbbs"},
        {92,	"bsbsbbbbsss"},
        {93,	"bsbsssbbbbs"},
        {94,	"bsssbsbbbbs"},
        {95,	"bsbbbbsbsss"},
        {96,	"bsbbbbsssbs"},
        {97,	"bbbbsbsbsss"},
        {98,	"bbbbsbsssbs"},
        {99,	"bsbbbsbbbbs"},
        {100,	"bsbbbbsbbbs"},
        {101,	"bbbsbsbbbbs"},
        {102,	"bbbbsbsbbbs"},
        //			{103,	"bbsbsbbbbss"},
        {103,	"bbsbssssbss"},
        {104,	"bbsbssbssss"},
        {105,	"bbsbssbbbss"},
        {106,	"bbsssbbbsbsbb"},
    };

    int i;
    IntString&inf = infs[iV];
    for (i = 0; i < 11; i++) {
        if (inf.psz[i] == 'b')	*pb += 1;
        pb++;
    }
    if (iV == 106) {
        *pb += 1;	pb++;
        *pb += 1;	pb++;
    }
    return pb;
}

int Barcode128::GetCheckDigit(const int iSet, char*pszCode)
{
    int	iSum = 0, iCurSet = 0, iChar128, iCharNext, iWeight, iPosition;

    iCurSet = iSet;
    if (iSet == SUB::SETA) {
        iSum = 103;
    }
    else
        if (iSet == SUB::SETB) {
            iSum = 104;
        }
        else
            if (iSet == SUB::SETC) {
                iSum = 105;
            }

    iPosition = 0;
    iWeight = 1;

    const int iNum = strlen(pszCode);
    while (iPosition < iNum) {
        if (iCurSet == SUB::SETC) {
            if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 101) {
                iChar128 = 101;
                iSum += (iWeight*iChar128);

                iPosition++;
                iWeight++;
                iCurSet = SUB::SETA;
            }
            else if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 100) {
                iChar128 = 100;
                iSum += (iWeight*iChar128);
                iPosition++;
                iWeight++;
                iCurSet = SUB::SETB;
            }
            else
                if (ga2_Code128[SUB::SETA][pszCode[iPosition]] == 102) {
                    iChar128 = 102;
                    iSum += (iWeight*iChar128);
                    iPosition++;
                    iWeight++;
                }
                else {
                    char chT = pszCode[iPosition + 2];
                    pszCode[iPosition + 2] = 0;
                    iChar128 = atol(&pszCode[iPosition]);
                    pszCode[iPosition + 2] = chT;

                    iSum += (iWeight*iChar128);
                    iPosition += 2;
                    iWeight++;
                }
        }
        else {
            int iTemp2 = pszCode[iPosition];
            if (iTemp2 < -1)	iTemp2 = iTemp2 & 255;

            iChar128 = ga2_Code128[iCurSet][iTemp2];

            iSum += (iWeight*iChar128);

            iPosition++;
            iWeight++;

            if (iCurSet == SUB::SETA) {
                if (iChar128 == 100)
                    iCurSet = SUB::SETB;
                else if (iChar128 == 99)
                    iCurSet = SUB::SETC;
            }
            else
                if (iCurSet == SUB::SETB) {
                    if (iChar128 == 101)		iCurSet = SUB::SETA;
                    else if (iChar128 == 99)	iCurSet = SUB::SETC;
                }
                else
                    if (iChar128 == 98) {
                        if (iCurSet == SUB::SETA)
                            iCharNext = ga2_Code128[SUB::SETB][pszCode[iPosition]];
                        else
                            iCharNext = ga2_Code128[SUB::SETA][pszCode[iPosition]];

                        iSum += (iWeight*iCharNext);
                        iPosition++;
                        iWeight++;
                    }
        }
    }
    return iSum % 103;
}
