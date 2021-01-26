#include "Common.h"
#include "mango.h"
#include "mango_Process.h"
#include "mango_DataInternal.h"

// Generate data for both sp scoring (pfSpScoreData) and xcorr analysis (FastXcorr).
mango_Process::mango_Process()
{
}

mango_Process::~mango_Process()
{
}


void mango_Process::ProcessData(char *szMZXML)
{
   int i;
   int ii;

   MSReader mstReader;
   Spectrum mstSpectrum;
   // We want to read only MS2 scans.
   vector<MSSpectrumType> msLevel;
   msLevel.push_back(MS2);

   mstReader.setFilter(msLevel);
   mstReader.readFile(szMZXML, mstSpectrum, 1);

   FILE *fpPeaks;
   FILE *fpout;
   char szOutput[1024];
   char szBaseName[1024];

   strcpy(szBaseName, szMZXML);
   if (!strcmp(szBaseName+strlen(szBaseName)-6, ".mzXML"))
      szBaseName[strlen(szBaseName)-6]='\0';
   else if (!strcmp(szBaseName+strlen(szBaseName)-5, ".mzML"))
      szBaseName[strlen(szBaseName)-5]='\0';

   if (1)
   {
      sprintf(szOutput, "%s.peaks", szBaseName);
      if ((fpPeaks=fopen(szOutput, "w")) == NULL)
      {
         printf(" Error - cannot write output %s\n", szOutput);
         return;
      }

      fprintf(fpPeaks, "scan\tintact_mass\tintact_charge\tpep1_mass\tpep1_charge\tpep2_mass\tpep2_charge\n");
      for (i=0; i<(int)pvSpectrumList.size(); i++)
      {
         for (ii=0; ii<(int)pvSpectrumList.at(i).pvdPrecursors.size(); ii++)
         {
            fprintf(fpPeaks, "%d\t", pvSpectrumList.at(i).iScanNumber);
            fprintf(fpPeaks, "%f\t", pvSpectrumList.at(i).dHardklorPrecursorNeutralMass);
            fprintf(fpPeaks, "%d\t", pvSpectrumList.at(i).iPrecursorCharge);
            fprintf(fpPeaks, "%f\t", pvSpectrumList.at(i).pvdPrecursors.at(ii).dNeutralMass1);
            fprintf(fpPeaks, "%d\t", pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge1);
            fprintf(fpPeaks, "%f\t", pvSpectrumList.at(i).pvdPrecursors.at(ii).dNeutralMass2);
            fprintf(fpPeaks, "%d\n", pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge2);
         }
      }

      fclose(fpPeaks);
      printf(" created:  %s\n", szOutput);
   }

   if (g_staticParams.options.iExportMGF)
   {
      sprintf(szOutput, "%s.mgf", szBaseName);
      if ((fpout=fopen(szOutput, "w"))==NULL)
      {
         printf(" Error - cannot write %s\n", szOutput);
         return;
      }

      fprintf(fpout, "CHARGE=2+ and 3+\n"); // should never need this

      for (i=0; i<(int)pvSpectrumList.size(); i++)
      {
         Spectrum mstSpectrum;           // For holding spectrum.
   
         // Loads in MSMS spectrum data.
         mstReader.readFile(NULL, mstSpectrum, pvSpectrumList.at(i).iScanNumber);
   
         if ((int)pvSpectrumList.at(i).pvdPrecursors.size() == 0)
            continue;
   
         int xx=0;

         for (int ii=0; ii<(int)pvSpectrumList.at(i).pvdPrecursors.size(); ii++)
         {
            double dMZ;

            dMZ = (pvSpectrumList.at(i).pvdPrecursors.at(ii).dNeutralMass1 + PROTON_MASS * pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge1)
               / pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge1;

            fprintf(fpout, "BEGIN IONS\n");
            fprintf(fpout, "PEPMASS=%f %f\n", dMZ, 1.0 );
            fprintf(fpout, "CHARGE=%d+\n", pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge1);
            fprintf(fpout, "SCANS=%d\n", pvSpectrumList.at(i).iScanNumber);
            fprintf(fpout, "TITLE=%s_%03d_A.%05d.%05d.%d\n", szBaseName, xx, pvSpectrumList.at(i).iScanNumber,
                  pvSpectrumList.at(i).iScanNumber, pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge1);
            PrintPeaks(mstSpectrum, fpout);
            fprintf(fpout, "END IONS\n");

            dMZ = (pvSpectrumList.at(i).pvdPrecursors.at(ii).dNeutralMass2 + PROTON_MASS * pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge2)
               / pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge2;

            fprintf(fpout, "BEGIN IONS\n");
            fprintf(fpout, "PEPMASS=%f %f\n", dMZ, 1.0);
            fprintf(fpout, "CHARGE=%d+\n", pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge2);
            fprintf(fpout, "SCANS=%d\n", pvSpectrumList.at(i).iScanNumber);
            fprintf(fpout, "TITLE=%s_%03d_B.%05d.%05d.%d\n", szBaseName, xx, pvSpectrumList.at(i).iScanNumber,
                  pvSpectrumList.at(i).iScanNumber, pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge2);
            PrintPeaks(mstSpectrum, fpout);
            fprintf(fpout, "END IONS\n");

            xx++;
         }
      }
   }
   else
   {
      sprintf(szOutput, "%s.ms2", szBaseName);
      if ((fpout=fopen(szOutput, "w"))==NULL)
      {
         printf(" Error - cannot write %s\n", szOutput);
         return;
      }

      for (i=0; i<(int)pvSpectrumList.size(); i++)
      {
         Spectrum mstSpectrum;           // For holding spectrum.
   
         // Loads in MSMS spectrum data.
         mstReader.readFile(NULL, mstSpectrum, pvSpectrumList.at(i).iScanNumber);
   
         if ((int)pvSpectrumList.at(i).pvdPrecursors.size() == 0)
            continue;
   
         fprintf(fpout, "S\t%d\t%d\t0.0\n", pvSpectrumList.at(i).iScanNumber, pvSpectrumList.at(i).iScanNumber);
         for (int ii=0; ii<(int)pvSpectrumList.at(i).pvdPrecursors.size(); ii++)
         {
            fprintf(fpout, "Z\t%d\t%f\n", pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge1, pvSpectrumList.at(i).pvdPrecursors.at(ii).dNeutralMass1 + PROTON_MASS);
            fprintf(fpout, "Z\t%d\t%f\n", pvSpectrumList.at(i).pvdPrecursors.at(ii).iCharge2, pvSpectrumList.at(i).pvdPrecursors.at(ii).dNeutralMass2 + PROTON_MASS);
         }

         PrintPeaks(mstSpectrum, fpout);
      }
   }

   printf(" created:  %s\n", szOutput);

   fclose(fpout);
}
   

