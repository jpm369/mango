/*
   Copyright 2017 University of Washington                          3-clause BSD license

   Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

   3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _MANGO_H_
#define _MANGO_H_

#define SIZE_BUF    8192
#define SIZE_FILE   512
#define MAX_PEAKS   2500

using namespace std;

#include "MSReader.h"
#include "Spectrum.h"
#include "MSObject.h"
#include "math.h"
#include <vector>
#include <cfloat>
#include "mango_Interfaces.h"
#include "mango_ProcessManager.h"

using namespace MSToolkit;

struct PrecursorsStruct
{
   int    iCharge1;
   int    iCharge2;
   int    iIntensity1;
   int    iIntensity2;
   double dNeutralMass1;
   double dNeutralMass2;
};

// MS/MS scan information
struct ScanDataStruct
{
   int    iScanNumber;
   int    iPrecursorScanNumber;
   int    iPrecursorCharge;   // charge state of intact precursor
   double dPrecursorMZ;       // intact precursor m/z
   double dPrecursorNeutralMass;       // intact precursor Neutralmass
   double dHardklorPrecursorNeutralMass;  // precursor neutral mass found from Hardklor

   vector<PrecursorsStruct> pvdPrecursors;
};

struct ParamsStruct
{
   int iResolution1;
   int iResolution2;
   int iChargeMin1;
   int iChargeMin2;
   int iChargeMax1;
   int iChargeMax2;

   double dToleranceMS1;
   double dToleranceMS2;
   double dMassReporter;
   double dMassMod;
};

extern vector<ScanDataStruct> pvSpectrumList;

void Usage(char *pszCmd);
void ProcessCmdLine(int argc,
                    char *argv[],
                    char *szParamsFile,
                    vector<InputFileInfo*> &pvInputFiles,
                    IMangoSearchManager *pSearchMgr);
void SetOptions(char *arg,
                bool *bPrintParams);
void LoadParameters(char *pszParamsFile,
                    IMangoSearchManager *pSearchMgr);
void PrintParams(void);




#endif // _MANGO_H_
