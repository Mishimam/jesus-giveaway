#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	void consoleClear(void) {
	iprintf("\x1b[2J");
}
	
	gfxInitDefault();
	
	PrintConsole topScreen, bottomScreen;
	
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

	consoleSelect(&topScreen);
	printf("\x1b[15;11H\x1b[36mYou have received a free ps2!!!");
	printf("\x1b[17;11H\x1b[36mAnd a free Wii U!!!");
	
	printf("\x1b[24;10HPress A to bring up Mii selector\n");
	printf("\x1b[25;10HPress B to bring up Sex selector\n");
	
	printf("\x1b[27;14HPress X to see an monkey");
	printf("\x1b[28;9H(Warning: Viewer Discretion advised)");
	printf("\x1b[30;16H\x1b[36mPress Start to exit.");

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; 
		
		bool didit = false;
		static MiiSelectorConf msConf;
		static MiiSelectorReturn msRet;
		static char miiname[36];
		static char miiauthor[30];

		if (kDown & KEY_A)
		{
			consoleClear();
			didit = true;
			miiSelectorInit(&msConf);
			miiSelectorLaunch(&msConf, &msRet);
		}
		
		if (kDown & KEY_B)
		{
			didit = true;
			miiSelectorInit(&msConf);
			miiSelectorSetTitle(&msConf, "Sex Selector");
			miiSelectorSetOptions(&msConf, MIISELECTOR_CANCEL|MIISELECTOR_GUESTS|MIISELECTOR_TOP|MIISELECTOR_GUESTSTART);
			miiSelectorSetInitialIndex(&msConf, 1);
			miiSelectorBlacklistUserMii(&msConf, 0);
			miiSelectorLaunch(&msConf, &msRet);
		}
		
		if (kDown & KEY_X)
		{
			aptSetChainloader(0x0004001000022400LL, 0);
			break;
		}
		
		consoleSelect(&bottomScreen);
		if (didit)
		{
			if (miiSelectorChecksumIsValid(&msRet))
			{
				if (!msRet.no_mii_selected)
				{
					consoleClear();
					printf("A Mii was selected.\n");
					miiSelectorReturnGetName(&msRet, miiname, sizeof(miiname));
					miiSelectorReturnGetAuthor(&msRet, miiauthor, sizeof(miiauthor));
					printf("Name: %s\n", miiname);
					printf("Author: %s\n", miiauthor);
					printf("Birthday: Month-%d/Day-%d\n", msRet.mii.mii_details.bday_month, msRet.mii.mii_details.bday_day);
					printf("Sex: %d\n", msRet.mii.mii_details.sex);
					printf("Color: %d\n", msRet.mii.mii_details.shirt_color);
					printf("Favorite: %d\n", msRet.mii.mii_details.favorite);
					printf("Eyebrow: %d\n", msRet.mii.eyebrow_details.style);
					printf("Eyebrow color: %d\n", msRet.mii.eyebrow_details.color);
					printf("Nose: %d\n", msRet.mii.nose_details.style);
				} else
					printf("No Mii was selected.\n");
			} else
				printf("Return checksum invalid.\n");
		}

		gfxFlushBuffers();
		gfxSwapBuffers();

		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
