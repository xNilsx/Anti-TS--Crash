/*
* TeamSpeak 3 Anti (img) Crash
* Copyright (c) 2014 xNilsx
*/

#ifdef _WIN32
#pragma warning (disable : 4100)
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <string>
#include "public_errors.h"
#include "public_errors_rare.h"
#include "public_definitions.h"
#include "public_rare_definitions.h"
#include "ts3_functions.h"
#include "plugin.h"

static struct TS3Functions ts3Functions;

#ifdef _WIN32
#define _strcpy(dest, destSize, src) strcpy_s(dest, destSize, src)
#else
#define _strcpy(dest, destSize, src) { strncpy(dest, src, destSize-1); (dest)[destSize-1] = '\0'; }
#endif

#define PLUGIN_API_VERSION 20

#define PATH_BUFSIZE 512
#define COMMAND_BUFSIZE 128
#define INFODATA_BUFSIZE 128
#define SERVERINFO_BUFSIZE 256
#define CHANNELINFO_BUFSIZE 512
#define RETURNCODE_BUFSIZE 128
#define sn

static char* pluginID = NULL;

#ifdef _WIN32
static int wcharToUtf8(const wchar_t* str, char** result) {
	int outlen = WideCharToMultiByte(CP_UTF8, 0, str, -1, 0, 0, 0, 0);
	*result = (char*)malloc(outlen);
	if (WideCharToMultiByte(CP_UTF8, 0, str, -1, *result, outlen, 0, 0) == 0) {
		*result = NULL;
		return -1;
	}
	return 0;
}
#endif

const char* ts3plugin_name() {
#ifdef _WIN32
	static char* result = NULL;
	if (!result) {
		const wchar_t* name = L"Teamspeak 3 Anti Crash";
		if (wcharToUtf8(name, &result) == -1) {
			result = "Teamspeak 3 Anti Crash";
		}
	}
	return result;
#else
	return "Teamspeak 3 Anti Crash";
#endif
}
const char* ts3plugin_version() {
	return "0.1";
}

int ts3plugin_apiVersion() {
	return PLUGIN_API_VERSION;
}

const char* ts3plugin_author() {
	return "xNilsx";
}

const char* ts3plugin_description() {
	return "Anti [img] Crash";
}

void ts3plugin_setFunctionPointers(const struct TS3Functions funcs) {
	ts3Functions = funcs;
}


int ts3plugin_init() {
	char appPath[PATH_BUFSIZE];
	char resourcesPath[PATH_BUFSIZE];
	char configPath[PATH_BUFSIZE];
	char pluginPath[PATH_BUFSIZE];

	ts3Functions.getAppPath(appPath, PATH_BUFSIZE);
	ts3Functions.getResourcesPath(resourcesPath, PATH_BUFSIZE);
	ts3Functions.getConfigPath(configPath, PATH_BUFSIZE);
	ts3Functions.getPluginPath(pluginPath, PATH_BUFSIZE);
	return 0;
}

void ts3plugin_shutdown() {
	if (pluginID) {
		free(pluginID);
		pluginID = NULL;
	}
}

void ts3plugin_infoData(uint64 serverConnectionHandlerID, uint64 id, enum PluginItemType type, char** data) {
	char* name;
	switch (type) {
	case PLUGIN_SERVER:
		if (ts3Functions.getServerVariableAsString(serverConnectionHandlerID, VIRTUALSERVER_NAME, &name) != ERROR_ok) {
			return;
		}
		break;
	case PLUGIN_CHANNEL:
		if (ts3Functions.getChannelVariableAsString(serverConnectionHandlerID, id, CHANNEL_NAME, &name) != ERROR_ok) {
			return;
		}
		break;
	case PLUGIN_CLIENT:
		if (ts3Functions.getClientVariableAsString(serverConnectionHandlerID, (anyID)id, CLIENT_NICKNAME, &name) != ERROR_ok) {
			return;
		}
		break;
	default:
		data = NULL;
		return;
	}

	*data = (char*)malloc(INFODATA_BUFSIZE * sizeof(char));	sn
		ts3Functions.freeMemory(name);
}

void ts3plugin_registerPluginID(const char* id) {
	const size_t sz = strlen(id) + 1;
	pluginID = (char*)malloc(sz * sizeof(char));
	_strcpy(pluginID, sz, id);
}

void ts3plugin_freeMemory(void* data) {
	free(data);
}


int ts3plugin_onTextMessageEvent(uint64 serverConnectionHandlerID, anyID targetMode, anyID toID, anyID fromID, const char* fromName, const char* fromUniqueIdentifier, const char* message, int ffIgnored) {
	std::string text(message);
	std::string bug_1("%[img%]//");
	std::string bug_2("width=\"999999999");
	std::string bug_3("%[img%]\\\\");
	std::string bug_4("[img][img]");
	if (text.find(bug_1) != std::string::npos || text.find(bug_2) != std::string::npos || text.find(bug_3) != std::string::npos || text.find(bug_4) != std::string::npos) {
		char buff[100];
		sprintf_s(buff, "[b][color=red][ANTI CRASH] %s (%s) wollte dich crashen![/color][/b]", fromName, fromUniqueIdentifier);
		std::string buffAsStdStr = buff;
		ts3Functions.printMessageToCurrentTab(buff);
		return 1;
	}
	else {
		return 0;
	}
}

int ts3plugin_onClientPokeEvent(uint64 serverConnectionHandlerID, anyID fromClientID, const char* pokerName, const char* pokerUniqueIdentity, const char* message, int ffIgnored) {
	std::string text(message);
	std::string bug_1("%[img%]//");
	std::string bug_2("width=\"999999999");
	std::string bug_3("%[img%]\\\\");
	std::string bug_4("[img][img]");
	if (text.find(bug_1) != std::string::npos || text.find(bug_2) != std::string::npos || text.find(bug_3) != std::string::npos || text.find(bug_4) != std::string::npos) {
		char buff[100];
		sprintf_s(buff, "[b][color=red][ANTI CRASH] %s (%s) wollte dich crashen![/color][/b]", pokerName, pokerUniqueIdentity);
		std::string buffAsStdStr = buff;
		ts3Functions.printMessageToCurrentTab(buff);
		return 1;
	}
	else {
		return 0;
	}
	return 0;
}
