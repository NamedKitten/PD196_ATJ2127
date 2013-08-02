#include "psp_includes.h"

typedef int (*MainFunc)(int);
//int main(int);
int ap_start(void *arg);

extern int8 main(void* data);
/*main��Ҫ���������ֵ����ϵͳ��Ա����*/
OS_STK ap_stack;
OS_STK *ptos=&ap_stack;

extern INT8U prio;

_declspec(dllexport) MainFunc GetMainFun(void)
{
	return ap_start;
}

void *process_start(void *app_param)
{
    int8 ret;
    ret = (int8)main((int)app_param, 0);
    libc_exit((int32)ret);
    return 0;
}

/*
�ú�����exece�������ã�exece�ǽ��̹��������á�
��֧��construct ���ԣ���mainǰ�������AP�еĺ���
���Բ���Ҫlibcrt��
arg��exece������*/
int ap_start(void *arg)
{
   pthread_param_t pthread_param;
   int8 process_struct_index = libc_get_process_struct();

   if (process_struct_index == -1)
   {
   	/*process too many*/
	return -1;
   }

   pthread_param.start_rtn = process_start;
   pthread_param.arg = arg;
   pthread_param.ptos = ptos;

   sys_os_sched_lock();

   if(libc_pthread_create(&pthread_param, prio, process_struct_index) < 0)
   {
      libc_free_process_struct(process_struct_index);
      sys_os_sched_unlock();
   	  return -1;
   }

    sys_os_sched_unlock();
    return 0;
}
//#pragma comment(lib,"..\\..\\bin\\debug\\lib\\comlib")

