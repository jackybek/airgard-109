#include "open62541.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>
#include "json_checker.h"
#include "json.h"               //https://raw.githubusercontent.com/udp/json-parser/master/json.h

xmlNodePtr srSeekChildNodeNamed(xmlNode* p, char* name, int len);
UA_ByteString loadFile(char* filename);
char* itoa(int num, char* str, int base);


// json library
static void print_depth_shift(int depth);
static void process_object(json_value *value, int depth);
static void process_array(json_value* value, int depth);
static void process_value(json_value* value, int depth);

static void print_depth_shift(int depth)
{
        int j;
        for (j=0; j < depth; j++) {
                printf(" ");
        }
}

static void process_object(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                print_depth_shift(depth);
                printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
                process_value(value->u.object.values[x].value, depth+1);
        }
}

static void process_array(json_value* value, int depth)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        printf("array\n");
        for (x = 0; x < length; x++) {
                process_value(value->u.array.values[x], depth);
        }
}

static void process_value(json_value* value, int depth)
{
        int j;
        if (value == NULL) {
                return;
        }
        if (value->type != json_object) {
                print_depth_shift(depth);
        }
        switch (value->type) {
                case json_none:
                        printf("none\n");
                        break;
                case json_object:
                        process_object(value, depth+1);
                        break;
                case json_array:
                        process_array(value, depth+1);
                        break;
                case json_integer:
                        printf("int: %10" PRId64 "\n", value->u.integer);
                        break;
                case json_double:
                        printf("double: %f\n", value->u.dbl);
                        break;
                case json_string:
                        printf("string: %s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        printf("bool: %d\n", value->u.boolean);
                        break;
        }
}
// end json libary


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













#ifdef XXX
static void
callback_new(UA_ByteString *encodedBuffer, UA_ByteString *topic)
{

     int i;

     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "SV_PubSub.c callback() : Message Received in MQTT Broker : %s ", (char*)topic->data);

     printf("==================================\n");
     printf("SV_PubSub: Start callback()=======\n");

     // printf("before processing: <%s>\n", (char *)encodedBuffer->data);
     // remove unwanted characters in reverse order
     for (i=strlen((char*)encodedBuffer->data); i>0; i--)
     {
        //printf("value of i is %d \n", i);
        if ((char)encodedBuffer->data[i] == '}')
                break;
     }
     encodedBuffer->data[i+1] ='\0';


     printf("after processing: \n%s\n", (char *)encodedBuffer->data);

     JSON_checker jc = new_JSON_checker(20);    // depth of tree = 20
     if (jc)
     {
        for (i=0; i < strlen((char*)encodedBuffer->data); i++)
        {
                int next_char = encodedBuffer->data[i];//getchar();
                if (next_char <= 0)
                        break;
                if (!JSON_checker_char(jc, next_char))
                {
                        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_PubSub.c : JSON_checker_char(): syntax error");
                        return;
                }
        }
     }
UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND,"After new_JSON_checker \n");

     if (jc)
     {
        if (!JSON_checker_done(jc))
        {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_PubSub.c : JSON_checker_done(): syntax error");
                return;
        }
     }
     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_PubSub.c : callback : JSON_Checker() success");
     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_PubSub.c : callback : Moving to section 2 : json_tokener");



     int stringlen = strlen((char*)encodedBuffer->data);
     UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_PubSub here 0 : encodedBuffer->data size is %d ", stringlen);
     stringlen++ ;

        json_char* json;
        json_value* value;

     if (stringlen > 0)      // jacky added 'if statement' on 3 Apr 2021
     {
        // convert UA_ByteString <encodedBuffer->data> to char*
        char* myCharString = calloc(stringlen+1, sizeof(char));
        strncpy(myCharString, (char*)encodedBuffer->data, stringlen-1);
        myCharString[stringlen]='\0';

        json = (json_char*)myCharString;
        value = json_parse(json,stringlen);
        if (value == NULL) {
                UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "SV_PubSub.c : Unable to parse data");
                free(myCharString);
                return;
        }

        process_value(value, 0);

        json_value_free(value);
        free(myCharString);
        return;
    }
}
#endif

