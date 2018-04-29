#include "Common.h"
#include "mango.h"
#include "mango_Process.h"
#include "mango_DataInternal.h"
#include "mango_ProcessManager.h"

#undef PERF_DEBUG

std::vector<InputFileInfo *>  g_pvInputFiles;
StaticParams                  g_staticParams;


/******************************************************************************
*
* MangoSearchManager class implementation.
*
******************************************************************************/

MangoSearchManager::MangoSearchManager()
{
   // Initialize the Mango version
   SetParam("# mango_version ", mango_version, mango_version);
}

MangoSearchManager::~MangoSearchManager()
{
   //std::vector calls destructor of every element it contains when clear() is called
   g_pvInputFiles.clear();

   //for (std::map<string, MangoParam*>::iterator it = _mapStaticParams.begin(); it != _mapStaticParams.end(); ++it)
   //   delete it->second;
   _mapStaticParams.clear();
}

bool MangoSearchManager::InitializeStaticParams()
{
   string strData;
   IntRange intRangeData;
   DoubleRange doubleRangeData;

   GetParamValue("mass_tolerance_relationship", g_staticParams.tolerances.dToleranceRelationship);
   GetParamValue("mass_tolerance_peptide", g_staticParams.tolerances.dTolerancePeptide);
   GetParamValue("reporter_neutral_mass", g_staticParams.options.dReporterMass);

   if (GetParamValue("instrument1", strData))
      strcpy(g_staticParams.options.szInstrument1, strData.c_str());
   GetParamValue("instrument2", strData);
      strcpy(g_staticParams.options.szInstrument2, strData.c_str());

   GetParamValue("resolution1", g_staticParams.options.iResolution1);
   GetParamValue("resolution2", g_staticParams.options.iResolution2);
   GetParamValue("centroided1", g_staticParams.options.iCentroided1);
   GetParamValue("centroided2", g_staticParams.options.iCentroided2);
   GetParamValue("export_mgf", g_staticParams.options.iExportMGF);

   return true;
}

void MangoSearchManager::AddInputFiles(vector<InputFileInfo*> &pvInputFiles)
{
   int numInputFiles = pvInputFiles.size();

   for (int i = 0; i < numInputFiles; i++)
      g_pvInputFiles.push_back(pvInputFiles.at(i));
}

void MangoSearchManager::SetOutputFileBaseName(const char *pszBaseName)
{
   strcpy(g_staticParams.inputFile.szBaseName, pszBaseName);
}

std::map<std::string, MangoParam*>& MangoSearchManager::GetParamsMap()
{
   return _mapStaticParams;
}

void MangoSearchManager::SetParam(const string &name, const string &strValue, const string& value)
{
   MangoParam *pParam = new TypedMangoParam<string>(MangoParamType_String, strValue, value);
   pair<map<string, MangoParam*>::iterator,bool> ret = _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   if (false == ret.second)
   {
      _mapStaticParams.erase(name);
      _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   }
}

bool MangoSearchManager::GetParamValue(const string &name, string& value)
{
   std::map<string, MangoParam*>::iterator it;
   it = _mapStaticParams.find(name);
   if (it == _mapStaticParams.end())
      return false;

   TypedMangoParam<string> *pParam = static_cast<TypedMangoParam<string>*>(it->second);
   value = pParam->GetValue();
   return true;
}

void MangoSearchManager::SetParam(const std::string &name, const string &strValue, const int &value)
{
   MangoParam *pParam = new TypedMangoParam<int>(MangoParamType_Int, strValue, value);
   pair<map<string, MangoParam*>::iterator,bool> ret = _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   if (false == ret.second)
   {
      _mapStaticParams.erase(name);
      _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   }
}

bool MangoSearchManager::GetParamValue(const string &name, int& value)
{
   std::map<string, MangoParam*>::iterator it;
   it = _mapStaticParams.find(name);
   if (it == _mapStaticParams.end())
      return false;

   TypedMangoParam<int> *pParam = static_cast<TypedMangoParam<int>*>(it->second);
   value = pParam->GetValue();
   return true;
}

