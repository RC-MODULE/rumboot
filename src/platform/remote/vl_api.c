

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>	

#include <platform/vl_api.h>    

int vl_recv(struct vl_instance *vl, struct vl_message* in_message){
 	memset(in_message,0,sizeof(struct vl_message));
	int header_len = (char*)(in_message->data)-(char*)(in_message);
	int bytes_read = recv(vl->sockfd, in_message, header_len, 0);
	if (bytes_read!= header_len) {
		rumboot_printf("VLAPI: sock recv error. read %d of %d\n", bytes_read, header_len);
		//std::cout << *in_message <<std::endl;
		return -1;
		//exit(4);
	}
    if (in_message->data_len){
    	int data_read = recv(vl->sockfd, in_message->data,  in_message->data_len, 0);
    	if (data_read!= in_message->data_len) {
			rumboot_printf("VLAPI: sock recv error. read %d of %d\n", bytes_read, in_message->data_len);
			//std::cout << *in_message <<std::endl;
			//exit(4);
			return -1;
		}
		bytes_read+=data_read;
    }
    //std::cout << "<<<   " << *in_message << std::endl;
    return bytes_read; 
}


int vl_send(struct vl_instance *vl, const struct vl_message* out_message){
	int header_len = (char*)out_message->data-(char*)out_message;
	int msg_len  = header_len+out_message->data_len;
	//std::cout << "   >>>" << *out_message << std::endl;
	int res=send(vl->sockfd, out_message,msg_len , 0);
	return res;
}


int static vl_sync=0;
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
	//vl->shared_mem_list = new vl_shmem[ans.value+1];
	vl->shared_mem_list = (struct vl_shmem*)malloc (sizeof(struct vl_shmem)*(ans.value+1));
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
    int fd = open(shmem->ipc_key, O_RDWR | O_CREAT, 0666);
    if (fd < 0){
        perror("Error creating shared memory: cannot open file");        
        return (void*)-1;
    }

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

    //if(err != 0){
    //    rumboot_printf("VLAPI: UnMapping Failed\n");
    //}
    return err;
}

// client  side fuction
void *vl_shmem_map(struct vl_shmem* shmem){
    int fd = open(shmem->ipc_key, O_RDWR | O_CREAT, 0666);
    if (fd < 0){
        perror("Error mappinng shared memory: cannot open file");        
        return (void*)-1;
    }
    void *result = mmap(NULL, shmem->size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);
    return result;
}


struct vl_instance *vl_create(const char *host, uint16_t port){
	struct sockaddr_in addr;
	struct in_addr **addr_list;
	struct hostent *he;
	struct vl_instance* vl = (struct vl_instance*) malloc(sizeof(struct vl_instance));

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
		rumboot_printf("VL_API: Trying to connect to %s\n", str);
		addr.sin_addr = *addr_list[i];
		if(connect(vl->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
		{
		    perror("connect");
			goto err_free_inst;
		} else {
			return vl;
		}
	}
	
	rumboot_printf("VL_API: FATAL: No more possible addresses to try\n");

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
	rumboot_printf("VLAPI: ack: %s\n",answer.data);
}

; /* 0.0.0.0, localhost, /tmp/my.sock */