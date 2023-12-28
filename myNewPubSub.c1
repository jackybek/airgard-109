#define UA_ENABLE_PUBSUB	// needed by ope62541.h to activate pub sub headers
#define UA_ENABLE_PUBSUB_ETH_UADP

#include "open62541.h"
#include "myNewPubSub.h"

#include <signal.h>

extern UA_NodeId ds1IgramPPId;
extern UA_NodeId ds1IgramDCId;
extern UA_NodeId ds1LaserPPId;
extern UA_NodeId ds1LaserDCId;

UA_NodeId connectionIdent, publishedDataSetIdent, writerGroupIdent;

void setupUadpRange(UA_Server* server)
{

	UA_String transportProfile =
        	UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-udp-uadp");
    	UA_NetworkAddressUrlDataType networkAddressUrl =
    		{ UA_STRING_NULL, UA_STRING("opc.udp://239.0.0.1:4840/") };

	runUadp(server, &transportProfile, &networkAddressUrl);
}

int runUadp(UA_Server* server, UA_String *transportProfile,
    UA_NetworkAddressUrlDataType *networkAddressUrl)
{
   // signal(SIGINT, stopHandler);
   // signal(SIGTERM, stopHandler);

	printf("\t----------------- in runUadp() funcion\n");
    UA_ServerConfig *config = UA_Server_getConfig(server);
    UA_ServerConfig_setMinimal(config, 4802, NULL);

    /* Details about the connection configuration and handling are located in
    * the pubsub connection tutorial */
    config->pubsubTransportLayers =
        (UA_PubSubTransportLayer *)UA_calloc(2, sizeof(UA_PubSubTransportLayer));
    if (!config->pubsubTransportLayers)
    {
        UA_Server_delete(server);
        return EXIT_FAILURE;
    }
    config->pubsubTransportLayers[0] = UA_PubSubTransportLayerUDPMP();
    config->pubsubTransportLayersSize++;

#ifdef UA_ENABLE_PUBSUB_ETH_UADP
    config->pubsubTransportLayers[1] = UA_PubSubTransportLayerEthernet();
    config->pubsubTransportLayersSize++;
#endif

    addPubSubConnection(server, transportProfile, networkAddressUrl);
	printf("\t----------------- after addPubSubConnection() funcion\n");

    // Published Dataset 1 (Diagnostics block)
    /* Create a PublishedDataSet based on a PublishedDataSetConfig. */
    UA_PublishedDataSetConfig publishedDataSetConfigDiagnostics;
    publishedDataSetConfigDiagnostics.publishedDataSetType = UA_PUBSUB_DATASET_PUBLISHEDITEMS;
    publishedDataSetConfigDiagnostics.name = UA_STRING("DataSet (Diagnostics)");

    UA_NodeId publishedDataSetIdentDiagnostics;
    UA_Server_addPublishedDataSet(server, &publishedDataSetConfigDiagnostics, &publishedDataSetIdentDiagnostics);
    addPublisherAirgardDiagnostics(server, publishedDataSetIdentDiagnostics);

    // DataSet Writer 1 (Diagnositcs block)
    /* Create a new Writer and connect it with an exsiting PublishedDataSet. */
    UA_DataSetWriterConfig dataSetWriterConfigDiagnostics;

    memset(&dataSetWriterConfigDiagnostics, 0, sizeof(UA_DataSetWriterConfig));
    dataSetWriterConfigDiagnostics.name = UA_STRING("DataSet Diagnostics DataSetWriter");
    dataSetWriterConfigDiagnostics.dataSetWriterId = 1;
    //The creation of delta messages is configured in the following line. Value
    // 0 -> no delta messages are created.
    dataSetWriterConfigDiagnostics.keyFrameCount = 10;

    UA_NodeId writeIdentifierDiagnostics;
    UA_Server_addDataSetWriter(server, writeIdentifierDiagnostics, publishedDataSetIdentDiagnostics,
	&dataSetWriterConfigDiagnostics, &writeIdentifierDiagnostics);


    //-- Published DataSet 2 (Status block)
    UA_PublishedDataSetConfig publishedDataSetConfigStatus;
    publishedDataSetConfigStatus.publishedDataSetType = UA_PUBSUB_DATASET_PUBLISHEDITEMS;
    publishedDataSetConfigStatus.name = UA_STRING("DataSet 2 (Status block)");

    UA_NodeId publishedDataSetIdentStatus;
    UA_Server_addPublishedDataSet(server, &publishedDataSetConfigStatus, &publishedDataSetIdentStatus);
    addPublisherAirgardStatus(server, publishedDataSetIdentStatus);

    // DataSet Writer 2 (Status block)
    /* Create a new Writer and connect it with an existing PublishedDataSet */
    // DataSetWriter ID 2 with Variant Encoding
    UA_DataSetWriterConfig dataSetWriterConfigStatus;

    memset(&dataSetWriterConfigStatus, 0, sizeof(UA_DataSetWriterConfig));
    dataSetWriterConfigStatus.name = UA_STRING("DataSet 2 DataSetWriter (Status block)");
    dataSetWriterConfigStatus.dataSetWriterId = 2;
    //The creation of delta messages is configured in the following line. Value
    // 0 -> no delta messages are created.
    dataSetWriterConfigStatus.keyFrameCount = 10;

    UA_NodeId writerIdentifierStatus;
    UA_Server_addDataSetWriter(server, writerIdentifierStatus, publishedDataSetIdentStatus,
        &dataSetWriterConfigStatus, &writerIdentifierStatus);


    // -- continue for Data block, Timestamp block, root block



}

