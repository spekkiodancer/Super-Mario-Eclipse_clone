#include <BetterSMS/module.hxx>

// READY GO TEXT PATCH FOR THIS BULLSHIT THING DADDY NINTENDO DID
SMS_WRITE_32(SMS_PORT_REGION(0x80171C30, 0x80167A40, 0, 0), 0x2C000005);
SMS_WRITE_32(SMS_PORT_REGION(0x80171C38, 0x80167A48, 0, 0), 0x38000005);

// Title Screen Never Fades to THP
// SMS_WRITE_32(SMS_PORT_REGION(0x8016D53C, 0x801628AC, 0, 0), 0x48000344);

// Flood Till Corona Beat
SMS_WRITE_32(SMS_PORT_REGION(0x8029961C, 0x802914b4, 0, 0), 0x38840077);

// Remove DEBS alerts
// SMS_WRITE_32(SMS_PORT_REGION(0x80142998, 0, 0, 0), 0x48000078);