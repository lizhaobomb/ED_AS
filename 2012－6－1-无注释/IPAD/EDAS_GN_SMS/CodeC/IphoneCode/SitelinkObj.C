// SitelinkObj.cpp: implementation of the CSitelinkObj class.
//
//////////////////////////////////////////////////////////////////////

//#include "sitemag.h"

#include "SitelinkObj.h"
#include "CommonDef.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSitelinkObj::CSitelinkObj(SITEMAG * psite,LOCALREVBLK * plocal)
{
	memcpy(&m_site,psite,SITEMAGLEN);
	m_plocal=plocal;
}

CSitelinkObj::~CSitelinkObj()
{
	
}