void addPublisherAirgardDiagnostics(UA_Server *server, UA_NodeId publishedDataSetId)
{
    if (!UA_NodeId_equal(&publishedDataSetId, &UA_NODEID_NULL))
    {

	printf("\t----------------- enter addPublisherAirgardDiagnostics() funcion\n");

        // Create and add fields to the PublishedDataSet
        UA_DataSetFieldConfig IgramPPConfig;
        memset(&IgramPPConfig, 0, sizeof(UA_DataSetFieldConfig));
        IgramPPConfig.field.variable.fieldNameAlias = UA_STRING("IgramPP");
        IgramPPConfig.field.variable.promotedField = false;
        IgramPPConfig.field.variable.publishParameters.publishedVariable = ds1IgramPPId;
        IgramPPConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;

        UA_DataSetFieldConfig IgramDCConfig;
        memset(&IgramDCConfig, 0, sizeof(UA_DataSetFieldConfig));
        IgramDCConfig.field.variable.fieldNameAlias = UA_STRING("IgramDC");
        IgramDCConfig.field.variable.promotedField = false;
        IgramDCConfig.field.variable.publishParameters.publishedVariable = ds1IgramDCId;
        IgramDCConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;

        UA_DataSetFieldConfig LaserPPConfig;
        memset(&LaserPPConfig, 0, sizeof(UA_DataSetFieldConfig));
        LaserPPConfig.field.variable.fieldNameAlias = UA_STRING("LaserPP");
        LaserPPConfig.field.variable.promotedField = false;
        LaserPPConfig.field.variable.publishParameters.publishedVariable = ds1LaserPPId;
        LaserPPConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;

        UA_DataSetFieldConfig LaserDCConfig;
        memset(&LaserDCConfig, 0, sizeof(UA_DataSetFieldConfig));
        LaserDCConfig.field.variable.fieldNameAlias = UA_STRING("LaserDC");
        LaserDCConfig.field.variable.promotedField = false;
        LaserDCConfig.field.variable.publishParameters.publishedVariable = ds1LaserDCId;
        LaserDCConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;

        UA_NodeId LaserPP, LaserDC, IgramPP, IgramDC;
        // add fields in reverse order, because all fields are added to the beginning of the list
        UA_Server_addDataSetField(server, publishedDataSetId, &IgramPPConfig, &LaserDC);
        UA_Server_addDataSetField(server, publishedDataSetId, &IgramDCConfig, &LaserPP);
        UA_Server_addDataSetField(server, publishedDataSetId, &LaserPPConfig, &IgramDC);
        UA_Server_addDataSetField(server, publishedDataSetId, &LaserDCConfig, &IgramPP);

printf("\t----------------- end of addPublisherAirgardDiagnostics() funcion\n");

    }
}

void addPublisherAirgardStatus(UA_Server *server, UA_NodeId publishedDataSetId)
{


printf("\t----------------- end of addPublisherAirgardStatus() funcion\n");

}

//---------------------------------------below this line is not required -----------------------

void addPubSubConnection(UA_Server *server, UA_String *transportProfile,
                    UA_NetworkAddressUrlDataType *networkAddressUrl)
{
    /* Details about the connection configuration and handling are located
     * in the pubsub connection tutorial */
    UA_PubSubConnectionConfig connectionConfig;
    memset(&connectionConfig, 0, sizeof(connectionConfig));
    connectionConfig.name = UA_STRING("UADP Connection 1");
    connectionConfig.transportProfileUri = *transportProfile;
    connectionConfig.enabled = UA_TRUE;
    UA_Variant_setScalar(&connectionConfig.address, networkAddressUrl,
                         &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE]);
    /* Changed to static publisherId from random generation to identify
     * the publisher on Subscriber side */
    connectionConfig.publisherId.numeric = 2234;
    UA_Server_addPubSubConnection(server, &connectionConfig, &connectionIdent);
}

