#ifndef _MANGOPROCESS_H_
#define _MANGOPROCESS_H_

#include "Common.h"
#include "mango_DataInternal.h"

#define NUMPEPTIDES 10

class mango_Process
{
public:

   mango_Process();
   ~mango_Process();

   static void ProcessData(char *szMZXML);
private:

   // Private static methods
   static void PrintPeaks(Spectrum mstSpectrum,
                          FILE *fpout);

   // Private member variables
};

#endif // _MANGOPROCESS_H_
