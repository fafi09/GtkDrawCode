#include "BarcodeBase.h"

void InitBarcodeBase()
{
    Clear();
    i_Ratio = 3;
}

void Clear()
{
    memset(ia_Buf, 0, sizeof(ia_Buf));
    i_LenBuf = 0;
}

int GetEncodeLength()	const
{
    wxByte*pb = (wxByte*)ia_Buf;
    int i, iLen = 0;
    for (i = 0; i < i_LenBuf; i++) {
        //wide is 3
        if (*pb & 2)	iLen += (i_Ratio - 1);
        pb++;
    }
    return iLen + i_LenBuf;
}

int GetBufferLength()	const
{
    return i_LenBuf;
}


const wxByte& GetAt(int i)	const
{
    return ia_Buf[i];
}

int GetRatio()	const
{
    return i_Ratio;
}

int SetRatio(int iRatio)
{
    i_Ratio = iRatio;
    if (i_Ratio <= 0)	i_Ratio = 1;
}

void DrawBarcode(wxDC& hDC, int iX, int iY0, int iY10, int iY11, const wxColour clrBar, const wxColour clrSpace, const int iPenW)
{
    wxPen hPenBar(clrBar, iPenW, wxPENSTYLE_SOLID);
    wxPen hPenSpace(clrSpace, iPenW, wxPENSTYLE_SOLID);

    wxPen hPenOld = hDC.GetPen();

    wxByte *pb = ia_Buf;
    int i0, iNum0 = i_LenBuf;

    wxByte bBar;
    int i1, iNum1;
    int iY;
    for (i0 = 0; i0 < iNum0; i0++) {
        bBar = *pb & 0x01;
        iNum1 = (*pb & 0x02) ? i_Ratio : 1;
        iY = (*pb & 0x04) ? iY11 : iY10;
        for (i1 = 0; i1 < iNum1; i1++) {
            if (bBar)	hDC.SetPen(hPenBar);
            else		hDC.SetPen(hPenSpace);

            hDC.DrawLine(iX, iY0, iX, iY);
            iX += iPenW;
        }
        pb++;
    }

    hDC.SetPen(hPenOld);
}
