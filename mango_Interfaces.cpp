#include "mango_Interfaces.h"
#include "mango_ProcessManager.h"

using namespace MangoInterfaces;

MangoSearchManager *g_pMangoSearchManager = NULL;

IMangoSearchManager *MangoInterfaces::GetMangoSearchManager()
{
   if (NULL == g_pMangoSearchManager)
   {
      g_pMangoSearchManager = new MangoSearchManager();
   }

   IMangoSearchManager *pMangoSearchMgr = static_cast<IMangoSearchManager*>(g_pMangoSearchManager);
   return pMangoSearchMgr;
}

void MangoInterfaces::ReleaseMangoSearchManager()
{
   if (NULL != g_pMangoSearchManager)
   {
      delete g_pMangoSearchManager;
      g_pMangoSearchManager = NULL;
   }
}


