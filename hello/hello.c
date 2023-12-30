/*
 * Hello world example
 */
#include <rtems.h>
#include <stdlib.h>
#include <stdio.h>
void hello()
{
  while (1)
  {
    // char s[100];
    // puts("输入：");
    // // scanf("%s", s);
    // // printf("%s\n", s);
    // printf("\nHello World,yangning!!!\n");
    // if (s[0] == 'q')
    //   exit(0);
    for(int i=1;i<=10000;i++){
        if(i%100){
          printf("%d",i);
        }
    }
  }
}
rtems_task Init(
    rtems_task_argument ignored)
{
  hello();
}

// ./waf configure --rtems=$HOME/devel/rtems-aarch64/rtems/6 --rtems-bsp=aarch64/raspberrypi4b