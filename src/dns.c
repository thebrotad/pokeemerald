#include "global.h"
#include "main.h"
#include "dns.h"
#include "palette.h"
#include "start_menu.h"
#include "overworld.h"
#include "battle_main.h"
#include "rtc.h"
#include "constants/map_types.h"
#include "constants/rgb.h"

#define DAWN_START_HOUR         6
#define DAY_START_HOUR          7
#define DUSK_START_HOUR         17
#define NIGHT_START_HOUR        18
#define LIGHTNING_START_HOUR    NIGHT_START_HOUR
#define LIGHTNING_END_HOUR      DAWN_START_HOUR

const struct LightingColour gLightingColours[] =
{
    {
        .paletteNum = 6,
        .colourNum = 10,
        .lightColour = RGB2(30, 30, 5),
    },
    {
        .paletteNum = 6,
        .colourNum = 9,
        .lightColour = RGB2(26, 25, 4),
    },
    {
        .paletteNum = 8,
        .colourNum = 10,
        .lightColour = RGB2(30, 30, 5),
    },
    {
        .paletteNum = 8,
        .colourNum = 9,
        .lightColour = RGB2(26, 25, 4),
    },
    {
        .paletteNum = 9,
        .colourNum = 10,
        .lightColour = RGB2(30, 30, 5),
    },
    {
        .paletteNum = 9,
        .colourNum = 9,
        .lightColour = RGB2(26, 25, 4),
    },
};

const u8 gDnsMapExceptions[] =
{
    MAP_TYPE_NONE,
    MAP_TYPE_INDOOR,
    MAP_TYPE_UNDERGROUND,
    MAP_TYPE_SECRET_BASE,
};

const struct DnsPalExceptions gOWPalExceptions = 
{
    .pal = {
        TRUE,     //0
        TRUE,     //1
        TRUE,     //2
        TRUE,     //3
        TRUE,     //4
        TRUE,     //5
        TRUE,     //6
        TRUE,     //7
        TRUE,     //8
        TRUE,     //9
        TRUE,     //10
        TRUE,     //11
        TRUE,     //12
        FALSE,    //13
        FALSE,    //14
        FALSE,    //15
        TRUE,     //16
        TRUE,     //17
        TRUE,     //18
        TRUE,     //19
        TRUE,     //20
        TRUE,     //21
        TRUE,     //22
        TRUE,     //23
        TRUE,     //24
        TRUE,     //25
        TRUE,     //26
        TRUE,     //27
        TRUE,     //28
        TRUE,     //29
        TRUE,     //30
        TRUE,     //31
    }
};

const struct DnsPalExceptions gCombatPalExceptions =  
{
    .pal = {
        FALSE,    //0
        FALSE,    //1
        TRUE,     //2
        TRUE,     //3
        TRUE,     //4
        FALSE,    //5
        TRUE,     //6
        TRUE,     //7
        TRUE,     //8
        TRUE,     //9
        FALSE,    //10
        TRUE,     //11
        TRUE,     //12
        TRUE,     //13
        TRUE,     //14
        TRUE,     //15
        FALSE,    //16
        FALSE,    //17
        FALSE,    //18
        FALSE,    //19
        FALSE,    //20
        FALSE,    //21
        FALSE,    //22
        FALSE,    //23
        FALSE,    //24
        FALSE,    //25
        FALSE,    //26
        FALSE,    //27
        FALSE,    //28
        FALSE,    //29
        FALSE,    //30
        FALSE,    //31
    }
};

const u16 gDayFilter = RGB2(0, 0, 0);
const u16 gNightFilter = RGB2(14, 14, 5);

const u16 gDawnFilters[] =
{
    RGB2(13, 13, 4),
    RGB2(13, 13, 4),
    RGB2(12, 13, 4),
    RGB2(12, 13, 4),
    RGB2(11, 12, 4),
    RGB2(10, 12, 4),
    RGB2(9, 12, 4),
    RGB2(8, 11, 4),
    RGB2(7, 11, 3),
    RGB2(6, 11, 3),
    RGB2(5, 10, 3),
    RGB2(4, 10, 3),
    RGB2(3, 9, 3),
    RGB2(2, 9, 3),
    RGB2(1, 8, 3),
    RGB2(0, 8, 3),
    RGB2(0, 7, 2),
    RGB2(0, 7, 2),
    RGB2(0, 6, 2),
    RGB2(0, 6, 2),
    RGB2(0, 5, 2),
    RGB2(0, 5, 2),
    RGB2(0, 4, 2),
    RGB2(0, 4, 1),
    RGB2(0, 3, 1),
    RGB2(0, 3, 1),
    RGB2(0, 2, 1),
    RGB2(0, 2, 1),
    RGB2(0, 1, 1),
    RGB2(0, 1, 1),
};

const u16 gSunsetFilters[] = 
{
    RGB2(0, 1, 1),
    RGB2(0, 1, 1),
    RGB2(0, 2, 1),
    RGB2(0, 2, 1),
    RGB2(0, 3, 1),
    RGB2(0, 3, 1),
    RGB2(0, 4, 1),
    RGB2(0, 4, 1),
    RGB2(0, 5, 2),
    RGB2(0, 5, 2),
    RGB2(0, 6, 2),
    RGB2(0, 6, 2),
    RGB2(0, 7, 2),
    RGB2(0, 7, 2),
    RGB2(0, 8, 2),
    RGB2(1, 8, 3),
    RGB2(2, 9, 3),
    RGB2(3, 9, 3),
    RGB2(4, 10, 3),
    RGB2(5, 10, 3),
    RGB2(6, 10, 3),
    RGB2(7, 11, 3),
    RGB2(8, 11, 4),
    RGB2(9, 11, 4),
    RGB2(10, 12, 4),
    RGB2(11, 12, 4),
    RGB2(12, 12, 4),
    RGB2(12, 13, 4),
    RGB2(13, 13, 4),
    RGB2(13, 13, 4),
};

