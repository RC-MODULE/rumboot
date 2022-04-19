

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include <platform/vl_api.h>    



/*
std::ostream& operator << (std::ostream& out, const struct vl_message& msg){
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
*/


int vl_recv(struct vl_instance *vl, struct vl_message* in_message){
 	memset(in_message,0,sizeof(struct vl_message));
	int header_len = (char*)(in_message->data)-(char*)(in_message);
	int bytes_read = recv(vl->sockfd, in_message, header_len, 0);
	if (bytes_read!= header_len) {
		printf("sock recv error. read %d of %d\n", bytes_read, header_len);
		//std::cout << *in_message <<std::endl;
		return -1;
		//exit(4);
	}
    if (in_message->data_len){
    	int data_read = recv(vl->sockfd, in_message->data,  in_message->data_len, 0);
    	if (data_read!= in_message->data_len) {
			printf("sock recv error. read %d of %d\n", bytes_read, in_message->data_len);
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
    //    printf("UnMapping Failed\n");
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
	//vl_instance* vl = new vl_instance;
	struct vl_instance* vl = (struct vl_instance*) malloc(sizeof(struct vl_instance));

	vl->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(vl->sockfd < 0)
	{
	    perror("socket");
	    exit(1);
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); 
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	if(connect(vl->sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
	    perror("connect");
	    exit(2);
	}
	return vl;
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
	printf("ack: %s\n",answer.data);
}

; /* 0.0.0.0, localhost, /tmp/my.sock */