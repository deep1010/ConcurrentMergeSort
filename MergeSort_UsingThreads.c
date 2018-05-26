#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int a[100000],n;
int temp[100000];
typedef struct node {
    int i;
    int j;
} N;
void merge(int l, int r)
{
    int mid = l+(r-l)/2;
    int i,j;
    i=l;
    j=mid+1;
    int k=0;
    while(i<=mid && j<=r)
    {
        if(a[i]<a[j])
        {
            temp[k]=a[i];
            i+=1;
        }
        else
        {
            temp[k]=a[j];
            j+=1;
        }
        k++;
    }
    while(j<=r)
    {
        temp[k]=a[j];
        k=k+1;
        j=j+1;
    }
    while(i<=mid)
    {
        temp[k++]=a[i++];
    }
    int s=l,t=0;
    while(s<=r && t<k)
        a[s++]=temp[t++];
    return;
}
void * mergesort(void *a)
{
        int ret;
        N n1, n2;
        N *p;
        p = (N *)a;
        int l=p->i;
        int r=p->j;
        int mid = l +(r-l)/2;
        pthread_t tid1, tid2;

        n2.i = mid+1;
        n2.j = r;
        n1.i = l;
        n1.j = mid;
        if (l >= r) return;
        if (ret = pthread_create(&tid2, NULL, mergesort, &n2)) 
        {
                printf("Error in creation of thread right-part merge\n");
                exit(1);
        }
        if (ret = pthread_create(&tid1, NULL, mergesort, &n1)) 
        {
                printf("Error in creation of thread for left-part merge\n");    
                exit(1);
        }
        pthread_join(tid2, NULL);
        pthread_join(tid1, NULL);
        merge(l, r);
        pthread_exit(NULL);
}
int main()
{
        int ret;
        scanf("%d",&n);
        for(int i=0;i<n;i++)
            scanf("%d",&a[i]);
        N m;
        m.i = 0;
        m.j = n-1;
        pthread_t tid;
        if (ret=pthread_create(&tid, NULL, mergesort,&m)) 
        {
                printf("Error in thread creation\n");
                exit(1);
        }
        pthread_join(tid,NULL);
        for (int i = 0; i < n; i++)
                        printf ("%d ", a[i]);
        printf ("\n");
        return 0;
}