void MangoSearchManager::SetParam(const string &name, const string &strValue, const double &value)
{
   MangoParam *pParam = new TypedMangoParam<double>(MangoParamType_Double, strValue, value);
   pair<map<string, MangoParam*>::iterator,bool> ret = _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   if (false == ret.second)
   {
      _mapStaticParams.erase(name);
      _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   }
}

bool MangoSearchManager::GetParamValue(const string &name, double& value)
{
   std::map<string, MangoParam*>::iterator it;
   it = _mapStaticParams.find(name);
   if (it == _mapStaticParams.end())
      return false;

   TypedMangoParam<double> *pParam = static_cast<TypedMangoParam<double>*>(it->second);
   value = pParam->GetValue();
   return true;
}

void MangoSearchManager::SetParam(const string &name, const string &strValue, const DoubleRange &value)
{
   MangoParam *pParam = new TypedMangoParam<DoubleRange>(MangoParamType_DoubleRange, strValue, value);
   pair<map<string, MangoParam*>::iterator,bool> ret = _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   if (false == ret.second)
   {
      _mapStaticParams.erase(name);
      _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   }
}

bool MangoSearchManager::GetParamValue(const string &name, DoubleRange &value)
{
   std::map<string, MangoParam*>::iterator it;
   it = _mapStaticParams.find(name);
   if (it == _mapStaticParams.end())
      return false;

   TypedMangoParam<DoubleRange> *pParam = static_cast<TypedMangoParam<DoubleRange>*>(it->second);
   value = pParam->GetValue();
   return true;
}

void MangoSearchManager::SetParam(const string &name, const string &strValue, const IntRange &value)
{
   MangoParam *pParam = new TypedMangoParam<IntRange>(MangoParamType_IntRange, strValue, value);
   pair<map<string, MangoParam*>::iterator,bool> ret = _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   if (false == ret.second)
   {
      _mapStaticParams.erase(name);
      _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   }
}

bool MangoSearchManager::GetParamValue(const string &name, IntRange &value)
{
   std::map<string, MangoParam*>::iterator it;
   it = _mapStaticParams.find(name);
   if (it == _mapStaticParams.end())
      return false;

   TypedMangoParam<IntRange> *pParam = static_cast<TypedMangoParam<IntRange>*>(it->second);
   value = pParam->GetValue();
   return true;
}

void MangoSearchManager::SetParam(const string &name, const string &strValue, const vector<double> &value)
{
   MangoParam *pParam = new TypedMangoParam< vector<double> >(MangoParamType_DoubleVector, strValue, value);
   pair<map<string, MangoParam*>::iterator,bool> ret = _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   if (false == ret.second)
   {
      _mapStaticParams.erase(name);
      _mapStaticParams.insert(std::pair<std::string, MangoParam*>(name, pParam));
   }
}

bool MangoSearchManager::GetParamValue(const string &name,  vector<double> &value)
{
   std::map<string, MangoParam*>::iterator it;
   it = _mapStaticParams.find(name);
   if (it == _mapStaticParams.end())
      return false;

   TypedMangoParam< vector<double> > *pParam = static_cast<TypedMangoParam< vector<double> >*>(it->second);
   value = pParam->GetValue();

   return true;
}

bool MangoSearchManager::IsValidMangoVersion(const string &version)
{
    // Major version number must match to current binary
    const char *pszMangoVersion = version.c_str();
    return strstr(mango_version, pszMangoVersion);
}


