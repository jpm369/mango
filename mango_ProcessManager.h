#ifndef _MANGOPROCESSMANAGER_H_
#define _MANGOPROCESSMANAGER_H_

#include "mango_Data.h"
#include "mango_Interfaces.h"

using namespace MangoInterfaces;

class MangoSearchManager : public IMangoSearchManager
{
public:
   MangoSearchManager();
   ~MangoSearchManager();

   std::map<std::string, MangoParam*>& GetParamsMap();

   // Methods inherited from IMangoSearchManager
   virtual bool DoAnalysis();
   virtual void AddInputFiles(vector<InputFileInfo*> &pvInputFiles);
   virtual void SetOutputFileBaseName(const char *pszBaseName);
   virtual void SetParam(const string &name, const string &strValue, const string &value);
   virtual bool GetParamValue(const string &name, string &value);
   virtual void SetParam(const string &name, const string &strValue, const int &value);
   virtual bool GetParamValue(const string &name, int &value);
   virtual void SetParam(const string &name, const string &strValue, const double &value);
   virtual bool GetParamValue(const string &name, double &value);
   virtual void SetParam(const string &name, const string &strValue, const DoubleRange &value);
   virtual bool GetParamValue(const string &name, DoubleRange &value);
   virtual void SetParam(const string &name, const string &strValue, const IntRange &value);
   virtual bool GetParamValue(const string &name, IntRange &value);
   virtual void SetParam(const string &name, const string &strValue, const vector<double> &value);
   virtual bool GetParamValue(const string &name, vector<double> &value);
   virtual bool IsValidMangoVersion(const string &version);


private:
   bool InitializeStaticParams();

   std::map<std::string, MangoParam*> _mapStaticParams;

   void READ_MZXMLSCANS(char *szMZXML);
   void READ_HK1(char *szHK);
   void READ_HK2(char *szHK);
   void GENERATE_HK(char *szHK);
   int WithinTolerance(double dMass1,
                       double dMass2,
                       double dPPM);
};

#endif
