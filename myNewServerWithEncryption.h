#include "open62541.h"

//#include <xml.h>
#include <libxml2/libxml/parser.h>
#include <libxml2/libxml/tree.h>
#include <libxml2/libxml/xmlreader.h>

// refer to https://github.com/open62541/open62541/blob/6aa4d8d9d2f76b0ba95acba06345ae38c3e4c068/tools/schema/Opc.UA.Types.bsd

void *StartOPCUAServer(void *x_void_ptr);
xmlNodePtr srSeekChildNodeNamed(xmlNode* p, char* name, int len);
int extract_element_names(xmlNode *a_node);
int run_UA_Agent(UA_Server *server, char* g_argv_ip, UA_NodeId r2_airgard_data_Id);
void *ConnectToAirgard(void *x_void_ptr);
int main(int argc, char *argv[]);

UA_ByteString loadFile(const char *const path);
static UA_Boolean allowAddNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_AddNodesItem *item);
static UA_Boolean allowDeleteNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_DeleteNodesItem *item);
static UA_Boolean allowBrowseNode(UA_Server *server, UA_AccessControl *ac,
                const UA_NodeId *sessionId, void *sessionContext,
                const UA_NodeId *nodeId, void *nodeContext);


#ifdef UA_ENABLE_HISTORIZING
static UA_Boolean allowHistoryUpdateUpdateData(UA_Server *server, UA_AccessControl *ac, 
		const UA_NodeId *sessionId, void *sessionContext, 
		const UA_NodeId *nodeId, UA_PerformUpdateType performInsertReplace, const UA_DataValue *value);
#endif
