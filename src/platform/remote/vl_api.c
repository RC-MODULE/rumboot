#define _GNU_SOURCE 1 

#include <sys/types.h>

#ifdef __linux__
#include <sys/socket.h>
#include <netinet/in.h>

#include <sys/mman.h>

#include <netdb.h>	
#include <sys/ucontext.h>
#include <arpa/inet.h>
#endif //__linux__



#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#include <mman_win.h>
#endif //WIN32




#include <string.h>

#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>


#include <signal.h>

#include <errno.h>

#ifndef RUMBOOT_PLATFORM
#include "vl_api.h"
#else
#include <platform/vl_api.h>
#endif

#ifdef __cplusplus
#include <ostream>
#include <iomanip>
std::ostream& operator << (std::ostream& out, const vl_message& msg)
{
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
				if (msg.data[i]=='\n')
					continue;
				if (msg.data[i]=='\r')
					continue;
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

void vl_print_message(const struct vl_message* msg)
{
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
			int i;
			for (i=0;i<msg->data_len-1;i++){
				if (msg->data[i]=='\n')
					continue;
				if (msg->data[i]=='\r')
					continue;
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
	    	int i=0;
	    	for (i=0;i<msg->data_len;i++){
			        uint8_t ch=msg->data[i];
	       		printf("%02x",ch);
	       	}
	    }
	}
	printf("\n");
}


uint64_t vl_get_ticks_per_us(struct vl_instance *vl)
{
	struct vl_message command;
	struct vl_message answer;
	command.op = 	VL_OP_GET_TICKS_PER_US;
	vl_transaction(vl,&command,&answer);
	return answer.value;
}

void vl_set_io_delay(struct vl_instance *vl, uint32_t read_min_delay, uint32_t write_min_delay )
{
	struct vl_message command;
	struct vl_message answer;
	command.op = 	VL_OP_SET_IO_DELAY;
	command.value = read_min_delay| (((uint64_t)write_min_delay)<<32);
	vl_transaction(vl,&command,&answer);
}

int vl_recv(struct vl_instance *vl, struct vl_message* in_message)
{
 	memset(in_message,0,sizeof(struct vl_message));
	int header_len = (char*)(in_message->data)-(char*)(in_message);
	int bytes_read = recv(vl->sockfd, (char*)in_message, header_len, 0);
	if (bytes_read!= header_len) {
		printf("VLAPI: sock recv error. read %d of %d\n", bytes_read, header_len);
		if (vl->error_handler) 
			vl->error_handler(VL_SOCKET_ERROR);
		return -1;
	}
    if (in_message->data_len){
    	int data_read = recv(vl->sockfd, in_message->data,  in_message->data_len, 0);
    	if (data_read!= in_message->data_len) {
			printf("VLAPI: sock recv error. read %d of %d\n", bytes_read, in_message->data_len);
			if (vl->error_handler) 
				vl->error_handler(VL_SOCKET_ERROR);
			return -1;
		}
		bytes_read+=data_read;
    }
    if (vl->verbose&VL_EVENT_RECV){
    	printf("  in:");
		vl_print_message(in_message);
    }
    return bytes_read; 
}


int vl_send(struct vl_instance *vl, const struct vl_message* out_message){
	int header_len = (char*)out_message->data-(char*)out_message;
	int msg_len  = header_len+out_message->data_len;
	if (vl->verbose&VL_EVENT_SEND){
		printf(" out:");
		vl_print_message(out_message);
	} 
	int res=send(vl->sockfd, (char*)out_message,msg_len , 0);
	if (res!=msg_len){
		printf("VLAPI: sock send error. send %d of %d\n", res,msg_len);
		if (vl->error_handler) 
			vl->error_handler(VL_SOCKET_ERROR);
		return -1;
	}
	return res;
}


int static vl_sync=0;
void vl_iowrite64(struct vl_instance *vl, uint64_t const value, uint64_t const base_addr)
{
	vl_iowrite32(vl,value,base_addr);
	vl_iowrite32(vl,value>>32,base_addr+4);
}