void addPublishedDataSet(UA_Server *server)
{
    /* The PublishedDataSetConfig contains all necessary public
    * informations for the creation of a new PublishedDataSet */
    UA_PublishedDataSetConfig publishedDataSetConfig;
    memset(&publishedDataSetConfig, 0, sizeof(UA_PublishedDataSetConfig));
    publishedDataSetConfig.publishedDataSetType = UA_PUBSUB_DATASET_PUBLISHEDITEMS;
    publishedDataSetConfig.name = UA_STRING("Demo PDS");
    /* Create new PublishedDataSet based on the PublishedDataSetConfig. */
    UA_Server_addPublishedDataSet(server, &publishedDataSetConfig, &publishedDataSetIdent);
}

void addDataSetField(UA_Server *server)
{
    /* Add a field to the previous created PublishedDataSet */
    UA_NodeId dataSetFieldIdent;
    UA_DataSetFieldConfig dataSetFieldConfig;
    memset(&dataSetFieldConfig, 0, sizeof(UA_DataSetFieldConfig));
    dataSetFieldConfig.dataSetFieldType = UA_PUBSUB_DATASETFIELD_VARIABLE;
    dataSetFieldConfig.field.variable.fieldNameAlias = UA_STRING("Server localtime");
    dataSetFieldConfig.field.variable.promotedField = UA_FALSE;
    dataSetFieldConfig.field.variable.publishParameters.publishedVariable =
    UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER_SERVERSTATUS_CURRENTTIME);
    dataSetFieldConfig.field.variable.publishParameters.attributeId = UA_ATTRIBUTEID_VALUE;
    UA_Server_addDataSetField(server, publishedDataSetIdent,
                              &dataSetFieldConfig, &dataSetFieldIdent);
}

void addWriterGroup(UA_Server *server)
{
    /* Now we create a new WriterGroupConfig and add the group to the existing
     * PubSubConnection. */
    UA_WriterGroupConfig writerGroupConfig;
    memset(&writerGroupConfig, 0, sizeof(UA_WriterGroupConfig));
    writerGroupConfig.name = UA_STRING("Demo WriterGroup");
    writerGroupConfig.publishingInterval = 100;
    writerGroupConfig.enabled = UA_FALSE;
    writerGroupConfig.writerGroupId = 100;
    writerGroupConfig.encodingMimeType = UA_PUBSUB_ENCODING_UADP;
    writerGroupConfig.messageSettings.encoding             = UA_EXTENSIONOBJECT_DECODED;
    writerGroupConfig.messageSettings.content.decoded.type = &UA_TYPES[UA_TYPES_UADPWRITERGROUPMESSAGEDATATYPE];
    /* The configuration flags for the messages are encapsulated inside the
     * message- and transport settings extension objects. These extension
     * objects are defined by the standard. e.g.
     * UadpWriterGroupMessageDataType */
    UA_UadpWriterGroupMessageDataType *writerGroupMessage  = UA_UadpWriterGroupMessageDataType_new();
    /* Change message settings of writerGroup to send PublisherId,
     * WriterGroupId in GroupHeader and DataSetWriterId in PayloadHeader
     * of NetworkMessage */
    writerGroupMessage->networkMessageContentMask = (UA_UadpNetworkMessageContentMask)(UA_UADPNETWORKMESSAGECONTENTMASK_PUBLISHERID |
                                                              (UA_UadpNetworkMessageContentMask)UA_UADPNETWORKMESSAGECONTENTMASK_GROUPHEADER |
                                                              (UA_UadpNetworkMessageContentMask)UA_UADPNETWORKMESSAGECONTENTMASK_WRITERGROUPID |
                                                              (UA_UadpNetworkMessageContentMask)UA_UADPNETWORKMESSAGECONTENTMASK_PAYLOADHEADER);
    writerGroupConfig.messageSettings.content.decoded.data = writerGroupMessage;
    UA_Server_addWriterGroup(server, connectionIdent, &writerGroupConfig, &writerGroupIdent);
    UA_Server_setWriterGroupOperational(server, writerGroupIdent);
    UA_UadpWriterGroupMessageDataType_delete(writerGroupMessage);
}

void addDataSetWriter(UA_Server *server)
{
    /* We need now a DataSetWriter within the WriterGroup. This means we must
     * create a new DataSetWriterConfig and add call the addWriterGroup function. */
    UA_NodeId dataSetWriterIdent;
    UA_DataSetWriterConfig dataSetWriterConfig;
    memset(&dataSetWriterConfig, 0, sizeof(UA_DataSetWriterConfig));
    dataSetWriterConfig.name = UA_STRING("Demo DataSetWriter");
    dataSetWriterConfig.dataSetWriterId = 62541;
    dataSetWriterConfig.keyFrameCount = 10;
    UA_Server_addDataSetWriter(server, writerGroupIdent, publishedDataSetIdent,
                               &dataSetWriterConfig, &dataSetWriterIdent);
}

