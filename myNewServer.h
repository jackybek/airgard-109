#include "open62541.h"

//#include <xml.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>

void *StartOPCUAServer(void *x_void_ptr);
xmlNodePtr srSeekChildNodeNamed(xmlNode* p, char* name, int len);
int extract_element_names(xmlNode *a_node);
int run_UA_Client(UA_Server *server, UA_NodeId r2_airgard_data_Id);
void *ConnectToAirgard(void *x_void_ptr);
int main(int argc, char *argv[]);




