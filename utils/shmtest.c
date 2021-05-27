
#define _GNU_SOURCE /* See feature_test_macros(7) */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <poll.h>
#include <stdio.h>
#include <unistd.h>

static int shmid;
static int ipc_id;
static int the_ipc_id;
void *segment;

static void *create_shared_memory(const char *skey, int id, size_t size)
{
        /* make the key: */
        key_t key;


        if ((key = ftok(skey, id)) == -1) { /*Here the file must exist */
                perror("ftok");
                exit(1);
        }

        /*  create the segment: */
        if ((shmid = shmget(key, size, 0644 | IPC_CREAT)) == -1) {
                perror("shmget");
                exit(1);
        }

        /* attach to the segment to get a pointer to it: */
        void *data = shmat(shmid, (void *)0, 0);
        if (data == (char *)(-1)) {
                perror("shmat");
                exit(1);
        }

        the_ipc_id = id;
        return data;
}

#define HEAP_SIZE (1 * 1024 * 1024)

int main()
{
    key_t key;

    system("touch ./rumboot.tmp");
    segment = create_shared_memory("./rumboot.tmp", ipc_id, HEAP_SIZE);

    if (-1 == shmdt(segment))
    {
        perror("shmdt: ");
        exit(1);
    }
    if (-1 == shmctl(shmid, IPC_RMID, NULL))
    {
        perror("shmctl: ");
        exit(1);
    }
}