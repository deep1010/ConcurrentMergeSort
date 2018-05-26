#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int *shm_array;

void SelectionSort(int l,int r)
{
    int i,j,indx=l;
    for(i=l;i<=r;i++)
    {
        indx=i;
        for(j=i;j<=r;j++)
        {
            if(shm_array[j]<=shm_array[indx])
                indx=j;
        }
        shm_array[i]=shm_array[indx]+shm_array[i]-(shm_array[indx]=shm_array[i]);
    }

    return;
}
 
void merge(int l, int mid, int r)
{
    int i=l, j=mid+1, k=0;
    int temp[r-l+1];
    for (; j<=r && i<=mid; )
    {
        if (shm_array[i] > shm_array[j])
        {
            temp[k]=shm_array[j];
            k++;
            j++;
        }
        else
        {
            temp[k]=shm_array[i];
            k++;
            i++;
        }
    }

    for (;j<=r;)
        temp[k++]=shm_array[j++];
 
    for (;i<=l;)
        temp[k++]=shm_array[i++];

    for (i=0;i<k;i++)
        shm_array[l+i] = temp[i];
}
 
void mergeSort(int l, int r)
{
    int status;
    int mid=l+(r-l)/2;
    int i,len=(r-l+1);

    if (len<=5)
    {
        SelectionSort(l, r);
        return;
    }

    pid_t lpid,rpid;
    if ((rpid = fork())<0)
    {
        printf("Right Child Proc. not created\n");
        _exit(-1);
    }
    else if (rpid==0)
    {
        mergeSort(mid+1,r);
        _exit(0);
    }
    else
    {
        if ((lpid = fork())<0)
        {
            printf("Left Child Proc. not created\n");
            _exit(-1);
        }
        else if(lpid==0)
        {
            mergeSort(l,mid);
            _exit(0);
        }
    }
    
    waitpid(rpid, &status, 0); 
    waitpid(lpid, &status, 0);
    merge(l, mid, r);
}
int main()
{
    int shmid,length=0;
    key_t key = IPC_PRIVATE;
 
    scanf("%d",&length);
 
    size_t SHM_SIZE = 4*length;
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if(shmid < 0)
    {
        printf("shmget\n");
        _exit(1);
    }
    shm_array = shmat(shmid, NULL, 0);
    if(shm_array == (int *) -1)
    {
        printf("shmat\n");
        _exit(1);
    }

    for (int i = 0; i < length; ++i)
        scanf("%d",&shm_array[i]);
     
    mergeSort(0, length-1);

    printf("\nFinal Array\n");
    
    for (int i = 0; i < length; ++i)
        printf("%d ",shm_array[i]);
    printf("\n");
    if (shmdt(shm_array) == -1)
    {
        printf("shmdt\n");
        _exit(1);
    }
 
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        printf("shmctl\n");
        _exit(1);
    }
 
    return 0;
}