#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

struct timeval stop, start;

//simple byte-to-byte copy
void* byte_memcpy(void *dst, const void *src, size_t ln)
{
    char * pDst = (char *) dst;
    char const * pSrc = (char const *) src;
    while (ln--)
    {
        *pDst++ = *pSrc++;
    }
    return (dst);
}

//for 4-bytes aligned data
void* byte4s_memcpy(void *dst, const void *src, size_t ln)
{
    uint32_t * pDst = (uint32_t *) dst;
    uint32_t const * pSrc = (uint32_t const *) src;
    //if (!(src & 0xFFFFFFFC) && !(dst & 0xFFFFFFFC))
    {
        while (ln >= 8)
        {
            *pDst++ = *pSrc++;
            *pDst++ = *pSrc++;
            ln -= 8;
        }

        while (ln >= 4)
        {
            *pDst++ = *pSrc++;
            ln -= 4;
        }
    }

    return (byte_memcpy(pDst,pSrc,ln));


}
//for 4-bytes aligned data
void* byte4_memcpy(void *dst, const void *src, size_t ln)
{
    uint32_t * pDst = (uint32_t *) dst;
    uint32_t const * pSrc = (uint32_t const *) src;
    //if (!(src & 0xFFFFFFFC) && !(dst & 0xFFFFFFFC))
    {
        while (ln >= 4)
        {
            *pDst++ = *pSrc++;
            ln -= 4;
        }
    }

    return (byte_memcpy(pDst,pSrc,ln));


}

//for 8 bytes aligned data
void* byte8_memcpy(void* dst, void* const src, size_t ln)
{
    uint64_t *pDst = (uint64_t*) dst;
    uint64_t const *pSrc = (uint64_t const*) src;
    //void* and unsigned int carry different sizes on different architectures
    //if (!(src & 0xFFFFFFFC) && !(dst & 0xFFFFFFFC))
    {
        while (ln >= 8)
        {
            *pDst++ = *pSrc++;
            ln -= 8;
        }
    }
    return (byte_memcpy(pDst,pSrc,ln));

}


int main()
{
    int sfd,dfd;
    char *src,*dst,*cp;
    struct stat s;
    size_t filesize;
     double timeofday;

    sfd = open("src.txt", O_RDONLY);
    if(sfd<0)
    {
        printf("Error in file opening to read!!\n");
        exit(2);
    }

    filesize = lseek(sfd, 0, SEEK_END);
    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, sfd, 0);

    dfd = open("dest.txt", O_RDWR | O_CREAT, 0666);
    if(dfd<0)
    {
        printf("Error in file opening to write!!\n");
        exit(3);
    }

    ftruncate(dfd, filesize);
    dst = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, dfd, 0);
    printf("***Copy data blocks through memcpy***\n\nfilesize=%zu bytes\n",filesize);

//time measurement of byte memory block copy
    gettimeofday(&start, NULL);
    byte_memcpy(dst,src, filesize);
    gettimeofday(&stop, NULL);
    timeofday=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);
    printf("byte block copy:time : %lf ms\n",timeofday*1000);

//time measurement of 4-byte memory block copy
    gettimeofday(&start, NULL);
    byte4_memcpy(dst,src, filesize);
    gettimeofday(&stop, NULL);
    timeofday=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);
    printf("byte4 block copy:time : %lf ms\n",timeofday*1000);

//time measurement of 4-byte memory block copy with improvement
     gettimeofday(&start, NULL);
     byte4s_memcpy(dst,src, filesize);
     gettimeofday(&stop, NULL);
     timeofday=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);
     printf("byte4_improved block copy:time : %lf ms\n",timeofday*1000);

//time measurement of 8-byte memory block copy
    gettimeofday(&start, NULL);
     byte8_memcpy(dst,src, filesize);
     gettimeofday(&stop, NULL);
    timeofday=(double) (stop.tv_usec - start.tv_usec) / 1000000 +(double) (stop.tv_sec - start.tv_sec);
    printf("byte8 block copy:time : %lf ms\n",timeofday*1000);


     close(sfd);
    close(dfd);
    return 0;
}

