#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pcap.h>

#include <netinet/in.h>

#include<net/ethernet.h>
#include<netinet/udp.h>
#include<netinet/tcp.h>
#include<netinet/ip.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <signal.h>

#define MAX_NETWORK_FLOWS 100000
#define MAX_TCP_PACKETS 500000

const char* interface;

void live_monitor(const char*);
void offline_monitor(const char*);

void decode_packet(u_char*, const struct pcap_pkthdr*, const u_char*);
void tcp_packet_info(const u_char*, int);
void udp_packet_info(const u_char*, int);

void sigint_handler(int);
void print_statistics();

struct tcp_packet
{
	char* source_ip;
	char* destination_ip;

	unsigned int source_port;
	unsigned int destination_port;
	unsigned int protocol;

	int ip_version;
	int header_length;
	int payload_length;

	unsigned int ack_number;
	unsigned int seq_number;
	int syn_flag;
	int fin_flag;
};

struct tcp_packet *tcp_packets;

void add_tcp_packet(char*, char*, unsigned int, unsigned int, int, int, int, int, unsigned int, unsigned int, int, int, struct tcp_packet**);
int check_packet_retransmission(char*, char*, unsigned int, unsigned int, int, int, int, int, unsigned int, unsigned int, int, int, int, int, struct tcp_packet**);

struct network_flow
{
	char* source_ip;
	char* destination_ip;

	unsigned int source_port;
	unsigned int destination_port;
	unsigned int protocol;
};

int network_flow_exists_in_array(char*, char*, unsigned int, unsigned int, int, struct network_flow**);
void add_network_flow(char*, char*, unsigned int, unsigned int, int, struct network_flow**);

struct network_flow *network_flows;

struct sockaddr_in source_ip, destination_ip;

int total_tcp_packets = 0;
int total_udp_packets = 0;
int total_skipped_packets = 0;
int total_packets = 0;

int total_network_flows = 0;
int total_tcp_flows = 0;
int total_udp_flows = 0;
int total_retransmissions = 0;

long total_tcp_bytes = 0;
long total_udp_bytes = 0;

int i, j;

void usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
  		   "Options:\n"
  		   "-i, <Network interface name>\n"
  		   "-r, <Packet capture filename>\n"
  		   "-h, Help message\n\n"
		   );
	exit(1);
}

void live_monitor(const char* interface)
{
	network_flows = malloc(sizeof(struct network_flow) * MAX_NETWORK_FLOWS);
	tcp_packets = malloc(sizeof(struct tcp_packet) * MAX_TCP_PACKETS);

	char error_buffer[PCAP_ERRBUF_SIZE];

	printf("  [+] Opening interface %s..\n", interface);

	pcap_t* interface_handle = pcap_open_live(interface, 65536, 1, 0, error_buffer);

	if(interface_handle == NULL)
	{
		fprintf(stderr, "  [-] Failed to open interface: %s\n", error_buffer);
		exit(1);
	}

	printf("  [+] Capturing packets..\n");

	pcap_loop(interface_handle , -1 , decode_packet , NULL);

	pcap_close(interface_handle);

	for (i = 0; i < MAX_NETWORK_FLOWS; i++)
	{
		if (network_flows[i].source_ip == NULL)
		{
			break;
		}

		free(network_flows[i].source_ip);
		free(network_flows[i].destination_ip);
	}

	for (i = 0; i < MAX_TCP_PACKETS; i++)
	{
		if (tcp_packets[i].source_ip == NULL)
		{
			break;
		}

		free(tcp_packets[i].source_ip);
		free(tcp_packets[i].destination_ip);
	}

	free(network_flows);
	free(tcp_packets);
}


