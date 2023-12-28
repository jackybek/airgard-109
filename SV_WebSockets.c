#include "open62541.h"
#include <libwebsockets.h>
#include <signal.h>
#if defined(WIN32)
#define HAVE_STRUCT_TIMESPEC
#if defined(pid_t)
#undef pid_t
#endif
#endif
#include <pthread.h>

#ifdef UA_ENABLE_WEBSOCKET_SERVER
 #define TRANSPORT_PROFILE_URI_WSS_BIN   "http://opcfoundation.org/UA_Profile/Transport/wss-uasc-uabinary"
 #define TRANSPORT_PROFILE_URI_WSS_JSON  "http://opcfoundation.org/UA-Profile/Transport/wss-uajson"
 #define BROKER_ADDRESS_URL_WSS         "opc.wss://192.168.1.165:443/"
#endif

int CreateServerWebSockets(UA_NodeId *publishedDataSetidentifier, UA_NetworkAddressUrlDataType *networkAddressUrl);

/*
 * lws-minimal-ws-server
 *
 * Written in 2010-2019 by Andy Green <andy@warmcat.com>
 *
 * This file is made available under the Creative Commons CC0 1.0
 * Universal Public Domain Dedication.
 *
 * This demonstrates the most minimal http server you can make with lws,
 * with an added websocket chat server using a ringbuffer.
 *
 * To keep it simple, it serves stuff in the subdirectory "./mount-origin" of
 * the directory it was started in.
 * You can change that by changing mount.origin.
 */

#include <libwebsockets.h>
#include <string.h>
#include <signal.h>

#define LWS_PLUGIN_STATIC
#include "protocol_lws_minimal.c"

static struct lws_protocols protocols[] = {
        { "http", lws_callback_http_dummy, 0, 0 },
        LWS_PLUGIN_PROTOCOL_MINIMAL,
        { NULL, NULL, 0, 0 } /* terminator */
};

static int interrupted;

static const struct lws_http_mount mount = {
        /* .mount_next */		NULL,		/* linked-list "next" */
        /* .mountpoint */		"/",		/* mountpoint URL */
        /* .origin */			"./"		//"./mount-origin", /* serve from dir */
        /* .def */			"index.html",	/* default filename */
        /* .protocol */			NULL,
        /* .cgienv */			NULL,
        /* .extra_mimetypes */		NULL,
        /* .interpret */		NULL,
        /* .cgi_timeout */		0,
        /* .cache_max_age */		0,
        /* .auth_mask */		0,
        /* .cache_reusable */		0,
        /* .cache_revalidate */		0,
        /* .cache_intermediaries */	0,
        /* .origin_protocol */		LWSMPRO_FILE,	/* files in a dir */
        /* .mountpoint_len */		1,		/* char count */
        /* .basic_auth_login_file */	NULL,
};

void sigint_handler(int sig)
{
        interrupted = 1;
}

int CreateServerWebSockets(UA_NodeId *publishedDataSetidentifier, UA_NetworkAddressUrlDataType *networkAddressUrl)
{
#ifdef NOTREQUIRED
        struct lws_context_creation_info info;
        struct lws_context *context;
        const char *p;
        int n = 0, logs = LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE
                        /* for LLL_ verbosity above NOTICE to be built into lws,
                         * lws must have been configured and built with
                         * -DCMAKE_BUILD_TYPE=DEBUG instead of =RELEASE */
                        /* | LLL_INFO */ /* | LLL_PARSER */ /* | LLL_HEADER */
                        /* | LLL_EXT */ /* | LLL_CLIENT */ /* | LLL_LATENCY */
                        /* | LLL_DEBUG */;

        signal(SIGINT, sigint_handler);

        //if ((p = lws_cmdline_option(argc, argv, "-d")))
        //        logs = atoi(p);

        lws_set_log_level(logs, NULL);
        lwsl_user("LWS minimal ws server (lws_ring) | visit http://localhost:443\n");

        memset(&info, 0, sizeof info); /* otherwise uninitialized garbage */
        info.port = 443;	// 7681
        info.mounts = &mount;
        info.protocols = protocols;
        info.options =
                LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;

        context = lws_create_context(&info);
        if (!context) {
                lwsl_err("lws init failed\n");
                return 1;
        }

        while (n >= 0 && !interrupted)
                n = lws_service(context, 0);

        lws_context_destroy(context);

        return 0;
#endif

	char *wssbrokeraddress = "192.168.1.165";

                printf("wssbrokeraddress : %s \n", wssbrokeraddress);
                char URI_wss[100];

                // set the WSS_Enable global flag
                UA_Boolean WSS_Enable = UA_TRUE;

                //similar to sprintf(URI_wss, "opc.wss://%s:7681", wssbrokeraddress);
                strcpy(URI_wss, "opc.wss://");
                strcat(URI_wss, wssbrokeraddress);
                strcat(URI_wss, ":443");	// 7681
                printf("URI_wss : <%s> \n", URI_wss);

                UA_String transportProfile = UA_STRING(TRANSPORT_PROFILE_URI_WSS_JSON);
                networkAddressUrl->url = UA_STRING(URI_wss);

		networkAddressUrl->networkInterface = UA_STRING("eth0"); // name of the interface defined in /etc/dhcpcd.conf

		// send avail publishedDataSetIdentifer to port opc.wss://192.168.1.165:7681
		//TODO


		return UA_STATUSCODE_GOOD;
}
