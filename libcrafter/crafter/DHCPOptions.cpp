/*
Copyright (c) 2012, Esteban Pellegrino
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ESTEBAN PELLEGRINO BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "DHCPOptions.h"
#include <cstdarg>

using namespace std;
using namespace Crafter;

static const string StringTag = "String";
static const string IPTag = "IP";
static const string GenericTag = "Generic";
static const string MessageTypeTag = "Message";
static const string ParameterListTag = "Parameter";

map<int,string> create_opt_map() {
	map<int,string> m;

	m[1] = "SubnetMask";
	m[2] = "TimeOffset";
	m[3] = "Router";
	m[4] = "TimeServer";
	m[5] = "NameServer";
	m[6] = "DomainServer";
	m[7] = "LogServer";
	m[8] = "QuotesServer";
	m[9] = "LPRServer";
	m[10] = "ImpressServer";
	m[11] = "RLPServer";
	m[12] = "Hostname";
	m[13] = "BootFileSize";
	m[14] = "MeritDumpFile";
	m[15] = "DomainName";
	m[16] = "SwapServer";
	m[17] = "RootPath";
	m[18] = "ExtensionFile";
	m[19] = "ForwardOn_Off";
	m[20] = "SrcRteOn_Off";
	m[21] = "PolicyFilter";
	m[22] = "MaxDGAssembly";
	m[23] = "DefaultIPTTL";
	m[24] = "MTUTimeout";
	m[25] = "MTUPlateau";
	m[26] = "MTUInterface";
	m[27] = "MTUSubnet";
	m[28] = "BroadcastAddress";
	m[29] = "MaskDiscovery";
	m[30] = "MaskSupplier";
	m[31] = "RouterDiscovery";
	m[32] = "RouterRequest";
	m[33] = "StaticRoute";
	m[34] = "Trailers";
	m[35] = "ARPTimeout";
	m[36] = "Ethernet";
	m[37] = "DefaultTCPTTL";
	m[38] = "KeepaliveTime";
	m[39] = "KeepaliveData";
	m[40] = "NISDomain";
	m[41] = "NISServers";
	m[42] = "NTPServers";
	m[43] = "VendorSpecific";
	m[44] = "NETBIOSNameSrv";
	m[45] = "NETBIOSDistSrv";
	m[46] = "NETBIOSNodeType";
	m[47] = "NETBIOSScope";
	m[48] = "XWindowFont";
	m[49] = "XWindowManager";
	m[50] = "AddressRequest";
	m[51] = "AddressTime";
	m[52] = "Overload";
	m[53] = "DHCPMsgType";
	m[54] = "DHCPServerId";
	m[55] = "ParameterList";
	m[56] = "DHCPMessage";
	m[57] = "DHCPMaxMsgSize";
	m[58] = "RenewalTime";
	m[59] = "RebindingTime";
	m[60] = "ClassId";
	m[61] = "ClientId";
	m[62] = "NetWare_IPDomain";
	m[63] = "NetWare_IPOption";
	m[64] = "NIS_Domain_Name";
	m[65] = "NIS_Server_Addr";
	m[66] = "Server_Name";
	m[67] = "Bootfile_Name";
	m[68] = "Home_Agent_Addrs";
	m[69] = "SMTP_Server";
	m[70] = "POP3_Server";
	m[71] = "NNTP_Server";
	m[72] = "WWW_Server";
	m[73] = "Finger_Server";
	m[74] = "IRC_Server";
	m[75] = "StreetTalk_Server";
	m[76] = "STDA_Server";
	m[77] = "User_Class";
	m[78] = "DirectoryAgent";
	m[79] = "ServiceScope";
	m[80] = "RapidCommit";
	m[81] = "ClientFQDN";
	m[82] = "RelayAgentInformation";
	m[83] = "iSNS";
	m[85] = "NDSServers";
	m[86] = "NDSTreeName";
	m[87] = "NDSContext";
	m[88] = "BCMCSControllerDomainNamelist";
	m[89] = "BCMCSControllerIPv4addressoption";
	m[90] = "Authentication";
	m[91] = "client_last_transaction_timeoption";
	m[92] = "associated_ipoption";
	m[93] = "ClientSystem";
	m[94] = "ClientNDI";
	m[95] = "LDAP";
	m[97] = "UUID_GUID";
	m[98] = "User_Auth";
	m[99] = "GEOCONF_CIVIC";
	m[100] = "PCode";

	return m;
}


map<int,string> create_mes_map() {
	map<int,string> m;
	m[1] = "DHCPDISCOVER";
	m[2] = "DHCPOFFER";
	m[3] = "DHCPREQUEST";
	m[4] = "DHCPDECLINE";
	m[5] = "DHCPACK";
	m[6] = "DHCPNAK";
	m[7] = "DHCPRELEASE";
	m[8] = "DHCPINFORM";
   return m;
}

/* A global table that maps the code with a readable string */
map<int,string> DHCPOptions::code_table = create_opt_map(); /* Please compiler, do RVO. Don't be a fool. */