void offline_monitor(const char* filename)
{
	network_flows = malloc(sizeof(struct network_flow) * MAX_NETWORK_FLOWS);
	tcp_packets = malloc(sizeof(struct tcp_packet) * MAX_TCP_PACKETS);

	char error_buffer[PCAP_ERRBUF_SIZE];

	printf("  [+] Opening file %s..\n", filename);

	pcap_t* file_handle = pcap_open_offline(filename, error_buffer);

	if(file_handle == NULL)
	{
		fprintf(stderr, "  [-] Failed to open interface: %s\n", error_buffer);
		exit(1);
	}

	printf("  [+] Reading packets..\n");

	pcap_loop(file_handle , -1 , decode_packet , NULL);

	pcap_close(file_handle);

	for (i = 0; i < MAX_NETWORK_FLOWS; i++)
	{
		if (network_flows[i].source_ip == NULL)
		{
			break;
		}

		free(network_flows[i].source_ip);
		free(network_flows[i].destination_ip);
	}

	for (i = 0; i < MAX_TCP_PACKETS; i++)
	{
		if (tcp_packets[i].source_ip == NULL)
		{
			break;
		}

		free(tcp_packets[i].source_ip);
		free(tcp_packets[i].destination_ip);
	}
	free(network_flows);
	free(tcp_packets);
}


int main(int argc, char *argv[])
{
	int ch;
	FILE* pcap_file;

	printf("+----------------------------------------------+\n");
	printf("|             Packet Capture Monitor           |\n");
	printf("+----------------------------------------------+\n");

  if (argc < 2)
  {
    usage();
  }

  while ((ch = getopt(argc, argv, "hi:r:")) != -1)
	{
		switch (ch)
		{
			case 'i':
				signal(SIGINT, sigint_handler);
				live_monitor(optarg);
				print_statistics();
				break;

			case 'r':
				offline_monitor(optarg);
				print_statistics();
				break;

			default:
				usage();
		}
	}
}


void decode_packet(u_char *args, const struct pcap_pkthdr *packet_header, const u_char *buffer)
{
	int size = packet_header->len;

	struct iphdr *iph = (struct iphdr*)(buffer + sizeof(struct ethhdr));

	total_packets++;

	if (iph->protocol == 6)
	{
		total_tcp_packets++;
		tcp_packet_info(buffer, size);
	}
	else if (iph->protocol == 17)
	{
		total_udp_packets++;
		udp_packet_info(buffer, size);
	}
	else
	{
		total_skipped_packets++;
	}
}

