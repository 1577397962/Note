#include <stdio.h>
#include <string.h>
#include <errno.h>
int main()
{
    FILE *fp;
    fp = fopen("1","r");
    if(!fp)
    {
        /*
        * errno：一个int类型的错误码
        * perror：c语言标准库里的函数，perror函数读取errno的错误码，并根据这个产生错误信息打印到标准输出。输出不够灵活
        * strerror：c语言标准库函数，strerror需要格式化参数，但是使用灵活
        * stderr：linux的标准错误输出,stdin标准输入，stdout标准输出
        */
        printf("errno = %d\n",errno);
        perror("error open file");
        printf("error is %s\n",strerror(errno));
        fprintf(stderr,"error message num is %d\n",errno);
        return -1;
    }
    fclose(fp);
    return 0;
}