
// error..
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// function type that is called for each filename
typedef int Myfunc(const char *,const struct stat *,int);
static Myfunc myfunc;
static int myftw(char *, Myfunc *);
static int dopath(Myfunc *);
static long nreg,ndir,nblk,nchr,nfifo,nslink,nsock,ntot;
char *path_alloc(int* size)
{
  char *p = malloc(256);
  if(!size) return NULL;
  p = malloc(256);
  if(p)
	*size = 256;
  else
	*size = 0;
  return p;
}

int main(int argc, char *argv[]) {
   int ret;
   if(argc != 2) {
      printf("Usage: ftw <starting-pathname>");
      exit(-1);
   }
   
    ret = myftw(argv[1],myfunc); // does it all
    ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;   
    if (ntot == 0) {
       ntot = 1; //avoid divided by 0,print 0 for all counts
    }

    printf("regular files = %7ld, %5.2f %%\n",nreg,nreg*100.0/ntot);
    printf("directories files = %7ld, %5.2f %%\n",ndir,ndir*100.0/ntot);
    printf("block files = %7ld, %5.2f %%\n",nblk,nblk*100.0/ntot);
    printf("char files = %7ld, %5.2f %%\n",nchr,nchr*100.0/ntot);
    printf("FIFOS files = %7ld, %5.2f %%\n",nfifo,nfifo*100.0/ntot);
    printf("symboliclinks files = %7ld, %5.2f %%\n",nslink,nslink*100.0/ntot);


} 

/**
Descend through the hierarchy, starting at "pathname"
The caller's func() is called for every file
*/

#define FTW_F 1  /*File other than directory*/
#define FTW_D 2 /*directory*/
#define FTW_DNR 3  /*directory that can't be read*/
#define FTW_NS 4 /*file that we can't stat*/

static char *fullpath;/*contains full path for every file*/
static int myftw(char *pathname,Myfunc *func) {
    int len;
    fullpath = path_alloc(&len);//malloc's for PATH_MAX + 1 bytes
    strncpy(fullpath,pathname,len);//protect against
    fullpath[len-1] = 0; //buffer overrun
    return (dopath(func));
}

/**
   Descend through the hierarchy,starting at "fullpath".
   if fullpath is anything other than a directory, we lstat() it,
   call func(), and return. For a directory, we call ourself
   recusively for each name in the directory
*/
static int dopath(Myfunc* func){ // we return whatever func() returns
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret;
    char *ptr;
    
    if(lstat(fullpath,&statbuf) < 0) { // stat error
       return (fuc(fullpath,&statbuf,FTW_NS));
    }
    
    if(S_ISDIR(statbuf.st_mode) == 0) { // not a directory
       return (func(fullpath,&statbuf, FTW_F));
    }
   
    if((ret = func(fullpath, &statbuf, FTW_D)) != 0) { 
        return ret;
    }
    
    ptr = fullpath + strlen(fullpath); // point to the end of fullpath
    *ptr ++ = '/';
    *ptr = 0;

    if((dp = opendir(fullpath)) == NULL) {
        return (func(fullpath,&statbuf,FTW_DNR));
    }

    while((dirp = readdir(dp)) != NULL){ 
       if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0){
          continue; // ignore dot and dot-dot
       } 
      strcpy(ptr,dirp->d_name); //append name after slash
      if((ret = dopath(func)) != 0) { // recursive
         break;//time to leave
     }
    }
    
   ptr[-1] = 0; //erase everything from slash onwards
   if(closeddir(dp) < 0) {
      printf("can not close directory %s\n",fullpath);
      return -1;
   }

   return ret;
}

static int myfunc(const char *pathname,const struct stat *statptr, int type) {
     switch(type) {
       case FTW_F:
            switch (statptr->st_mode & S_IFMT) {
       	          case S_IFREG: nreg++; break;
                  case S_IFBLK: nblk++; break;
                  case S_IFCHR: nchr++; break;
		  case S_IFIFO: nfifo++; break;
                  case S_IFLNK: nslink ++; break;
		  case S_IFSOCK: nsock ++; break;
		  case S_IFDIR:
                          /*directories should have type = FTW_D*/
                          printf("For S_IFDIR for %s",pathname);
                          return;   
            } break;
	case FTW_D:
           ndir ++; break;
        case FTW_DNR:
 	   printf("can't read directory %s\n",pathname);
           break;
        case FTW_NS:
           printf("stat error for %s\n",pathname);
           break;

        default:
           printf("unknow type %d for pathname %s", type, pathname);
     } 
    return 0;
}