void mango_Process::PrintPeaks(Spectrum mstSpectrum,
                               FILE *fpout)
{
   int ii= 0;
   double dIon,
          dIntensity;
   
   while(true)
   {
      if (ii >= mstSpectrum.size())
         break;
   
      dIon = mstSpectrum.at(ii).mz;
      dIntensity = mstSpectrum.at(ii).intensity;
      ii++;
   
      if (     !(dIon > 310.16-0.1 && dIon < 310.16+0.1)        // clear out contaminant peaks
            && !(dIon > 311.16-0.1 && dIon < 311.16+0.1)
            && !(dIon > 430.21-0.1 && dIon < 430.21+0.1)
            && !(dIon > 431.22-0.1 && dIon < 431.22+0.1)
            && !(dIon > 655.36-0.1 && dIon < 655.36+0.1)
            && !(dIon > 677.38-0.1 && dIon < 677.38+0.1)
            && !(dIon > g_staticParams.options.dReporterMass+PROTON_MASS-0.1    // also remove 752 reporter ion and isotopes
            &&   dIon < g_staticParams.options.dReporterMass+PROTON_MASS+0.1)
            && !(dIon > g_staticParams.options.dReporterMass+2*PROTON_MASS-0.1
            &&   dIon < g_staticParams.options.dReporterMass+2*PROTON_MASS+0.1)
            && !(dIon > g_staticParams.options.dReporterMass+3*PROTON_MASS-0.1
            &&   dIon < g_staticParams.options.dReporterMass+3*PROTON_MASS+0.1))
      {
         fprintf(fpout, "%0.4f %d\n", dIon, (int)dIntensity);
      }
   }
}
