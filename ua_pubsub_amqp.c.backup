/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * @author Waheed Ejaz (IMD, University of Rostock)
 */


#include "ua_amqp_adaptor.h"
#include <open62541/plugin/log_stdout.h>
//#include "open62541/util.h"


/**
 * Delete all allocated memory
 * @param c pointer to the AMQP PubSubChannel
 * @param bDisconnect Set to true if disconnect is required
 */
static void UA_Destroy_AmqpChannel(UA_PubSubChannel *c, UA_Boolean bDisconnect)
{
    if (!c) {
        return;
    }

    /* Check and free AMQP Proactor Data */
    if (c->handle) {
        UA_AmqpContext *ctx = (UA_AmqpContext *)c->handle;

        if (bDisconnect) { UA_AmqpDisconnect(ctx);}
        if (ctx->ua_connection) { UA_free(ctx->ua_connection);}
        if (ctx->driver) { UA_free(ctx->driver);}
        UA_free(c->handle);
    }
    UA_free(c);
}

/**
 * @brief Deletes all memory allocated memory
 * @param channel pointer to generic pubsub channel
 */
static UA_StatusCode UA_PubSubChannelAMQP_close(UA_PubSubChannel *channel) {
    if(channel->state == UA_PUBSUB_CHANNEL_CLOSED)
        return UA_STATUSCODE_GOOD;

    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: Closing PubSubChannel.");
    UA_Destroy_AmqpChannel(channel, true);

    return UA_STATUSCODE_GOOD;
}



/**
 * Open AMQP connection, connects to AMQP-1.0 broker
 * @param amqpConnectionConfig
 * @return Pointer to PubSubChannel if successful otherwise null
 */
static UA_PubSubChannel *UA_PubSubChannelAMQP_open(UA_PubSubConnectionConfig *amqpConnectionConfig)
{
    UA_StatusCode retVal = UA_STATUSCODE_GOOD;

    /*******************************************
     *   Memory Allocation
     *******************************************/
    /* Create a new pubsub channel */
    UA_PubSubChannel *ua_amqpChannel = (UA_PubSubChannel *) UA_calloc(1, sizeof(UA_PubSubChannel));
    /* Create a new AMQP Proactor database */
    UA_AmqpContext *amqpCtx = (UA_AmqpContext *) UA_calloc(1, sizeof(UA_AmqpContext));

    if(!ua_amqpChannel || !amqpCtx){
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "ua_pubsub_amqp.c :PubSub AMQP Connection creation failed. Out of memory.");
        UA_Destroy_AmqpChannel(ua_amqpChannel, false);
        return NULL;
    }

    /* Initialize sub-elements in AMQP Context */
    amqpCtx->ua_connection = (UA_Connection*)UA_calloc(1, sizeof(UA_Connection));
    amqpCtx->driver = (pn_connection_driver_t *) UA_calloc(1, sizeof(pn_connection_driver_t));

    if(!amqpCtx->ua_connection || !amqpCtx->driver) {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: Context creation failed. Out of memory.");
        UA_Destroy_AmqpChannel(ua_amqpChannel, false);
        return NULL;
    }

    // Link the amqpCtx to amqpChannel
    ua_amqpChannel->handle = (void*) amqpCtx;
    amqpCtx->sequence_no = 0;
    /****************************************
     *     Validate Connection Config
     ***************************************/

    /* Get the url form connection config*/
    UA_NetworkAddressUrlDataType address;

    if(UA_Variant_hasScalarType(&amqpConnectionConfig->address,
                                &UA_TYPES[UA_TYPES_NETWORKADDRESSURLDATATYPE])){
        address = *(UA_NetworkAddressUrlDataType *)amqpConnectionConfig->address.data;
    } else {
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "PubSub AMQP Connection creation failed. Invalid Address.");
        return NULL;
    }

    /* Validate AMQP URL */
    UA_String hostname, path;
    UA_UInt16 networkPort;
    if(UA_parseEndpointUrl(&address.url, &hostname, &networkPort, &path) != UA_STATUSCODE_GOOD){
        UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER,
                     "ua_pubsub_amqp.c :: AMQP PubSub Connection creation failed. Invalid URL.");

	printf("networkPort 	: %d \n", networkPort);
	printf("hostname	: %s \n", hostname);
	printf("path		: %s \n", path);

        UA_Destroy_AmqpChannel(ua_amqpChannel, false);
        return NULL;
    }

    /* Build the url, replace amqp with tcp */
    UA_STACKARRAY(UA_Byte, addressAsChar, 10 + (sizeof(char) * path.length));
    memcpy((char*)addressAsChar, "opc.tcp://", 10);
    memcpy((char*)&addressAsChar[10],(char*)path.data, path.length);
    address.url.data = addressAsChar;
    address.url.length = 10 + (sizeof(char) * path.length);


    /*********************************************
     *         Start the Connection
     ********************************************/
    retVal = UA_AmqpConnect(amqpCtx, address);

    if (UA_STATUSCODE_GOOD == retVal) {
        return ua_amqpChannel;
    } else {
        UA_Destroy_AmqpChannel(ua_amqpChannel, true);
        return NULL;
    }
}