bool MangoSearchManager::DoAnalysis()
{
   if (!InitializeStaticParams())
      return false;

   for (int i=0; i<(int)g_pvInputFiles.size(); i++)
   {
      char szHK1[SIZE_FILE];
      char szHK2[SIZE_FILE];
      char szMZXML[SIZE_FILE];

      // First read through mzXML file, grab all MS/MS scans and
      // their precursor m/z.  Require precursor charge be 4+
      // or larger?
      strcpy(szMZXML, g_pvInputFiles.at(i)->szFileName);
      strcpy(szHK1, szMZXML);
      szHK1[strlen(szHK1)-5]='\0';
      strcpy(szHK2, szHK1);
      strcat(szHK1, "hk1");        // ms1 hardklor run
      strcat(szHK2, "hk2");        // ms2 hardklor run

      // This first pass read simply gets all ms/ms scans and their measured precursor m/z
      READ_MZXMLSCANS(szMZXML);

      // Next, go to Hardklor .hk1 file to get accurate precursor m/z
      READ_HK1(szHK1);

      // Now, read through .hk2 file to find accurate peptide masses that add up to precursor
      READ_HK2(szHK2);

      int iCount=0;
      for (int ii=0; ii<(int)pvSpectrumList.size(); ii++)
      {
         if (pvSpectrumList.at(ii).pvdPrecursors.size() > 0)
            iCount++;
      }
      printf(" #spectra with relationship: %d    #total spectra:  %d\n", iCount, (int)pvSpectrumList.size());

      // Now open fasta file and get a list of all peptides with masses close to
      mango_Process::ProcessData(szMZXML);

      pvSpectrumList.clear();

      printf("\n");
   }

   printf("\n Done.\n\n");
   return 1;
}


void MangoSearchManager::READ_MZXMLSCANS(char *szMZXML)
{
   MSReader mstReader;
   Spectrum mstSpectrum;
   int iFileLastScan;
   int iScanNumber=1;
   int iMS1ScanNumber=0;

   printf(" reading %s ... ", szMZXML); fflush(stdout);

   // We want to read only MS1/MS2 scans.
   vector<MSSpectrumType> msLevel;
   msLevel.push_back(MS1);
   msLevel.push_back(MS2);
   msLevel.push_back(MS3);

   mstReader.setFilter(msLevel);
   mstReader.readFile(szMZXML, mstSpectrum, 1);
   iFileLastScan = mstReader.getLastScan();

   for (iScanNumber = 1 ; iScanNumber <= iFileLastScan; iScanNumber++)
   {
      // Loads in MSMS spectrum data.
      mstReader.readFile(NULL, mstSpectrum, iScanNumber);

      if (mstSpectrum.getMsLevel() == 1)
      {
         iMS1ScanNumber = iScanNumber;
      }
      else if (mstSpectrum.getMsLevel() == 2)
      {
         struct ScanDataStruct pData;
        
         if (mstSpectrum.sizeZ() > 0)
            pData.iPrecursorCharge = mstSpectrum.atZ(0).z;
         else
            pData.iPrecursorCharge = 0;

         pData.dPrecursorMZ = mstSpectrum.getMZ();
         pData.iScanNumber = iScanNumber;
         pData.iPrecursorScanNumber = iMS1ScanNumber;
         pData.dHardklorPrecursorNeutralMass = 0.0;

         pvSpectrumList.push_back(pData);
      }
      else
      {
         continue;  // skip any MS3 scans
      }

      if (!(iScanNumber%200))
      {
         printf("%3d%%", (int)(100.0*iScanNumber/iFileLastScan));
         fflush(stdout);
         printf("\b\b\b\b");
      }
   }

   printf("100%%\n");
}


