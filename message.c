#include "message.h"

#include <stdio.h>
#include <string.h>
#include <limits.h>

void parse_block(unsigned short block) {
    char qr = block >> (CHAR_BIT + 7);
    char opcode = (block >> (CHAR_BIT + 3)) & 0xF;
    char aa = (block >> (CHAR_BIT + 2)) & 0x1;
    char tc = (block >> (CHAR_BIT + 1)) & 0x1;
    char rd = (block >> (CHAR_BIT)) & 0x1;
    char ra = (block >> (CHAR_BIT - 1)) & 0x1;
    char ad = (block >> 5) & 0x1;
    char cd = (block >> 4) & 0x1;
    char rcode = block & 0xF;

    printf("\tQR: %d\tOPCODE: %d\tAA: %d", qr, opcode, aa);
    printf("\tTC: %d\tRD: %d\tRA: %d", tc, rd, ra);
    printf("\tAD: %d\t CD: %d\tRCODE: %d\n", ad, cd, rcode);
}

unsigned short parse_bytes(char *data)  {
    return (((unsigned short)data[0]) << CHAR_BIT) + data[1];
}

void parse_message(char *data, int length) {
    unsigned short id, block, qdcount, ancount, nscount, arcount;
    id = parse_bytes(data);
    block = parse_bytes(data + 2);
    qdcount = parse_bytes(data + 4);
    ancount = parse_bytes(data + 6);
    nscount = parse_bytes(data + 8);
    arcount = parse_bytes(data + 10);

    printf("\n\t== HEADER ==\n");
    printf("\tID: %d\n", id);

    parse_block(block);

    printf("\tQDCOUNT: %d\n", qdcount);
    printf("\tANCOUNT: %d\n", ancount);
    printf("\tNSCOUNT: %d\n", nscount);
    printf("\tARCOUNT: %d\n", arcount);

    // parse question section
    
    // for qname, start at the 13th byte, get length of part, and pull it in
    // keep pulling in parts until NUL byte
    printf("\t== QUESTION ==\n");
    int offset = 12;
    printf("\tQNAME: ");

    unsigned char part_length;
    while ((part_length = (unsigned char)data[offset++]) > 0) {
        char str[part_length + 1];
        // 2. strncpy from offset and take part_length +1 bytes
        strncpy(str, data + offset, part_length + 1);
        str[part_length] = '\0';
        // 3. print out that part
        printf("%s.", str);
        // 4. advance offset by part_length
        offset += part_length;
    }
    printf("\n");

    printf("\tQTYPE: %d\n", parse_bytes(data + offset));
    offset += 2;
    printf("\tQCLASS: %d\n", parse_bytes(data + offset));
    offset += 2;
}


