#ifndef _MANGODATAINTERNAL_H_
#define _MANGODATAINTERNAL_H_

#include "mango_Data.h"

class MangoSearchManager;

#define PROTON_MASS                 1.00727646688
#define C13_DIFF                    1.00335483

#define FLOAT_ZERO                  1e-6     // 0.000001


struct msdata                    // used in the preprocessing
{
   double dIon;
   double dIntensity;
};


struct Options             // output parameters
{
   double dReporterMass;
   char szInstrument1[64];
   char szInstrument2[64];
   int iResolution1;
   int iResolution2;
   int iCentroided1;
   int iCentroided2;
   int iExportMGF;
   int iOffset;

   Options& operator=(Options& a)
   {
      dReporterMass = a.dReporterMass;
      iResolution1 = a.iResolution1;
      iResolution2 = a.iResolution2;
      iCentroided1 = a.iCentroided1;
      iCentroided2 = a.iCentroided2;
      iExportMGF = a.iExportMGF;
      iOffset = a.iOffset;
      strcpy(szInstrument1, a.szInstrument1);
      strcpy(szInstrument2, a.szInstrument2);
      return *this;
   }
};


struct SpectrumInfoInternal
{
   int iArraySize;         // m/z versus intensity array
   int iScanNumber;
};

struct ToleranceParams
{
   double dTolerancePeptide;        // tolerance from param file
   double dToleranceRelationship;   // tolerance from param file

   ToleranceParams()
   {
      dTolerancePeptide = 20;
      dToleranceRelationship = 10;
   }

   ToleranceParams& operator=(ToleranceParams& a)
   {
      dTolerancePeptide = a.dTolerancePeptide;
      dToleranceRelationship = a.dToleranceRelationship;

      return *this;
   }
};


// static user params, won't change per thread - can make global!
struct StaticParams
{
   Options         options;
   InputFileInfo   inputFile;
   ToleranceParams tolerances;

   StaticParams()
   {
      options.dReporterMass = 751.40508;
      options.iResolution1 = 49505;
      options.iResolution2 = 49505;
      options.iCentroided1 = 1;
      options.iCentroided2 = 1;
      options.iExportMGF = 0;
      options.iOffset = 2;
      strcpy(options.szInstrument1, "Orbitrap");
      strcpy(options.szInstrument2, "Orbitrap");
   }

   StaticParams& operator=(StaticParams& a)
   {
      inputFile = a.inputFile;
      tolerances = a.tolerances;
      return *this;
   }

};

extern StaticParams g_staticParams;

extern vector<InputFileInfo*>  g_pvInputFiles;

#endif // _MANGODATAINTERNAL_H_
