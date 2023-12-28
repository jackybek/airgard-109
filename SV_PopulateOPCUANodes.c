#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <netdb.h>
//#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>	// inet_addr
#include <unistd.h>
#include <pthread.h>

#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>

#define MAX_BUFFER_SIZE 20000   // max number of bytes we can get at once
#define PORT 20004

static int PopulateOPCUANodes_firstTime = 1;
static int sockfd;
static struct hostent* he;
static struct sockaddr_in airgard_addr_data;        /* connector's address information */

void PopulateOPCUANodes(char* g_argv_ip);
int ExtractXMLElementNames(xmlNode * a_node);

void PopulateOPCUANodes(char* g_argv_ip)    // g_argv_ip refers to the sensor IP
{
    int numbytes;
    char buffer[MAX_BUFFER_SIZE];
    //int sockfd;
    //struct hostent* he;
    //struct sockaddr_in airgard_addr_data;        /* connector's address information */

    // opening port 20004 for reading the streams from Airgard
    printf("============================================================\n");
    printf("Entering PopulateOPCUANodes(): g_argv_ip = %s \n", g_argv_ip);
    printf("firstTime is %d \n", PopulateOPCUANodes_firstTime);
    printf("============================================================\n");

    if (PopulateOPCUANodes_firstTime==1)
    {
    	if ((he = gethostbyname(g_argv_ip)) == NULL)
    	{
        	//herror("gethostbyname");
	        exit(-1);
    	}

    	printf("NA_PopulateOPCUANodes.c : before sockfd = socket(AF_INET, SOCK_STREAM, 0)\n");
    	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    	{
        	perror("socket : 20004 creation failed");
        	exit(-1);
    	}

    	//airgard_addr_data.sin_addr = *((struct in_addr *) he-> h_addr);
    	airgard_addr_data.sin_addr.s_addr = inet_addr(g_argv_ip);
    	airgard_addr_data.sin_port = htons(PORT);    /* short, network byte order */
    	airgard_addr_data.sin_family = AF_INET;      /* host byte order */
    	bzero(&(airgard_addr_data.sin_zero), 8);     /* zero the rest of the struct */

    	printf("NA_PopulateOPCUANodes.c : Before connect(sockfd) \n");
    	//if (connect(sockfd, (struct sockaddr *)&airgard_addr_data, sizeof(struct sockaddr)) < 0)
    	if (connect(sockfd, (struct sockaddr*)&airgard_addr_data, sizeof(airgard_addr_data)) < 0) 
    	{
        	perror("connection 20004 failed");
        	exit(-1);
    	}
    	// end opening port 20004 for incoming streams
   }
   PopulateOPCUANodes_firstTime = 0;
   //while (1)
    //{
        xmlTextReaderPtr xml_TextReaderPtr, xmlPostPtr;
        xmlDocPtr myxmlDocPtr;
        xmlNodePtr myxmlNodePtr;

        xmlDoc* myxmlDoc = NULL;
        xmlNode* root_element = NULL;
        //int firstTime = 0;

        char ConvertToXmlBuffer[MAX_BUFFER_SIZE];
        int NoOfAttributes;

        xmlChar* MeasurementTime;

	printf("NA_PopulateOPCUANodes.c : Before recv() function \n");
        if ((numbytes = recv(sockfd, buffer, MAX_BUFFER_SIZE, 0)) == -1)
        {
            perror("recv");
            exit(-1);
        }
        buffer[numbytes] = '\0';

        // step 0 : Add XML header
        sprintf(ConvertToXmlBuffer, "%s\n%s\n\n",
            "<?xml version=\"1.0\" standalone=\"no\"?>",
            "<!DOCTYPE AirGard SYSTEM \"AirGardSubjects.dtd\">");
        strcat(ConvertToXmlBuffer, buffer);

        //printf("Received in pid=%d, stream=\n\n%s\n\n", getpid(), ConvertToXmlBuffer);
        //else
        //    printf("Iterations number : %d\n", ++counter);


        //---------------------------
        //Documentation can be found at http://www.xmlsoft.org/html/libxml-xmlreader.html#xmlReaderForMemory

	// initialisation function for the XML Parser.  Call once befre processing in case of use in multi-threaded program
        //	printf("here : 11111111\n");
	xmlInitParser();
        // initialisation function for the Parser Context
        //	printf("here : 22222222\n");

	xmlParserCtxtPtr myxmlParserCtxtPtr;			// crash here!!!
	/*
        if (xmlInitParserCtxt(myxmlParserCtxtPtr) == -1)
        {
            printf("Error : xmlInitParserCtxt\n");
            //exit(-1);
        }
	*/
        // create a parse context for an XML in-memory document
        //printf("here : 33333333\n"); 
	myxmlParserCtxtPtr = xmlCreateDocParserCtxt(ConvertToXmlBuffer);
        if (myxmlParserCtxtPtr == NULL)
        {
            printf("Error : xmlCreateDocParseCtxt\n");
            // exit(-1);
        }
        printf("successfully parse the context (AirGardSubjects.dtd)\n");

	/*
        // alternate step 0 : parse an XML in-memory document and build a tree.  This reuses the existing @ctxt parser context
        xmlDocPtr myxmlCtxtReadDoc = xmlCtxtReadDoc(myxmlParserCtxtPtr,
            ConvertToXmlBuffer,
            NULL,
            NULL,
            XML_PARSE_DTDVALID);
        if (myxmlCtxtReadDoc == NULL)
        {
            printf("Error : xmlCtxtReadDoc\n");
            // exit(-1);
        }
        printf("Success : xmlCtxtReadDoc\n");
	*/

        // step 0 : parse an XML in-memory document build a tree.  This reuses the existing @ctxt parse context
        xmlDocPtr myxmlCtxtReadMemory = xmlCtxtReadMemory(myxmlParserCtxtPtr,
			ConvertToXmlBuffer,
			sizeof(ConvertToXmlBuffer),
			NULL,
			NULL,
			XML_PARSE_DTDVALID);
        if (myxmlCtxtReadMemory == NULL)
        {
            printf("Error : xmlCtxtReadMemory\n");
            // exit(-1);
        }
        printf("Success : xmlCtxtReadMemory\n");

        // step 1 : create xmlReaderPtr
        // xmlReaderForMemory :
        xml_TextReaderPtr = xmlReaderForMemory(ConvertToXmlBuffer, sizeof(ConvertToXmlBuffer), NULL, NULL, XML_PARSE_DTDVALID);
        if (xml_TextReaderPtr == NULL)
        {
            printf("Step 0: Fail at xmlReaderForMemory\n");
            // exit(-1);
        }
        printf("Success : xmlReaderForMemory\n");

	/*
        // alternate step 1 : create xmlDocPtr
        // xmlReadDoc : Parse an XML in-memory document and build a tree
        myxmlDocPtr = xmlReadDoc(ConvertToXmlBuffer, NULL, NULL, XML_PARSE_DTDVALID);
        if (myxmlDocPtr == NULL)
        {
            printf("Error : xmlReadDoc\n");
            // exit(-1);
        }
	else printf("Success : xmlReadDoc \n"); */

        // step 2 :
        // xmlParseMemory : parse an XML in-memory block and builds a tree
        myxmlDocPtr = xmlParseMemory(ConvertToXmlBuffer, sizeof(ConvertToXmlBuffer));
        if (myxmlDocPtr == NULL)
        {
            printf("Error : xmlParseMemory\n");
            // exit(-1);
        }
        printf("Success : xmlParseMemory\n");

	/* alternate step 2:
        // xmlParseDoc : parse an XML in-memory document and builds a tree
        myxmlDocPtr = xmlParseDoc(ConvertToXmlBuffer);
        if (myxmlDocPtr == NULL)
        {
            printf("Error : xmlParseDoc\n");
            // exit(-1);
        }
        printf("Success : xmlParseDoc\n");*/

	//-- end

        myxmlDoc = myxmlDocPtr;
        root_element = xmlDocGetRootElement(myxmlDoc);
        ExtractXMLElementNames(root_element);
        xmlFreeDoc(myxmlDoc);

        //int retval = run_UA_Client(server, r2_airgard_data_Id);// need to pass all the variables into run_UA_Client to write into UA Address space
        // r2_airgard_data_Id needs to be declared at the front of this function instead of inside the WHILE loop (with the rest)

        //int retval = StartOPCUAClient(server, r2_airgard_data_Id);
        //return (r2_airgard_data_Id);

       // sleep(9);
    //}
    printf("============================================================\n");
    printf("Leaving PopulateOPCUANodes(): g_argv_ip = %s \n", g_argv_ip);
    printf("============================================================\n");
}