void MangoSearchManager::READ_HK1(char *szHK)
{
   FILE *fp;
   char szBuf[SIZE_BUF];
   int iListCt;       // this will keep an index of pvSpectrumList
   long lFP = 0;
   long lEndFP;

   printf(" reading %s ... ", szHK); fflush(stdout);

   if ( (fp=fopen(szHK, "r"))== NULL)
   {
      // Cannot read Hardklor file; try to generate it.
      GENERATE_HK(szHK);

      // Instead of reporting error above, generate HK1 file here.
      // This requires creating a Hardklor params file and executing hardklor.

      // run Hardklor

      // now try to re-open .hk file
      if ( (fp=fopen(szHK, "r"))== NULL)
      {
         printf(" Error ... cannot create/read %s file.\n", szHK);
         exit(1);
      }
   }

   fseek(fp, 0, SEEK_END);
   lEndFP = ftell(fp);  // store current
   rewind(fp);

   iListCt = 0;
   while (fgets(szBuf, SIZE_BUF, fp))
   {
      if (szBuf[0]=='S')
      {
         int iScanNumber;

         sscanf(szBuf, "S\t%d\t", &iScanNumber);

         while (iListCt < (int)pvSpectrumList.size() && pvSpectrumList.at(iListCt).iPrecursorScanNumber < iScanNumber)
            iListCt++;

         if (iListCt < (int)pvSpectrumList.size() && pvSpectrumList.at(iListCt).iPrecursorScanNumber == iScanNumber)
         {

            // Get accurate precursor m/z from deconvoluted MS1 peaks
            lFP = ftell(fp);  // store current

            while (fgets(szBuf, SIZE_BUF, fp))
            {
               if (szBuf[0] == 'S')
               {
                  fseek(fp, lFP, SEEK_SET);
                  break;
               }

               lFP = ftell(fp);

               if (szBuf[0] == 'P')
               {
                  double dMass;
                  double dMS2PrecursorMass;
                  int iCharge;
                  int iIntensity;

                  sscanf(szBuf, "P\t%lf\t%d\t%d\t", &dMass, &iCharge, &iIntensity);


                  if (pvSpectrumList.at(iListCt).iPrecursorCharge > 0)
                  {
                     if (iCharge == pvSpectrumList.at(iListCt).iPrecursorCharge)
                     {
                        dMS2PrecursorMass = pvSpectrumList.at(iListCt).dPrecursorMZ * iCharge - iCharge*PROTON_MASS;

                        if (WithinTolerance(dMass, dMS2PrecursorMass, g_staticParams.tolerances.dTolerancePeptide))
                        {
                           // if current mass diff is less than stored mass diff
                           if (fabs(dMass - dMS2PrecursorMass) < fabs(pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass - dMS2PrecursorMass))
                           {
                              pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass = dMass;
                           }
                        }
                     }
                  }
                  else
                  {
                     // we only have an MS2 m/z and no charge.  So need to take hardklor mass + charge,
                     // apply charge to MS2 m/z and go from there

                     dMS2PrecursorMass = pvSpectrumList.at(iListCt).dPrecursorMZ * iCharge - iCharge*PROTON_MASS;

                     if (WithinTolerance(dMass, dMS2PrecursorMass, g_staticParams.tolerances.dTolerancePeptide))
                     {
                        // if current mass diff is less than stored mass diff
                        if (fabs(dMass - dMS2PrecursorMass) < fabs(pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass - dMS2PrecursorMass))
                        {
                           pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass = dMass;
                        }
                     }
                  }
               }
            }
         }
      
         if (iScanNumber%500)
         {
            printf("%3d%%", (int)(100.0*lFP/lEndFP));
            fflush(stdout);
            printf("\b\b\b\b");
         }

      }
   }
   printf("100%%\n");
   fclose(fp);
}