/* A global table that maps the message type with a readable string */
map<int,string> DHCPOptions::mess_table = create_mes_map(); /* Please compiler, do RVO. Don't be a fool. */

/* -------- DHCP Options */

DHCPOptions::DHCPOptions(short_word code, string tag) : code(code), tag(tag) { }

Payload DHCPOptions::GetData() const {
	/* Payload to return */
	Payload ret_payload;
	/* Get the code of the options */
	byte net_code = code;
	ret_payload.SetPayload((const byte*)&net_code,sizeof(byte));

	/* Get the length of the options */
	byte net_length = data.GetSize();
	ret_payload.AddPayload((const byte*)&net_length,sizeof(byte));

	/* Finally, concatenate the data */
	ret_payload.AddPayload(data);

	return ret_payload;
}

/* Print DHCP options header */
void DHCPOptions::Print() const {
	cout << "  < DHCPOptions (" << dec << GetSize() << " bytes) " << ":: ";

	map<int,string>::const_iterator it_code = DHCPOptions::code_table.find(code);
	if(it_code != DHCPOptions::code_table.end())
		cout << "Code = " << DHCPOptions::code_table[code] << " ; " ;
	else
		cout << "Code = " << code << " ; " ;

	cout << "Length = " <<  data.GetSize() << " ; " ;
	cout << "Data = ";
	PrintData();
	cout << " > " << endl;
}

DHCPOptions::~DHCPOptions() { }

/* -------- DHCP String */

/* Constructor */
DHCPOptionsString::DHCPOptionsString(short_word code, const string& str_data) : DHCPOptions(code,StringTag), str_data(str_data) {
	/* Now, set the payload */
	SetPayload();
}

/* Print string data */
void DHCPOptionsString::PrintData() const { std::cout << str_data; }

/* Set a payload from a string */
void DHCPOptionsString::SetPayload() {
	/* Set the payload from the string */
	data.SetPayload(str_data.c_str());
}

/* Destructor */
DHCPOptionsString::~DHCPOptionsString() { }

/* -------- DHCP IP */

/* Constructor */
DHCPOptionsIP::DHCPOptionsIP(short_word code, const std::vector<std::string>& ip_addresses) : DHCPOptions(code,IPTag) {
	this->ip_addresses = ip_addresses;
	/* Now, set the payload */
	SetPayload();
}

/* Print string data */
void DHCPOptionsIP::PrintData() const {
	vector<string>::const_iterator it_ip;
	for(it_ip = ip_addresses.begin() ; it_ip != ip_addresses.end() ; it_ip++)
		if(it_ip != ip_addresses.end() - 1)
			cout << (*it_ip) << " ; ";
		else
			cout << (*it_ip);
}

/* Set a payload from a string */
void DHCPOptionsIP::SetPayload() {
	vector<string>::const_iterator it_ip;
	for(it_ip = ip_addresses.begin() ; it_ip != ip_addresses.end() ; it_ip++) {
		/* Get the IP in network byte order */
		in_addr_t bin_ip = inet_addr((*it_ip).c_str());
		/* Set the payload from the string */
		data.AddPayload((byte *)&bin_ip, sizeof(in_addr_t));
	}
}

/* Destructor */
DHCPOptionsIP::~DHCPOptionsIP() { }

/* -------- DHCP Generic */

/* Constructor */
DHCPOptionsGeneric::DHCPOptionsGeneric(short_word code, const byte* gen_data, size_t length) : DHCPOptions(code,GenericTag) {
	this->gen_data.SetPayload(gen_data,length);
	/* Now, set the payload */
	SetPayload();
}

