#include <StdAfx.h>
#include <MrMapi/MMNamedProps.h>
#include <MrMapi/mmcli.h>
#include <core/mapi/processor/mapiProcessor.h>
#include <core/mapi/extraPropTags.h>
#include <core/smartview/SmartView.h>
#include <core/utility/strings.h>
#include <core/utility/registry.h>
#include <core/utility/output.h>
#include <core/mapi/mapiFunctions.h>

void PrintGuid(LPGUID lpGuid)
{
	wprintf(
		L"%.8lX-%.4X-%.4X-%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X",
		lpGuid->Data1,
		lpGuid->Data2,
		lpGuid->Data3,
		lpGuid->Data4[0],
		lpGuid->Data4[1],
		lpGuid->Data4[2],
		lpGuid->Data4[3],
		lpGuid->Data4[4],
		lpGuid->Data4[5],
		lpGuid->Data4[6],
		lpGuid->Data4[7]);
}

void DoNamedProps(_In_opt_ LPMDB lpMDB)
{
	auto hRes = S_OK;
	wprintf(L"Dumping named properties...\n");

	ULONG cPropNames = 0;
	LPSPropTagArray pProps = nullptr;
	LPMAPINAMEID* pNames = nullptr;
	hRes = WC_MAPI(lpMDB->GetNamesFromIDs(&pProps, nullptr, 0, &cPropNames, &pNames));

	if (SUCCEEDED(hRes))
	{
		wprintf(L"\n");
		for (std::uint32_t i = 0; i < cPropNames; i++)
		{
			wprintf(L"[%08x] (", pProps->aulPropTag[i]);
			PrintGuid(pNames[i]->lpguid);
			switch (pNames[i]->ulKind)
			{
			case MNID_STRING:
				wprintf(L":%ws)\n", pNames[i]->Kind.lpwstrName);
				break;
			case MNID_ID:
				wprintf(L":0x%08x)\n", pNames[i]->Kind.lID);
				break;
			}
		}
	}
	else
	{
		wprintf(L"FAILED. Error 0x%x\n", hRes);
	}

	if (pProps) MAPIFreeBuffer(pProps);
	if (pNames) MAPIFreeBuffer(pNames);
}