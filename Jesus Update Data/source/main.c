/*
	Hello World example made by Aurelio Mannara for libctru
	This code was modified for the last time on: 12/12/2014 21:00 UTC+1
*/

#include <3ds.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	void consoleClear(void) {
	iprintf("\x1b[2J");
}
	
	gfxInitDefault();
	
	//In this example we need one PrintConsole for each screen
	PrintConsole topScreen, bottomScreen;
	
	//Initialize console for both screen using the two different PrintConsole we have defined
	consoleInit(GFX_TOP, &topScreen);
	consoleInit(GFX_BOTTOM, &bottomScreen);

	//Move the cursor to row 15 and column 19 and then prints "Hello World!"
	//To move the cursor you have to print "\x1b[r;cH", where r and c are respectively
	//the row and column where you want your cursor to move
	//The top screen has 30 rows and 50 columns
	//The bottom screen has 30 rows and 40 columns
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
		//Scan all the inputs. This should be done once for each frame
		hidScanInput();

		//hidKeysDown returns information about which buttons have been just pressed (and they weren't in the previous frame)
		u32 kDown = hidKeysDown();

		if (kDown & KEY_START) break; // break in order to return to hbmenu
		
		bool didit = false;
		static MiiSelectorConf msConf;
		static MiiSelectorReturn msRet;
		static char miiname[36];
		static char miiauthor[30];

		if (kDown & KEY_A)
		{
			consoleClear();
			didit = true;
			// Ensure that the config is initalized and that all
			// defaults are set properly. (failing to do this to a new
			// MiiSelecorConf at least once before launching the miiSelector
			// can result in undefined behavior)
			miiSelectorInit(&msConf);
			miiSelectorLaunch(&msConf, &msRet);
		}
		
		if (kDown & KEY_B)
		{
			didit = true;
			miiSelectorInit(&msConf);
			// Sets title of Mii selector.
			miiSelectorSetTitle(&msConf, "Sex Selector");
			// Choose and option to enable. Any options not listed will be disabled.
			// 0 can be used to select no option.
			miiSelectorSetOptions(&msConf, MIISELECTOR_CANCEL|MIISELECTOR_GUESTS|MIISELECTOR_TOP|MIISELECTOR_GUESTSTART);
			// Start on the Mii with the database index of 1.
			// This is a guest Mii since the option MIISELECTOR_GUESTSTART is enabled.
			// If MIISELECTOR_GUESTSTART was not enabled, this would be a user Mii.
			miiSelectorSetInitialIndex(&msConf, 1);
			// Blacklist main user Mii
			miiSelectorBlacklistUserMii(&msConf, 0);
			miiSelectorLaunch(&msConf, &msRet);
		}
		
		if (kDown & KEY_X) // If the A button got pressed, start the app launch
		{
			aptSetChainloader(0x0004001000022400LL, 0); // *EUR* camera app title ID
			break;
		}
		
		consoleSelect(&bottomScreen);
		if (didit)
		{
			// Check that the data in the miiSelector return buffer
			// is correct.
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
					// Keep in mind that not all things align with their order in Mii Maker.
					// https://www.3dbrew.org/wiki/Mii#Mii_values
					printf("Eyebrow: %d\n", msRet.mii.eyebrow_details.style);
					printf("Eyebrow color: %d\n", msRet.mii.eyebrow_details.color);
					printf("Nose: %d\n", msRet.mii.nose_details.style);
				} else
					printf("No Mii was selected.\n");
			} else
				printf("Return checksum invalid.\n");
		}

		// Flush and swap framebuffers
		gfxFlushBuffers();
		gfxSwapBuffers();

		//Wait for VBlank
		gspWaitForVBlank();
	}

	gfxExit();
	return 0;
}