void vl_iowrite32(struct vl_instance *vl, uint32_t const value, uint64_t const base_addr)
{
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
void vl_iowrite16(struct vl_instance *vl, uint16_t const value, uint64_t const base_addr)
{
	vl_iowrite32(vl, value, base_addr);
}
void vl_iowrite8 (struct vl_instance *vl, uint8_t  const value, uint64_t const base_addr)
{
	vl_iowrite32(vl, value, base_addr);
}

uint64_t vl_ioread64 (struct vl_instance *vl, uint64_t const base_addr)
{
	uint64_t res64;
	res64 = vl_ioread32(vl,base_addr);
	res64|= ((uint64_t)vl_ioread32(vl,base_addr+4))<<32;
	return res64;
}
uint16_t vl_ioread16 (struct vl_instance *vl, uint64_t const base_addr)
{
	return 0;//vl_ioread32 (vl, base_addr);
}
uint8_t vl_ioread8  (struct vl_instance *vl, uint64_t const base_addr)
{
	return 0;//vl_ioread32 (vl, base_addr);	
}

uint32_t vl_ioread32(struct vl_instance *vl, uint64_t const base_addr)
{
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

int vl_transaction(struct vl_instance *vl, const struct vl_message* send_message, struct vl_message* recv_message)
{
	int res=vl_send(vl,send_message);
	do {
		vl_recv(vl,recv_message); 
	} while (recv_message->op!=VL_OP_FINISHED);
	return recv_message->value;	
}

uint32_t vl_wait(struct vl_instance *vl, uint32_t irq_mask, const uint64_t clocks)
{
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_WAIT;
	cmd.addr 	= irq_mask;
	cmd.value	= clocks;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	do {
		vl_recv(vl,&ans); 
		if (ans.op==VL_OP_TERMINATED){
			vl_print_message(&ans);
			if (vl->error_handler)
				vl->error_handler(VL_OP_TERMINATED);
		}
	} while (ans.op!=VL_OP_FINISHED);
	return ans.value;
}

uint64_t vl_get_uptime(struct vl_instance *vl)
{
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

struct vl_shmem* vl_shmem_list(struct vl_instance *vl)
//;, bool automap)
{
	if (vl->shared_mem_list!=0) 
		return vl->shared_mem_list;
	struct vl_message cmd,ans;
	cmd.sync	= vl_sync++;
	cmd.op 		= VL_OP_SHMEM_LIST;
	cmd.addr 	= 0;
	cmd.value 	= 0;
	cmd.data_len= 0;
	int res=vl_send(vl,&cmd);
	vl_recv(vl,&ans);  // accept
	
	vl_recv(vl,&ans);  // started & first shmem
	

	//if (ans.value>16){
	//	printf ("ERROR: incorrect number of shared memories=%d\n",ans.value);
	//	return 0;
	//}
	vl->shared_mem_list = (struct vl_shmem*) calloc (1, sizeof(struct vl_shmem)*(ans.value+1));	
	struct vl_shmem* shmem=vl->shared_mem_list;

	memcpy(shmem,ans.data,ans.data_len);
	//if (automap) 
	//	vl_shmem_map(shmem);
	shmem++;

	int i;
	for (i=1; i< ans.value; i++, shmem++){
		vl_recv(vl,&ans); 
		memcpy(shmem,ans.data,ans.data_len);
		//if (automap) 
		//	vl_shmem_map(shmem);
	}
	shmem->size=0; 	// last terminating shem must have size=0 
	if (ans.op!=VL_OP_FINISHED){
		printf("ERROR: Incorrect shmem_list answer \n");
	}
	return vl->shared_mem_list;	
}

// server  side fuction
void *vl_shmem_create(struct vl_shmem* shmem)
{
	//char template[] = 

    //int fd = open(shmem->ipc_key, O_RDWR | O_CREAT, 0666);
    int fd = mkstemp(shmem->ipc_key);
    //printf("fd=%d\n",fd);
    if (fd ==-1){
        perror("Error creating temp shared memory file ");        
        perror(shmem->ipc_key);
        return (void*)-1;
    }
#ifdef __linux__
    char tmp[256];
    sprintf(tmp,"/proc/self/fd/%d",fd);
    readlink(tmp,shmem->ipc_key,sizeof(shmem->ipc_key));
#endif
#ifdef WIN32
	GetFullPathName(shmem->ipc_key, sizeof(shmem->ipc_key), shmem->ipc_key, NULL);
#endif
    printf("shmem_key:%s\n",shmem->ipc_key);
    
    int ok = lseek(fd, shmem->size - 1, SEEK_SET);
    if (ok == -1) {
        close(fd);
        perror("Error creating shared memory: calling lseek() to 'stretch' the file");
        return (void*)-1;
    }

    // write just one byte at the end 
    ok = write(fd, "", 1);
    if (ok < 0) {
        perror("Error creating shared memory:  writing a byte at the end of the file");
        close(fd);
        return (void*)-1;
    }

    
    shmem->ptr = mmap(NULL, shmem->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return shmem->ptr;
}

void *vl_shmem_map(struct vl_shmem* shmem){

	if (access(shmem->ipc_key, R_OK) == 0) {
		printf("VL_API: Using shared memory transport acceleration\n");
    	int fd = open(shmem->ipc_key, O_RDWR | O_CREAT, 0666);
    	if (fd < 0){
    	    perror("ERROR mapping shared memory: ");        
    	    return (void*)-1;
    	}
    	shmem->ptr = (uint8_t*)mmap(NULL, shmem->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    	if (shmem->ptr== MAP_FAILED){
    		perror("ERROR mapping shared memory: ");        
    	}

    	//printf("shmem info:\n");
    	//printf(" id      :%d\n",shmem->id);
    	//printf(" name    :%s\n",shmem->name);
    	//printf(" ipc_key :%s\n",shmem->ipc_key);
    	//printf(" sys_addr:%08x\n",shmem->sys_addr);
    	//printf(" size    :%x\n",shmem->size);
    	//printf(" ptr     :%08x\n",shmem->ptr);
    	close(fd);
		return shmem->ptr;
	}
	return 0;
	#ifdef INTERNAL_MAPPING
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
	#endif
}

// client  side fuction
int vl_shmem_unmap(struct vl_shmem* shmem)
{
    int err = munmap(shmem->ptr, shmem->size);
    if(err == -1){
        printf("VLAPI: UnMapping Failed\n");
    }
    return err;
}


#ifdef INTERNAL_MAPPING


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
	// Append the map to internal list 
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

#endif


struct vl_instance *vl_create(const char *host, uint16_t port){
	struct sockaddr_in addr;
	struct in_addr **addr_list;
	struct hostent *he;
	//struct vl_instance* vl = (struct vl_instance*) malloc(sizeof(*vl));
	struct vl_instance* vl = (struct vl_instance*) calloc(1,sizeof(*vl));

#ifdef WIN32
    // Initialize Winsock
    int iResult;
    WSADATA wsaData;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
    	perror("WSAStartup");
	    goto err_free_inst;
        //printf("WSAStartup failed: %d\n", iResult);
        //exit(1);
    }
#endif
	

	vl->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(vl->sockfd ==-1)
	{
	    perror("socket");
	    goto err_free_inst;
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

	int i;
	for(i = 0; addr_list[i] != NULL; i++) {
		char str[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, addr_list[i], str, INET_ADDRSTRLEN);
		printf("VL_API: Trying to connect to %s:%d\n", str,port);
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
	//shutdown(vl->sockfd,SHUT_RDWR);
	struct vl_shmem* shmem=vl->shared_mem_list;
	while (shmem->size){
		vl_shmem_unmap(shmem);
		shmem++;
	}
	free(vl->shared_mem_list);
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
uint32_t vl_fread(struct vl_instance *vl, const char* path, const uint64_t addr ){
	printf ("reading file %s to %Xh ...\n",path,addr);	    
	void* virt= vl_phys_to_virt(vl,addr);
	if (virt){
		FILE *file = fopen(path,"rb");
		if (!file) {
			printf("ERROR: cannot open file  %s\n",path); 
			return 0;
		}		
		fseek(file,0,SEEK_END);
		size_t file_size=ftell(file);
		void* fit = vl_phys_to_virt(vl,addr+file_size);
		if (fit==0){
			printf("ERROR: file size cannot fit memory\n");
			return 0;
		}
		rewind(file);
		size_t res=fread(virt,1, file_size,file);
		fclose(file);
		//printf ("%d bytes were read\n",i );
		return res;
	}
	return 0;
}

uint32_t vl_fwrite(struct vl_instance *vl, const char* path, const uint64_t addr, const uint64_t size){
	printf ("writing file %s from %Xh of %d bytes...\n",path,addr,size);	    
    
	void* virt= vl_phys_to_virt(vl,addr);
	if (virt){
		FILE* file = fopen(path,"wb");
		if (!file) {
			printf("ERROR: cannot open file  %s\n",path); 
			return 0;
		}		
		size_t res=fwrite(virt,1,size,file);
		fclose(file);
		//printf (" %d bytes from %X were written to file %s...\n",size,base_addr,path);
		return res;
	}
	return 0;
}

uint64_t  			vl_virt_to_phys  (struct vl_instance *vl,volatile void *addr){
	if (vl->shared_mem_list==0){
		printf("ERROR: vl->shared_mem_list=0\n");
		return 0;
	}
	struct vl_shmem* shmem=vl->shared_mem_list;
	while (shmem->size){
		if (addr>=(void*)shmem->ptr && addr<(void*)(shmem->ptr+shmem->size)){
			return shmem->sys_addr+(uint8_t*)addr-(uint8_t*)shmem->ptr;
		}
		shmem++;
	}
	printf ("ERROR: no shared memory found at virt[%x]\n",addr );
	return 0;
}

void*				vl_phys_to_virt (struct vl_instance *vl,uint64_t base_addr)
{
	if (vl->shared_mem_list==0){
		printf("ERROR: vl->shared_mem_list=0\n");
		return 0;
	}
	struct vl_shmem* shmem=vl->shared_mem_list;
	while (shmem->size){
		if (base_addr>=shmem->sys_addr && base_addr<shmem->sys_addr+shmem->size){
			return shmem->ptr+base_addr-shmem->sys_addr;
		}
		shmem++;
	}
	printf ("ERROR: no shared memory found at phys[%x]\n",base_addr );
	return 0;
}

void vl_set_error_handler(struct vl_instance *vl, void (*error_handler)(int)){
	vl->error_handler=error_handler;
}

void vl_shutdown(struct vl_instance *vl){
	struct vl_message cmd;
	struct vl_message answer;
	cmd.op=VL_OP_SHUTDOWN;
	vl_transaction(vl,&cmd,&answer);
}
void vl_soft_reset(struct vl_instance *vl){
	struct vl_message cmd;
	struct vl_message answer;
	cmd.op=VL_OP_SOFT_RESET;
	vl_transaction(vl,&cmd,&answer);
}
void vl_hard_reset(struct vl_instance *vl){
	struct vl_message cmd;
	struct vl_message answer;
	cmd.op=VL_OP_HARD_RESET;
	vl_transaction(vl,&cmd,&answer);
}
int	vl_get_api_version(struct vl_instance *vl){
	struct vl_message cmd;
	struct vl_message answer;
	cmd.op=VL_OP_GET_API_VERSION;
	vl_transaction(vl,&cmd,&answer);	
	return answer.value;
}

int vl_trace(struct vl_instance *vl, int trace_id, const char* path){
	struct vl_message cmd;
	struct vl_message answer;
	cmd.op=VL_OP_TRACE;
	cmd.addr=trace_id;
	if (path==0)
		cmd.value=false;
	else{
		cmd.value=true;
		strcpy(cmd.data,path);
		cmd.data_len=strlen(path)+1;

	}

	printf("*********** %s\n",path);
	vl_transaction(vl,&cmd,&answer);	
	return answer.value;	
}
; /* 0.0.0.0, localhost, /tmp/my.sock */