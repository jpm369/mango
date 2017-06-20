#include <string>

#ifndef _MANGODATA_H_
#define _MANGODATA_H_

#define SIZE_BUF                    8192
#define SIZE_FILE                   512

struct DoubleRange
{
   double dStart;
   double dEnd;

   DoubleRange()
   {
      dStart = 0.0;
      dEnd = 0.0;
   }

   DoubleRange(const DoubleRange& a)
   {
      dStart = a.dStart;
      dEnd = a.dEnd;
   }

   DoubleRange(double dStart_in, double dEnd_in)
   {
      dStart = dStart_in;
      dEnd = dEnd_in;
   }

   DoubleRange& operator=(DoubleRange& a)
   {
      dStart = a.dStart;
      dEnd = a.dEnd;
      return *this;
   }
};

struct IntRange
{
   int iStart;
   int iEnd;

   IntRange()
   {
      iStart = 0;
      iEnd = 0;
   }

   IntRange(const IntRange& a)
   {
      iStart = a.iStart;
      iEnd = a.iEnd;
   }

   IntRange(int iStart_in, int iEnd_in)
   {
      iStart = iStart_in;
      iEnd = iEnd_in;
   }

   IntRange& operator=(IntRange& a)
   {
      iStart = a.iStart;
      iEnd = a.iEnd;
      return *this;
   }
};


struct InputFileInfo
{
   int  iInputType;
   int  iFirstScan;
   int  iLastScan;
   char szFileName[SIZE_FILE];
   char szBaseName[SIZE_FILE];

   InputFileInfo()
   {
      iInputType = 0;
      iFirstScan = 0;
      iLastScan = 0;

      szFileName[0] = '\0';
      szBaseName[0] = '\0';
   }

   InputFileInfo(const InputFileInfo& inputObj)
   {
      iInputType = inputObj.iInputType;
      iFirstScan = inputObj.iFirstScan;
      iLastScan = inputObj.iLastScan;

      szBaseName[0] = '\0';
      strcpy(szBaseName, inputObj.szBaseName);

      szFileName[0] = '\0';
      strcpy(szFileName, inputObj.szFileName);
   }

   InputFileInfo(char *pszFileName)
   {
      iInputType = 0;
      iFirstScan = 0;
      iLastScan = 0;

      szBaseName[0] = '\0';

      pszFileName[0] = '\0';
      strcpy(szFileName, pszFileName);
   }

   InputFileInfo& operator = (InputFileInfo &inputObj)
   {
      iInputType = inputObj.iInputType;
      iFirstScan = inputObj.iFirstScan;
      iLastScan = inputObj.iLastScan;

      szBaseName[0] = '\0';
      strcpy(szBaseName, inputObj.szBaseName);

      szFileName[0] = '\0';
      strcpy(szFileName, inputObj.szFileName);
      return *this;
   }
};

enum MangoParamType
{
   MangoParamType_Unknown = 0,
   MangoParamType_Int,
   MangoParamType_Double,
   MangoParamType_String,
   MangoParamType_VarMods,
   MangoParamType_DoubleRange,
   MangoParamType_IntRange,
   MangoParamType_EnzymeInfo,
   MangoParamType_DoubleVector
};


// A virtual class that provides a generic data structure to store any Mango
// parameter so that we can store all parameters in one data container
// (e.g. std::map). The specific type of parameter will use the TypedMangoParam
// class which inherits from this class and specifies _paramType and
// _strValue, a string representation of the value of the param

class MangoParam
{
public:
   MangoParam(MangoParamType paramType, const string& strValue)
      : _paramType(paramType), _strValue(strValue) {}
   virtual ~MangoParam() {}
   string& GetStringValue() { return _strValue; }
private:
   MangoParamType _paramType;
   string _strValue;
};


// A templated class to store Mango parameters of any type, specifying the type
// T upon creation. It inherits from MangoParam so after creation, an object of
// this class type can be stored as a MangoParam and cast back to
// TypedMangoParam to access the GetValue() method when needed.

template< typename T >
class TypedMangoParam : public MangoParam
{
public:
   TypedMangoParam (MangoParamType paramType, const string& strValue, const T& value)
      : MangoParam(paramType, strValue), _value(value) {}

   T& GetValue() { return _value; }

private:
   T _value;
};

#endif // _MANGODATA_H_
