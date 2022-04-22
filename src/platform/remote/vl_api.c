#define _GNU_SOURCE 1
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <netdb.h>	
#include <sys/ucontext.h>
#include <signal.h>
#include <arpa/inet.h>
#include <errno.h>

#ifndef RUMBOOT_PLATFORM
#include "vl_api.h"
#else
#include <platform/vl_api.h>
#endif

#ifdef __cplusplus
std::ostream& operator << (std::ostream& out, const vl_message& msg){
	out << std::setfill('0') << std::hex 
	<< " sync=0x" << std::setw(2*sizeof(msg.sync)) 		<< (int)(msg.sync) 
	<< " op=0x"   << std::setw(2*sizeof(msg.op)) 		<< (int)(msg.op)
	<< " addr=0x" << std::setw(2*sizeof(msg.addr)) 		<< msg.addr
	<< " val=0x"  << std::setw(2*sizeof(msg.value))		<< msg.value
	<< " len=0x"  << std::setw(2*sizeof(msg.data_len))	<< (int)(msg.data_len);
	if (msg.data_len==4)
		out << " intdata=" << std::dec << msg.intdata[0];
		//out << " intdata=0x" << std::setw(8) << msg.intdata[0];
	else if (msg.data_len){
	    bool data_is_text=false;
	    if (msg.data[msg.data_len-1]==0){
			data_is_text=true;
			for (int i=0;i<msg.data_len-1;i++){
				if (msg.data[i]<' ' || msg.data[i]>'~'){
					data_is_text=false;
					break;
				}
			}
	    }
		if (data_is_text)
			out <<  " data= "<< "'"<< msg.data << "'";
	    else {
	    	out <<  " data= ";
	    	for (int i=0;i<msg.data_len;i++){
			        uint8_t ch=msg.data[i];
	       		out << std::setfill('0') << std::hex << std::setw(2) << (unsigned)(ch);
	       	}
	    }
	}
	return out;    
}

#endif

void vl_print_message(const struct vl_message* msg){
	printf (" sync=0x%04x",msg->sync);
	printf (" op=0x%02x",msg->op);
	printf (" addr=0x%08llx",msg->addr);
	printf (" val=0x%08llx",msg->value);
	printf (" len=0x%02x",msg->data_len);

	if (msg->data_len==4)
		printf(" intdata=%d",msg->intdata[0]);
	else if (msg->data_len){
	    bool data_is_text=false;
	    if (msg->data[msg->data_len-1]==0){
			data_is_text=true;
			for (int i=0;i<msg->data_len-1;i++){
				if (msg->data[i]<' ' || msg->data[i]>'~'){
					data_is_text=false;
					break;
				}
			}
	    }
		if (data_is_text)
			printf(" data='%s'",msg->data);
	    else {
	    	printf(" data= ");
	    	for (int i=0;i<msg->data_len;i++){
			        uint8_t ch=msg->data[i];
	       		printf("%02x",ch);
	       	}
	    }
	}
	printf("\n");
}


uint64_t vl_get_ticks_per_us(struct vl_instance *vl)
{
	return vl->ticks_per_us; 
}

void vl_set_rw_min_time(struct vl_instance *vl, uint32_t read_min_time, uint32_t write_min_time )
{
	struct vl_message command;
	struct vl_message answer;
	command.op = 	VL_OP_SET_MIN_TIME;
	command.value = read_min_time| (((uint64_t)write_min_time)<<32);
	vl_transaction(vl,&command,&answer);
}

int vl_recv(struct vl_instance *vl, struct vl_message* in_message){
 	memset(in_message,0,sizeof(struct vl_message));
	int header_len = (char*)(in_message->data)-(char*)(in_message);
	int bytes_read = recv(vl->sockfd, in_message, header_len, 0);
	if (bytes_read!= header_len) {
		printf("VLAPI: sock recv error. read %d of %d\n", bytes_read, header_len);
		if (vl->error_handler) 
			vl->error_handler(-1);
		return -1;
	}
    if (in_message->data_len){
    	int data_read = recv(vl->sockfd, in_message->data,  in_message->data_len, 0);
    	if (data_read!= in_message->data_len) {
			printf("VLAPI: sock recv error. read %d of %d\n", bytes_read, in_message->data_len);
			if (vl->error_handler) 
				vl->error_handler(-1);
			return -1;
		}
		bytes_read+=data_read;
    }
    if (vl->debug){
    	printf("  in:");
		vl_print_message(in_message);
    }
    return bytes_read; 
}