/* Set a payload from a string */
void DHCPOptionsGeneric::SetPayload() {
	data = gen_data;
}

/* Destructor */
DHCPOptionsGeneric::~DHCPOptionsGeneric() { }

/* -------- DHCP Message Type */

/* Constructor */
DHCPOptionsMessageType::DHCPOptionsMessageType(short_word code, byte type) : DHCPOptions(code,MessageTypeTag), type(type) {
	/* Now, set the payload */
	SetPayload();
}

/* Set a payload from a string */
void DHCPOptionsMessageType::SetPayload() {
	data.SetPayload(&type,sizeof(byte));
}

/* Print string data */
void DHCPOptionsMessageType::PrintData() const {
	map<int,string>::const_iterator it_code = DHCPOptions::mess_table.find(type);
	if(it_code != DHCPOptions::mess_table.end())
		cout << DHCPOptions::mess_table[type];
	else
		cout << "0x" << hex << (word)type;
}

/* Destructor */
DHCPOptionsMessageType::~DHCPOptionsMessageType() { }

/* -------- DHCP Parameter List */

/* Constructor */
DHCPOptionsParameterList::DHCPOptionsParameterList(short_word code, const byte* par_data, size_t length) : DHCPOptions(code,ParameterListTag) {
	this->par_data.SetPayload(par_data,length);
	/* Now, set the payload */
	SetPayload();
}

/* Set a payload from a string */
void DHCPOptionsParameterList::SetPayload() {
	data = par_data;
}

/* Print string data */
void DHCPOptionsParameterList::PrintData() const {
	byte* raw_data = new byte[par_data.GetSize()];
	par_data.GetPayload(raw_data);
	for(size_t i = 0 ; i < par_data.GetSize() ; i++) {
		byte dhcpcode = raw_data[i];
		map<int,string>::const_iterator it_code = DHCPOptions::code_table.find(dhcpcode);
		if(i != par_data.GetSize() - 1)
			if(it_code != DHCPOptions::code_table.end())
				cout << DHCPOptions::code_table[dhcpcode] << " ; " ;
			else
				cout << "0x" << hex << (word)dhcpcode << " ; " ;
		else
			if(it_code != DHCPOptions::code_table.end())
				cout << DHCPOptions::code_table[dhcpcode];
			else
				cout << "0x" << hex << (word)dhcpcode;

	}
}

/* Destructor */
DHCPOptionsParameterList::~DHCPOptionsParameterList() { }

bool ValidateIP(const string& ipAddress) {
    struct sockaddr_in sa;
    int result = inet_pton(AF_INET, ipAddress.c_str(), &(sa.sin_addr));
    return result != 0;
}

/* This can get the DHCP options data from a string. It could be an IP or a generic name */
DHCPOptions* Crafter::CreateDHCPOption(short_word code, const string& str) {
	/* Just a generic name... */
	return new DHCPOptionsString(code,str);
}

DHCPOptions* Crafter::CreateDHCPOption(short_word code,const vector<string>& ip_addresses) {
	return new DHCPOptionsIP(code,ip_addresses);
}

/* This get the value of the DHCP options data from a word, short or a byte */
DHCPOptions* Crafter::CreateDHCPOption(short_word code, word value, byte type_tag) {

	if(code == DHCPOptions::DHCPMsgType) {
		return new DHCPOptionsMessageType(code,value);
	}

	switch(type_tag) {
		case DHCPOptions::BYTE:
			return new DHCPOptionsNumber<byte>(code,value);
			break;
		case DHCPOptions::SHORT:
			return new DHCPOptionsNumber<short_word>(code,value);
			break;
		case DHCPOptions::WORD:
			return new DHCPOptionsNumber<word>(code,value);
			break;
	}

	return 0;
}

/* This put raw_data into the DHCP options data. Ultimately, this is the function to use */
DHCPOptions* Crafter::CreateDHCPOption(short_word code, const byte* raw_data, size_t length) {
	if(code == DHCPOptions::DHCPMsgType) {
		if(length >= 1)
			return new DHCPOptionsMessageType(code,raw_data[0]);
	} else if(code == DHCPOptions::ParameterList)
		return new DHCPOptionsParameterList(code,raw_data,length);
	else
		return new DHCPOptionsGeneric(code,raw_data,length);

	return 0;
}