const u16 gPaletteTagExceptions[] =
{
    0xD6FF, //TAG_HEALTHBOX_PAL
    0xD704, //TAG_HEALTHBAR_PAL
    0xD710, //TAG_STATUS_SUMMARY_BAR_PAL
    0xD712, //TAG_STATUS_SUMMARY_BALLS_PAL
};


static u16 DnsApplyFilterToColour(u16 colour, u16 filter);
static u16 DnsApplyProportionalFilterToColour(u16 colour, u16 filter);
static void DoDnsLightning();
static u16 GetDNSFilter();
static bool8 IsMapDNSException();
static bool8 IsSpritePaletteTagDnsException(u8 palNum);
static bool8 IsLightActive();

ALIGNED(4) EWRAM_DATA static u16 sDnsPaletteDmaBuffer[512] = {0};


void DnsTransferPlttBuffer(void *src, void *dest)
{
    if ((gMain.callback2 == CB2_Overworld || gMain.callback2 == CB2_OverworldBasic || gMain.callback2 == BattleMainCB2)
		&& !IsMapDNSException()) 
    {
        DmaCopy16(3, sDnsPaletteDmaBuffer, dest, PLTT_SIZE);
    }
    else
    {
        DmaCopy16(3, src, dest, PLTT_SIZE);
    }
}

void DnsApplyFilters()
{
    u8 palNum, colNum;
    u16 colour, rgbFilter;
    struct DnsPalExceptions palExceptionFlags;

    rgbFilter = GetDNSFilter();

    palExceptionFlags = gMain.inBattle ? gCombatPalExceptions : gOWPalExceptions;

    for (palNum = 0; palNum < 32; palNum++)
        if (palExceptionFlags.pal[palNum] && (palNum < 15 || !IsSpritePaletteTagDnsException(palNum - 16)))
            for (colNum = 0; colNum < 16; colNum++)
                sDnsPaletteDmaBuffer[palNum * 16 + colNum] = DnsApplyProportionalFilterToColour(gPlttBufferFaded[palNum * 16 + colNum], rgbFilter);
        else
            for (colNum = 0; colNum < 16; colNum++)
                sDnsPaletteDmaBuffer[palNum * 16 + colNum] = gPlttBufferFaded[palNum * 16 + colNum];      

    if (!IsMapDNSException() && (gLocalTime.hours >= LIGHTNING_START_HOUR || gLocalTime.hours < LIGHTNING_END_HOUR) && !gMain.inBattle)
        DoDnsLightning();
}

static u16 DnsApplyFilterToColour(u16 colour, u16 filter)
{
    u16 red, green, blue;

    red = (colour & 0x1F) - (filter & 0x1F);
    green = ((colour & 0x3E0) - (filter & 0x3E0)) >> 5;
    blue = ((colour & 0x7C00) - (filter & 0x7C00)) >> 10;

    return RGB2(red <= 31 ? red : 0, green <= 31 ? green : 0, blue <= 31 ? blue : 0);
}

static u16 DnsApplyProportionalFilterToColour(u16 colour, u16 filter)
{
    u32 red, green, blue;

    red = (colour & 0x1F) * (0x1F - (filter & 0x1F)) >> 5;
    green = ((colour & 0x3E0) >> 5) * ((0x3E0 - (filter & 0x3E0)) >> 5) >> 5;
    blue = ((colour & 0x7C00) >> 10) * ((0x7C00 - (filter & 0x7C00)) >> 10) >> 5;

    return RGB2(red <= 31 ? red : 0, green <= 31 ? green : 0, blue <= 31 ? blue : 0);  
}

static u16 GetDNSFilter()
{
    u8 hour = gLocalTime.hours;    
    u8 minutes = gLocalTime.minutes;   

	if (hour >= NIGHT_START_HOUR)
		return gNightFilter;
	else if (hour >= DUSK_START_HOUR)
		return gSunsetFilters[minutes >> 1];
	else if (hour >= DAY_START_HOUR)
		return gDayFilter;
	else if (hour >= DAWN_START_HOUR)
		return gDawnFilters[minutes >> 1];

    return gNightFilter;
}

static void DoDnsLightning()
{
    u8 i;

    for (i = 0; i < sizeof(gLightingColours)/sizeof(gLightingColours[0]); i++)
    {
        u16 colourSlot = gLightingColours[i].paletteNum * 16 + gLightingColours[i].colourNum;
		sDnsPaletteDmaBuffer[colourSlot] = gLightingColours[i].lightColour;
    }
}

static bool8 IsMapDNSException()
{
    u8 i;
    for (i=0; i < sizeof(gDnsMapExceptions)/sizeof(gDnsMapExceptions[0]); i++)
        if (gMapHeader.mapType == gDnsMapExceptions[i])
            return TRUE;
    return FALSE;
}

static bool8 IsSpritePaletteTagDnsException(u8 palNum)
{
    u8 i;

    for (i = 0; i < sizeof(gPaletteTagExceptions)/sizeof(gPaletteTagExceptions[0]); i++)
        if (GetSpritePaletteTagByPaletteNum(palNum) == gPaletteTagExceptions[i])
            return TRUE;
    return FALSE;
}