int vl_send(struct vl_instance *vl, const struct vl_message* out_message){
	int header_len = (char*)out_message->data-(char*)out_message;
	int msg_len  = header_len+out_message->data_len;
	if (vl->debug){
		printf(" out:");
		vl_print_message(out_message);
	} 
	int res=send(vl->sockfd, out_message,msg_len , 0);
	if (res!=msg_len){
		printf("VLAPI: sock send error. send %d of %d\n", res,msg_len);
		if (vl->error_handler) 
			vl->error_handler(-1);
		return -1;
	}
	return res;
}


int static vl_sync=0;
void vl_iowrite64(struct vl_instance *vl, uint64_t const value, uint32_t const base_addr){
	vl_iowrite32(vl,value,base_addr);
	vl_iowrite32(vl,value>>32,base_addr+4);
}

void vl_iowrite32(struct vl_instance *vl, uint32_t const value, uint32_t const base_addr){
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_WRITE;
	cmd.addr 	= base_addr;
	cmd.value	= value;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	do {
		vl_recv(vl,&ans); 
	} while (ans.op!=VL_OP_FINISHED);
	
}
void vl_iowrite16(struct vl_instance *vl, uint16_t const value, uint32_t const base_addr){
	vl_iowrite32(vl, value, base_addr);
}
void vl_iowrite8 (struct vl_instance *vl, uint8_t  const value, uint32_t const base_addr){
	vl_iowrite32(vl, value, base_addr);
}

uint64_t vl_ioread64 (struct vl_instance *vl, uint32_t const base_addr){
	#if 0 //This shit doesn't work
	union{
		uint32_t res32[2];
		uint64_t res64;
	};
	res32[0] = vl_ioread32(vl,base_addr);
	res32[1] = vl_ioread32(vl,base_addr);
	return res64;
	#endif
	return 0;
}

uint16_t vl_ioread16 (struct vl_instance *vl, uint32_t const base_addr){
	return vl_ioread32 (vl, base_addr);
}
uint8_t vl_ioread8  (struct vl_instance *vl, uint32_t const base_addr){
	return vl_ioread32 (vl, base_addr);	
}

uint32_t vl_ioread32(struct vl_instance *vl, uint32_t const base_addr){
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_READ;
	cmd.addr 	= base_addr;
	cmd.value	= 0;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	do {
		vl_recv(vl,&ans); 
	} while (ans.op!=VL_OP_FINISHED);
	return ans.value;
}

int vl_transaction(struct vl_instance *vl, const struct vl_message* send_message, struct vl_message* recv_message){
	int res=vl_send(vl,send_message);
	do {
		vl_recv(vl,recv_message); 
	} while (recv_message->op!=VL_OP_FINISHED);
	return recv_message->value;	
}

uint32_t vl_wait(struct vl_instance *vl, uint32_t irq_mask, const uint64_t clocks){
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_WAIT;
	cmd.addr 	= irq_mask;
	cmd.value	= clocks;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	do {
		vl_recv(vl,&ans); 
	} while (ans.op!=VL_OP_FINISHED);
	return ans.value;
}

uint64_t vl_get_uptime(struct vl_instance *vl){
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_UPTIME;
	cmd.addr 	= 0;
	cmd.value 	= 0;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	do {
		vl_recv(vl,&ans); 
	} while (ans.op!=VL_OP_FINISHED);
	return ans.value;
}

