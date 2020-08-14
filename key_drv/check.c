#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/stat.h>
#include <sys/mman.h>
// #include <fcntl.h>
// #include <unistd.h>
#include <errno.h>
#include <linux/input.h>

void touch(int *x, int *y)
{
	int fd = open("/dev/input/event0", O_RDWR);
	if(fd<0)
	{
		perror("open");
		return ;
	}
	
	struct input_event buf;
	int tmp_x=0, tmp_y=0;
	while(1)
	{
		read(fd, &buf, sizeof(buf));
	
		if(buf.type==EV_ABS && buf.code==ABS_X)
		{
			tmp_x = buf.value;
		}	
		
		if(buf.type==EV_ABS && buf.code==ABS_Y)
		{
			tmp_y = buf.value;
		}
		
		if(tmp_x!=0 && tmp_y!=0)
		{
			*x = tmp_x;
			*y = tmp_y;
			break;
		}
	}
	close(fd);
}

int main(int argc,char **argv)
{
	
	int fd;
	fd = open("/dev/myled_drv",O_RDWR);
	if(fd < 0)
	{
		
		perror("open:");
		return -1;
		
	}
	
	int fd1;
	fd1 = open("/dev/mykey_drv",O_RDWR);
	if(fd1 < 0)
	{
		
		perror("open:");
		return -1;
		
	}
	

	//睡眠3秒
	// sleep(2);
	char buf1[2] = {'1','0'};
	char buf2[2] = {'1','1'};
	char buf3[2] = {'2','0'};
	char buf4[2] = {'2','1'};
	char buf5[2] = {'3','0'};
	char buf6[2] = {'3','1'};
	char buf7[2] = {'4','0'};
	char buf8[2] = {'4','1'};
	

	// for(int i=0;i<5;i++)
	// {
	// 	write(fd,buf1,2);
	// 	usleep(50000);
	// 	write(fd,buf2,2);
	// 	usleep(50000);
	// 	write(fd,buf3,2);
	// 	usleep(50000);
	// 	write(fd,buf4,2);
	// 	usleep(50000);
	// 	write(fd,buf5,2);
	// 	usleep(50000);
	// 	write(fd,buf6,2);
	// 	usleep(50000);
	// 	write(fd,buf7,2);
	// 	usleep(50000);
	// 	write(fd,buf8,2);
	// 	usleep(50000);


	// 	write(fd,buf8,2);
	// 	usleep(50000);
	// 	write(fd,buf7,2);
	// 	usleep(50000);
	// 	write(fd,buf6,2);
	// 	usleep(50000);
	// 	write(fd,buf5,2);
	// 	usleep(50000);
	// 	write(fd,buf4,2);
	// 	usleep(50000);
	// 	write(fd,buf3,2);
	// 	usleep(50000);
	// 	write(fd,buf2,2);
	// 	usleep(50000);
	// 	write(fd,buf1,2);
	// 	usleep(50000);
		

	// }
	char key_val=0;
	int len;
	// int x ,y;
	while(1)
	{
		// touch(&x,&y);
		// // printf("%d,%d\n",x/100,y/100 );

		// buf1[0] = (x/100%4 + 49);
		// buf1[1] = (y/100%2 + 48);

		// write(fd,buf1,2);
		// sleep(1);
		len =read(fd1,&key_val,1);
		
		if(len > 0)
		{
			if(key_val & 0x01)
			{
				printf("K2 Down\n");
				// printf("buf1:%d\n",key_val );
				//点灯
				write(fd,buf1,2);
				
			}

			if(key_val & 0x02)
			{
				//灭灯
				printf("K3 Down\n");
				// printf("buf1:%d\n",key_val );
				
			}
			
			if(key_val & 0x04)
			{
				printf("K4 Down\n");
				// printf("buf1:%d\n",key_val );
			}
			
			if(key_val & 0x08)
			{
				printf("K6 Down\n");
				// printf("buf1:%d\n",key_val );
			}

		}
		

	}

	// sleep(3);
	// write(fd,"b",1);
	sleep(3);
	//关闭myled的设备
	close(fd);
	
	return 0;
	
	
}