/**
 * @brief send AMQP message to broker
 * @param channel Pointer to the PubSub channel
 * @param transportSettings Transport settings of Datawriter
 * @param buf Message to send
 */
static UA_StatusCode UA_PubSubChannelAMQP_send(UA_PubSubChannel *channel,
                          UA_ExtensionObject *transportSettings, const UA_ByteString *buf)
{
    UA_AmqpContext *amqpCtx = (UA_AmqpContext*)channel->handle;
    if (transportSettings != NULL && transportSettings->encoding == UA_EXTENSIONOBJECT_DECODED){
      //&& transportSettings->content.decoded.type->typeIndex == UA_TYPES_BROKERWRITERGROUPTRANSPORTDATATYPE) {

        UA_BrokerWriterGroupTransportDataType *brokerTransportSettings =
            (UA_BrokerWriterGroupTransportDataType*)transportSettings->content.decoded.data;

        UA_StatusCode ret = publishAmqp(amqpCtx, brokerTransportSettings->queueName, buf);

        if(ret){
            if (ret != UA_STATUSCODE_BADWAITINGFORRESPONSE) {
                channel->state = UA_PUBSUB_CHANNEL_ERROR;
            }
            UA_LOG_ERROR(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: Publish failed");
        }else{
            UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: Publish");
        }
    }else{
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: Transport settings not found.");
    }

    return UA_STATUSCODE_GOOD;
}

// added by Jacky
static UA_StatusCode
UA_uaQos_toAmqpQos(UA_BrokerTransportQualityOfService uaQos, UA_Byte *qos){
    switch (uaQos){
        case UA_BROKERTRANSPORTQUALITYOFSERVICE_BESTEFFORT:
        case UA_BROKERTRANSPORTQUALITYOFSERVICE_ATMOSTONCE:
            *qos = 0;
            break;
        case UA_BROKERTRANSPORTQUALITYOFSERVICE_ATLEASTONCE:
            *qos = 1;
            break;
        case UA_BROKERTRANSPORTQUALITYOFSERVICE_EXACTLYONCE:
            *qos = 2;
            break;
        default:
            break;
    }
    return UA_STATUSCODE_GOOD;
}

// added by Jacky
/**
 * Subscribe to topic specified in brokerTransportSettings->queueName.
 *
 * @return UA_STATUSCODE_GOOD on success
 */

static UA_StatusCode
UA_PubSubChannelAMQP_regist(UA_PubSubChannel *channel, UA_ExtensionObject *transportSettings,
                            void (*callback)(UA_ByteString *encodedBuffer, UA_ByteString *topic)) {

printf("ua_pubsub_amqp.c : Entering UA_PubSubChannelAMQP_regist() \n");

    if(channel->state != UA_PUBSUB_CHANNEL_RDY){
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP regist failed. Channel closed.");
        return UA_STATUSCODE_BADCONNECTIONCLOSED;
    }

    UA_PubSubChannelDataMQTT *channelDataAMQP = (UA_PubSubChannelDataMQTT *) channel->handle;
    channelDataAMQP->callback = callback;

    if(transportSettings != NULL && transportSettings->encoding == UA_EXTENSIONOBJECT_DECODED){
            //&& transportSettings->content.decoded.type->typeIndex == UA_TYPES_BROKERDATASETREADERTRANSPORTDATATYPE){
                UA_BrokerDataSetReaderTransportDataType *brokerTransportSettings =
                    (UA_BrokerDataSetReaderTransportDataType*)transportSettings->content.decoded.data;

        UA_Byte qos = 0;
        UA_uaQos_toAmqpQos(brokerTransportSettings->requestedDeliveryGuarantee, &qos);
        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: register");
        return subscribeMqtt(channelDataAMQP, brokerTransportSettings->queueName, qos);
    }else{
         return UA_STATUSCODE_BADARGUMENTSMISSING;
    }
}

// added by Jacky
/**
 * Remove subscription specified in brokerTransportSettings->queueName.
 *
 * @return UA_STATUSCODE_GOOD on success
 */
static UA_StatusCode
UA_PubSubChannelAMQP_unregist(UA_PubSubChannel *channel, UA_ExtensionObject *transportSettings) {
    if(channel->state != UA_PUBSUB_CHANNEL_RDY){
        UA_LOG_WARNING(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: unregister failed. Channel closed.");
        return UA_STATUSCODE_BADCONNECTIONCLOSED;
    }

    UA_PubSubChannelDataMQTT * channelDataAMQP = (UA_PubSubChannelDataMQTT *) channel->handle;
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: unregister");
    if(transportSettings != NULL && transportSettings->encoding == UA_EXTENSIONOBJECT_DECODED){
            //&& transportSettings->content.decoded.type->typeIndex == UA_TYPES_BROKERWRITERGROUPTRANSPORTDATATYPE){
        UA_BrokerWriterGroupTransportDataType *brokerTransportSettings =
                (UA_BrokerWriterGroupTransportDataType*)transportSettings->content.decoded.data;

        UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_SERVER, "PubSub AMQP: unregister");
        return unSubscribeMqtt(channelDataAMQP, brokerTransportSettings->queueName);
    }else{
         return UA_STATUSCODE_BADARGUMENTSMISSING;
    }
}


/**
 * UA_PubSubChannel_yield
 * @param channel pointer to the pubsub channel
 * @param timeout timeout for receiver
 * @return UA_STATUSCODE_GOOD or UA_PUBSUB_BADINTERNALERROR
 */
static UA_StatusCode UA_PubSubChannelAMQP_yield(UA_PubSubChannel *channel, UA_UInt16 timeout)
{
    UA_StatusCode ret = UA_STATUSCODE_BADINVALIDARGUMENT;
    if(channel == NULL){
        return ret;
    }

    if(channel->state == UA_PUBSUB_CHANNEL_ERROR){
        return UA_STATUSCODE_BADINTERNALERROR;
    }

    UA_AmqpContext *amqpCtx = (UA_AmqpContext *) channel->handle;
    ret = yieldAmqp(amqpCtx, timeout);
    if(ret != UA_STATUSCODE_GOOD){
        channel->state = UA_PUBSUB_CHANNEL_ERROR;
        return ret;
    }

    return ret;
}
/**
 * Generate a new AMQP channel. Based on the given configuration.
 *
 * @param connectionConfig connection configuration
 * @return  ref to created channel, NULL on error
 */
static UA_PubSubChannel *
TransportLayerAMQP_addChannel(UA_PubSubConnectionConfig *connectionConfig) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "ua_pubsub_amqp.c :: PubSub AMQP: channel requested.");
    UA_PubSubChannel * pubSubChannel = UA_PubSubChannelAMQP_open(connectionConfig);

    if(pubSubChannel) {
        pubSubChannel->regist = UA_PubSubChannelAMQP_regist;
        pubSubChannel->unregist = UA_PubSubChannelAMQP_unregist;
        //pubSubChannel->regist = NULL;
        //pubSubChannel->unregist = NULL;
        pubSubChannel->send = UA_PubSubChannelAMQP_send;
        pubSubChannel->close = UA_PubSubChannelAMQP_close;
        pubSubChannel->yield = UA_PubSubChannelAMQP_yield;

        pubSubChannel->connectionConfig = connectionConfig;
    }

    return pubSubChannel;
}

UA_PubSubTransportLayer
UA_PubSubTransportLayerAMQP(void) {
    UA_PubSubTransportLayer pubSubTransportLayer;
    pubSubTransportLayer.transportProfileUri = UA_STRING("http://opcfoundation.org/UA-Profile/Transport/pubsub-amqp");
    pubSubTransportLayer.createPubSubChannel = &TransportLayerAMQP_addChannel;
    return pubSubTransportLayer;
}