void tcp_packet_info(const u_char* buffer, int size)
{
	struct ethhdr *eth = (struct ethhdr *)buffer;

	unsigned short iphdrlen;

	struct iphdr *iph = (struct iphdr *)(buffer  + sizeof(struct ethhdr));
	iphdrlen =iph->ihl * 4;

	memset(&source_ip, 0, sizeof(source_ip));
	source_ip.sin_addr.s_addr = iph->saddr;

	memset(&destination_ip, 0, sizeof(destination_ip));
	destination_ip.sin_addr.s_addr = iph->daddr;

	struct tcphdr *tcph = (struct tcphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

	int header_size =  sizeof(struct ethhdr) + iphdrlen + tcph->doff * 4;

	printf("+----------------------------------------------+\n");
	printf("|                  Packet Info                 |\n");
	printf("+----------------------------------------------+\n");
	printf("  [+] Source IP Address: %s\n", inet_ntoa(source_ip.sin_addr));
	printf("  [+] Destination IP Address: %s\n", inet_ntoa(destination_ip.sin_addr));
	printf("  [+] Source Port: %u\n", ntohs(tcph->source));
	printf("  [+] Destination Port: %u\n", ntohs(tcph->dest));
	printf("  [+] Protocol: TCP\n");
	printf("  [+] IP Version: IPv%d\n", iph->version);
	printf("  [+] Header Length: %d bytes\n", tcph->doff * 4);
	printf("  [+] Payload Length: %d bytes\n", size - header_size);
	// printf("  [+] Sequence Number    : %u\n", ntohl(tcph->seq));
	// printf("  [+] Acknowledgement Number : %d\n", ntohl(tcph->ack_seq));

	total_tcp_bytes = total_tcp_bytes + size;

	if (check_packet_retransmission(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(tcph->source), ntohs(tcph->dest), iph->protocol, iph->version, tcph->doff * 4, size - header_size, ntohl(tcph->seq), ntohl(tcph->ack_seq), ntohs(tcph->window), tcph->syn, tcph->fin, tcph->ack, &tcp_packets))
	{
		printf("  [+] Retransmitted\n");
	}

	add_tcp_packet(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(tcph->source), ntohs(tcph->dest), iph->protocol, iph->version, tcph->doff * 4, size - header_size, ntohl(tcph->seq), ntohl(tcph->ack_seq), tcph->syn, tcph->fin, &tcp_packets);

	if (total_tcp_flows == 0)
	{
		add_network_flow(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(tcph->source), ntohs(tcph->dest), iph->protocol, &network_flows);
		total_tcp_flows++;
		total_network_flows++;
	}

	if (!network_flow_exists_in_array(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(tcph->source), ntohs(tcph->dest), iph->protocol, &network_flows))
	{
		add_network_flow(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(tcph->source), ntohs(tcph->dest), iph->protocol, &network_flows);
		total_tcp_flows++;
		total_network_flows++;
	}
}

void udp_packet_info(const u_char* buffer, int size)
{
	struct ethhdr *eth = (struct ethhdr *)buffer;

	unsigned short iphdrlen;

	struct iphdr *iph = (struct iphdr *)(buffer  + sizeof(struct ethhdr) );
	iphdrlen =iph->ihl * 4;

	memset(&source_ip, 0, sizeof(source_ip));
	source_ip.sin_addr.s_addr = iph->saddr;

	memset(&destination_ip, 0, sizeof(destination_ip));
	destination_ip.sin_addr.s_addr = iph->daddr;

	struct udphdr *udph = (struct udphdr*)(buffer + iphdrlen + sizeof(struct ethhdr));

	int header_size = sizeof(struct ethhdr) + iphdrlen + sizeof udph;

	printf("+----------------------------------------------+\n");
	printf("|                  Packet Info                 |\n");
	printf("+----------------------------------------------+\n");
	printf("  [+] Source IP Address: %s\n", inet_ntoa(source_ip.sin_addr));
	printf("  [+] Destination IP Address: %s\n", inet_ntoa(destination_ip.sin_addr));
	printf("  [+] Source Port: %u\n", ntohs(udph->source));
	printf("  [+] Destination Port: %u\n", ntohs(udph->dest));
	printf("  [+] Protocol: UDP\n");
	printf("  [+] IP Version: IPv%d\n", iph->version);
	printf("  [+] Header Length: %ld bytes\n", sizeof udph);
	printf("  [+] Payload Length: %d bytes\n", size - header_size);

	total_udp_bytes = total_udp_bytes + size;

	if (total_udp_flows == 0)
	{
		add_network_flow(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(udph->source), ntohs(udph->dest), iph->protocol, &network_flows);
		total_udp_flows++;
		total_network_flows++;
	}

	if (!network_flow_exists_in_array(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(udph->source), ntohs(udph->dest), iph->protocol, &network_flows))
	{
		add_network_flow(inet_ntoa(source_ip.sin_addr), inet_ntoa(destination_ip.sin_addr), ntohs(udph->source), ntohs(udph->dest), iph->protocol, &network_flows);
		total_udp_flows++;
		total_network_flows++;
	}
}

int network_flow_exists_in_array(char *source_ip, char* destination_ip, unsigned int source_port, unsigned int destination_port, int protocol, struct network_flow **network_flows)
{
  for (i = 0; i < total_packets; i++)
  {
		if ((*network_flows)[i].source_ip == NULL || (*network_flows)[i].destination_ip == NULL)
		{
			return 0;
		}
		if (!strcmp((*network_flows)[i].source_ip, source_ip)
		&& !strcmp((*network_flows)[i].destination_ip, destination_ip)
		&& ((*network_flows)[i].source_port == source_port)
		&& ((*network_flows)[i].destination_port == destination_port)
		&& ((*network_flows)[i].protocol == protocol))
		{
				return 1;
		}
  }
}

void add_network_flow(char* source_ip, char* destination_ip, unsigned int source_port, unsigned int destination_port, int protocol, struct network_flow **network_flows)
{
 	(*network_flows)[total_network_flows].source_ip = malloc(16);
	(*network_flows)[total_network_flows].destination_ip = malloc(16);

	strcpy((*network_flows)[total_network_flows].source_ip, source_ip);
	strcpy((*network_flows)[total_network_flows].destination_ip, destination_ip);
	(*network_flows)[total_network_flows].source_port = source_port;
	(*network_flows)[total_network_flows].destination_port = destination_port;
	(*network_flows)[total_network_flows].protocol = protocol;
}

void add_tcp_packet(char* source_ip, char* destination_ip, unsigned int source_port, unsigned int destination_port, int protocol, int ip_version, int header_length, int payload_length, unsigned int seq_number, unsigned int ack_number, int syn_flag, int fin_flag, struct tcp_packet** tcp_packets)
{
	(*tcp_packets)[total_packets - 1].source_ip = malloc(16);
	(*tcp_packets)[total_packets - 1].destination_ip = malloc(16);

	strcpy((*tcp_packets)[total_packets - 1].source_ip, source_ip);
	strcpy((*tcp_packets)[total_packets - 1].destination_ip, destination_ip);
	(*tcp_packets)[total_packets - 1].source_port = source_port;
	(*tcp_packets)[total_packets - 1].destination_port = destination_port;
	(*tcp_packets)[total_packets - 1].protocol = protocol;
	(*tcp_packets)[total_packets - 1].ip_version = ip_version;
	(*tcp_packets)[total_packets - 1].header_length = header_length;
	(*tcp_packets)[total_packets - 1].payload_length = payload_length;
	(*tcp_packets)[total_packets - 1].ack_number = ack_number;
	(*tcp_packets)[total_packets - 1].seq_number = seq_number;
	(*tcp_packets)[total_packets - 1].syn_flag = syn_flag;
	(*tcp_packets)[total_packets - 1].fin_flag = fin_flag;
}

int check_packet_retransmission(char* source_ip, char* destination_ip, unsigned int source_port, unsigned int destination_port, int protocol, int ip_version, int header_length, int payload_length, unsigned int seq_number, unsigned int ack_number, int window, int syn_flag, int fin_flag, int ack_flag, struct tcp_packet** tcp_packets)
{
	for (i = 0; i < total_tcp_packets; i++)
	{
		if ((*tcp_packets)[i].source_ip != NULL && (*tcp_packets)[i].destination_ip != NULL)
		{
			if (!strcmp((*tcp_packets)[i].source_ip, source_ip)
			&& !strcmp((*tcp_packets)[i].destination_ip, destination_ip)
			&& ((*tcp_packets)[i].source_port == source_port)
			&& ((*tcp_packets)[i].destination_port == destination_port)
			&& ((*tcp_packets)[i].protocol == protocol)
			&& ((*tcp_packets)[i].ip_version == ip_version)
			&& ((*tcp_packets)[i].header_length == header_length)
		  && ((*tcp_packets)[i].ack_number == ack_number)
			&& ((*tcp_packets)[i].seq_number - 1 != ack_number)
			&& ((*tcp_packets)[i].seq_number < seq_number)
			&& ((*tcp_packets)[i].payload_length <= payload_length)
			&& ((*tcp_packets)[i].syn_flag == 1 || (*tcp_packets)[i].fin_flag == 1 || (*tcp_packets)[i].payload_length > 0)
			)
			{
				total_retransmissions++;
				return 1;
			}
		}
		else
		{
			return 0;
		}
	}
}

void print_statistics()
{
	printf("+----------------------------------------------+\n");
	printf("|                  Statistics                  |\n");
	printf("+----------------------------------------------+\n");
	printf("  [+] Total network flows captured: %d\n", total_network_flows);
	printf("  [+] Total TCP network flows captured: %d\n", total_tcp_flows);
	printf("  [+] Total UDP network flows captured: %d\n", total_udp_flows);
	printf("  [+] Total packets captured: %d\n", total_packets);
	printf("  [+] Total TCP packets captured: %d\n", total_tcp_packets);
	printf("  [+] Total UDP packets captured: %d\n", total_udp_packets);
	printf("  [+] Total skipped packets captured: %d\n", total_skipped_packets);
	// printf("  [+] Total retransmitted packets: %d\n", total_retransmissions);
	printf("  [+] Total bytes of TCP packets captured: %ld\n", total_tcp_bytes);
	printf("  [+] Total bytes of UDP packets captured: %ld\n", total_udp_bytes);
}

void sigint_handler(int sig)
{
    signal(SIGINT, sigint_handler);
		printf("\n");
    print_statistics();
		exit(1);
}