void MangoSearchManager::READ_HK2(char *szHK)
{
   FILE *fp;
   char szBuf[SIZE_BUF];
   int iListCt;       // this will keep an index of pvSpectrumList
   long lFP = 0;
   long lEndFP;

   printf(" reading %s ... ", szHK); fflush(stdout);

   if ( (fp=fopen(szHK, "r"))== NULL)
   {
      // Cannot read Hardklor file; try to generate it.
      GENERATE_HK(szHK);

      // Instead of reporting error above, generate HK1 file here.
      // This requires creating a Hardklor params file and executing hardklor.

      // run Hardklor

      // now try to re-open .hk file
      if ( (fp=fopen(szHK, "r"))== NULL)
      {
         printf(" Error ... cannot create/read %s file.\n", szHK);
         exit(1);
      }
   }

   fseek(fp, 0, SEEK_END);
   lEndFP = ftell(fp);  // store current
   rewind(fp);

   iListCt = 0;
   while (fgets(szBuf, SIZE_BUF, fp))
   {
      if (szBuf[0]=='S')
      {
         int iScanNumber;

         sscanf(szBuf, "S\t%d\t", &iScanNumber);

         while (iListCt < (int)pvSpectrumList.size() && pvSpectrumList.at(iListCt).iScanNumber < iScanNumber)
            iListCt++;

         if (iListCt < (int)pvSpectrumList.size() && pvSpectrumList.at(iListCt).iScanNumber == iScanNumber)
         {
            int iNumPeaks=0;

            struct
            {
               double dNeutralMass;
               int iCharge;
               int iIntensity;
            } pPeaks[MAX_PEAKS];

            lFP = ftell(fp);  // store current

            // fallback to using MS2 m/z and charge when no hardklor match
            if (pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass == 0 && pvSpectrumList.at(iListCt).iPrecursorCharge > 0)
            {
                pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass = (pvSpectrumList.at(iListCt).dPrecursorMZ
                   * pvSpectrumList.at(iListCt).iPrecursorCharge)
                   - pvSpectrumList.at(iListCt).iPrecursorCharge*PROTON_MASS;
            }

            // Read in all deconvoluted peaks in this MS/MS scan
            while (fgets(szBuf, SIZE_BUF, fp))
            {
               if (szBuf[0] == 'S')
               {
                  fseek(fp, lFP, SEEK_SET);
                  break;
               }

               lFP = ftell(fp);

               if (szBuf[0] == 'P')
               {
                  double dMass;
                  int iCharge;
                  int iIntensity;

                  sscanf(szBuf, "P\t%lf\t%d\t%d\t", &dMass, &iCharge, &iIntensity);

                  if (iNumPeaks == MAX_PEAKS)
                  {
                     printf(" Error, scan %d has MAX_PEAKS (%d) number of deconvoluted peaks.\n", iScanNumber, MAX_PEAKS);
                     printf(" Need to increase definition of MAX_PEAKS\n");
                     exit(1);
                  }

                  pPeaks[iNumPeaks].dNeutralMass = dMass;
                  pPeaks[iNumPeaks].iCharge = iCharge;
                  pPeaks[iNumPeaks].iIntensity = iIntensity;

                  iNumPeaks++;
               }
            }

            // At this point, we know precursor m/z and precursor charge and have read all ms/ms peaks.
            // Must find 2 peptides that add up to intact cross-link (ms1+ms2+reporter)
            // where the charge states of the peptides can't be larger than precursor charge.
            int i;
            int ii;

            for (i=0; i<iNumPeaks; i++)
            {
               for (ii=i; ii<iNumPeaks; ii++)
               {
                  // Placing check here that the peptide masses must be greater than some minimum
                  if (pPeaks[i].dNeutralMass >= 800.0 && pPeaks[ii].dNeutralMass >= 800.0)
                  {
                     double dCombinedMass = pPeaks[i].dNeutralMass + pPeaks[ii].dNeutralMass + g_staticParams.options.dReporterMass;


                     if (WithinTolerance(dCombinedMass, pvSpectrumList.at(iListCt).dHardklorPrecursorNeutralMass, g_staticParams.tolerances.dToleranceRelationship))
                     {
                        if (pPeaks[i].iCharge + pPeaks[ii].iCharge <= pvSpectrumList.at(iListCt).iPrecursorCharge
                              && pPeaks[i].iCharge < pvSpectrumList.at(iListCt).iPrecursorCharge
                              && pPeaks[ii].iCharge < pvSpectrumList.at(iListCt).iPrecursorCharge)
                        {
                           struct PrecursorsStruct pTmp;
                           int a=i;
                           int b=ii;;

                           if (pPeaks[i].dNeutralMass > pPeaks[ii].dNeutralMass)
                           {
                              a=ii;
                              b=i;
                           }
                           pTmp.dNeutralMass1 = pPeaks[a].dNeutralMass;
                           pTmp.dNeutralMass2 = pPeaks[b].dNeutralMass;
                           pTmp.iCharge1 = pPeaks[a].iCharge;
                           pTmp.iCharge2 = pPeaks[b].iCharge;
                           pTmp.iIntensity1 = pPeaks[a].iIntensity;
                           pTmp.iIntensity2 = pPeaks[b].iIntensity;

                           // Do a quick check here and push_back only if the two
                           // masses are not very similar to existing masses
                           
                           bool bMassesAlreadyPresent = false;

                           for (int iii=0; iii<(int)pvSpectrumList.at(iListCt).pvdPrecursors.size(); iii++)
                           {
                              if (WithinTolerance(pTmp.dNeutralMass1, pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).dNeutralMass1,
                                       g_staticParams.tolerances.dTolerancePeptide)
                                    && WithinTolerance(pTmp.dNeutralMass2, pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).dNeutralMass2,
                                       g_staticParams.tolerances.dTolerancePeptide))
                              {
                                 bMassesAlreadyPresent = true;

                                 // Can have same peak in different charge states so store charge state that is most intense
                                 if (pTmp.iCharge1 != pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iCharge1)
                                 {
                                    if (pTmp.iIntensity1 > pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iIntensity1)
                                    {
                                       pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iIntensity1 = pTmp.iIntensity1;
                                       pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iCharge1 = pTmp.iCharge1;
                                       pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).dNeutralMass1 = pTmp.dNeutralMass1;
                                    }
                                 }
                                 if (pTmp.iCharge2 != pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iCharge2)
                                 {
                                    if (pTmp.iIntensity2 > pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iIntensity2)
                                    {
                                       pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iIntensity2 = pTmp.iIntensity2;
                                       pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).iCharge2 = pTmp.iCharge2;
                                       pvSpectrumList.at(iListCt).pvdPrecursors.at(iii).dNeutralMass2 = pTmp.dNeutralMass2;
                                    }
                                 }

                                 break;
                              }
                           }

                           if (!bMassesAlreadyPresent)
                              pvSpectrumList.at(iListCt).pvdPrecursors.push_back(pTmp);
                        }
                     }
                  }
               }
            }
         }
      
         printf("%3d%%", (int)(100.0*lFP/lEndFP));
         fflush(stdout);
         printf("\b\b\b\b");

      }
   }
   printf("100%%\n");
   fclose(fp);
}


