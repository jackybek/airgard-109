void setupUadpRange(UA_Server* server);
int runUadp(UA_Server* server, UA_String* transportProfile, UA_NetworkAddressUrlDataType* networkAddressUrl);
void addPublisherAirgardDiagnostics(UA_Server* server, UA_NodeId publishedDataSetId);
void addPublisherAirgardStatus(UA_Server* server, UA_NodeId publishedDataSetId);

//---- to be remove kater
void addPubSubConnection(UA_Server *server, UA_String *transportProfile,UA_NetworkAddressUrlDataType *networkAddressUrl);
void addPublishedDataSet(UA_Server *server);
void addDataSetField(UA_Server *server);
void addWriterGroup(UA_Server *server);
void addDataSetWriter(UA_Server *server);
