//#include "open62541.h"
#include <open62541/plugin/log_stdout.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>

xmlNodePtr srSeekChildNodeNamed(xmlNode* p, char* name, int len);
UA_ByteString loadFile(char* filename);
char* itoa(int num, char* str, int base);

xmlNodePtr srSeekChildNodeNamed(xmlNode* p, char* name, int len)
{
	xmlNodePtr curr_node;

	if (p == NULL || name == NULL)
		return NULL;
	for (curr_node = p; curr_node; curr_node = curr_node->next)
	{
		printf("In for loop : curr_node content : %s\n", (char*)xmlNodeGetContent(curr_node));

		printf("In the loop : (Len=%d) Comparing <curr_node->name> with <name> | %s : %s \n", len, curr_node->name, name);
		if (strncmp(curr_node->name, name, len) == 0)	// look for xmlNode with tag = <name>
		{
			// found seatch string <name>
			printf("----------------Found In srSeekChildNodeNamed() [line 195] : searching for %s, found %s; returning the xmlNodePtr\n", name, curr_node->name);
			return curr_node;
		}
	}
}

/*
UA_ByteString loadFile(char* filename)
{
	int filesize = 0;
	UA_Byte* filecontent;
	FILE* fp;

	fp = fopen(filename, "r");
	fseek(fp, 0L, SEEK_END);
	filesize = ftell(fp) + 1;
	rewind(fp);

	filecontent = (UA_Byte*)calloc(filesize, sizeof(char));
	fread(filecontent, sizeof(char), filesize, fp);

	UA_ByteString output;
	output.length = filesize;
	output.data = filecontent;

	return output;

}
*/

// Implementation of itoa()

char* itoa(int num, char* buffer, int base) {
    int curr = 0;

    if (num == 0) {
        // Base case
        buffer[curr++] = '0';
        buffer[curr] = '\0';
        return buffer;
    }

    int num_digits = 0;

    if (num < 0) {
        if (base == 10) {
            num_digits ++;
            buffer[curr] = '-';
            curr ++;
            // Make it positive and finally add the minus sign
            num *= -1;
        }
        else
            // Unsupported base. Return NULL
            return NULL;
    }

    num_digits += (int)floor(log(num) / log(base)) + 1;

    // Go through the digits one by one
    // from left to right
    while (curr < num_digits) {
        // Get the base value. For example, 10^2 = 1000, for the third digit
        int base_val = (int) pow(base, num_digits-1-curr);

        // Get the numerical value
        int num_val = num / base_val;

        char value = num_val + '0';
        buffer[curr] = value;

        curr ++;
        num -= base_val * num_val;
    }
    buffer[curr] = '\0';
    return buffer;
}