struct vl_shmem* vl_shmem_list(struct vl_instance *vl){
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_SHMEM_LIST;
	cmd.addr 	= 0;
	cmd.value 	= 0;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	vl_recv(vl,&ans);  // accept
	vl_recv(vl,&ans);  // started
	vl->shared_mem_list = (struct vl_shmem*) malloc (sizeof(struct vl_shmem)*(ans.value+1));
	int indx=0;
	memcpy(&(vl->shared_mem_list[indx++]),ans.data,ans.data_len);
	while (ans.op!=VL_OP_FINISHED){
		vl_recv(vl,&ans); 
		memcpy(&(vl->shared_mem_list[indx++]),ans.data,ans.data_len);
	};
	return vl->shared_mem_list;	
}

// server  side fuction
void *vl_shmem_create(struct vl_shmem* shmem){
	//char template[] = 

    //int fd = open(shmem->ipc_key, O_RDWR | O_CREAT, 0666);
    int fd = mkstemp(shmem->ipc_key);
    //printf("fd=%d\n",fd);
    if (fd ==-1){
        perror("Error creating temp shared memory file ");        
        perror(shmem->ipc_key);
        return (void*)-1;
    }
    char tmp[256];
    sprintf(tmp,"/proc/self/fd/%d",fd);
    readlink(tmp,shmem->ipc_key,sizeof(shmem->ipc_key));
    printf("shmem_key:%s\n",shmem->ipc_key);
    
    int ok = lseek(fd, shmem->size - 1, SEEK_SET);
    if (ok == -1) {
        close(fd);
        perror("Error creating shared memory: calling lseek() to 'stretch' the file");
        return (void*)-1;
    }

    /* write just one byte at the end */
    ok = write(fd, "", 1);
    if (ok < 0) {
        perror("Error creating shared memory:  writing a byte at the end of the file");
        close(fd);
        return (void*)-1;
    }

    
    void *result = mmap(NULL, shmem->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return result;
}

// client  side fuction
int vl_shmem_unmap(void* ptr, uint64_t size){
    int err = munmap(ptr, size);
    if(err == -1){
        printf("VLAPI: UnMapping Failed\n");
    }
    return err;
}

struct shm_internal_mapping {
	struct vl_instance *inst;
	struct vl_shmem    *shm;
	void *start;
	void *end;
	struct shm_internal_mapping *next;
};

static struct shm_internal_mapping *imaps;


static int shm_internal_mapping_find(void *addr, struct vl_instance **vl, struct vl_shmem **shm)
{

}

/*
The instruction to decode is at (void *)context->gregs[REG_IP] on x86, 
and at (void *)context->gregs[REG_RIP] on x86-64. 

The function must skip the instruction by incrementing context->gregs[REG_IP]/context->gregs[REG_RIP]/etc. 
by the number of bytes in the machine instruction. If you don't, SIGSEGV will just be raised again and again, 
with the program code stuck in that instruction!

*/

unsigned int eacdx(uint8_t code) {
	switch (code & 0xF) {
		case 0:
			printf("EAX\n");
			return REG_EAX;
		case 1:
			printf("ECX\n");
			return REG_ECX;
		case 2:
			printf("EDX\n");
			return REG_EDX;
		case 3:
			printf("EBX\n");
			return REG_EBX;
		default:
			printf("DECODE FAILED\n");
			exit(1);
	}
}

static void emulate(struct shm_internal_mapping *map, mcontext_t *const context, void *addr)
{
	uintptr_t offset = (uintptr_t) (addr - map->start);
	printf("VA_API: Emulating access to: %p (off %p) (%s)\n", addr, offset, map->shm->name);
	uint8_t *ptr = context->gregs[REG_EIP];
	printf("EIP: %p\n", ptr);
	printf("OPCODEZ: %p: %x %x %x %x %x %x %x %x \n", ptr, ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7]);
	if (*ptr == 0xc7) {
    	/*13f7:	c7 40 04 55 55 aa aa 	movl   $0xaaaa5555,0x4(%eax) */
		uint32_t *data = (uint32_t*) &ptr[3];
		printf("DATA: %x REG %d\n", *data, eacdx(ptr[1]));
		//context->gregs[eacdx(ptr[1])] = data;
		context->gregs[REG_EIP]+=7;
	} else {
		printf("EMULATION FAILED: UNKNOWN OPCODEZ\n");
		_exit(1);
	}

}


