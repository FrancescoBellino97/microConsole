/**
 ******************************************************************************
 * @file    cart.c
 * @author  Bellino Francesco
 * @brief   Cart source code.
 *
 ******************************************************************************
 */

#include <cart.h>

#if DEBUG==true
static const char *cart_lic_name();
static const char *cart_type_name();
static void print_cart_info();
#endif

typedef struct
{
    char filename[MAX_TITLE_LEN];
    u32 rom_size;
    u8 rom_data[ROM_SIZE];
    u8 ram_data[RAM_SIZE];
    rom_header *header;
} cart_context;

static cart_context cart_ctx;

#if DEBUG==true
static const char *ROM_TYPES[] =
{
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char *LIC_CODE[0xA5] =
{
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"
};
#endif


/**
  * @brief  Load the Cartridge in memory and print some informations
  * @param  cart:	Cartridge filename with path
  * @retval bool:	true if no error, false otherwise
  */
bool cart_load(char *cart)
{
	/* Save filename in Cart context variable */
    snprintf(cart_ctx.filename, sizeof(cart_ctx.filename), "%s", cart);

    /* Open Cartridge file */
    FILE *fp = fopen(cart, "r");

    if (!fp)
    {
#if DEBUG==true
        printf("Failed to open: %s\n", cart);
#endif
        return false;
    }

#if DEBUG==true
    printf("Opened: %s\n", cart_ctx.filename);
#endif

    /* Go to EOF to get the Cartridge size */
    fseek(fp, 0, SEEK_END);
    cart_ctx.rom_size = ftell(fp);

    /* Restart File Pointer */
    rewind(fp);

    /* Load ROM bank 0 and Bank 1 data from Cartrdige to RAM*/
    fread(cart_ctx.rom_data, END_ROM_BANK1, 1, fp);

    /* Skip VRAM data*/
    fread(NULL, END_VRAM-START_VRAM, 1, fp);

    /* Load External RAM data from Cartrdige to RAM */
    fread(NULL, END_EXT_RAM-START_EXT_RAM, 1, fp);

    /* Close Cartridge file */
    fclose(fp);

    /* Skip to 0x100 (start of Cartridge Header) */
    cart_ctx.header = (rom_header *)(cart_ctx.rom_data + 0x100);
    cart_ctx.header->title[15] = 0;

#if DEBUG==true
    print_cart_info();
#endif

    /* Checksum algorithm */
    u16 x = 0;
    for (u16 i=0x0134; i<=0x014C; i++)
    {
        x = x - cart_ctx.rom_data[i] - 1;
    }

#if DEBUG==true
    printf("\t Checksum : %2.2X (%s)\n", cart_ctx.header->checksum, (x & 0xFF) ? "PASSED" : "FAILED");
#endif

    return true;
}

/**
  * @brief  Read data from Cartridge at a specific address
  * @param  address:	address to read
  * @retval uint8_t:	data read
  */
u8 cart_read(u16 address)
{
	if(address < END_ROM_BANK1)
	{
		/* Read and return the address */
		return cart_ctx.rom_data[address];
	}
	/* Read and return the address */
	return cart_ctx.rom_data[address - START_EXT_RAM];
}

/**
  * @brief  Write data from Cartridge at a specific address (NOT IMPLEMENTED)
  * @param  address:	address to write
  * @retval None
  */
void cart_write(u16 address, u8 value)
{
    /* Write the address (NOT IMPLEMENTED) */
    NO_IMPL
}

#if DEBUG == true
/**
  * @brief  Get the license name from the new license code
  * @param  None
  * @retval char*:	license name
  */
static const char *cart_lic_name()
{
	/* Convert the Code read to the New License Code */
    if (cart_ctx.header->new_lic_code <= 0xA4) {
        return LIC_CODE[cart_ctx.header->lic_code];
    }

    /* Code read unknown */
    return "UNKNOWN";
}
#endif

#if DEBUG == true
/**
  * @brief  Get the cart type
  * @param  None
  * @retval char*:	cart type
  */
static const char *cart_type_name()
{
	/* Convert the type read to the Cartridge Type */
    if (cart_ctx.header->type <= 0x22)
    {
        return ROM_TYPES[cart_ctx.header->type];
    }

    /* Code read unknown */
    return "UNKNOWN";
}
#endif

#if DEBUG==true
/**
  * @brief  Print informations about the Cartridge loaded
  * @param  None
  * @retval None
  */
static void print_cart_info()
{
	printf("Cartridge Loaded:\n");
	printf("\t Title    : %s\n", cart_ctx.header->title);
	printf("\t Type     : %2.2X (%s)\n", cart_ctx.header->type, cart_type_name());
	printf("\t ROM Size : %d KB\n", 32 << cart_ctx.header->rom_size);
	printf("\t RAM Size : %2.2X\n", cart_ctx.header->ram_size);
	printf("\t LIC Code : %2.2X (%s)\n", cart_ctx.header->lic_code, cart_lic_name());
	printf("\t ROM Vers : %2.2X\n", cart_ctx.header->version);
}
#endif