void MangoSearchManager::GENERATE_HK(char *szHK)
{
   int iResolution;
   int iCentroided;
   int iMSLevel;
   int iChargeMin;
   int iChargeMax;
   char szConf[SIZE_FILE];
   char szBaseName[SIZE_FILE];
   char szCmd[SIZE_BUF];
   char szInstrument[64];

   strcpy(szBaseName, szHK);
   szBaseName[strlen(szBaseName)-4]='\0';

   if (!strcmp(szHK+strlen(szHK)-4, ".hk1"))
   {
      sprintf(szConf, "%s.conf1", szBaseName);

      strcpy(szInstrument, g_staticParams.options.szInstrument1);
      iResolution = g_staticParams.options.iResolution1;
      iCentroided = g_staticParams.options.iCentroided1;

      iMSLevel = 1;
      iChargeMin = 4; //iChargeMin1;
      iChargeMax = 8; //iChargeMax1;
   }
   else
   {
      sprintf(szConf, "%s.conf2", szBaseName);

      strcpy(szInstrument, g_staticParams.options.szInstrument2);
      iResolution = g_staticParams.options.iResolution2;
      iCentroided = g_staticParams.options.iCentroided2;

      iMSLevel = 2;
      iChargeMin = 1;//iChargeMin2;
      iChargeMax = 3;//iChargeMax2;
   }

   FILE *fp;
   if ((fp=fopen(szConf, "w"))==NULL)
   {
      fprintf(stderr, " Error - cannot read or write %s\n", szConf);
      exit(1);
   }
  
   fprintf(fp, "# Hardkor parameter file\n");
   fprintf(fp, "isotope_data = ISOTOPE.DAT\n");
   fprintf(fp, "hardklor_data = Hardklor.dat\n");
   fprintf(fp, "\n");
   fprintf(fp, "\n");
   fprintf(fp, "# Parameters used to described the data being input to Hardklor\n");
   fprintf(fp, "instrument = %s\n", szInstrument);
   fprintf(fp, "resolution = %d\n", iResolution);
   fprintf(fp, "centroided = %d\n", iCentroided);
   fprintf(fp, "\n");
   fprintf(fp, "# Parameters used in preprocessing spectra prior to analysis\n");
   fprintf(fp, "ms_level = %d\n", iMSLevel);
   fprintf(fp, "scan_range_min = 0\n");
   fprintf(fp, "scan_range_max = 0\n");
   fprintf(fp, "signal_to_noise = 0\n");
   fprintf(fp, "sn_window = 250.0\n");
   fprintf(fp, "static_sn = 0\n");
   fprintf(fp, "boxcar_averaging = 0\n");
   fprintf(fp, "boxcar_filter = 0\n");
   fprintf(fp, "\n");
   fprintf(fp, "boxcar_filter_ppm = 10\n");
   fprintf(fp, "mz_min = 0\n");
   fprintf(fp, "mz_max = 0\n");
   fprintf(fp, "smooth = 0\n");
   fprintf(fp, "\n");
   fprintf(fp, "# Parameters used to customize the Hardklor analysis.\n");
   fprintf(fp, "algorithm = Version2\n");
   fprintf(fp, "charge_algorithm = Quick\n");
   fprintf(fp, "\n");
   fprintf(fp, "charge_min = %d\n", iChargeMin);
   fprintf(fp, "charge_max = %d\n", iChargeMax);
   fprintf(fp, "correlation = 0.50\n");
   fprintf(fp, "averagine_mod = 0\n");
   fprintf(fp, "\n");
   fprintf(fp, "mz_window = 5.25\n");
   fprintf(fp, "sensitivity = 3\n");
   fprintf(fp, "\n");
   fprintf(fp, "depth = 2\n");
   fprintf(fp, "\n");
   fprintf(fp, "max_features = 10\n");
   fprintf(fp, "\n");
   fprintf(fp, "# Parameters used to customize the Hardklor output\n");
   fprintf(fp, "distribution_area = 0\n");
   fprintf(fp, "xml = 0\n");
   fprintf(fp, "\n");
   fprintf(fp, "%s.mzXML %s.hk%d\n", szBaseName, szBaseName, iMSLevel);

   fclose(fp);

   printf("\n");
   sprintf(szCmd, "hardklor %s", szConf);
   system(szCmd);
}


int MangoSearchManager::WithinTolerance(double dMass1,
                                        double dMass2,
                                        double dPPM)
{
   if (dMass1 < 600.0 || dMass1 > 6000.0)  //FIX ... need to match this to hash range
      return 0;
   if (dMass2 < 600.0 || dMass2 > 6000.0)
      return 0;

   if (     1E6 * fabs(dMass1              - dMass2)/dMass2 <= dPPM
         || 1E6 * fabs(dMass1 +   1.003355 - dMass2)/dMass2 <= dPPM 
         || 1E6 * fabs(dMass1 + 2*1.003355 - dMass2)/dMass2 <= dPPM 
         || 1E6 * fabs(dMass1 -   1.003355 - dMass2)/dMass2 <= dPPM 
         || 1E6 * fabs(dMass1 - 2*1.003355 - dMass2)/dMass2 <= dPPM)
      return 1;
   else
      return 0;
}