static void sigsegv_handler(int signum, siginfo_t *info, void *context)
{
	struct shm_internal_mapping *pos = imaps;
	while (pos) {
		if ((info->si_addr >= pos->start) && (info->si_addr < pos->end)) {
			break;
		}
		pos = pos->next;
	}

	if (!pos) {
		printf("EMULATION FAILED\n");
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_handler = SIG_DFL;
        act.sa_flags = 0;
        if (sigaction(SIGSEGV, &act, NULL) == 0)
            raise(SIGSEGV);
        else
            raise(SIGKILL);
	}

	const int saved_errno = errno;
    emulate(pos, &((ucontext_t *)context)->uc_mcontext, info->si_addr);
    errno = saved_errno;
	printf("EMULATION SUCCESS\n");
}

static void *vl_shm_internal_mapping_create(struct vl_instance *inst, struct vl_shmem *shm)
{
	struct shm_internal_mapping *map = (struct shm_internal_mapping*) calloc(1, sizeof(*map));
	map->shm = shm;
	map->inst = inst;
	map->start = mmap(NULL, shm->size, PROT_NONE,
	                MAP_PRIVATE | MAP_ANONYMOUS, -1, (off_t)0);
	if (map->start != MAP_FAILED)
	    map->end = (void *)(shm->size + (char *)map->start);
	else {
		return 0;
	}
	/* Append the map to internal list */
	if (!imaps) {
		imaps = map;
	} else {
		struct shm_internal_mapping *pos = imaps;
		while (pos->next) {
			pos = pos->next;
		}
		pos->next = map;
	}

	return map->start;
}


void *vl_shmem_map(struct vl_shmem* shmem){

	if (access(shmem->ipc_key, R_OK) == 0) {
		printf("VL_API: Using shared memory transport acceleration\n");
    	int fd = open(shmem->ipc_key, O_RDWR | O_CREAT, 0666);
    	if (fd < 0){
    	    perror("Error mapping shared memory: cannot open file");        
    	    return (void*)-1;
    	}
    	void *result = mmap(NULL, shmem->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    	close(fd);
		return result;
	}
	printf("VL_API: Using mirrored memory emulation\n");
	struct sigaction act;
    sigemptyset(&act.sa_mask);
	act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sigsegv_handler;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SA_SIGINFO);
    if (sigaction(SIGSEGV, &act, NULL) == -1)
        return NULL;

    return vl_shm_internal_mapping_create(NULL, shmem);
}


struct vl_instance *vl_create(const char *host, uint16_t port){
	struct sockaddr_in addr;
	struct in_addr **addr_list;
	struct hostent *he;
	//struct vl_instance* vl = (struct vl_instance*) malloc(sizeof(*vl));
	struct vl_instance* vl = (struct vl_instance*) calloc(1,sizeof(*vl));
	

	vl->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(vl->sockfd < 0)
	{
	    perror("socket");
	    exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);


	if ( (he = gethostbyname( host ) ) == NULL) 
	{
		perror("gethostbyname");
		goto err_free_inst;
	}

	addr_list = (struct in_addr **) he->h_addr_list;

	for(int i = 0; addr_list[i] != NULL; i++) {
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, addr_list[i], str, INET_ADDRSTRLEN);
		printf("VL_API: Trying to connect to %s\n", str);
		addr.sin_addr = *addr_list[i];
		if(connect(vl->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
		    perror("connect");
			goto err_free_inst;
		} else {
			return vl;
		}
	}
	
	printf("VL_API: FATAL: No more possible addresses to try\n");

err_free_inst:
	free(vl);
	return NULL;
}

int vl_destroy(struct vl_instance *vl){
	close(vl->sockfd);
	free (vl->shared_mem_list);
	free(vl);
	return 0;
}

void vl_disconnect(struct vl_instance *vl){
	struct vl_message disconnect;
	struct vl_message answer;
	disconnect.op=VL_OP_DISCONNECT;
	vl_transaction(vl,&disconnect,&answer);
	printf("VLAPI: ack: %s\n",answer.data);
}

; /* 0.0.0.0, localhost, /tmp/my.sock */