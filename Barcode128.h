#ifndef BARCODE128_H
#define BARCODE128_H

#include "BarcodeBase.h"

class Barcode128 : public BarcodeBase
{
    public:
        Barcode128();
        virtual ~Barcode128();
        bool Encode128A(const char* pszCode) { return P_Encode128((char*)pszCode, SUB::SETA); }
        bool Encode128B(const char* pszCode) { return P_Encode128((char*)pszCode, SUB::SETB); }
        bool Encode128C(const char* pszCode) { return P_Encode128((char*)pszCode, SUB::SETC); }
        void Draw128(wxDC& hDC, int iX, int iY0, int iY1, const wxColour clrBar, const wxColour clrSpace, const int iPenW);

    private:
        struct SUB {
            enum {
                SETA = 0,
                SETB = 1,
                SETC = 2,
            };
        };
        bool P_Encode128(char*pszCode, const int iSetIn);
        wxByte* P_GetBarSpace128(wxByte*pb, int iV);
        int GetCheckDigit(const int iSet, char*pszCode);

};

#endif // BARCODE128_H
